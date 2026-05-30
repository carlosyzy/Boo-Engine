# 五子棋游戏示例

## 基本用法

### 1. 创建五子棋场景

```cpp
#include "gobang.h"
#include "engine/boo.h"

void createGobangGame() {
    Gobang gobang;
    gobang.createScene();
}
```

### 2. 注册到启动流程

在 `launch.cpp` 中注册五子棋场景：

```cpp
#include "launch.h"
#include "engine/boo.h"
#include "gobang/gobang.h"

void Launch::onLaunch() {
    LOGI("Launch::onLaunch");
    
    // 启用五子棋游戏
    Gobang gobang;
    gobang.createScene();
}
```

## 自定义配置

### 修改棋盘大小

```cpp
// 在 GobangGame 类中修改
const int BOARD_SIZE = 19; // 改为19x19棋盘
```

### 修改棋子大小

```cpp
// 在 GobangGame 类中修改
const int CELL_SIZE = 30; // 改为30像素的单元格
```

### 修改AI思考时间

```cpp
// 在 scheduleNextMove 方法中修改
m_aiTimerId = Boo::game->scheduleOnce(&GobangGame::aiTurn, this, 1.0f); // 改为1秒
```

## 高级用法

### 1. 添加玩家交互

```cpp
// 在 GobangGame 类中添加触摸事件处理
void GobangGame::  OnAwake() {
    // 现有代码...
    
    // 添加触摸事件
    auto node = getNode();
    node->onTouch([this](Boo::TouchEvent event) {
        if (event.type == Boo::TouchType::Ended) {
            // 处理触摸事件
            int x = (event.x + OFFSET_X) / CELL_SIZE;
            int y = (OFFSET_Y - event.y) / CELL_SIZE;
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                placePiece(x, y, m_currentPlayer);
            }
        }
    });
}
```

### 2. 实现更智能的AI

```cpp
void GobangGame::aiTurn() {
    if (m_gameOver) return;
    
    // 简单的AI逻辑：优先攻击
    int bestX = -1, bestY = -1;
    int bestScore = -1;
    
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (m_board[y][x] == 0) {
                // 评估位置分数
                int score = evaluatePosition(x, y, m_currentPlayer);
                if (score > bestScore) {
                    bestScore = score;
                    bestX = x;
                    bestY = y;
                }
            }
        }
    }
    
    if (bestX != -1 && bestY != -1) {
        std::cout << "AI " << (m_currentPlayer == 1 ? "Black" : "White") 
                  << " places at (" << bestX << ", " << bestY << ")" << std::endl;
        placePiece(bestX, bestY, m_currentPlayer);
    } else {
        // 随机落子作为备选
        int x, y;
        do {
            x = std::rand() % BOARD_SIZE;
            y = std::rand() % BOARD_SIZE;
        } while (m_board[y][x] != 0);
        
        std::cout << "AI " << (m_currentPlayer == 1 ? "Black" : "White") 
                  << " places at (" << x << ", " << y << ")" << std::endl;
        placePiece(x, y, m_currentPlayer);
    }
}

int GobangGame::evaluatePosition(int x, int y, int player) {
    // 简单的位置评估逻辑
    int score = 0;
    
    // 检查四个方向
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    
    for (auto& dir : directions) {
        int count = 1;
        int space = 0;
        
        // 向一个方向检查
        for (int i = 1; i < 5; i++) {
            int nx = x + dir[0] * i;
            int ny = y + dir[1] * i;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                if (m_board[ny][nx] == player) {
                    count++;
                } else if (m_board[ny][nx] == 0) {
                    space++;
                    break;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        
        // 向相反方向检查
        for (int i = 1; i < 5; i++) {
            int nx = x - dir[0] * i;
            int ny = y - dir[1] * i;
            if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                if (m_board[ny][nx] == player) {
                    count++;
                } else if (m_board[ny][nx] == 0) {
                    space++;
                    break;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        
        // 根据连续棋子数量计算分数
        if (count >= 5) {
            score += 10000; // 获胜
        } else if (count == 4 && space >= 1) {
            score += 1000; // 冲四
        } else if (count == 3 && space >= 2) {
            score += 100; // 活三
        } else if (count == 2 && space >= 2) {
            score += 10; // 活二
        }
    }
    
    return score;
}
```

### 3. 添加游戏状态显示

```cpp
void GobangGame::createBoardVisual() {
    // 现有代码...
    
    // 添加游戏状态文本
    auto statusNode = new Boo::Node2D("StatusText");
    node->addChild(statusNode);
    statusNode->setPosition(0, 320, 0);
    
    auto statusText = dynamic_cast<Boo::UIText*>(statusNode->addComponent("UIText"));
    if (statusText) {
        statusText->setText("Gobang Game");
        statusText->setFontSize(24);
        statusText->setColor(0.3f, 0.3f, 0.3f, 1.0f);
    }
}

void GobangGame::placePiece(int x, int y, int player) {
    // 现有代码...
    
    // 更新游戏状态
    auto node = getNode();
    auto statusNode = node->findChild("StatusText");
    if (statusNode) {
        auto statusText = dynamic_cast<Boo::UIText*>(statusNode->getComponent("UIText"));
        if (statusText) {
            std::string playerStr = (m_currentPlayer == 1) ? "Black" : "White";
            statusText->setText("Turn: " + playerStr);
        }
    }
}
```

## 故障排除

### 1. 棋子图片不显示

- 确保图片路径正确：`gobang/textures/piece-black.png` 和 `gobang/textures/piece-white.png`
- 确保在 `  OnAwake()` 方法中调用了 `Boo::assetManager->loadAsset()` 加载资源

### 2. 游戏逻辑错误

- 检查棋盘坐标系统：`m_board[y][x]` 表示行和列
- 确保胜负判断逻辑正确：检查四个方向的连续棋子
- 验证AI落子逻辑：确保只在空位落子

### 3. 性能问题

- 对于大棋盘，考虑优化AI算法
- 使用对象池管理棋子节点，减少内存开销
- 避免在每帧中进行复杂计算