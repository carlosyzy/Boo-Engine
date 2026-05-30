#pragma once
#include "core/asset/asset-struct.h"
namespace Boo
{

    class AssetBuiltin
    {
    private:
        void _initDefaultTextureLogo();
        void _initDefaultShaderUI();
        void _initDefaultShaderUIMask();
        void _initDefaultShaderUnlit();
        void _initDefaultMaterialUI();
        void _initDefaultMaterialUIMask();
        void _initDefaultMaterialUnlit();
        void _initDefaultMeshUI();
        void _initDefaultFont();

    public:
        AssetBuiltin();
        void init();
        ~AssetBuiltin();
    };

} // namespace Boo