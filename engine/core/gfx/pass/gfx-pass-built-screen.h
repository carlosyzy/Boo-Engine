#pragma once
#include "gfx-pass.h"
class GfxPassBuiltScreen : public GfxPass
{
protected:
    void create() override;
public:
    GfxPassBuiltScreen(std::string name);
    ~GfxPassBuiltScreen();
};