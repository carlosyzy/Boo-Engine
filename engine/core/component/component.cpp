#include "component.h"
#include "boo.h"
#include "log.h"
#include "core/util/uuid-util.h"
#include "core/scene/node.h"

namespace Boo
{

    Component::Component(std::string name, Node *node, std::string uuid) : _isAwaked(false),
                                                                           _uuid(uuid),
                                                                           _name(name),
                                                                           _layer(EComponentLayer::Default),
                                                                           _node(node),
                                                                           _isEnabled(true),
                                                                           _isEnabledInHierarchy(false)

    {
        if (this->_uuid.size() <= 0)
        {
            this->_uuid = UuidUtil::generateUUID();
        }
    }
    void Component::setProperty(json &data)
    {
        // 从json中获取组件的属性
        // uuid
        if (data.contains("uuid") && data["uuid"].is_string())
        {
            this->_uuid = data["uuid"].get<std::string>();
        }
        // 可见enable
        if (data.contains("enable") && data["enable"].is_boolean())
        {
            this->setEnabled(data["enable"].get<bool>());
        }
    }

    void Component::setEnabled(bool enabled)
    {
        this->_isEnabled = enabled;
        if (this->_node == nullptr)
            return;
        this->setNodeActiveInHierarchy(this->_node->getActiveInHierarchy());
    }

    void Component::setNodeActiveInHierarchy(bool isActiveInHierarchy)
    {
        bool isEnableInHierarchy = isActiveInHierarchy && this->_isEnabled;
        if (this->_isEnabledInHierarchy == isEnableInHierarchy)
        {
            return; // 状态未改变
        }
        this->_isEnabledInHierarchy = isEnableInHierarchy;
        if (this->_isEnabledInHierarchy)
        {
            if (!this->_isAwaked)
            {
                this->_isAwaked = true;
                this->OnAwake();
            }
            this->OnEnable();
        }
        else
        {
            this->OnDisable();
        }
    }
    /**
     * @brief 组件唤醒函数
     * 组件被添加到节点后,第一次激活会调用Awake函数
     */
    void Component::OnAwake()
    {
    }
    /**
     * @brief 组件启用函数
     * 组件被激活后,会调用Enable函数
     */
    void Component::OnEnable()
    {
    }

    void Component::Update(float deltaTime)
    {
    }
    void Component::LateUpdate(float deltaTime)
    {
    }
    /**
     * @brief 组件禁用函数
     * 组件被禁用后,会调用Disable函数
     */
    void Component::OnDisable()
    {
    }
    void Component::destroy()
    {
        game->addCompClearCaches(this);
    }

    Component::~Component()
    {
        this->_node = nullptr;
    }

} // namespace Boo
