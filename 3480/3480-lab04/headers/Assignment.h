#pragma once

#include "globals.h"
#include "imgui.h"

class Texture;

class Assignment {
	public:
		std::string name;

		Assignment() {}
		Assignment(const std::string& _name) : name(_name) { }
		virtual ~Assignment() { }

		virtual void render() { }
		virtual void render(s_ptr<Texture> screen) { }
		virtual void renderUI() { }
};