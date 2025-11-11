#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>

class GfxContext;
class GfxRenderer;

class GfxMgr
{
private:
	GfxMgr(const GfxMgr&) = delete;            /* // 禁用拷贝构造 */
	GfxMgr& operator=(const GfxMgr&) = delete; /* // 禁用赋值操作符 */
	GfxMgr();
	~GfxMgr();

	GfxContext* _context;
	GfxRenderer* _renderer;

	size_t _currentFrame = 0;

public:
	static GfxMgr* getInstance();
	void showVersion();
	/**
	 * 初始化
	 */
	void init();
	void initTestInfo();
	/*  // 采样次数 */
	void _testMSAASample();
	VkSampleCountFlagBits _getMaxMSAAUsableSampleCount();

	void update();
	void resetSwapChain();
	/**
	 * 创建渲染通道
	 */
	void createRenderPass(std::string name);
	/**
	 * 创建管线
	 */
	void createPipeline(std::string passName, std::string pipelineName);

	/**
	 * @brief 创建模型渲染对象
	 *
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
	/*  // /**
	 //  * @brief 重置模型渲染对象状态
	 //  *
	 //  * @param id 物体ID
	 //  * @param renderPassType 渲染通道类型
	 //  * @param pipelineType 管线类型
	 //  */
	 // void resetGfxObjectRendererState(std::string id, std::string renderPassType, std::string pipelineType); */
	 /**
	  * @brief 销毁模型渲染对象
	  *
	  * @param id 物体ID
	  * @param renderPassType 渲染通道类型
	  * @param pipelineType 管线类型
	  */
	void destroyObject(std::string id);
	void setObjectModelMatrix(std::string id, const std::array<float, 16>& modelMatrix);
	void setObjectViewMatrix(std::string id, const std::array<float, 16>& viewMatrix);
	void setObjectProjMatrix(std::string id, const std::array<float, 16>& projMatrix);
	void setObjectTexture(const std::string& id, const  std::string& texture);
	void setObjectColor(std::string id, float r, float g, float b, float a);
	void setObjectPipeline(std::string id, std::string pipeline);
	void setUIObjectMask(std::string id, float x, float y, float width, float height, float angle);


	/**
	* @brief 创建纹理
	*
	* @param textureUuid 纹理UUID
	* @param width 宽度
	* @param height 高度
	* @param channels 通道数
	* @param pixels 像素数据
	*/
	void createTexture(std::string texture, uint32_t width, uint32_t height, uint32_t channels, const std::vector<uint8_t>* pixels);
	/**
	 * @brief 销毁纹理
	 *
	 * @param textureUuid 纹理UUID
	 */
	void destroyTexture(std::string texture);
	/**
	 * @brief 判断纹理是否存在
	 *
	 * @param texture 纹理UUID
	 * @return true 存在
	 * @return false 不存在
	 */
	bool isExistTexture(std::string texture);
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

	/**
	 * @brief 提交渲染对象
	 * @param id 物体ID
	 */
	void submit(std::string id);

	VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer* buffer, VkDeviceMemory* memory, VkDeviceSize size, void* data);
	uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties);
	std::vector<char> readShaderFile(const std::string& filename);
	static void Log(const std::string tag, const std::string msg)
	{
		std::cout << "Boo-Engine: " << tag << ":" << msg << std::endl;
	}
};