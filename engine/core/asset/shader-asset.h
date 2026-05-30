#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
class GfxShader;
namespace Boo
{
	const std::string ShaderVertexAsset = "Vert";
	const std::string ShaderFragmentAsset = "Frag";

	class ShaderAsset : public Asset
	{
	private:
		std::string _glslData = "";
		GfxShader *gfxShader = nullptr;
	protected:
	public:
		ShaderAsset();
		ShaderAsset(std::string uuid);
		ShaderAsset(std::string uuid, std::string path, std::string name);
		void create(const std::string &type, const std::string &glslData, const std::map<std::string, int> &macros);
		void create(const std::vector<uint32_t> &data);
		void destroy() override;
		~ShaderAsset() {}
	};

} // namespace Boo