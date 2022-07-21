//
// Created by Lossa on 2022/7/21.
//

#ifndef NODE_TWAIN_TWAINSDK_H
#define NODE_TWAIN_TWAINSDK_H

#include <napi.h>
#include "TwainSession.h"

class TwainSDK : public Napi::ObjectWrap<TwainSDK> {
public:
    TwainSDK(const Napi::CallbackInfo &info);

    virtual Napi::Value getState(const Napi::CallbackInfo &info);

    virtual Napi::Value getDataSources(const Napi::CallbackInfo &info);

    virtual Napi::Value getDefaultSource(const Napi::CallbackInfo &info);

    virtual Napi::Value setDefaultSource(const Napi::CallbackInfo &info);

    virtual Napi::Value openDataSource(const Napi::CallbackInfo &info);

    virtual Napi::Value addEventListener(const Napi::CallbackInfo &info);

    virtual Napi::Value getCapability(const Napi::CallbackInfo &info);

    virtual Napi::Value setCapability(const Napi::CallbackInfo &info);

    virtual Napi::Value test(const Napi::CallbackInfo &info);

private:
    static Napi::FunctionReference callback;
    TwainSession session;
};


#endif //NODE_TWAIN_TWAINSDK_H
