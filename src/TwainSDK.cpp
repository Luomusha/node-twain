//
// Created by Lossa on 2022/7/21.
//

#include "TwainSDK.h"

Napi::FunctionReference callback;

TwainSDK::TwainSDK(const Napi::CallbackInfo &info) : Napi::ObjectWrap<TwainSDK>(info) {
    Napi::Object configure = info[0].As<Napi::Object>();

    Napi::Object version = configure.Get("version").As<Napi::Object>();

    Napi::Number versionCountry = version.Get("country").As<Napi::Number>();
    Napi::Number versionLanguage = version.Get("language").As<Napi::Number>();
    Napi::Number versionMajorNum = version.Get("majorNum").As<Napi::Number>();
    Napi::Number versionMinorNum = version.Get("minorNum").As<Napi::Number>();
    Napi::String versionInfo = version.Get("info").As<Napi::String>();
    Napi::String productName = configure.Get("productName").As<Napi::String>();
    Napi::String productFamily = configure.Get("productFamily").As<Napi::String>();
    Napi::String manufacturer = configure.Get("manufacturer").As<Napi::String>();

    TW_IDENTITY identity;
    identity.Id = 0;
    identity.Version.Country = versionCountry.Int32Value();
    identity.Version.Language = versionLanguage.Int32Value();
    identity.Version.MajorNum = versionMajorNum.Int32Value();
    identity.Version.MinorNum = versionMinorNum.Int32Value();
    strcpy((char *) identity.Version.Info, versionInfo.Utf8Value().c_str());
    strcpy((char *) identity.ProductName, productName.Utf8Value().c_str());
    strcpy((char *) identity.ProductFamily, productFamily.Utf8Value().c_str());
    strcpy((char *) identity.Manufacturer, manufacturer.Utf8Value().c_str());
    identity.SupportedGroups = DF_APP2 | DG_IMAGE | DG_CONTROL;
    identity.ProtocolMajor = TWON_PROTOCOLMAJOR;
    identity.ProtocolMinor = TWON_PROTOCOLMINOR;

    session.fillIdentity(identity);

    session.loadDSM();     // state 1 -> state 2
    session.openDSM();     // state 2 -> state 3
}

Napi::Value TwainSDK::getState(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    return Napi::Number::New(env, session.state);
}

Napi::Value TwainSDK::getDataSources(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    session.getSources();

    uint32_t i = 0;
    Napi::Array array = Napi::Array::New(env, session.sources.size());
    for (auto &&it: session.sources) {
        array[i++] = Napi::String::New(env, reinterpret_cast<char *>(it.ProductName));
    }
    return array;
}

Napi::Value TwainSDK::getDefaultSource(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    TW_UINT16 rc = session.getDefaultDS();
    return Napi::String::New(env, reinterpret_cast<char *>(session.source.ProductName));
}

Napi::Value TwainSDK::setDefaultSource(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    std::string productName = info[0].As<Napi::String>().Utf8Value();

    TW_UINT16 rc = session.setDefaultDS(productName);

    return Napi::Boolean::New(env, rc == TWRC_SUCCESS);
}

Napi::Value TwainSDK::openDataSource(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);

    TW_UINT16 rc = TWRC_SUCCESS;
    rc = session.openDS() & rc;
    rc = session.setCallback() & rc;
    if (rc == TWRC_SUCCESS) {
        deferred.Resolve(Napi::String::New(info.Env(), "OK"));
    } else {
        deferred.Reject(Napi::String::New(info.Env(), "Reject"));
    }
    return deferred.Promise();
}

Napi::Value TwainSDK::addEventListener(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    callback = Napi::Persistent(info[0].As<Napi::Function>());
    return Napi::Boolean::New(env, true);
}

Napi::Value TwainSDK::getCapability(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    TW_UINT16 CAP = info[0].As<Napi::Number>().Uint32Value();

    TW_CAPABILITY cap;
    cap.Cap = CAP;

    TW_UINT16 rc = session.getCap(cap);
// The following structures combinations are implimented and found in the TWAIN specifications
//              BOOL  INT8  INT16  INT32  UINT8  UINT16  UINT32  STR32  STR64  STR128  STR255  STR1024  UNI512  FIX32  FRAME
// OneValue      x           x      x             x       x       x             x       x                        x      x
// Array                                   x      x       x       x                                              x      x
// Enumeration   x           x                    x       x       x                     x                        x      x
// Range                     x      x             x       x                                                      x
    if (rc != TWRC_SUCCESS) {
        return Napi::Boolean::New(env, false);
    }
    if (cap.ConType == TWON_RANGE) {
        pTW_RANGE pRange = (pTW_RANGE) session.lockMemory(cap.hContainer);
        Napi::Object rangeResult = Napi::Object::New(env);
        switch (pRange->ItemType) {
            case TWTY_INT8:
            case TWTY_INT16:
            case TWTY_INT32:
            case TWTY_UINT8:
            case TWTY_UINT16:
            case TWTY_UINT32:
                rangeResult.Set("minValue", Napi::Number::New(env, pRange->MinValue));
                rangeResult.Set("maxValue", Napi::Number::New(env, pRange->MaxValue));
                rangeResult.Set("stepSize", Napi::Number::New(env, pRange->StepSize));
                rangeResult.Set("defaultValue", Napi::Number::New(env, pRange->DefaultValue));
                rangeResult.Set("currentValue", Napi::Number::New(env, pRange->CurrentValue));
//            case TWTY_FIX32:
//                rangeResult.Set("minValue", Napi::Number::New(env, fix32ToFloat(pRange->MinValue)));
//                rangeResult.Set("maxValue", Napi::Number::New(env, fix32ToFloat(pRange->MaxValue)));
//                rangeResult.Set("stepSize", Napi::Number::New(env, fix32ToFloat(pRange->StepSize)));
//                rangeResult.Set("defaultValue", Napi::Number::New(env, fix32ToFloat(pRange->DefaultValue)));
//                rangeResult.Set("currentValue", Napi::Number::New(env, fix32ToFloat(pRange->CurrentValue)));
        }
        return rangeResult;
    } else if (cap.ConType == TWON_ARRAY) {
        pTW_ARRAY pArray = (pTW_ARRAY) session.lockMemory(cap.hContainer);
        Napi::Array arr = Napi::Array::New(env, pArray->NumItems);
        for (TW_UINT32 index = 0; index < pArray->NumItems; index++) {
            switch (pArray->ItemType) {
                case TWTY_INT8:
                case TWTY_INT16:
                case TWTY_INT32:
                case TWTY_UINT8:
                case TWTY_UINT16:
                case TWTY_UINT32:
                    arr[index] = Napi::Number::New(env, pArray->ItemList[index]);
            }
        }
        return arr;
    } else if (cap.ConType == TWON_ONEVALUE) {
        pTW_ONEVALUE pOne = (pTW_ONEVALUE) session.lockMemory(cap.hContainer);
        switch (pOne->ItemType) {
            case TWTY_INT8:
            case TWTY_INT16:
            case TWTY_INT32:
            case TWTY_UINT8:
            case TWTY_UINT16:
            case TWTY_UINT32:
                return Napi::Number::New(env, pOne->Item);
            case TWTY_BOOL:
                return Napi::Boolean::New(env, pOne->Item);
            case TWTY_STR32: {
                pTW_STR32 str32 = ((pTW_STR32) (&pOne->Item));
                return Napi::String::New(env, reinterpret_cast<char *>(str32));
            }
            case TWTY_STR64: {
                pTW_STR64 str64 = ((pTW_STR64) (&pOne->Item));
                return Napi::String::New(env, reinterpret_cast<char *>(str64));
            }
            case TWTY_STR128: {
                pTW_STR128 str128 = ((pTW_STR128) (&pOne->Item));
                return Napi::String::New(env, reinterpret_cast<char *>(str128));
            }
            case TWTY_STR255: {
                pTW_STR255 str255 = ((pTW_STR255) (&pOne->Item));
                return Napi::String::New(env, reinterpret_cast<char *>(str255));
            }
        }
    } else if (cap.ConType == TWON_ENUMERATION) {
        pTW_ENUMERATION pEnum = (pTW_ENUMERATION) session.lockMemory(cap.hContainer);
        Napi::Object enumResult = Napi::Object::New(env);
        Napi::Array list = Napi::Array::New(env, pEnum->NumItems);
        switch (pEnum->ItemType) {
            case TWTY_INT8:
            case TWTY_INT16:
            case TWTY_INT32:
            case TWTY_UINT8:
            case TWTY_UINT16:
            case TWTY_UINT32:
                for (TW_UINT32 index = 0; index < pEnum->NumItems; index++) {
                    list[index] = pEnum->ItemList[index];
                }
            case TWTY_BOOL:
                for (TW_UINT32 index = 0; index < pEnum->NumItems; index++) {
                    list[index] = pEnum->ItemList[index];
                }
        }
        enumResult.Set("currentIndex", pEnum->CurrentIndex);
        enumResult.Set("defaultIndex", pEnum->DefaultIndex);
        enumResult.Set("itemList", list);
        return enumResult;
    }
    return Napi::Boolean::New(env, false);
}

Napi::Value TwainSDK::setCapability(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    TW_UINT16 CAP = info[0].As<Napi::Number>().Uint32Value();
    TW_UINT16 ITEM_TYPE = info[0].As<Napi::Number>().Uint32Value();
    TW_HANDLE hResult = NULL;
    switch (ITEM_TYPE) {
        case TWTY_STR32:
        case TWTY_STR64:
        case TWTY_STR128:
        case TWTY_STR255:
            hResult = session.allocMemory(sizeof(TW_ONEVALUE) + session.getTWTypeSize(ITEM_TYPE));
        default:
            hResult = session.allocMemory(sizeof(TW_ONEVALUE));
    }

    pTW_ONEVALUE pOne = (pTW_ONEVALUE) (session.lockMemory(hResult));
    pOne->ItemType = ITEM_TYPE;
    session.unlockMemory(hResult);
    pOne = NULL;

    TW_CAPABILITY cap;
    cap.Cap = CAP;
    cap.ConType = TWON_ONEVALUE;
    cap.hContainer = hResult;

    return Napi::Boolean::New(env, true);
}

Napi::Value TwainSDK::enableDataSource(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    session.enableDS(NULL);
    return Napi::Boolean::New(env, true);
}

Napi::Value TwainSDK::test(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    return Napi::Boolean::New(env, true);
}

TW_UINT16 TwainSession::dsmCallback(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest, TW_UINT32 uiDG, TW_UINT16 uiDAT, TW_UINT16 uiMSG, TW_MEMREF pData) {
    std::cout << "Trigger callback" << std::endl;
    callback.Call({});
    return TWRC_SUCCESS;
}
