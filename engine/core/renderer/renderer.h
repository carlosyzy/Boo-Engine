#pragma once
#include <vector>
#include <unordered_map>
class Camera;
class Scene;
class Node;

#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

class Camera;
class Node;

class Renderer
{
private:
    void _renderCameras(Camera *camera, Scene *scene);
    void _walkNode(Camera *camera,Node *node);
public:
    Renderer(/* args */);
    void init();
    void render(std::unordered_map<std::string, Camera *> &cameras,Scene *scene);
    ~Renderer();
};
