#pragma once
#include "gfx-pass.h"
class GfxPassBuiltUI : public GfxPass
{
protected:
    void create() override;
public:
    GfxPassBuiltUI(std::string name);
    ~GfxPassBuiltUI();
};