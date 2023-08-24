#pragma once

#include <cassert>
#include <functional>
#include <queue>

#include "category.hpp"
#include "time.hpp"

class SceneNode;

struct Command
{
	std::function<void(SceneNode&, Time)> action;
	unsigned category = Category::None;
};

template <typename Object, typename Function>
std::function<void(SceneNode&, Time)> derivedAction(Function fn)
{
	return [=](SceneNode &node, Time dt)
	{
		assert(dynamic_cast<Object*>(&node) != nullptr);
		fn(static_cast<Object&>(node), dt);
	};
}

typedef std::queue<Command> CommandQueue;
