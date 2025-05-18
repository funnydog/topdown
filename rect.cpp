#include "rect.hpp"

template<>
glm::vec2
Rect<glm::vec2>::center() const
{
	return size * 0.5f + pos;
}

template<>
glm::ivec2
Rect<glm::ivec2>::center() const
{
	return size / 2 + pos;
}
