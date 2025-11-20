#pragma once
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cstdint>
#include <unordered_map>
#include <sstream>
#include <vulkan/vulkan_core.h>


class GfxContext;
class GfxTexture;
class GfxPass;
struct GfxPassStruct;
class GfxShader;
class GfxPipeline;
struct GfxPipelineStruct;
class GfxQueue;
class GfxObject;

class GfxRenderer
{
private:
	GfxContext* _context;
	std::map<std::string, GfxTexture*> _textures;
	/**
	 * @brief 渲染通道
	 * 渲染通道
	 */
	std::map<std::string, GfxPass*> _passes;
	/**
	 * @brief 着色器
	 * 着色器
	 */
	std::map<std::string, GfxShader*> _shaders;
	/**
	 * @brief 渲染管线
	 * 渲染管线
	 */
	std::map<std::string, GfxPipeline*> _pipelines;
	/**
	 * @brief 渲染对象
	 * 渲染对象
	 */
	std::map<std::string, GfxObject*> _objects;

	std::map<std::string, GfxQueue*> _queues;

	void _Log(std::string msg);

	/**
	 * @brief 初始化默认UI通道
	 */
	void _initDefaultUIPasses();
	void _initDefaultUIShaders();
	void _initDefaultUIPipeline();
	void _initDefaultUIMaskPipeline();

public:
	GfxRenderer(GfxContext* context);
	void init();


	void frameRenderer(uint32_t imageIndex, std::vector<VkCommandBuffer>& commandBuffers);
	void cleanRendererState();
	void resetRendererState();
	/**
	 * 创建通道
	 */
	void createRenderPass(std::string name, GfxPassStruct passStruct);
	// void createPipeline(std::string passName, std::string pipelineName);
	void createPipeline(std::string pipelineName, GfxPipelineStruct pipelineStruct);

	/**
	 * @brief 创建模型渲染对象
	 * 旧的,过时的
	 * @param id  物体ID
	 * @param renderPassType 渲染通道类型
	 * @param pipelineType 管线类型
	 * @param points 顶点数据
	 * @param colors 颜色数据
	 * @param normals 法线数据
	 * @param uvs uv数据
	 * @param indices 索引数据
	 */
	void createObject(std::string id, std::string renderPassType, std::vector<float> points, std::vector<float> colors, std::vector<float> normals, std::vector<float> uvs, std::vector<uint32_t> indices);
	/**
	 * @brief 创建UI渲染对象
	 *
	 * @param id 物体ID
	 * @param pipelineType 管线类型
	 * @param points 顶点数据
	 * @param colors 颜色数据
	 * @param normals 法线数据
	 * @param uvs uv数据
	 * @param indices 索引数据
	 */
	void createUIObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
	/**
	 * @brief 创建UI遮罩渲染对象
	 * 旧的,过时的
	 * @param id 物体ID
	 * @param pipelineType 管线类型
	 * @param points 顶点数据
	 * @param colors 颜色数据
	 * @param normals 法线数据
	 * @param uvs uv数据
	 * @param indices 索引数据
	 */
	void createUIMaskObject(std::string id, std::vector<float> &points, std::vector<float> &colors, std::vector<float> &normals, std::vector<float> &uvs, std::vector<uint32_t> &indices);
	void setObjectPass(std::string id, std::string pass);
	void setObjectPipeline(std::string id, std::string pipeline);
	/**
	 * @brief 设置UI遮罩行为
	 *
	 * @param id 物体ID
	 * @param behavior 行为 0 不遮罩 1 遮罩
	 */
	void setObjectUIMaskBehavior(std::string id, uint32_t behavior);
	void setObjectModelMatrix(std::string id, std::array<float, 16> modelMatrix);
	void setObjectViewMatrix(std::string id, std::array<float, 16> viewMatrix);
	void setObjectProjMatrix(std::string id, std::array<float, 16> projMatrix);
	void setObjectTexture(const std::string& id, const  std::string& texture);
	void setObjectColor(std::string id, float r, float g, float b, float a);
	void destroyObject(std::string id);


	/*   // /**
	  //  * @brief 重置模型渲染对象状态
	  //  *
	  //  * @param id 物体ID
	  //  * @param renderPassType 渲染通道类型
	  //  * @param pipelineType 管线类型
	  //  */
	  // void resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType); */



	  /**
	   * @brief 提交渲染对象
	   * @param id 物体ID
	   */
	void submitObjectRender(std::string id);
	/**
	 * 创建 or 销毁渲染纹理
	 */
	void createTexture(std::string textureUuid, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t>* pixels);
	void destroyTexture(std::string textureUuid);
	bool isExistTexture(std::string textureUuid);

	/**
	 * @brief 创建着色器
	 *
	 * @param shaderName 着色器名称
	 * @param buffer 着色器字节码
	 */
	void createGlslShader(const std::string& shaderName, const std::string& shaderType, const std::string& data, const std::map<std::string, std::string>& macros);
	/**
	 * @brief 创建SPIR-V着色器
	 */
	void createSpirvShader(const std::string& shaderName, const std::vector<char>& data);
	~GfxRenderer();
};