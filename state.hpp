#pragma once

#include <memory>

#include "event.hpp"
#include "rendertarget.hpp"
#include "resources.hpp"
#include "time.hpp"

class State
{
public:
	typedef std::unique_ptr<State> Ptr;
public:
	virtual ~State() = default;
	virtual bool update(Time dt) = 0;
	virtual bool handleEvent(const Event &event) = 0;
	virtual void draw(RenderTarget &target) = 0;
};
