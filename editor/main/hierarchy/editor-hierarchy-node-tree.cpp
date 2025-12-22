#include "editor-hierarchy-node-tree.h"

#include "../../boo-editor.h"

#include "../../../engine/core/scene/scene.h"
#include "../../../engine/core/scene/node-2d.h"
#include "../../../engine/core/renderer/ui/ui-sprite.h"
#include "../../../engine/core/renderer/ui/ui-text.h"
#include "../../../engine/core/input/input.h"

EditorHierarchyNodeTree::EditorHierarchyNodeTree(std::string name, Node *node, std::string uuid) : Component(name, node, uuid)
{
    this->_layer = NodeLayer::Node2D;
    this->_topLen = 0.0f;
    this->_leftLen = 0.0f;
    this->_nodeIndex = 0;
}

void EditorHierarchyNodeTree::_deserialized()
{
    Component::_deserialized();
}
void EditorHierarchyNodeTree::Awake()
{
    Component::Awake();
    this->_initContent();
}
void EditorHierarchyNodeTree::_initContent()
{
    // 在当前组件的子节点中获取第一个名字为Content的节点
    // 获取失败时创建
    Node *content = this->_node->getChildByName("Content");
    if (content != nullptr)
    {
        this->_ndContent = dynamic_cast<Node2D *>(content);
    }
    else
    {
        this->_ndContent = new Node2D("Content");
        this->_node->addChild(this->_ndContent);
    }
    this->_ndContent->setAnchor(0, 1);
    this->_ndContent->setSize(300.0f, 200.0f);
    // 确定后不需要渲染组件
    // this->_spContent = static_cast<UISprite *>(this->_ndContent->addComponent("UISprite"));
    // this->_spContent->setColor(34.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
}

void EditorHierarchyNodeTree::Enable()
{
    Component::Enable();
    this->_isDirty;
    // 事件系统
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    this->_contentTouchID = node2d->onNodeInputEvent(NodeInput::TOUCH_END, &EditorHierarchyNodeTree::_onTreeContentTouchEvent, this);
    this->_contentHoverID = node2d->onNodeInputEvent(NodeInput::CURSOR_HOVER, &EditorHierarchyNodeTree::_onTreeContentHoverEvent, this);
}
void EditorHierarchyNodeTree::setNode(Node *node)
{
    // this->_nodes.clear();
    this->_rootNode = node;
    this->_isDirty = true;
    this->_setTrees(node, this->_uiTreeData, 0);
}
void EditorHierarchyNodeTree::setScene(Scene *scene)
{
    // this->_nodes.clear();
    this->_isDirty = true;
    this->_rootScene = scene;
    // 直接设置场景
    this->_uiTreeData.name = scene->getName();
    this->_uiTreeData.uuid = scene->getUuid();
    this->_uiTreeData.isFold = false;
    this->_uiTreeData.ndBind = nullptr;
    this->_uiTreeData.icon = "ic-scene.png";
    this->_uiTreeData.layer = 0;
    this->_uiTreeData.children.resize(scene->getChildren().size());
    // this->_nodes[scene->getUuid()] = scene;
    std::cout << "EditorHierarchyNodeTree::setScene: " << scene->getName() << std::endl;
    for (int i = 0; i < scene->getChildren().size(); i++)
    {
        this->_setTrees(scene->getChildren()[i], this->_uiTreeData.children[i], 1);
    }
}
void EditorHierarchyNodeTree::_setTrees(Node *root, NodeTreeStructure &uiTreeData, int layer)
{
    // this->_nodes[root->getUuid()] = root;
    std::cout << "EditorHierarchyNodeTree::_setTrees: " << root->getName() << std::endl;
    uiTreeData.name = root->getName();
    uiTreeData.uuid = root->getUuid();
    if (root->getLayer() == NodeLayer::Node2D)
    {
        uiTreeData.icon = "ic-2d.png";
    }
    else
    {
        uiTreeData.icon = "ic-3d.png";
    }
    uiTreeData.isFold = false;
    uiTreeData.ndBind = nullptr;
    uiTreeData.layer = layer;
    uiTreeData.children.resize(root->getChildren().size());
    for (int i = 0; i < root->getChildren().size(); i++)
    {
        this->_setTrees(root->getChildren()[i], uiTreeData.children[i], layer + 1);
    }
}
void EditorHierarchyNodeTree::updateTree()
{
    if (this->_rootNode != nullptr)
    {
        this->setNode(this->_rootNode);
    }
    else if (this->_rootScene != nullptr)
    {
        this->setScene(this->_rootScene);
    }
}

void EditorHierarchyNodeTree::Update(float deltaTime)
{
    Component::Update(deltaTime);
}
void EditorHierarchyNodeTree::LateUpdate(float deltaTime)
{
    Component::LateUpdate(deltaTime);
    if (!this->_isEnabledInHierarchy)
        return; // 组件未激活
    if (this->_isDirty || this->_node->hasFrameTransformFlag())
    {
        std::cout << "UITree UITree::update  0" << std::endl;
        this->_updateTreeContent();
        this->_isDirty = false;
    }
}
void EditorHierarchyNodeTree::_updateTreeContent()
{
    std::cout << "UITree UITree::update  1" << std::endl;
    this->_nodeIndex = 0;
    this->_treeNodes.clear();
    this->_treeNodeDataMap.clear();
    // this->_foldUIMap.clear();

    Node2D *node = dynamic_cast<Node2D *>(this->_node);
    const Size &size = node->getSize();
    if (this->_uiTreeData.layer >= 0)
    {
        this->_updateTreesItems(this->_uiTreeData);
    }
    // 刷新节点池状态
    for (int i = this->_nodeIndex; i < this->_nodePools.size(); i++)
    {
        std::cout << "UITree UITree::update  2:" << this->_nodePools.size() << "  " << i << std::endl;
        this->_nodePools[i]->setActive(false);
    }
    this->_contentWidth = size.getWidth() - 10.0f;
    this->_contentHeight = this->_nodeIndex * 25.0f;
    // 重新计算_topLen
    if (this->_contentHeight - this->_topLen < size.getHeight())
    {
        // 底部已经到底部
        this->_topLen = this->_contentHeight - size.getHeight();
    }
    if (this->_topLen < 0.0f)
    {
        // 顶部已经到底部
        this->_topLen = 0.0f;
    }
    this->_contentX = -size.getWidth() / 2.0f + this->_leftLen;
    this->_contentY = size.getHeight() / 2.0f + this->_topLen;
    this->_ndContent->setPosition(this->_contentX, this->_contentY, 0.0f);
    this->_ndContent->setSize(this->_contentWidth, this->_contentHeight);
    std::cout << "UITree UITree::update  3" << this->_contentWidth << " " << this->_contentHeight << std::endl;
}
void EditorHierarchyNodeTree::_updateTreesItems(NodeTreeStructure &uiTreeData)
{
    uiTreeData.ndBind = nullptr;
    for (int i = this->_nodePools.size(); i <= this->_nodeIndex; i++)
    {
        this->_createNodeItem();
    }
    Node2D *node = this->_nodePools[this->_nodeIndex];
    node->setActive(true);
    this->_updateTreeItemSelect(node,uiTreeData);

}
void EditorHierarchyNodeTree::_updateTreeItemSelect(Node2D *ndItem,NodeTreeStructure &uiTreeData)
{
     Node *ndSelect = ndItem->getChildByName("NodeTreeItemSelect");
      Node2D *ndSelect2d = dynamic_cast<Node2D *>(ndSelect);
      UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect2d->getComponent("UISprite"));
    if(this->_selectTreeItem!=nullptr && uiTreeData.uuid == this->_selectTreeItem->uuid){
      spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 1.0f);
      return;
    }
    if(this->_hoverTreeItem!=nullptr && uiTreeData.uuid == this->_hoverTreeItem->uuid){
      spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.3f);
      return;
    }
    spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.0f);
}

    // /* std::random_device rd;
    //  std::mt19937 gen(rd());
    //  std::uniform_real_distribution<double> randomGet(0.0, 1.0);*/

    // Node2D *root = dynamic_cast<Node2D *>(this->_node);
    // const Size &size = root->getSize();

    // float offset = 10.0f;
    // float width = 0;
    // // float itemWidth = 10.0f;
    // // float itemHeight = 22.0f;
    // Node2D *node = nullptr;
    // UISprite *sp = nullptr;
    // Node2D *ndSelect = nullptr;
    // UISprite *spSelect = nullptr;
    // Node2D *ndFold = nullptr;
    // UISprite *spFold = nullptr;
    // Node2D *ndIcon = nullptr;
    // UISprite *spIcon = nullptr;
    // Node2D *ndName = nullptr;
    // UIText *txtName = nullptr;

    // if (this->_nodeIndex >= this->_nodePools.size())
    // {
    //    for (int i = 0; i < uiTreeData.children.size(); i++)
    //    {
    //        this->_createNodeItem(uiTreeData.children[i]);
    //    }
    // }
    // else
    // {
    //     node = this->_nodePools[this->_nodeIndex];
    //     Node *select = node->getChildByName("NodeTreeItemSelect");
    //     Node *fold = node->getChildByName("NodeTreeItemFold");
    //     Node *icon = node->getChildByName("NodeTreeItemIcon");
    //     Node *name = node->getChildByName("NodeTreeItemName");
    //     if (select != nullptr)
    //     {
    //         ndSelect = dynamic_cast<Node2D *>(select);
    //         spSelect = dynamic_cast<UISprite *>(ndSelect->getComponent("UISprite"));
    //     }
    //     if (fold != nullptr)
    //     {
    //         ndFold = dynamic_cast<Node2D *>(fold);
    //         spFold = dynamic_cast<UISprite *>(ndFold->getComponent("UISprite"));
    //     }
    //     if (icon != nullptr)
    //     {
    //         ndIcon = dynamic_cast<Node2D *>(icon);
    //         spIcon = dynamic_cast<UISprite *>(ndIcon->getComponent("UISprite"));
    //     }
    //     if (name != nullptr)
    //     {
    //         ndName = dynamic_cast<Node2D *>(name);
    //         txtName = dynamic_cast<UIText *>(ndName->getComponent("UIText"));
    //     }
    // }
    // node->setName(uiTreeData.name);
    // uiTreeData.ndBind = node;
    // uiTreeData.index = this->_nodeIndex;
    // this->_treeNodes.push_back(node);
    // this->_treeNodeDataMap.emplace(node->getUuid(), &uiTreeData);
    // // this->_foldUIMap.emplace(ndFold->getUuid(), &uiTreeData);
    // node->setActive(true);
    // ndSelect->setSize(1000.0f, this->_itemHeight);
    // ndSelect->setPosition(0.0f, 0.0f, 0.0f);
    // width += (uiTreeData.layer * offset);
    // // 折叠图标
    // ndFold->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
    // if (uiTreeData.isFold)
    // {
    //     TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-arrow-right.png");
    //     spFold->setTextureAsset(tex);
    // }
    // else
    // {
    //     TextureAsset *tex = BooEditor::cache->getEditorTexture("ic-arrow-bottom.png");
    //     spFold->setTextureAsset(tex);
    // }
    // if (uiTreeData.children.size() > 0)
    // {
    //     ndFold->setActive(true);
    // }
    // else
    // {
    //     ndFold->setActive(false);
    // }

    // width += ndFold->getSize().getWidth();
    // // 图标
    // ndIcon->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
    // TextureAsset *tex = BooEditor::cache->getEditorTexture(uiTreeData.icon);
    // spIcon->setTextureAsset(tex);

    // width += ndIcon->getSize().getWidth();
    // // 创建名字
    // txtName->setText(uiTreeData.name);
    // float itemNameWidth = ndName->getSize().getWidth();
    // float itemNameHight = ndName->getSize().getHeight();
    // ndName->setSize(itemNameWidth, itemNameHight);
    // width += itemNameWidth;
    // width += 5.0f * 3;

    // // 设置总宽度
    // if (width < size.getWidth() - 3)
    // {
    //     width = size.getWidth() - 3;
    // }
    // // 折叠按钮
    // float startX = -width / 2.0 + (uiTreeData.layer * offset);
    // ndFold->setPosition(startX + ndFold->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    // // 图标
    // startX += ndFold->getSize().getWidth() + 5.0f;
    // ndIcon->setPosition(startX + ndIcon->getSize().getWidth() / 2.0, 0.0f, 0.0f);
    // // 名字
    // startX += ndIcon->getSize().getWidth() + 5.0f;
    // ndName->setPosition(startX + itemNameWidth / 2.0, 0.0f, 0.0f);

    // node->setSize(width, this->_itemHeight);
    // node->setPosition(width / 2.0f, -this->_itemHeight / 2.0f - this->_nodeIndex * this->_itemHeight, 0.0f);
    // if (width > this->_contentWidth)
    // {
    //     this->_contentWidth = width;
    // }
    // // 递归更新子节点
    // this->_nodeIndex++;
    // if (!uiTreeData.isFold)
    // {
    //     for (int i = 0; i < uiTreeData.children.size(); i++)
    //     {
    //         this->_updateTreesItems(uiTreeData.children[i]);
    //     }
    // }
// }
void EditorHierarchyNodeTree::_createNodeItem(NodeTreeStructure &tree)
{
        // item 节点
       Node2D *node = new Node2D("NodeTreeItem");
        this->_ndContent->addChild(node);
        // 后续删除
        // sp = dynamic_cast<UISprite *>(node->addComponent("UISprite"));
        // sp->setColor(340.0f / 255.0f, 42.0f / 255.0f, 53.0f / 255.0f, 1.0f);
        // sp->setTextureAsset("resources/texture/ic-default.png");
        // sp->setMaterialAsset(nullptr);
        // node->onNodeInputEvent(NodeInput::TOUCH_END, &UITree::_onTreeItemTouchEvent, this);
        // node->onNodeInputEvent(NodeInput::CURSOR_HOVER, &UITree::_onTreeItemCursorHoverEvent, this);
        // 选择框
        Node2D *ndSelect = new Node2D("NodeTreeItemSelect");
        node->addChild(ndSelect);
        ndSelect->setSize(1000.0f, this->_itemHeight);
       UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect->addComponent("UISprite"));
        spSelect->setColor(0.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.0f);
        // 折叠符号
       Node2D * ndFold = new Node2D("NodeTreeItemFold");
        node->addChild(ndFold);
        ndFold->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
       UISprite * spFold = dynamic_cast<UISprite *>(ndFold->addComponent("UISprite"));
        spFold->setMaterialAsset(nullptr);
=        // 图标
       Node2D * ndIcon = new Node2D("NodeTreeItemIcon");
        node->addChild(ndIcon);
        ndIcon->setSize(this->_itemHeight * 0.8, this->_itemHeight * 0.8);
       UISprite * spIcon = dynamic_cast<UISprite *>(ndIcon->addComponent("UISprite"));
        spIcon->setMaterialAsset(nullptr);
        // 名字
      Node2D *  ndName = new Node2D("NodeTreeItemName");
        ndName->setSize(this->_itemHeight * 0.7, this->_itemHeight * 0.7);
        node->addChild(ndName);
       UIText * txtName = dynamic_cast<UIText *>(ndName->addComponent("UIText"));
        txtName->setColor(204.0f / 255.0f, 207.0f / 255.0f, 213.0f / 255.0f, 1.0f);
        txtName->setMaterialAsset(nullptr);
        this->_nodePools.push_back(node);
}






void EditorHierarchyNodeTree::_onTreeContentTouchEvent(NodeInputResult &result)
{
    Node2D *root = dynamic_cast<Node2D *>(this->_node);
    const Size &size = root->getSize();
    std::string uuid = result.node->getUuid();
    std::string name = result.node->getName();
    float worldX = result.worldX;
    float worldY = result.worldY;
    float localX = result.localX;
    float localY = result.localY;
    for (auto node : this->_treeNodes)
    {
        std::string name = node->getName();
        float h = size.getHeight();
        float _y = node->getPosition().getY();
        _y = _y - this->_topLen;
        _y = _y + h / 2.0f;
        if (localY >= _y - this->_itemHeight / 2.0f && localY <= _y + this->_itemHeight / 2.0f)
        {
            NodeTreeStructure *_tree = this->_treeNodeDataMap[node->getUuid()];
            // if (_tree != nullptr && _tree->children.size() > 0)
            // {
            //     // 判断是否点击了折叠按钮
            //     float foldX = -1 * node->getSize().getWidth() / 2.0f + _tree->layer * 10;
            //     float foldY = _y;
            //     if (localX >= foldX && localX <= foldX + ndFold->getSize().getWidth())
            //     {
            //         _tree->isFold = !_tree->isFold;
            //         this->_updateTreesItems(*_tree);
            //     }
            //     // if(localX>=startX&&localX<=startX+ndFold->getSize().getWidth()){
            //     //     _tree->isFold=!_tree->isFold;
            //     //     this->_updateTreesItems(*_tree);
            //     // }
            // }
            this->_refreshTreeItemState(node, 1);
            return;
        }
    }
    std::cout << "click null node: " << std::endl;
    this->_refreshTreeItemState(nullptr, 0);
}
void EditorHierarchyNodeTree::_onTreeContentHoverEvent(NodeInputResult &result)
{
    Node2D *root = dynamic_cast<Node2D *>(this->_node);
    const Size &size = root->getSize();
    std::string uuid = result.node->getUuid();
    std::string name = result.node->getName();
    float worldX = result.worldX;
    float worldY = result.worldY;
    float localX = result.localX;
    float localY = result.localY;
    for (auto node : this->_treeNodes)
    {
        std::string name = node->getName();
        float h = size.getHeight();
        float _y = node->getPosition().getY();
        _y = _y - this->_topLen;
        _y = _y + h / 2.0f;
        if (localY >= _y - this->_itemHeight / 2.0f && localY <= _y + this->_itemHeight / 2.0f)
        {
            this->_refreshTreeItemState(node, 2);
            return;
        }
    }
    this->_refreshTreeItemState(nullptr, 2);
}
void EditorHierarchyNodeTree::_refreshTreeItemState(Node2D *ndItem, int state)
{
    if (state == 0)
    {
        // 取消悬浮和选中
        this->_refreshTreeItemUI(this->_hoverTreeItem, 0);
        this->_refreshTreeItemUI(this->_selectTreeItem, 0);
        this->_hoverTreeItem = nullptr;
        this->_selectTreeItem = nullptr;
    }
    else if (state == 1)
    {
        // 选中
        NodeTreeStructure *_tree = this->_treeNodeDataMap[ndItem->getUuid()];
        if (_tree == nullptr)
        {
            return;
        }
        if (this->_selectTreeItem != nullptr && this->_selectTreeItem->uuid == _tree->uuid)
        {
            // 重命名
            this->_refreshTreeItemUI(this->_selectTreeItem, 1);
        }
        else
        {
            this->_refreshTreeItemUI(this->_selectTreeItem, 0);
            this->_selectTreeItem = _tree;
            this->_refreshTreeItemUI(this->_selectTreeItem, 1);
        }
    }
    else if (state == 2)
    {
        // 悬浮
        if (ndItem == nullptr)
        {
            if (this->_hoverTreeItem != nullptr && this->_selectTreeItem != nullptr && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
            {
                this->_hoverTreeItem = nullptr;
                return;
            }
            else
            {
                this->_refreshTreeItemUI(this->_hoverTreeItem, 0);
                this->_hoverTreeItem = nullptr;
            }
            return;
        }
        NodeTreeStructure *_tree = this->_treeNodeDataMap[ndItem->getUuid()];
        if (_tree == nullptr)
        {
            return;
        }
        // 重置上一个悬浮
        if (this->_hoverTreeItem != nullptr && this->_selectTreeItem != nullptr && this->_hoverTreeItem->uuid == this->_selectTreeItem->uuid)
        {
            // 排除选中的
            this->_hoverTreeItem = _tree;
            return;
        }
        this->_refreshTreeItemUI(this->_hoverTreeItem, 0);

        // 新的悬浮
        if (this->_selectTreeItem != nullptr && this->_selectTreeItem->uuid == _tree->uuid)
        {
            // 悬浮在选中的上面，不处理
            this->_hoverTreeItem = _tree;
            return;
        }
        else
        {
            this->_hoverTreeItem = _tree;
            this->_refreshTreeItemUI(this->_hoverTreeItem, 2);
        }
    }
}
void EditorHierarchyNodeTree::_refreshTreeItemUI(NodeTreeStructure *tree, int state)
{
    if (tree == nullptr)
    {
        return;
    }
    Node *ndItem = tree->ndBind;
    if (ndItem == nullptr)
    {
        return;
    }
    Node *ndSelect = ndItem->getChildByName("NodeTreeItemSelect");
    if (ndSelect == nullptr)
    {
        return;
    }
    Node2D *ndSelect2d = dynamic_cast<Node2D *>(ndSelect);
    if (ndSelect2d == nullptr)
    {
        return;
    }
    UISprite *spSelect = dynamic_cast<UISprite *>(ndSelect2d->getComponent("UISprite"));
    if (spSelect == nullptr)
    {
        return;
    }

    if (state == 0)
    {
        // 正常
        spSelect->setColor(82.0f / 250.0f, 82.0f / 255.0f, 82.0f / 255.0f, 0.0f);
    }
    else if (state == 1)
    {
        // 选中
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 1.0f);
    }
    else if (state == 2)
    {
        // 悬浮
        spSelect->setColor(9.0f / 250.0f, 74.0f / 255.0f, 93.0f / 255.0f, 0.3f);
    }
}

void EditorHierarchyNodeTree::Disable()
{
    Component::Disable();
    Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
    node2d->offNodeInputEvent(this->_contentTouchID);
    node2d->offNodeInputEvent(this->_contentHoverID);
}
void EditorHierarchyNodeTree::destroy()
{
    Component::destroy();
}
EditorHierarchyNodeTree::~EditorHierarchyNodeTree()
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