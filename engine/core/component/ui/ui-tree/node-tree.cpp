#include "node-tree.h"
#include "../../../scene/node.h"
#include "../../../scene/node-2d.h"
#include "../../../scene/scene.h"

NodeTree::NodeTree(Node *node, std::string uuid) : UITree(node, uuid)
{
}

void NodeTree::setNode(Node *node)
{
    this->_rootNode = node;
    this->_isDirty = true;
    this->_setTrees(node, this->_uiTreeData, 0);
}
void NodeTree::setScene(Scene *scene)
{
    this->_isDirty = true;
    this->_rootScene = scene;
    // 直接设置场景
    this->_uiTreeData.name = scene->getName();
    this->_uiTreeData.uuid = scene->getUuid();
    this->_uiTreeData.isFold = false;
    // this->_uiTreeData.ndBind = nullptr;  res/ic-scene.png
    this->_uiTreeData.layer = 0;
    this->_uiTreeData.iconKey = this->_iconMaps.iconScene;
    this->_uiTreeData.children.resize(scene->getChildren().size());
    for (int i = 0; i < scene->getChildren().size(); i++)
    {
        this->_setTrees(scene->getChildren()[i], this->_uiTreeData.children[i], 1);
    }
}
void NodeTree::updateTree()
{
    if (this->_rootNode != nullptr)
    {
        this->setNode(this->_rootNode);
    }
    else if (this->_rootScene != nullptr)
    {
        this->setScene(this->_rootScene);
    }
    UITree::updateTree();
}
void NodeTree::_setTrees(Node *root, UITreeStructure &uiTreeData, int layer)
{
    uiTreeData.name = root->getName();
    uiTreeData.uuid = root->getUuid();
    uiTreeData.isFold = false;
    // uiTreeData.ndBind = root;
    uiTreeData.layer = layer;
    if (root->getLayer() == NodeLayer::Node2D)
    {
        uiTreeData.iconKey = this->_iconMaps.iconNode2D;
    }
    else if (root->getLayer() == NodeLayer::Node3D)
    {
        uiTreeData.iconKey = this->_iconMaps.iconNode3D;
    }
    uiTreeData.children.resize(root->getChildren().size());
    for (int i = 0; i < root->getChildren().size(); i++)
    {
        this->_setTrees(root->getChildren()[i], uiTreeData.children[i], layer + 1);
    }
}
void NodeTree::Update(float deltaTime)
{
    UITree::Update(deltaTime);
}
void NodeTree::LateUpdate(float deltaTime)
{
    UITree::LateUpdate(deltaTime);
}
void NodeTree::Render()
{
    UITree::Render();
}
void NodeTree::destroy()
{
    UITree::destroy();
}
NodeTree::~NodeTree()
{
}

// // json j;
// //     // 添加键值对
// //     j["name"] = "张三";
// //     j["age"] = 25;
// //     j["is_student"] = false;
// //     j["hobbies"] = {"阅读", "编程", "游泳"}; // 数组
// //     j["address"] = {                         // 嵌套对象
// //                     {"city", "北京"},
// //                     {"street", "海淀区"}};

// //     // 序列化为字符串并输出
// //     // dump(4) 中的 4 表示使用 4 个空格进行格式化缩进，便于阅读
// //     std::cout << j.dump(4) << std::endl;

// // // 将 JSON 以 MessagePack 格式保存为二进制文件
// // void save_json_as_binary(const string& filename, const json& j) {
// //     ofstream file(filename, ios::out | ios::binary);
// //     if (!file) {
// //         throw runtime_error("无法打开文件进行写入: " + filename);
// //     }

// //     // 将 JSON 转换为 MessagePack 二进制数据
// //     vector<uint8_t> msgpack_data = json::to_msgpack(j);

// //     // 写入二进制数据
// //     file.write(reinterpret_cast<const char*>(msgpack_data.data()), msgpack_data.size());
// //     file.close();

// //     cout << "已保存二进制文件: " << filename
// //          << " (大小: " << msgpack_data.size() << " 字节)" << endl;
// // }

// // // 从二进制文件读取并解析 JSON
// // json load_json_from_binary(const string& filename) {
// //     ifstream file(filename, ios::in | ios::binary | ios::ate);
// //     if (!file) {
// //         throw runtime_error("无法打开文件进行读取: " + filename);
// //     }

// //     // 获取文件大小
// //     streamsize size = file.tellg();
// //     file.seekg(0, ios::beg);

// //     // 读取二进制数据
// //     vector<uint8_t> buffer(size);
// //     if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
// //         // 从 MessagePack 解析为 JSON
// //         return json::from_msgpack(buffer);
// //     }

// //     throw runtime_error("读取文件失败: " + filename);
// // }