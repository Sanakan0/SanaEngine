#include "SGUI/Panels/Panel.h"

namespace SGUI::Panels{

void Panel::DrawTick(float deltat){
    DrawImpl(deltat);
}

}