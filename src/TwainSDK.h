//
// Created by Lossa on 2022/7/21.
//

#ifndef NODE_TWAIN_TWAINSDK_H
#define NODE_TWAIN_TWAINSDK_H

#include <napi.h>
#include <chrono>
#include <thread>
#include "TwainSession.h"

class TwainSDK : public Napi::ObjectWrap<TwainSDK> {
public:

    TwainSDK(const Napi::CallbackInfo &info);

    ~TwainSDK();

    virtual Napi::Value getState(const Napi::CallbackInfo &info);

    virtual Napi::Value getDataSources(const Napi::CallbackInfo &info);

    virtual Napi::Value getDefaultSource(const Napi::CallbackInfo &info);

    virtual Napi::Value setDefaultSource(const Napi::CallbackInfo &info);

    virtual Napi::Value openDataSource(const Napi::CallbackInfo &info);

    virtual Napi::Value addEventListener(const Napi::CallbackInfo &info);

    virtual Napi::Value getCapability(const Napi::CallbackInfo &info);

    virtual Napi::Value setCapability(const Napi::CallbackInfo &info);

    virtual Napi::Value enableDataSource(const Napi::CallbackInfo &info);

    virtual Napi::Value scan(const Napi::CallbackInfo &info);
private:
    TwainSession session;
};


class TwainWorker : public Napi::AsyncWorker {
public:
    TwainWorker(Napi::Function& callback, std::string& echo)
            : AsyncWorker(callback), echo(echo) {}

    ~TwainWorker() {}
    // This code will be executed on the worker thread
    void Execute() override {
        // Need to simulate cpu heavy task
//        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "async thread--------" << std::endl;
    }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::String::New(Env(), echo)});
    }

private:
    std::string echo;
};


#endif //NODE_TWAIN_TWAINSDK_H
