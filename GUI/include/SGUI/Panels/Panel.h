#pragma once
namespace SGUI::Panels{

class Panel{
public:
    void DrawTick();
    virtual void DrawImpl() = 0;
    virtual ~Panel() = default;
};


}