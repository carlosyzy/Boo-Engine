#pragma once
#include <iostream>
#include <string>

class Node;
enum class NodeLayer;

class Component
{

protected:
    std::string _uuid;
    std::string _name;
    NodeLayer _layer;
    Node *_node;
    bool _isEnabled = true;
    int _priority;
public:
    Component(Node *node, std::string uuid = "");
    bool isEnabled() { return this->_isEnabled; }
    void setEnabled(bool enabled);
    Node *getNode() { return this->_node; }
    NodeLayer layer() { return this->_layer; }
    virtual void update(float deltaTime) = 0;
    virtual void lateUpdate(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void destroy();
    virtual ~Component();
};
