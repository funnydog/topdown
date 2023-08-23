#pragma once

#include <algorithm>
#include <glm/glm.hpp>

template <typename T>
struct Rect
{
	constexpr Rect();
	constexpr Rect(T pos, T size);

	template <typename U>
	explicit Rect(const Rect<U> &rectangle);

	T pos;
	T size;
};

typedef Rect<glm::vec2> FloatRect;
typedef Rect<glm::ivec2> IntRect;

template <typename T>
constexpr
Rect<T>::Rect()
	: pos()
	, size()
{
}

template <typename T>
constexpr
Rect<T>::Rect(T pos, T size)
	: pos(pos)
	, size(size)
{
}

template <typename T>
template <typename U>
Rect<T>::Rect(const Rect<U> &other)
	: pos(static_cast<T>(other.pos))
	, size(static_cast<T>(other.size))
{
}

template <typename T>
constexpr bool
operator==(const Rect<T> &lhs, const Rect<T> &rhs)
{
	return lhs.pos.x == rhs.pos.x
		&& lhs.pos.y == rhs.pos.y
		&& lhs.size.x == rhs.size.x
		&& lhs.size.y == rhs.size.y;
}

template <typename T>
constexpr bool
operator!=(const Rect<T> &lhs, const Rect<T> &rhs)
{
	return !(lhs == rhs);
}
