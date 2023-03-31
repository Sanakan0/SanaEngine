#pragma once
namespace SGUI::Panels{

class Panel{
public:
    void DrawTick(float deltat);
    virtual void DrawImpl(float deltat) = 0;
    virtual ~Panel() = default;
};


}