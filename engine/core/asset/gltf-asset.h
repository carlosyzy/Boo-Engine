#pragma once
#include "asset.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <variant>
// #include <fastgltf/core.hpp>
// #include <fastgltf/tools.hpp>
// #include <fastgltf/glm_element_traits.hpp>
//参考: https://fastgltf.readthedocs.io/latest/guides.html#iterating-over-the-node-hierarchy
#include "core/math/mat4.h"

namespace Boo
{
	// gltf中的节点结构
	struct GLTFNode
	{
		/**
		 * 节点名称
		 */
		std::string name;
		/**
		 * 本地变换矩阵
		 */
		Mat4 local;
		/**
		 * 相对场景的路径
		 */
		std::string path;
		/**
		 * 网格UUID列表
		 * 空字符串表示没有网格
		 */
		std::string meshUuid;
		/**
		 * 子节点
		 */
		std::vector<GLTFNode> children;
	};
	class MeshAsset;

	class GLTFAsset : public Asset
	{
	private:
		/**
		 * 根节点
		 */
		GLTFNode _root;
        // std::unique_ptr<fastgltf::Asset> m_asset;
		
		/**
		 * 网格索引映射
		 */
		std::map<size_t,std::string> _meshIndexMap;
		std::map<std::string,MeshAsset*> _meshAssets;
		
        
        // void _parseScenes();
        // void _parseNode(GLTFNode &parent, size_t nodeIdx);
        // void _fmatToMat4(Mat4 &mat, const fastgltf::math::fmat4x4& fmat);
        // void _parseMeshes(const fastgltf::Node &node,GLTFNode &nodeData);
        // void _parsePrimitive(int index,const fastgltf::Primitive& primitive, FMeshPrimitive &primitiveData);
	public:
		GLTFAsset();
		GLTFAsset(std::string uuid);
		GLTFAsset(std::string uuid, std::string path, std::string name);
		// void create(std::unique_ptr<fastgltf::Asset> m_asset);
		const GLTFNode &getRoot();
		std::map<std::string,MeshAsset*> &getMeshs();
		MeshAsset *getMesh(std::string meshName);
		void destroy() override;
		~GLTFAsset();
	};

} // namespace Boo