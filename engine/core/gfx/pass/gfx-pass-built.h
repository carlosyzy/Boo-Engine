#pragma once
#include "gfx-pass.h"
class GfxPassBuilt : public GfxPass
{
protected:
    void _create() override;
public:
    GfxPassBuilt(std::string name);
    ~GfxPassBuilt();
};