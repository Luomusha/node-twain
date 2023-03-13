//
// Created by Lossa on 2022/7/4.
//

#ifndef NODE_TWAIN_TWAINAPP_H
#define NODE_TWAIN_TWAINAPP_H

#include <iostream>
#include <vector>
#include <string>

#include "twain/Common.h"

#ifdef TWH_CMP_GNU
#include <dlfcn.h>
#endif

#ifndef TWH_CMP_MSC
typedef void *HWND;
#endif

#define LIBRARY "/Library/Frameworks/TWAINDSM.framework/Versions/Current/TWAINDSM"

class TwainSession {

public:
    std::vector <TW_IDENTITY> sources;  // sources list. Used by getSources/openDS/setDefaultDS
    TW_IDENTITY source;                 //set Source by user
    TW_UINT16 state = 1;

    void fillIdentity(TW_IDENTITY id);

    /**
     * state 1 -> 2
     * @return
     */
    TW_UINT16 loadDSM();

    TW_UINT16 freeDSM();

    TW_UINT16 entry(TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, TW_MEMREF pData, pTW_IDENTITY pDataSource = NULL);

    /**
     * state 2 -> 3
     * @return
     */
    TW_UINT16 openDSM();

    TW_UINT16 closeDSM();

    TW_UINT16 getDefaultDS();

    TW_UINT16 setDefaultDS(std::string name);

    TW_UINT16 getSources();

    /**
     * state 3 -> 4
     * @return
     */
    TW_UINT16 openDS();
    TW_UINT16 openDS(std::string name);

    TW_UINT16 closeDS();

    TW_UINT16 getCap(TW_CAPABILITY& Cap);

    TW_UINT16 getCurrentCap(TW_CAPABILITY &cap);

    TW_UINT16 setCap(TW_UINT16 Cap, const int value, TW_UINT16 type);

    TW_UINT16 setCallback();

    /**
     * state 4 -> 5
     * @return
     */
    TW_UINT16 enableDS();

    TW_UINT16 disableDS();

    TW_UINT16 getImageInfo();

    TW_UINT16 scan(TW_UINT32 mech, std::string fileName);

    TW_HANDLE allocMemory(TW_UINT32 _size);

    void freeMemory(TW_HANDLE _hMemory);

    TW_MEMREF lockMemory(TW_HANDLE _hMemory);

    void unlockMemory(TW_HANDLE _hMemory);

    int getTWTypeSize(const TW_UINT16 itemType);

private:
#ifdef TWH_CMP_MSC
    HMODULE
#else
    void *
#endif
    pDSMLibrary = 0;
    DSMENTRYPROC dsmEntry = 0;
    TW_ENTRYPOINT gDSMEntry = {0};
    TW_IDENTITY identity;
    pTW_IDENTITY pSource;               //set source by DS
    HWND parent;

    TW_STATUS status;
    TW_IMAGEINFO imageInfo;
    TW_USERINTERFACE ui;

    void transferNative();
    void transferFile(TW_UINT16 fileFormat,std::string);
    void transferMemory();

    bool parseCapability(TW_CAPABILITY *pCap, TW_UINT32& val);

    static const std::string convertReturnCodeToString(const TW_UINT16 value);

    static const std::string convertConditionCodeToString(const TW_UINT16 value);

    static const std::string convertDataGroupToString(const TW_UINT16 value);

    static const std::string convertDataArgTypeToString(const TW_UINT16 value);

    static const std::string convertMessageToString(const TW_UINT16 value);

    static const std::string convertCapToString(const TW_UINT16 value);

    static const std::string convertConTypeToString(const TW_UINT16 value);

    static const std::string convertImageFileFormatToExt(const TW_UINT16 value);

    static float fix32ToFloat(const TW_FIX32& fix32);

    static TW_FIX32 floatToFix32(float floater);
};


#endif //NODE_TWAIN_TWAINAPP_H
