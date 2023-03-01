#pragma once
#include <functional>
#include <stdint.h>
namespace SCore{

using ListenerID = uint64_t;

template <typename ... ArgsType>
class Event{
public:
    using Callback = std::function<void(ArgsType...)>;
    

};

}