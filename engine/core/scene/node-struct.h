#pragma once
enum class NodeVisibility
{
	Default = 1 << 0,
	Node3D = 1 << 1,
	Node2D = 1 << 10,
};