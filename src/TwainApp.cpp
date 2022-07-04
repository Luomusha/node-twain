//
// Created by Lossa on 2022/7/4.
//

#include "TwainApp.h"

Napi::Object TwainApp::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "TwainApp", {
            InstanceMethod<&TwainApp::connectDSM>("connectDSM", static_cast<napi_property_attributes>(napi_writable |
                                                                                                 napi_configurable)),
    });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("TwainApp", func);
    return exports;
}

TwainApp::TwainApp(const Napi::CallbackInfo &info) : Napi::ObjectWrap<TwainApp>(info) {

}

Napi::Value TwainApp::connectDSM(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, true);
}