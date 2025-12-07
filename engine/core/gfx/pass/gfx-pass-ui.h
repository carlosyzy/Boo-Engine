#pragma once
#include "gfx-pass.h"
class GfxPassUI : public GfxPass
{

protected:
    void _create() override;
public:
    GfxPassUI(std::string name);
    ~GfxPassUI();
};