#pragma once

#include <glfw3.h>

#include "debug_logger.h"

namespace gl_impl {

struct window_metadata
{
	int width, height;
	const char *title;
};

class window
{
public:
	window(const window_metadata &wm)
		: created_(false), glwindow_(nullptr), data_(wm)
	{}

	void setup_window()
	{
		if (glfwInit() != GLFW_TRUE)
			debug::print(debug::sig::err, "GLFW failed to initialize");

		glwindow_ = glfwCreateWindow(data_.width, data_.height, data_.title, NULL, NULL);
		created_ = true;

		glfwMakeContextCurrent(glwindow_);
		glfwSetInputMode(glwindow_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSwapInterval(1);
	}

	void cursor_pos(double *x, double *y)
	{
		glfwGetCursorPos(glwindow_, x, y);
	}

	int is_pressed(int key)
	{
		int state = glfwGetKey(glwindow_, key);
		return state;
	}

	void flip()
	{
		glfwSwapBuffers(glwindow_);
	}

	void poll_events()
	{
		glfwPollEvents();
		//glfwWaitEvents();
	}

	void clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool should_close()
	{
		return glfwWindowShouldClose(glwindow_);
	}

	~window()
	{
		if (created_)
			glfwTerminate();
	}

private:
	bool created_;
	GLFWwindow *glwindow_;
	window_metadata data_;
};

}