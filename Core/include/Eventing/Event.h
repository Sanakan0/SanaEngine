#pragma once
#include <functional>
#include <stdint.h>
#include <vector>
namespace SCore{

using ListenerID = uint64_t;

template <typename ... ArgsType>
class Event{
public:
    using Callback = std::function<void(ArgsType...)>;
    void AddListener(Callback call){
        calls_.push_back(call);
    }

    void Invoke(ArgsType... args){
        for (auto func:calls_){
            func(args...);
        }
    }
private:
    std::vector<Callback> calls_;
};

}