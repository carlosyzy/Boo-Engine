#pragma once
#include "gfx-pass.h"
class GfxPassBuiltUI : public GfxPass
{
protected:
    void _create() override;
public:
    GfxPassBuiltUI(std::string name);
    ~GfxPassBuiltUI();
};