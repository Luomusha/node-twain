//
// Created by Lossa on 2022/7/21.
//

#ifndef NODE_TWAIN_EVENTEMITTER_H
#define NODE_TWAIN_EVENTEMITTER_H

#include <napi.h>
#include <chrono>
#include <thread>

class EventEmitter : public Napi::AsyncWorker {
public:
    EventEmitter(Napi::Function &callback, std::string &echo) : AsyncWorker(callback), echo(echo) {}

    ~EventEmitter() {};

    void Execute() override {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void OnOK() override {
        Napi::HandleScope scope(Env());
        Callback().Call({Env().Null(), Napi::String::New(Env(), echo)});
    }
private:
    std::string echo;
};


#endif //NODE_TWAIN_EVENTEMITTER_H
