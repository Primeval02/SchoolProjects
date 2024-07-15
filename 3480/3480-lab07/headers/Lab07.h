#pragma once

#include "Assignment.h"

class Lab07 : public Assignment {
public:
	Lab07() : Assignment("Lab 07", true) { }
	virtual ~Lab07() { }

	virtual void init();
	virtual void render(s_ptr<Framebuffer> framebuffer);
	virtual void renderUI();
};
