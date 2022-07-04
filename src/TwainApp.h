//
// Created by Lossa on 2022/7/4.
//

#ifndef NODE_TWAIN_TWAINAPP_H
#define NODE_TWAIN_TWAINAPP_H

#include "twain.h"
#include <napi.h>

class TwainApp : public Napi::ObjectWrap<TwainApp> {

public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    TwainApp(const Napi::CallbackInfo& info);

    Napi::Value connectDSM(const Napi::CallbackInfo& info);

private:
    TW_IDENTITY identity;
    TW_INT16 state;
};


#endif //NODE_TWAIN_TWAINAPP_H
