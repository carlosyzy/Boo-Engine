# 五子棋游戏 API

## 核心组件

### GobangGame 组件

`GobangGame` 是五子棋游戏的核心组件，负责游戏逻辑和界面管理。

#### 方法

| 方法名 | 描述 | 参数 | 返回值 |
|--------|------|------|--------|
| `  OnAwake()` | 组件初始化，加载资源并创建棋盘 | 无 | 无 |
| `destroy()` | 组件销毁，清理资源 | 无 | 无 |
| `placePiece(int x, int y, int player)` | 在指定位置放置棋子 | x: 横坐标<br>y: 纵坐标<br>player: 玩家编号(1=黑, 2=白) | 无 |
| `checkWin(int x, int y, int player)` | 检查是否获胜 | x: 横坐标<br>y: 纵坐标<br>player: 玩家编号 | bool: 是否获胜 |
| `restartGame()` | 重新开始游戏 | 无 | 无 |

#### 属性

| 属性名 | 类型 | 描述 |
|--------|------|------|
| `BOARD_SIZE` | int | 棋盘大小，默认15 |
| `CELL_SIZE` | int | 单元格大小，默认40 |
| `OFFSET_X` | int | 棋盘X轴偏移量 |
| `OFFSET_Y` | int | 棋盘Y轴偏移量 |
| `m_currentPlayer` | int | 当前玩家 |
| `m_gameOver` | bool | 游戏是否结束 |

## 引擎 API

### Boo 引擎核心 API

| API | 描述 | 用途 |
|-----|------|------|
| `Boo::game->scheduleOnce()` | 调度一次性任务 | 实现AI思考时间 |
| `Boo::game->unschedule()` | 取消调度任务 | 清理定时器 |
| `Boo::assetManager->loadAsset()` | 加载资源 | 加载棋子图片 |
| `Boo::view->getDesignWidth()` | 获取设计宽度 | 背景尺寸适配 |
| `Boo::view->getDesignHeight()` | 获取设计高度 | 背景尺寸适配 |

### 节点和组件 API

| API | 描述 | 用途 |
|-----|------|------|
| `new Boo::Node2D()` | 创建2D节点 | 创建棋盘、线条、棋子 |
| `node->addComponent()` | 添加组件 | 添加UISprite组件 |
| `node->addChild()` | 添加子节点 | 构建场景层次 |
| `node->setSize()` | 设置节点大小 | 调整元素尺寸 |
| `node->setPosition()` | 设置节点位置 | 定位元素 |
| `sprite->setTexture()` | 设置纹理 | 显示棋子图片 |
| `sprite->setColor()` | 设置颜色 | 调整元素颜色 |

## 资源 API

| 资源路径 | 类型 | 用途 |
|----------|------|------|
| `gobang/textures/piece-black.png` | 纹理 | 黑棋图片 |
| `gobang/textures/piece-white.png` | 纹理 | 白棋图片 |
| `Boo::AssetBuiltinTexture::Default` | 内置纹理 | 背景和棋盘纹理 |

## 示例代码

### 创建五子棋场景

```cpp
void Gobang::createScene() {
    m_scene = new Boo::Scene("GobangScene");
    Boo::game->openScene(m_scene);
    
    auto root2D = m_scene->getRoot2D();
    
    auto cameraNode = new Boo::Node2D("Camera");
    root2D->addChild(cameraNode);
    cameraNode->addComponent("Camera");
    
    auto gameNode = new Boo::Node2D("GobangGame");
    root2D->addChild(gameNode);
    gameNode->addComponent("GobangGame");
}
```

### 注册五子棋场景

```cpp
void Launch::onLaunch() {
    LOGI("Launch::onLaunch");
    
    // 启用五子棋游戏
    Gobang gobang;
    gobang.createScene();
}
```