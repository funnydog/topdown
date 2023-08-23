#pragma once

#include <glm/glm.hpp>

class Canvas
{
public:
	virtual ~Canvas() = default;
	virtual glm::ivec2 getSize() const = 0;
	virtual bool setActive(bool active = true) = 0;
};
