#pragma once

namespace Category
{
enum Type
{
	None   = 0,
	Scene  = 1 << 0,
	Player = 1 << 1,
	Enemy  = 1 << 2,
};
}
