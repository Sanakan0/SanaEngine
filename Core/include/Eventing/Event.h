#pragma once
#include <functional>
#include <stdint.h>
namespace SCore{

using ListenerID = uint64_t;

template <typename... ArgTypes>
class Event{
public:
    using Callback = std::function<void(ArgTypes...)>;
    

};

}