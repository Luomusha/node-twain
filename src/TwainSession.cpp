//
// Created by Lossa on 2022/7/4.
//
#include <napi.h>
#include "TwainSession.h"

TW_UINT16 message;

TW_UINT16 dsmCallback(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest, TW_UINT32 uiDG, TW_UINT16 uiDAT, TW_UINT16 uiMSG, TW_MEMREF pData) {
    std::cout << "Trigger callback" << std::endl;
    switch(uiMSG) {
        case MSG_XFERREADY:
            std::cout << "Callback:" << "MSG_XFERREADY" << std::endl;
            message = uiMSG;
            break;
    }
    return TWRC_SUCCESS;
}

void TwainSession::fillIdentity(TW_IDENTITY id) {
    identity = id;
}

TW_UINT16 TwainSession::loadDSM() {
    TW_UINT16 rc = TWRC_FAILURE;
    // load library
    std::cout << "Load Library:" << kTWAIN_DSM_DIR << kTWAIN_DSM_DLL_NAME << std::endl;
    pDSMLibrary = LOADLIBRARY(kTWAIN_DSM_DIR kTWAIN_DSM_DLL_NAME);
    if (pDSMLibrary == NULL) {
        std::cout << "Error - Could not load DSM:" << kTWAIN_DSM_DIR << kTWAIN_DSM_DLL_NAME << std::endl;
#ifdef TWH_CMP_GNU
        std::cerr << "App - dlopen: " << dlerror() << std::endl;
#endif //TWH_CMP_GNU
        return rc;
    }

    // Windows 验证TWAINDSM.dll签名
#ifdef TWH_CMP_MSC
    WCHAR szPath[MAX_PATH];
    if(GetModuleFileNameW(pDSMLibrary, szPath, MAX_PATH)) {
    //    if(!VerifyEmbeddedSignature(szPath)) {
    //        // Only continue using the DSM from trusted distributor
    //        freeDSM();
    //        return rc;
    //    }
    }
#endif //TWH_CMP_MSC

    // load function
    dsmEntry = (DSMENTRYPROC) LOADFUNCTION(pDSMLibrary, "DSM_Entry");

#ifdef TWH_CMP_MSC // dlsym returning NULL is not an error on Unix
    if (dsmEntry == NULL) {
        std::cerr << "Error - Could not find DSM_Entry function in DSM:" << LIBRARY << std::endl;
        return rc;
    }
#endif //TWH_CMP_MSC

#ifdef TWH_CMP_GNU
    bool error;
    if(error = dlerror() != NULL) {
        std::cerr << "App - dlsym:" << error << std::endl;
        return rc;
    }
#endif //TWH_CMP_GNU

    state = 2;
    rc = TWRC_SUCCESS;
    return rc;
}

TW_UINT16 TwainSession::freeDSM() {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state == 2) {
        UNLOADLIBRARY(pDSMLibrary);
        pDSMLibrary = NULL;
        dsmEntry = NULL;
        rc = TWRC_SUCCESS;
        state = 1;
        std::cout << "* State 2 -> 1 *" << std::endl;
    } else {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
    }
    return rc;
}

TW_UINT16 TwainSession::entry(TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData, pTW_IDENTITY pDataSource) {
    TW_UINT16 rc = TWRC_FAILURE;
    status.ConditionCode = TWCC_SUCCESS;
    rc = dsmEntry(&identity, pDataSource, DG, DAT, MSG, pData);
    std::cout << "Triplet:"
              << convertDataGroupToString(DG)
              << " / "
              << convertDataArgTypeToString(DAT)
              << " / "
              << convertMessageToString(MSG)
              << std::endl;
    std::cout << "RC:" << convertReturnCodeToString(rc) << std::endl;
    if ((rc == TWRC_FAILURE) || (rc == TWRC_CHECKSTATUS)) {
        TW_UINT16 erc = dsmEntry(&identity, pDataSource, DG_CONTROL, DAT_STATUS, MSG_GET, (TW_MEMREF) &status);
        std::cout << "---------- ERROR -----------" << std::endl;
        std::cout << "RC:" << convertReturnCodeToString(erc) << std::endl;
        std::cout << "CC:" << convertConditionCodeToString(status.ConditionCode) << std::endl;
    }

    return rc;
}

TW_UINT16 TwainSession::openDSM() {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state != 2) {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
        std::cout << "You need to load the DSM first." << std::endl;
        return rc;
    }
    rc = entry(DG_CONTROL, DAT_PARENT, MSG_OPENDSM, (TW_MEMREF) &parent);
    if (rc == TWRC_SUCCESS && ((identity.SupportedGroups & DF_DSM2) == DF_DSM2)) {
        gDSMEntry.Size = sizeof(TW_ENTRYPOINT);
        rc = entry(DG_CONTROL, DAT_ENTRYPOINT, MSG_GET, (TW_MEMREF) &gDSMEntry);
    }
    if (rc == TWRC_SUCCESS) {
        state = 3;
    }
    return rc;
}

TW_UINT16 TwainSession::closeDSM() {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state == 3) {
        rc = entry(DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, (TW_MEMREF) &parent);
        if (rc == TWRC_SUCCESS) {
            parent = NULL;
        }
    } else {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
    }
    return rc;
}

TW_UINT16 TwainSession::getDefaultDS() {
    if (state < 3) {
        std::cout << "getDefaultDS :: You need to open the DSM first." << state << std::endl;
        return TWRC_FAILURE;
    }
    memset(&source, 0, sizeof(TW_IDENTITY));
    TW_UINT16 rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, (TW_MEMREF) &source);
    switch (rc) {
        case TWRC_SUCCESS:
            break;
        case TWRC_FAILURE:
            std::cerr << "Failed to get the data source info!" << std::endl;
    }
    return rc;
};

TW_UINT16 TwainSession::setDefaultDS(std::string name) {
    if (state < 3) {
        std::cout << "setDefaultDS :: You need to open the DSM first." << state << std::endl;
        return TWRC_FAILURE;
    } else if (state > 3) {
        std::cout << "A source has already been opened, please close it first." << std::endl;
        return TWRC_FAILURE;
    }
    TW_UINT16 rc = TWRC_FAILURE;

    for (auto &it: sources) {
        if (std::string(reinterpret_cast<char *>(it.ProductName)) == name) {
            pSource = &it;
            rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_SET, (TW_MEMREF) pSource);
            switch (rc) {
                case TWRC_SUCCESS:
                    break;
                case TWRC_FAILURE:
                    std::cerr << "Failed to get the data source info!" << std::endl;
                    break;
            }
        }
    }

    return rc;
};

TW_UINT16 TwainSession::getSources() {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state < 3) {
        std::cout << "getSources :: You need to open the DSM first." << state << std::endl;
        return rc;
    }
    
    // assert(true == sources.empty());
    if(true != sources.empty()){
        sources.clear();
    }

    TW_IDENTITY temp;
    rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_GETFIRST, (TW_MEMREF) &temp);

    if (rc == TWRC_SUCCESS) {
        sources.push_back(temp);
    } else if (rc == TWRC_ENDOFLIST) {
        std::cout << "TWRC_ENDOFLIST" << rc << std::endl;
    }

    while (rc == TWRC_SUCCESS) {
        rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_GETNEXT, (TW_MEMREF) &temp);
        if (rc == TWRC_SUCCESS) {
            sources.push_back(temp);
        } else if (rc == TWRC_ENDOFLIST) {
            std::cout << "while TWRC_ENDOFLIST" << rc << std::endl;
        }
    }

    return rc;
}

TW_UINT16 TwainSession::openDS() {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state != 3) {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
        std::cout << "OpenDS Failed" << std::endl;
    }
    rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, (TW_MEMREF) pSource);
    std::cout << "openDS:rc:::::" << rc << std::endl;
    if (rc == TWRC_SUCCESS) {
        state = 4;
    }
    return rc;
}

TW_UINT16 TwainSession::openDS(std::string name) {
    TW_UINT16 rc = TWRC_FAILURE;
    if (state != 3) {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
        std::cout << "OpenDS Failed" << std::endl;
    }

    for (auto &it: sources) {
        if (std::string(reinterpret_cast<char *>(it.ProductName)) == name) {
            pSource = &it;
            rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, (TW_MEMREF) pSource);
            switch (rc) {
                case TWRC_SUCCESS:
                    state = 4;
                    break;
                case TWRC_FAILURE:
                    std::cerr << "Failed to get the data source info!" << std::endl;
                    break;
            }
            return rc;
        }
    }
    TW_IDENTITY defaultDS;
    defaultDS.Id = 0;

    rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, (TW_MEMREF) &defaultDS);
    std::cout << "openDS:rc:::::" << rc << std::endl;
    if (rc == TWRC_SUCCESS) {
        state = 4;
    }
    return rc;
}

TW_UINT16 TwainSession::closeDS() {
    TW_UINT16 rc = TWRC_FAILURE;
    if(state != 4) {
        status.ConditionCode = TWCC_SEQERROR;
        rc = TWRC_FAILURE;
        std::cout << "CloseDS Failed" << std::endl;
    }
    rc = entry(DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, (TW_MEMREF) pSource);
    if(rc == TWRC_SUCCESS) {
        state = 3;
    }
    return rc;
}

TW_UINT16 TwainSession::getCap(TW_CAPABILITY &cap) {
    if (state < 4) {
        std::cout << "You need to open a data source first." << std::endl;
        return TWCC_SEQERROR;
    }
    if (cap.hContainer != NULL) {
        freeMemory(cap.hContainer);
        cap.hContainer = NULL;
    }

    cap.ConType = TWON_DONTCARE16;
    TW_UINT16 rc = entry(DG_CONTROL, DAT_CAPABILITY, MSG_GET, (TW_MEMREF) &cap, pSource);
    std::cout << "CAP_ORDER:" << convertCapToString(cap.Cap) << std::endl;
    std::cout << "CAP_TYPE :" << convertConTypeToString(cap.ConType) << std::endl;
    return rc;
}

TW_UINT16 TwainSession::getCurrentCap(TW_CAPABILITY &cap) {
     if (state < 4) {
        std::cout << "You need to open a data source first." << std::endl;
        return TWCC_SEQERROR;
    }
    if (cap.hContainer != NULL) {
        freeMemory(cap.hContainer);
        cap.hContainer = NULL;
    }

    cap.ConType = TWON_ONEVALUE;
    TW_UINT16 rc = entry(DG_CONTROL, DAT_CAPABILITY, MSG_GETCURRENT, (TW_MEMREF) &cap, pSource);
    std::cout << "CAP_ORDER:" << convertCapToString(cap.Cap) << std::endl;
    std::cout << "CAP_TYPE :" << convertConTypeToString(cap.ConType) << std::endl;
    return rc;
}

TW_UINT16 TwainSession::setCap(TW_UINT16 Cap, const int value, TW_UINT16 type) {
    TW_UINT16 rc = TWRC_FAILURE;
    TW_CAPABILITY cap;
    cap.Cap = Cap;
    cap.ConType = TWON_ONEVALUE;
    cap.hContainer = allocMemory(sizeof(TW_ONEVALUE));
    if (cap.hContainer == NULL) {
        return rc;
    }
    pTW_ONEVALUE pVal = (pTW_ONEVALUE) lockMemory(cap.hContainer);
    pVal->ItemType = type;
    switch (type) {
        case TWTY_INT8:
            *(TW_INT8 *) &pVal->Item = (TW_INT8) value;
            break;
        case TWTY_INT16:
            *(TW_INT16 *) &pVal->Item = (TW_INT16) value;
            break;
        case TWTY_INT32:
            *(TW_INT32 *) &pVal->Item = (TW_INT32) value;
            break;
        case TWTY_UINT8:
            *(TW_UINT8 *) &pVal->Item = (TW_UINT8) value;
            break;
        case TWTY_UINT16:
            *(TW_UINT16 *) &pVal->Item = (TW_UINT16) value;
            break;
        case TWTY_UINT32:
            memcpy(&pVal->Item, &value, sizeof(TW_UINT32));
            break;
        case TWTY_BOOL:
            memcpy(&pVal->Item, &value, sizeof(TW_BOOL));
            break;
    }

    rc = entry(DG_CONTROL, DAT_CAPABILITY, MSG_SET, (TW_MEMREF) &cap);

    return rc;
}

TW_UINT16 TwainSession::setCallback() {
    if (state != 4) {
        return TWRC_FAILURE;
    }
    TW_CALLBACK callback = {0};
    callback.RefCon = 0;
    callback.CallBackProc = (TW_MEMREF) dsmCallback;
    TW_UINT16 rc = entry(DG_CONTROL, DAT_CALLBACK, MSG_REGISTER_CALLBACK, (TW_MEMREF) &callback, &source);
    return rc;
}

TW_UINT16 TwainSession::enableDS() {
    if (state < 3) {
        status.ConditionCode = TWCC_SEQERROR;
        std::cout << "enableDS :: You need to open the DSM first." << state << std::endl;
        return TWRC_FAILURE;
    }
    if (state < 4) {
        status.ConditionCode = TWCC_SEQERROR;
        std::cout << "enableDS :: You need to open the DS first." << state << std::endl;
        return TWRC_FAILURE;
    }
    if (state == 5) {
        status.ConditionCode = TWCC_SEQERROR;
        std::cout << "enableDS :: You have enabled the DS." << state << std::endl;
        return TWRC_FAILURE;
    }
    std::cout << "Before message:" << message << std::endl;

    ui.ShowUI = false;
    ui.ModalUI = false;
    ui.hParent = NULL;
    TW_UINT16 rc = entry(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, (TW_MEMREF) &ui, pSource);
    if (rc == TWRC_SUCCESS) {
        state = 5;
    }
    std::cout << "After message:" << message << std::endl;
    // todo 轮训/callback取到MSG
    if (message == MSG_XFERREADY) {
        state = 6;
    }
    return rc;
};

TW_UINT16 TwainSession::disableDS() {
    if (state < 5) {
        std::cout << "You need to enable the data source first" << std::endl;
        return TWRC_FAILURE;
    }
    TW_UINT16 rc = entry(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, (TW_MEMREF) &ui,pSource);
    if (rc == TWRC_SUCCESS) {
        state = 4;
    }
    return rc;
}

TW_UINT16 TwainSession::getImageInfo() {
    memset(&imageInfo, 0, sizeof(imageInfo));
    std::cout << "Getting the image info..." << std::endl;
    TW_UINT16 rc = entry(DG_IMAGE, DAT_IMAGEINFO, MSG_GET, (TW_MEMREF) &imageInfo, pSource);
    if (rc == TWRC_SUCCESS) {
        std::cout << "Image information - ImageWidth:" << imageInfo.ImageWidth << std::endl;
        std::cout << "Image information - ImageLength:" << imageInfo.ImageLength << std::endl;
        std::cout << "Image information - BitsPerSample:" << imageInfo.BitsPerSample << std::endl;
        std::cout << "Image information - BitsPerPixel:" << imageInfo.BitsPerPixel << std::endl;
        std::cout << "Image information - PixelType:" << imageInfo.PixelType << std::endl;
        std::cout << "Image information - Planar:" << imageInfo.Planar << std::endl;
        std::cout << "Image information - Compression:" << imageInfo.Compression << std::endl;
    }
    return rc;
}

TW_UINT16 TwainSession::scan(TW_UINT32 mech, std::string fileName) {
    if(state != 6) {
        std::cout << "A scan cannot be initiated unless we are in state 6" << std::endl;
        return TWRC_FAILURE;
    }

    TW_UINT16 rc = getImageInfo();
    if (TWRC_SUCCESS != rc) {
        return rc;
    };

    switch (mech) {
        case TWSX_NATIVE: {
            transferNative();
            break;
        }
        case TWSX_FILE: {
            TW_CAPABILITY cap;
            cap.Cap = ICAP_IMAGEFILEFORMAT;
            cap.hContainer = 0;
            TW_UINT16 rc = getCurrentCap(cap);
            if( rc == TWRC_SUCCESS){
                pTW_ONEVALUE pEnum = (pTW_ONEVALUE)lockMemory(cap.hContainer);
                std::cout << pEnum->Item << std::endl;
                transferFile(pEnum->Item, fileName);
            }
            break;
        }
        case TWSX_MEMORY: {
            transferMemory();
            break;
        }
        case TWSX_MEMFILE: {
            transferMemory();
            break;
        }
    }
    return TWRC_SUCCESS;
}

void TwainSession::transferNative() {
    std::cout << "starting a TWSX_NATIVE transfer..." << std::endl;
    bool bPendingXfers = true;
    TW_UINT16 rc = TWRC_FAILURE;

    while (bPendingXfers) {
        TW_MEMREF hImg = NULL;

        rc = entry(DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, (TW_MEMREF) &hImg, pSource);

        switch (rc) {
            case TWRC_XFERDONE:
                // save hImg

                // checking to see if there are more images to transfer
                TW_PENDINGXFERS pendXfers;
                memset(&pendXfers, 0, sizeof(pendXfers));
                rc = entry(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, (TW_MEMREF) &pendXfers, pSource);

                if (rc == TWRC_SUCCESS) {
                    std::cout << "Remaining images to transfer" << pendXfers.Count << std::endl;
                    if (pendXfers.Count == 0) {
                        bPendingXfers = false;
                    }
                } else {
                    bPendingXfers = false;
                    std::cerr << "Failed to properly end the transfer" << std::endl;
                }


                break;
            case TWRC_CANCEL:
                bPendingXfers = false;
                std::cout << "Cancel transfer image" << std::endl;
                break;
            case TWRC_FAILURE:
                bPendingXfers = false;
                std::cout << "Failed transfer image" << std::endl;
                break;
        }
    }
    state = 5;
    return;
}

void TwainSession::transferFile(TW_UINT16 fileFormat, std::string fileName) {
    std::cout << "starting a TWSX_FILE transfer..." << std::endl;
    std::string ext = convertImageFileFormatToExt(fileFormat);
    std::cout << ext << std::endl;

    bool bPendingXfers = true;
    TW_UINT16 rc = TWRC_SUCCESS;
    TW_SETUPFILEXFER fileXfer;
    memset(&fileXfer, 0, sizeof(fileXfer));
    std::cout << "Test::" << fileXfer.Format << std::endl;
    fileXfer.Format = fileFormat;
    strcpy(fileXfer.FileName, (fileName + ext).c_str());
    
//    TW_STR255 str;
//    snprintf((char *)fileXfer.FileName, str);
    // fileXfer.FileName[0] = 'i';
    // fileXfer.FileName[1] = 'm';
    // fileXfer.FileName[2] = '.';
    // fileXfer.FileName[3] = 't';
    // fileXfer.FileName[4] = 'i';
    // fileXfer.FileName[5] = 'f';
    // fileXfer.FileName[6] = 'f';

    while (bPendingXfers) {
        rc = entry(DG_CONTROL, DAT_SETUPFILEXFER, MSG_SET, (TW_MEMREF) &fileXfer, pSource);
        if (rc != TWRC_SUCCESS) {
            std::cerr << "Error while trying to setup the file transfer" << std::endl;
            break;
        }

        rc = entry(DG_IMAGE, DAT_IMAGEFILEXFER, MSG_GET, NULL, pSource);
        if (rc == TWRC_XFERDONE) {
            // rc = entry(DG_CONTROL, DAT_SETUPFILEXFER, MSG_GETDEFAULT, (TW_MEMREF) &fileXfer, pSource);
            std::cout << "file saved..." << fileXfer.FileName << std::endl;
            std::cout << "Checking to see if there are more images to transfer..." << std::endl;
            TW_PENDINGXFERS pendXfers;
            memset(&pendXfers, 0, sizeof(pendXfers));
            rc = entry(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, (TW_MEMREF) &pendXfers, pSource);

            if (rc == TWRC_SUCCESS) {
                if (pendXfers.Count == 0) {
                    bPendingXfers = false;
                }
            } else {
                std::cerr << "Failed to properly end the transfer" << std::endl;
                bPendingXfers = false;
            }
        } else if (rc == TWRC_CANCEL) {
            std::cerr << "Cancel to transfer image" << std::endl;
            break;
        } else if (rc == TWRC_FAILURE) {
            std::cerr << "Failed to transfer image" << std::endl;
            break;
        }
    }
    state = 5;
    return;
}

void TwainSession::transferMemory() {
    std::cout << "starting a TWSX_MEMORY transfer..." << std::endl;
    bool bPendingXfers = true;
    TW_SETUPMEMXFER sourceBufferSizes;
    TW_UINT16 rc = TWRC_FAILURE;
    while (bPendingXfers) {
        rc = entry(DG_CONTROL, DAT_SETUPMEMXFER, MSG_GET, (TW_MEMREF) &sourceBufferSizes);
        if (rc != TWRC_SUCCESS) {
            std::cerr << "Error while trying to get the buffer sizes from the source" << std::endl;
            break;
        }

        TW_IMAGEMEMXFER memXferBufferTemplate;
        memXferBufferTemplate.Compression = TWON_DONTCARE16;
        memXferBufferTemplate.BytesPerRow = TWON_DONTCARE32;
        memXferBufferTemplate.Columns = TWON_DONTCARE32;
        memXferBufferTemplate.Rows = TWON_DONTCARE32;
        memXferBufferTemplate.XOffset = TWON_DONTCARE32;
        memXferBufferTemplate.YOffset = TWON_DONTCARE32;
        memXferBufferTemplate.BytesWritten = TWON_DONTCARE32;
        memXferBufferTemplate.Memory.Flags = TWMF_APPOWNS | TWMF_POINTER;
        memXferBufferTemplate.Memory.Length = sourceBufferSizes.Preferred;

        TW_HANDLE hMem = (TW_HANDLE) allocMemory(sourceBufferSizes.Preferred);
        if (hMem == NULL) {
            std::cerr << "Error allocating memory" << std::endl;
            break;
        }
        memXferBufferTemplate.Memory.TheMem = (TW_MEMREF) lockMemory(hMem);
        TW_IMAGEMEMXFER memXferBuf;
        bool bScanStarted = false;
        int nBytePerRow = (((imageInfo.ImageWidth * imageInfo.BitsPerPixel) + 7) / 8);

        std::cout << "starting the memory transfer..." << std::endl;

        while (true) {
            memcpy(&memXferBuf, &memXferBufferTemplate, sizeof(memXferBufferTemplate));
            memset(memXferBuf.Memory.TheMem, 0, memXferBuf.Memory.Length);

            rc = entry(DG_IMAGE, DAT_IMAGEFILEXFER, MSG_GET, (TW_MEMREF) &(memXferBuf));
            if (rc == TWRC_SUCCESS || rc == TWRC_XFERDONE) {

            } else if (rc == TWRC_CANCEL) {
                std::cerr << "Canceled transfer while trying to get strip of data from the source" << std::endl;
                break;
            } else if (rc == TWRC_FAILURE) {
                std::cerr << "Error while trying to get strip of data from the source" << std::endl;
                break;
            }
        }
    }
    state = 5;
    return;
}

bool TwainSession::parseCapability(TW_CAPABILITY *pCap, TW_UINT32 &val) {
    bool result = false;
    if (pCap->hContainer == NULL) {
        return result;
    } else if (pCap->ConType == TWON_ENUMERATION) {
        pTW_ENUMERATION pCapPT = (pTW_ENUMERATION) lockMemory(pCap->hContainer);
        switch (pCapPT->ItemType) {
            case TWTY_INT32:
                val = (TW_INT32) ((pTW_INT32) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_UINT32:
                val = (TW_INT32) ((pTW_UINT32) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_INT16:
                val = (TW_INT32) ((pTW_INT16) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_UINT16:
                val = (TW_INT32) ((pTW_UINT16) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_INT8:
                val = (TW_INT32) ((pTW_INT8) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_UINT8:
                val = (TW_INT32) ((pTW_UINT8) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
            case TWTY_BOOL:
                val = (TW_INT32) ((pTW_BOOL) (&pCapPT->ItemList))[pCapPT->CurrentIndex];
                result = true;
                break;
        }
        unlockMemory(pCap->hContainer);
    } else if (pCap->ConType == TWON_ONEVALUE) {
        pTW_ONEVALUE pCapPT = (pTW_ONEVALUE) lockMemory(pCap->hContainer);
        if (pCapPT->ItemType < TWTY_FIX32) {
            val = pCapPT->Item;
            result = true;
        }
        unlockMemory(pCap->hContainer);
    } else if (pCap->ConType == TWON_RANGE) {
        pTW_RANGE pCapPT = (pTW_RANGE) lockMemory(pCap->hContainer);
        if (pCapPT->ItemType < TWTY_FIX32) {
            val = pCapPT->CurrentValue;
            result = true;
        }
        unlockMemory(pCap->hContainer);
    }
    return result;
}

TW_HANDLE TwainSession::allocMemory(TW_UINT32 _size) {
    if (gDSMEntry.DSM_MemAllocate) {
        return gDSMEntry.DSM_MemAllocate(_size);
    }
#ifdef TWH_CMP_MSC
    return ::GlobalAlloc(GPTR, _size);
#else
    return 0;
#endif
}

void TwainSession::freeMemory(TW_HANDLE _hMemory) {
    if (gDSMEntry.DSM_MemFree) {
        return gDSMEntry.DSM_MemFree(_hMemory);
    }
#ifdef TWH_CMP_MSC
    ::GlobalFree(_hMemory);
#endif
    return;
}

TW_MEMREF TwainSession::lockMemory(TW_HANDLE _hMemory) {
    if (gDSMEntry.DSM_MemLock) {
        return gDSMEntry.DSM_MemLock(_hMemory);
    }

#ifdef TWH_CMP_MSC
    return (TW_MEMREF)::GlobalLock(_hMemory);
#else
    return 0;
#endif
}

void TwainSession::unlockMemory(TW_HANDLE _hMemory) {
    if (gDSMEntry.DSM_MemUnlock) {
        return gDSMEntry.DSM_MemUnlock(_hMemory);
    }
#ifdef TWH_CMP_MSC
    ::GlobalUnlock(_hMemory);
#endif
    return;
}

const std::string TwainSession::convertReturnCodeToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case TWRC_SUCCESS:
            text = "TWRC_SUCCESS";
            break;
        case TWRC_FAILURE:
            text = "TWRC_FAILURE";
            break;
        case TWRC_CHECKSTATUS:
            text = "TWRC_CHECKSTATUS";
            break;
        case TWRC_CANCEL:
            text = "TWRC_CANCEL";
            break;
        case TWRC_DSEVENT:
            text = "TWRC_DSEVENT";
            break;
        case TWRC_NOTDSEVENT:
            text = "TWRC_NOTDSEVENT";
            break;
        case TWRC_XFERDONE:
            text = "TWRC_XFERDONE";
            break;
        case TWRC_ENDOFLIST:
            text = "TWRC_ENDOFLIST";
            break;
        case TWRC_INFONOTSUPPORTED:
            text = "TWRC_INFONOTSUPPORTED";
            break;
        case TWRC_DATANOTAVAILABLE:
            text = "TWRC_DATANOTAVAILABLE";
            break;
        case TWRC_BUSY:
            text = "TWRC_BUSY";
            break;
        case TWRC_SCANNERLOCKED:
            text = "TWRC_SCANNERLOCKED";
            break;
        default:
            text = "Unknown";
    }
    return text;
};

const std::string TwainSession::convertConditionCodeToString(TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case TWCC_SUCCESS:
            text = "TWCC_SUCCESS";
            break;
        case TWCC_BUMMER:
            text = "TWCC_BUMMER";
            break;
        case TWCC_LOWMEMORY:
            text = "TWCC_LOWMEMORY";
            break;
        case TWCC_NODS:
            text = "TWCC_NODS";
            break;
        case TWCC_MAXCONNECTIONS:
            text = "TWCC_MAXCONNECTIONS";
            break;
        case TWCC_OPERATIONERROR:
            text = "TWCC_OPERATIONERROR";
            break;
        case TWCC_BADCAP:
            text = "TWCC_BADCAP";
            break;
        case TWCC_BADPROTOCOL:
            text = "TWCC_BADPROTOCOL";
            break;
        case TWCC_BADVALUE:
            text = "TWCC_BADVALUE";
            break;
        case TWCC_SEQERROR:
            text = "TWCC_SEQERROR";
            break;
        case TWCC_BADDEST:
            text = "TWCC_BADDEST";
            break;
        case TWCC_CAPUNSUPPORTED:
            text = "TWCC_CAPUNSUPPORTED";
            break;
        case TWCC_CAPBADOPERATION:
            text = "TWCC_CAPBADOPERATION";
            break;
        case TWCC_CAPSEQERROR:
            text = "TWCC_CAPSEQERROR";
            break;
        case TWCC_DENIED:
            text = "TWCC_DENIED";
            break;
        case TWCC_FILEEXISTS:
            text = "TWCC_FILEEXISTS";
            break;
        case TWCC_FILENOTFOUND:
            text = "TWCC_FILENOTFOUND";
            break;
        case TWCC_NOTEMPTY:
            text = "TWCC_NOTEMPTY";
            break;
        case TWCC_PAPERJAM:
            text = "TWCC_PAPERJAM";
            break;
        case TWCC_PAPERDOUBLEFEED:
            text = "TWCC_PAPERDOUBLEFEED";
            break;
        case TWCC_FILEWRITEERROR:
            text = "TWCC_FILEWRITEERROR";
            break;
        case TWCC_CHECKDEVICEONLINE:
            text = "TWCC_CHECKDEVICEONLINE";
            break;
        case TWCC_INTERLOCK:
            text = "TWCC_INTERLOCK";
            break;
        case TWCC_DAMAGEDCORNER:
            text = "TWCC_DAMAGEDCORNER";
            break;
        case TWCC_FOCUSERROR:
            text = "TWCC_FOCUSERROR";
            break;
        case TWCC_DOCTOOLIGHT:
            text = "TWCC_DOCTOOLIGHT";
            break;
        case TWCC_DOCTOODARK:
            text = "TWCC_DOCTOODARK";
            break;
        case TWCC_NOMEDIA:
            text = "TWCC_NOMEDIA";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertDataGroupToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case DG_CONTROL:
            text = "DG_CONTROL";
            break;
        case DG_IMAGE:
            text = "DG_IMAGE";
            break;
        case DG_AUDIO:
            text = "DG_AUDIO";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertDataArgTypeToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case DAT_NULL:
            text = "DAT_NULL";
            break;
        case DAT_CUSTOMBASE:
            text = "DAT_CUSTOMBASE";
            break;
        case DAT_CAPABILITY:
            text = "DAT_CAPABILITY";
            break;
        case DAT_EVENT:
            text = "DAT_EVENT";
            break;
        case DAT_IDENTITY:
            text = "DAT_IDENTITY";
            break;
        case DAT_PARENT:
            text = "DAT_PARENT";
            break;
        case DAT_PENDINGXFERS:
            text = "DAT_PENDINGXFERS";
            break;
        case DAT_SETUPMEMXFER:
            text = "DAT_SETUPMEMXFER";
            break;
        case DAT_SETUPFILEXFER:
            text = "DAT_SETUPFILEXFER";
            break;
        case DAT_STATUS:
            text = "DAT_STATUS";
            break;
        case DAT_USERINTERFACE:
            text = "DAT_USERINTERFACE";
            break;
        case DAT_XFERGROUP:
            text = "DAT_XFERGROUP";
            break;
        case DAT_CUSTOMDSDATA:
            text = "DAT_CUSTOMDSDATA";
            break;
        case DAT_DEVICEEVENT:
            text = "DAT_DEVICEEVENT";
            break;
        case DAT_FILESYSTEM:
            text = "DAT_FILESYSTEM";
            break;
        case DAT_PASSTHRU:
            text = "DAT_PASSTHRU";
            break;
        case DAT_CALLBACK:
            text = "DAT_CALLBACK";
            break;
        case DAT_STATUSUTF8:
            text = "DAT_STATUSUTF8";
            break;
        case DAT_CALLBACK2:
            text = "DAT_CALLBACK2";
            break;
        case DAT_METRICS:
            text = "DAT_METRICS";
            break;
        case DAT_TWAINDIRECT:
            text = "DAT_TWAINDIRECT";
            break;
        case DAT_IMAGEINFO:
            text = "DAT_IMAGEINFO";
            break;
        case DAT_IMAGELAYOUT:
            text = "DAT_IMAGELAYOUT";
            break;
        case DAT_IMAGEMEMXFER:
            text = "DAT_IMAGEMEMXFER";
            break;
        case DAT_IMAGENATIVEXFER:
            text = "DAT_IMAGENATIVEXFER";
            break;
        case DAT_IMAGEFILEXFER:
            text = "DAT_IMAGEFILEXFER";
            break;
        case DAT_CIECOLOR:
            text = "DAT_CIECOLOR";
            break;
        case DAT_GRAYRESPONSE:
            text = "DAT_GRAYRESPONSE";
            break;
        case DAT_RGBRESPONSE:
            text = "DAT_RGBRESPONSE";
            break;
        case DAT_JPEGCOMPRESSION:
            text = "DAT_JPEGCOMPRESSION";
            break;
        case DAT_PALETTE8:
            text = "DAT_PALETTE8";
            break;
        case DAT_EXTIMAGEINFO:
            text = "DAT_EXTIMAGEINFO";
            break;
        case DAT_FILTER:
            text = "DAT_FILTER";
            break;
        case DAT_AUDIOFILEXFER:
            text = "DAT_AUDIOFILEXFER";
            break;
        case DAT_AUDIOINFO:
            text = "DAT_AUDIOINFO";
            break;
        case DAT_AUDIONATIVEXFER:
            text = "DAT_AUDIONATIVEXFER";
            break;
        case DAT_ICCPROFILE:
            text = "DAT_ICCPROFILE";
            break;
        case DAT_IMAGEMEMFILEXFER:
            text = "DAT_IMAGEMEMFILEXFER";
            break;
        case DAT_ENTRYPOINT:
            text = "DAT_ENTRYPOINT";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertMessageToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case MSG_NULL:
            text = "MSG_NULL";
            break;
        case MSG_CUSTOMBASE:
            text = "MSG_CUSTOMBASE";
            break;
        case MSG_GET:
            text = "MSG_GET";
            break;
        case MSG_GETCURRENT:
            text = "MSG_GETCURRENT";
            break;
        case MSG_GETDEFAULT:
            text = "MSG_GETDEFAULT";
            break;
        case MSG_GETFIRST:
            text = "MSG_GETFIRST";
            break;
        case MSG_GETNEXT:
            text = "MSG_GETNEXT";
            break;
        case MSG_SET:
            text = "MSG_SET";
            break;
        case MSG_RESET:
            text = "MSG_RESET";
            break;
        case MSG_QUERYSUPPORT:
            text = "MSG_QUERYSUPPORT";
            break;
        case MSG_GETHELP:
            text = "MSG_GETHELP";
            break;
        case MSG_GETLABEL:
            text = "MSG_GETLABEL";
            break;
        case MSG_GETLABELENUM:
            text = "MSG_GETLABELENUM";
            break;
        case MSG_SETCONSTRAINT:
            text = "MSG_SETCONSTRAINT";
            break;
        case MSG_XFERREADY:
            text = "MSG_XFERREADY";
            break;
        case MSG_CLOSEDSREQ:
            text = "MSG_CLOSEDSREQ";
            break;
        case MSG_CLOSEDSOK:
            text = "MSG_CLOSEDSOK";
            break;
        case MSG_DEVICEEVENT:
            text = "MSG_DEVICEEVENT";
            break;
        case MSG_OPENDSM:
            text = "MSG_OPENDSM";
            break;
        case MSG_CLOSEDSM:
            text = "MSG_CLOSEDSM";
            break;
        case MSG_OPENDS:
            text = "MSG_OPENDS";
            break;
        case MSG_CLOSEDS:
            text = "MSG_CLOSEDS";
            break;
        case MSG_USERSELECT:
            text = "MSG_USERSELECT";
            break;
        case MSG_DISABLEDS:
            text = "MSG_DISABLEDS";
            break;
        case MSG_ENABLEDS:
            text = "MSG_ENABLEDS";
            break;
        case MSG_ENABLEDSUIONLY:
            text = "MSG_ENABLEDSUIONLY";
            break;
        case MSG_PROCESSEVENT:
            text = "MSG_PROCESSEVENT";
            break;
        case MSG_ENDXFER:
            text = "MSG_ENDXFER";
            break;
        case MSG_STOPFEEDER:
            text = "MSG_STOPFEEDER";
            break;
        case MSG_CHANGEDIRECTORY:
            text = "MSG_CHANGEDIRECTORY";
            break;
        case MSG_CREATEDIRECTORY:
            text = "MSG_CREATEDIRECTORY";
            break;
        case MSG_DELETE:
            text = "MSG_DELETE";
            break;
        case MSG_FORMATMEDIA:
            text = "MSG_FORMATMEDIA";
            break;
        case MSG_GETCLOSE:
            text = "MSG_GETCLOSE";
            break;
        case MSG_GETFIRSTFILE:
            text = "MSG_GETFIRSTFILE";
            break;
        case MSG_GETINFO:
            text = "MSG_GETINFO";
            break;
        case MSG_GETNEXTFILE:
            text = "MSG_GETNEXTFILE";
            break;
        case MSG_RENAME:
            text = "MSG_RENAME";
            break;
        case MSG_COPY:
            text = "MSG_COPY";
            break;
        case MSG_AUTOMATICCAPTUREDIRECTORY:
            text = "MSG_AUTOMATICCAPTUREDIRECTORY";
            break;
        case MSG_PASSTHRU:
            text = "MSG_PASSTHRU";
            break;
        case MSG_REGISTER_CALLBACK:
            text = "MSG_REGISTER_CALLBACK";
            break;
        case MSG_RESETALL:
            text = "MSG_RESETALL";
            break;
        case MSG_SETTASK:
            text = "MSG_SETTASK";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertCapToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case CAP_CUSTOMBASE:
            text = "CAP_CUSTOMBASE";
            break;
        case CAP_XFERCOUNT:
            text = "CAP_XFERCOUNT";
            break;
        case ICAP_COMPRESSION:
            text = "ICAP_COMPRESSION";
            break;
        case ICAP_PIXELTYPE:
            text = "ICAP_PIXELTYPE";
            break;
        case ICAP_UNITS:
            text = "ICAP_UNITS";
            break;
        case ICAP_XFERMECH:
            text = "ICAP_XFERMECH";
            break;
        case CAP_AUTHOR:
            text = "CAP_AUTHOR";
            break;
        case CAP_CAPTION:
            text = "CAP_CAPTION";
            break;
        case CAP_FEEDERENABLED:
            text = "CAP_FEEDERENABLED";
            break;
        case CAP_FEEDERLOADED:
            text = "CAP_FEEDERLOADED";
            break;
        case CAP_TIMEDATE:
            text = "CAP_TIMEDATE";
            break;
        case CAP_SUPPORTEDCAPS:
            text = "CAP_SUPPORTEDCAPS";
            break;
        case CAP_EXTENDEDCAPS:
            text = "CAP_EXTENDEDCAPS";
            break;
        case CAP_AUTOFEED:
            text = "CAP_AUTOFEED";
            break;
        case CAP_CLEARPAGE:
            text = "CAP_CLEARPAGE";
            break;
        case CAP_FEEDPAGE:
            text = "CAP_FEEDPAGE";
            break;
        case CAP_REWINDPAGE:
            text = "CAP_REWINDPAGE";
            break;
        case CAP_INDICATORS:
            text = "CAP_INDICATORS";
            break;
        case CAP_PAPERDETECTABLE:
            text = "CAP_PAPERDETECTABLE";
            break;
        case CAP_UICONTROLLABLE:
            text = "CAP_UICONTROLLABLE";
            break;
        case CAP_DEVICEONLINE:
            text = "CAP_DEVICEONLINE";
            break;
        case CAP_AUTOSCAN:
            text = "CAP_AUTOSCAN";
            break;
        case CAP_THUMBNAILSENABLED:
            text = "CAP_THUMBNAILSENABLED";
            break;
        case CAP_DUPLEX:
            text = "CAP_DUPLEX";
            break;
        case CAP_DUPLEXENABLED:
            text = "CAP_DUPLEXENABLED";
            break;
        case CAP_ENABLEDSUIONLY:
            text = "CAP_ENABLEDSUIONLY";
            break;
        case CAP_CUSTOMDSDATA:
            text = "CAP_CUSTOMDSDATA";
            break;
        case CAP_ENDORSER:
            text = "CAP_ENDORSER";
            break;
        case CAP_JOBCONTROL:
            text = "CAP_JOBCONTROL";
            break;
        case CAP_ALARMS:
            text = "CAP_ALARMS";
            break;
        case CAP_ALARMVOLUME:
            text = "CAP_ALARMVOLUME";
            break;
        case CAP_AUTOMATICCAPTURE:
            text = "CAP_AUTOMATICCAPTURE";
            break;
        case CAP_TIMEBEFOREFIRSTCAPTURE:
            text = "CAP_TIMEBEFOREFIRSTCAPTURE";
            break;
        case CAP_TIMEBETWEENCAPTURES:
            text = "CAP_TIMEBETWEENCAPTURES";
            break;
        case CAP_MAXBATCHBUFFERS:
            text = "CAP_MAXBATCHBUFFERS";
            break;
        case CAP_DEVICETIMEDATE:
            text = "CAP_DEVICETIMEDATE";
            break;
        case CAP_POWERSUPPLY:
            text = "CAP_POWERSUPPLY";
            break;
        case CAP_CAMERAPREVIEWUI:
            text = "CAP_CAMERAPREVIEWUI";
            break;
        case CAP_DEVICEEVENT:
            text = "CAP_DEVICEEVENT";
            break;
        case CAP_SERIALNUMBER:
            text = "CAP_SERIALNUMBER";
            break;
        case CAP_PRINTER:
            text = "CAP_PRINTER";
            break;
        case CAP_PRINTERENABLED:
            text = "CAP_PRINTERENABLED";
            break;
        case CAP_PRINTERINDEX:
            text = "CAP_PRINTERINDEX";
            break;
        case CAP_PRINTERMODE:
            text = "CAP_PRINTERMODE";
            break;
        case CAP_PRINTERSTRING:
            text = "CAP_PRINTERSTRING";
            break;
        case CAP_PRINTERSUFFIX:
            text = "CAP_PRINTERSUFFIX";
            break;
        case CAP_LANGUAGE:
            text = "CAP_LANGUAGE";
            break;
        case CAP_FEEDERALIGNMENT:
            text = "CAP_FEEDERALIGNMENT";
            break;
        case CAP_FEEDERORDER:
            text = "CAP_FEEDERORDER";
            break;
        case CAP_REACQUIREALLOWED:
            text = "CAP_REACQUIREALLOWED";
            break;
        case CAP_BATTERYMINUTES:
            text = "CAP_BATTERYMINUTES";
            break;
        case CAP_BATTERYPERCENTAGE:
            text = "CAP_BATTERYPERCENTAGE";
            break;
        case CAP_CAMERASIDE:
            text = "CAP_CAMERASIDE";
            break;
        case CAP_SEGMENTED:
            text = "CAP_SEGMENTED";
            break;
        case CAP_CAMERAENABLED:
            text = "CAP_CAMERAENABLED";
            break;
        case CAP_CAMERAORDER:
            text = "CAP_CAMERAORDER";
            break;
        case CAP_MICRENABLED:
            text = "CAP_MICRENABLED";
            break;
        case CAP_FEEDERPREP:
            text = "CAP_FEEDERPREP";
            break;
        case CAP_FEEDERPOCKET:
            text = "CAP_FEEDERPOCKET";
            break;
        case CAP_AUTOMATICSENSEMEDIUM:
            text = "CAP_AUTOMATICSENSEMEDIUM";
            break;
        case CAP_CUSTOMINTERFACEGUID:
            text = "CAP_CUSTOMINTERFACEGUID";
            break;
        case CAP_SUPPORTEDCAPSSEGMENTUNIQUE:
            text = "CAP_SUPPORTEDCAPSSEGMENTUNIQUE";
            break;
        case CAP_SUPPORTEDDATS:
            text = "CAP_SUPPORTEDDATS";
            break;
        case CAP_DOUBLEFEEDDETECTION:
            text = "CAP_DOUBLEFEEDDETECTION";
            break;
        case CAP_DOUBLEFEEDDETECTIONLENGTH:
            text = "CAP_DOUBLEFEEDDETECTIONLENGTH";
            break;
        case CAP_DOUBLEFEEDDETECTIONSENSITIVITY:
            text = "CAP_DOUBLEFEEDDETECTIONSENSITIVITY";
            break;
        case CAP_DOUBLEFEEDDETECTIONRESPONSE:
            text = "CAP_DOUBLEFEEDDETECTIONRESPONSE";
            break;
        case CAP_PAPERHANDLING:
            text = "CAP_PAPERHANDLING";
            break;
        case CAP_INDICATORSMODE:
            text = "CAP_INDICATORSMODE";
            break;
        case CAP_PRINTERVERTICALOFFSET:
            text = "CAP_PRINTERVERTICALOFFSET";
            break;
        case CAP_POWERSAVETIME:
            text = "CAP_POWERSAVETIME";
            break;
        case CAP_PRINTERCHARROTATION:
            text = "CAP_PRINTERCHARROTATION";
            break;
        case CAP_PRINTERFONTSTYLE:
            text = "CAP_PRINTERFONTSTYLE";
            break;
        case CAP_PRINTERINDEXLEADCHAR:
            text = "CAP_PRINTERINDEXLEADCHAR";
            break;
        case CAP_PRINTERINDEXMAXVALUE:
            text = "CAP_PRINTERINDEXMAXVALUE";
            break;
        case CAP_PRINTERINDEXNUMDIGITS:
            text = "CAP_PRINTERINDEXNUMDIGITS";
            break;
        case CAP_PRINTERINDEXSTEP:
            text = "CAP_PRINTERINDEXSTEP";
            break;
        case CAP_PRINTERINDEXTRIGGER:
            text = "CAP_PRINTERINDEXTRIGGER";
            break;
        case CAP_PRINTERSTRINGPREVIEW:
            text = "CAP_PRINTERSTRINGPREVIEW";
            break;
        case CAP_SHEETCOUNT:
            text = "CAP_SHEETCOUNT";
            break;
        case ICAP_AUTOBRIGHT:
            text = "ICAP_AUTOBRIGHT";
            break;
        case ICAP_BRIGHTNESS:
            text = "ICAP_BRIGHTNESS";
            break;
        case ICAP_CONTRAST:
            text = "ICAP_CONTRAST";
            break;
        case ICAP_CUSTHALFTONE:
            text = "ICAP_CUSTHALFTONE";
            break;
        case ICAP_EXPOSURETIME:
            text = "ICAP_EXPOSURETIME";
            break;
        case ICAP_FILTER:
            text = "ICAP_FILTER";
            break;
        case ICAP_FLASHUSED:
            text = "ICAP_FLASHUSED";
            break;
        case ICAP_GAMMA:
            text = "ICAP_GAMMA";
            break;
        case ICAP_HALFTONES:
            text = "ICAP_HALFTONES";
            break;
        case ICAP_HIGHLIGHT:
            text = "ICAP_HIGHLIGHT";
            break;
        case ICAP_IMAGEFILEFORMAT:
            text = "ICAP_IMAGEFILEFORMAT";
            break;
        case ICAP_LAMPSTATE:
            text = "ICAP_LAMPSTATE";
            break;
        case ICAP_LIGHTSOURCE:
            text = "ICAP_LIGHTSOURCE";
            break;
        case ICAP_ORIENTATION:
            text = "ICAP_ORIENTATION";
            break;
        case ICAP_PHYSICALWIDTH:
            text = "ICAP_PHYSICALWIDTH";
            break;
        case ICAP_PHYSICALHEIGHT:
            text = "ICAP_PHYSICALHEIGHT";
            break;
        case ICAP_SHADOW:
            text = "ICAP_SHADOW";
            break;
        case ICAP_FRAMES:
            text = "ICAP_FRAMES";
            break;
        case ICAP_XNATIVERESOLUTION:
            text = "ICAP_XNATIVERESOLUTION";
            break;
        case ICAP_YNATIVERESOLUTION:
            text = "ICAP_YNATIVERESOLUTION";
            break;
        case ICAP_XRESOLUTION:
            text = "ICAP_XRESOLUTION";
            break;
        case ICAP_YRESOLUTION:
            text = "ICAP_YRESOLUTION";
            break;
        case ICAP_MAXFRAMES:
            text = "ICAP_MAXFRAMES";
            break;
        case ICAP_TILES:
            text = "ICAP_TILES";
            break;
        case ICAP_BITORDER:
            text = "ICAP_BITORDER";
            break;
        case ICAP_CCITTKFACTOR:
            text = "ICAP_CCITTKFACTOR";
            break;
        case ICAP_LIGHTPATH:
            text = "ICAP_LIGHTPATH";
            break;
        case ICAP_PIXELFLAVOR:
            text = "ICAP_PIXELFLAVOR";
            break;
        case ICAP_PLANARCHUNKY:
            text = "ICAP_PLANARCHUNKY";
            break;
        case ICAP_ROTATION:
            text = "ICAP_ROTATION";
            break;
        case ICAP_SUPPORTEDSIZES:
            text = "ICAP_SUPPORTEDSIZES";
            break;
        case ICAP_THRESHOLD:
            text = "ICAP_THRESHOLD";
            break;
        case ICAP_XSCALING:
            text = "ICAP_XSCALING";
            break;
        case ICAP_YSCALING:
            text = "ICAP_YSCALING";
            break;
        case ICAP_BITORDERCODES:
            text = "ICAP_BITORDERCODES";
            break;
        case ICAP_PIXELFLAVORCODES:
            text = "ICAP_PIXELFLAVORCODES";
            break;
        case ICAP_JPEGPIXELTYPE:
            text = "ICAP_JPEGPIXELTYPE";
            break;
        case ICAP_TIMEFILL:
            text = "ICAP_TIMEFILL";
            break;
        case ICAP_BITDEPTH:
            text = "ICAP_BITDEPTH";
            break;
        case ICAP_BITDEPTHREDUCTION:
            text = "ICAP_BITDEPTHREDUCTION";
            break;
        case ICAP_UNDEFINEDIMAGESIZE:
            text = "ICAP_UNDEFINEDIMAGESIZE";
            break;
        case ICAP_IMAGEDATASET:
            text = "ICAP_IMAGEDATASET";
            break;
        case ICAP_EXTIMAGEINFO:
            text = "ICAP_EXTIMAGEINFO";
            break;
        case ICAP_MINIMUMHEIGHT:
            text = "ICAP_MINIMUMHEIGHT";
            break;
        case ICAP_MINIMUMWIDTH:
            text = "ICAP_MINIMUMWIDTH";
            break;
        case ICAP_AUTODISCARDBLANKPAGES:
            text = "ICAP_AUTODISCARDBLANKPAGES";
            break;
        case ICAP_FLIPROTATION:
            text = "ICAP_FLIPROTATION";
            break;
        case ICAP_BARCODEDETECTIONENABLED:
            text = "ICAP_BARCODEDETECTIONENABLED";
            break;
        case ICAP_SUPPORTEDBARCODETYPES:
            text = "ICAP_SUPPORTEDBARCODETYPES";
            break;
        case ICAP_BARCODEMAXSEARCHPRIORITIES:
            text = "ICAP_BARCODEMAXSEARCHPRIORITIES";
            break;
        case ICAP_BARCODESEARCHPRIORITIES:
            text = "ICAP_BARCODESEARCHPRIORITIES";
            break;
        case ICAP_BARCODESEARCHMODE:
            text = "ICAP_BARCODESEARCHMODE";
            break;
        case ICAP_BARCODEMAXRETRIES:
            text = "ICAP_BARCODEMAXRETRIES";
            break;
        case ICAP_BARCODETIMEOUT:
            text = "ICAP_BARCODETIMEOUT";
            break;
        case ICAP_ZOOMFACTOR:
            text = "ICAP_ZOOMFACTOR";
            break;
        case ICAP_PATCHCODEDETECTIONENABLED:
            text = "ICAP_PATCHCODEDETECTIONENABLED";
            break;
        case ICAP_SUPPORTEDPATCHCODETYPES:
            text = "ICAP_SUPPORTEDPATCHCODETYPES";
            break;
        case ICAP_PATCHCODEMAXSEARCHPRIORITIES:
            text = "ICAP_PATCHCODEMAXSEARCHPRIORITIES";
            break;
        case ICAP_PATCHCODESEARCHPRIORITIES:
            text = "ICAP_PATCHCODESEARCHPRIORITIES";
            break;
        case ICAP_PATCHCODESEARCHMODE:
            text = "ICAP_PATCHCODESEARCHMODE";
            break;
        case ICAP_PATCHCODEMAXRETRIES:
            text = "ICAP_PATCHCODEMAXRETRIES";
            break;
        case ICAP_PATCHCODETIMEOUT:
            text = "ICAP_PATCHCODETIMEOUT";
            break;
        case ICAP_FLASHUSED2:
            text = "ICAP_FLASHUSED2";
            break;
        case ICAP_IMAGEFILTER:
            text = "ICAP_IMAGEFILTER";
            break;
        case ICAP_NOISEFILTER:
            text = "ICAP_NOISEFILTER";
            break;
        case ICAP_OVERSCAN:
            text = "ICAP_OVERSCAN";
            break;
        case ICAP_AUTOMATICBORDERDETECTION:
            text = "ICAP_AUTOMATICBORDERDETECTION";
            break;
        case ICAP_AUTOMATICDESKEW:
            text = "ICAP_AUTOMATICDESKEW";
            break;
        case ICAP_AUTOMATICROTATE:
            text = "ICAP_AUTOMATICROTATE";
            break;
        case ICAP_JPEGQUALITY:
            text = "ICAP_JPEGQUALITY";
            break;
        case ICAP_FEEDERTYPE:
            text = "ICAP_FEEDERTYPE";
            break;
        case ICAP_ICCPROFILE:
            text = "ICAP_ICCPROFILE";
            break;
        case ICAP_AUTOSIZE:
            text = "ICAP_AUTOSIZE";
            break;
        case ICAP_AUTOMATICCROPUSESFRAME:
            text = "ICAP_AUTOMATICCROPUSESFRAME";
            break;
        case ICAP_AUTOMATICLENGTHDETECTION:
            text = "ICAP_AUTOMATICLENGTHDETECTION";
            break;
        case ICAP_AUTOMATICCOLORENABLED:
            text = "ICAP_AUTOMATICCOLORENABLED";
            break;
        case ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE:
            text = "ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE";
            break;
        case ICAP_COLORMANAGEMENTENABLED:
            text = "ICAP_COLORMANAGEMENTENABLED";
            break;
        case ICAP_IMAGEMERGE:
            text = "ICAP_IMAGEMERGE";
            break;
        case ICAP_IMAGEMERGEHEIGHTTHRESHOLD:
            text = "ICAP_IMAGEMERGEHEIGHTTHRESHOLD";
            break;
        case ICAP_SUPPORTEDEXTIMAGEINFO:
            text = "ICAP_SUPPORTEDEXTIMAGEINFO";
            break;
        case ICAP_FILMTYPE:
            text = "ICAP_FILMTYPE";
            break;
        case ICAP_MIRROR:
            text = "ICAP_MIRROR";
            break;
        case ICAP_JPEGSUBSAMPLING:
            text = "ICAP_JPEGSUBSAMPLING";
            break;
        case ACAP_XFERMECH:
            text = "ACAP_XFERMECH";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertImageFileFormatToExt(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case TWFF_PICT:
            text = ".pict";
            break;
        case TWFF_BMP:
            text = ".bmp";
            break;
        case TWFF_XBM:
            text = ".xbm";
            break;
        case TWFF_JFIF:
            text = ".jpeg";
            break;
        case TWFF_FPX:
            text = ".fpx";
            break;
        case TWFF_TIFF:
        case TWFF_TIFFMULTI:
            text = ".tiff";
            break;
        case TWFF_PNG:
            text = ".png";
            break;
        case TWFF_SPIFF:
            text = ".spiff";
            break;
        case TWFF_EXIF:
            text = ".exif";
            break;
        case TWFF_JP2:
            text = ".jp2";
            break;
        case TWFF_JPN:
            text = ".jpn";
            break;
        case TWFF_JPX:
            text = ".jpx";
            break;
        case TWFF_DEJAVU:
            text = ".dejavu";
            break;
        case TWFF_PDF:
        case TWFF_PDFA:
        case TWFF_PDFA2:
            text = ".pdf";
            break;

        default:
            text = "Unknown";
    }
    return text;
}

const std::string TwainSession::convertConTypeToString(const TW_UINT16 value) {
    const char *text = NULL;
    switch (value) {
        case TWON_ARRAY:
            text = "TWON_ARRAY";
            break;
        case TWON_ENUMERATION:
            text = "TWON_ENUMERATION";
            break;
        case TWON_ONEVALUE:
            text = "TWON_ONEVALUE";
            break;
        case TWON_RANGE:
            text = "TWON_RANGE";
            break;
        default:
            text = "Unknown";
    }
    return text;
}

int TwainSession::getTWTypeSize(const TW_UINT16 itemType) {
    int typeSize = 0;

    switch(itemType)
    {
        case TWTY_INT8:
            typeSize = sizeof(TW_INT8);
            break;
        case TWTY_INT16:
            typeSize = sizeof(TW_INT16);
            break;
        case TWTY_INT32:
            typeSize = sizeof(TW_INT32);
            break;
        case TWTY_UINT8:
            typeSize = sizeof(TW_UINT8);
            break;
        case TWTY_UINT16:
            typeSize = sizeof(TW_UINT16);
            break;
        case TWTY_UINT32:
            typeSize = sizeof(TW_UINT32);
            break;
        case TWTY_BOOL:
            typeSize = sizeof(TW_BOOL);
            break;
        case TWTY_FIX32:
            typeSize = sizeof(TW_FIX32);
            break;
        case TWTY_FRAME:
            typeSize = sizeof(TW_FRAME);
            break;
        case TWTY_STR32:
            typeSize = sizeof(TW_STR32);
            break;
        case TWTY_STR64:
            typeSize = sizeof(TW_STR64);
            break;
        case TWTY_STR128:
            typeSize = sizeof(TW_STR128);
            break;
        case TWTY_STR255:
            typeSize = sizeof(TW_STR255);
            break;
        case TWTY_STR1024:
            typeSize = sizeof(TW_STR1024);
            break;
        case TWTY_UNI512:
            typeSize = sizeof(TW_UNI512);
            break;
        case TWTY_HANDLE:
            typeSize = sizeof(TW_HANDLE);
            break;
        default:
            break;
    }
    return typeSize;
}

TW_FIX32 TwainSession::floatToFix32(float floater) {
    TW_FIX32 fix32;
    TW_BOOL sign = (floater < 0) ? TRUE : FALSE;
    TW_INT32 value = (TW_INT32) (floater * 65536.0 + (sign ? (-0.5) : 0.5));
    fix32.Whole = (TW_UINT16) (value >> 16);
    fix32.Frac = (TW_UINT16) (value & 0x0000ffffL);
    return fix32;
}

float TwainSession::fix32ToFloat(const TW_FIX32& fix32) {
    return float(fix32.Whole) + float(fix32.Frac / 65536.0);
}
