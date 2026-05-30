#include "camera.h"
#include "boo.h"
#include "log.h"
#include "core/gfx/gfx-manager.h"
#include "core/gfx/base/gfx-render-texture.h"
#include "core/renderer/renderer-mgr.h"

namespace Boo
{

    Camera::Camera(std::string name, Node *node, std::string uuid) : Component(name, node, uuid),
                                                                     _priority(0),
                                                                     _groupIds(0),
                                                                     _projection(ECameraProjection::Perspective),
                                                                     _fov(60.0f),
                                                                     _nearClip(0.1f),
                                                                     _farClip(2000.0f),
                                                                     _orthoHeight(1.0f),
                                                                     _isOnScreen(true)

    {
        this->_matView = Mat4::identity();
        this->_matProj = Mat4::identity();
        this->_layer = EComponentLayer::Default;
    }
    void Camera::OnAwake()
    {
        Component::OnAwake();
        this->_createRenderPipeline();
    }
    void Camera::setProperty(json &data)
    {
        Component::setProperty(data);
        // 优先级
        if (data.contains("priority") && data["priority"].is_number_integer())
        {
            this->_priority = data["priority"].get<int>();
        }
        // 分组
        if (data.contains("groupIds") && data["groupIds"].is_number_integer())
        {
            this->_groupIds = data["groupIds"].get<int>();
        }
        // 相机类型
        if (data.contains("projection") && data["projection"].is_number_integer())
        {
            this->_projection = (ECameraProjection)data["projection"].get<int>();
        }
        // FOV
        if (data.contains("fov") && data["fov"].is_number())
        {
            this->_fov = data["fov"].get<float>();
        }
        // 近裁剪平面
        if (data.contains("nearClip") && data["nearClip"].is_number())
        {
            this->_nearClip = data["nearClip"].get<float>();
        }
        // 远裁剪平面
        if (data.contains("farClip") && data["farClip"].is_number())
        {
            this->_farClip = data["farClip"].get<float>();
        }
        // 正交高度
        if (data.contains("orthoHeight") && data["orthoHeight"].is_number())
        {
            this->_orthoHeight = data["orthoHeight"].get<float>();
        }
        // 是否可见
        if (data.contains("isOnScreen") && data["isOnScreen"].is_boolean())
        {
            this->_isOnScreen = data["isOnScreen"].get<bool>();
        }
        this->_updateProjectionMatrix();
    }
    void Camera::_createRenderPipeline()
    {
        GfxManager::getInstance()->createRenderQueue(this->_uuid, this->_priority, view->getWidth(), view->getHeight());
    }
    void Camera::OnEnable()
    {
        Component::OnEnable();
        this->_updateProjectionMatrix();
        RendererMgr::getInstance()->mountCamera(this);
    }
    void Camera::setIsOnScreen(bool isOnScreen)
    {
        this->_isOnScreen = isOnScreen;
    }
    bool Camera::getIsOnScreen()
    {
        return this->_isOnScreen;
    }
    void Camera::updateViewSize()
    {
        GfxManager::getInstance()->resizeRenderQueue(this->_uuid, view->getWidth(), view->getHeight());
        this->_updateProjectionMatrix();
    }
    void Camera::setPriority(int priority)
    {
        this->_priority = priority;
        GfxManager::getInstance()->setRenderQueuePriority(this->_uuid, this->_priority);
    }
    int Camera::getPriority()
    {
        return this->_priority;
    }
    void Camera::setGroupIDs(int groupIds)
    {
        this->_groupIds = groupIds;
    }
    void Camera::addGroupID(int groupId)
    {
        this->_groupIds |= groupId;
    }
    int Camera::getGroupIDs()
    {
        return this->_groupIds;
    }
    void Camera::addGroupID(ENodeGroup groupId)
    {
        this->_groupIds |= (int)groupId;
    }

    void Camera::setProjection(ECameraProjection projection)
    {
        this->_projection = projection;
        this->_updateProjectionMatrix();
    }
    void Camera::Update(float deltaTime)
    {
        Component::Update(deltaTime);
    }
    void Camera::LateUpdate(float deltaTime)
    {
        Component::LateUpdate(deltaTime);
        if (this->_node->hasFrameTransformFlag())
        {
            this->_updateViewMatrix();
        }
    }
    void Camera::Render()
    {
        const Vec3 &position = this->_node->getWorldPosition();
        std::array<float, 4> cameraPosition = {position.getX(), position.getY(), position.getZ(), 0.0f};
        GfxManager::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), this->_isOnScreen, cameraPosition);
    }
    void Camera::_updateViewMatrix()
    {
        // 1. 获取节点世界矩阵的逆作为初始视图矩阵
        const Mat4 &worldMat = this->_node->getWorldMatrix();
        Mat4::inverse(worldMat, this->_matView);
        // 2. 从逆矩阵中提取 forward 向量（第2列的负值）
        // 列主序: m[col*4+row]
        // m02 = 第0列第2行, m06 = 第1列第2行, m10 = 第2列第2行
        Vec3 forward;
        forward.setX(-this->_matView.getM2());
        forward.setY(-this->_matView.getM6());
        forward.setZ(-this->_matView.getM10());

        // 3. 移除缩放: matView = matView × ScaleMatrix
        const Vec3 &worldScale = this->_node->getWorldScale();
        Mat4 scaleMat;
        scaleMat.scale(worldScale);
        Mat4 tempMat;
        Mat4::multiply(this->_matView, scaleMat, tempMat);
        this->_matView = tempMat;
    }
    void Camera::_updateProjectionMatrix()
    {
        Mat4 worldMat = this->_node->getWorldMatrix();
        if (this->_projection == ECameraProjection::Ortho)
        {
            float left = -(float)view->getWidth() / 2.0f;
            float right = (float)view->getWidth() / 2.0f;
            float bottom = -(float)view->getHeight() / 2.0f;
            float top = (float)view->getHeight() / 2.0f;
            this->_nearClip = -1000.0f;
            this->_farClip = 1000.0f;
            Mat4::ortho(this->_matProj, left, right, bottom, top, this->_nearClip, this->_farClip, -1.0f, 0);
        }
        else if (this->_projection == ECameraProjection::Perspective)
        {
            float fovRad = this->_fov * 3.14159f / 180.0f;
            float aspectRatio = (float)view->getWidth() / (float)view->getHeight();
            Mat4::perspective(this->_matProj, fovRad, aspectRatio, this->_nearClip, this->_farClip, -1.0f, 0);
        }
    }
    void Camera::OnDisable()
    {
        Component::OnDisable();
        RendererMgr::getInstance()->unmountCamera(this);
    }
    void Camera::OnDestroy()
    {
        GfxManager::getInstance()->delRenderQueue(this->_uuid);
    }
    void Camera::destroy()
    {
        Component::destroy();
    }
    Camera::~Camera()
    {
    }

} // namespace Boo
