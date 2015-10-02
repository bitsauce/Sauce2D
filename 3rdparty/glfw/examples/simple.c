//========================================================================
// Simple GLFW example
// Copyright (c) Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <exception>
#include <sstream>
#include <thread>
#include <mutex>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <memory>
#include <queue>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

#define SCREEN_W 1980
#define SCREEN_H 1200

double xpos = 0, xposPrev = 0;

double lerp(const double v0, const double v1, const double t)
{
	return v0 * (1.0 - t) + v1 * t;
}

void update(double dt)
{
	xposPrev = xpos;
	xpos = fmod(xpos + 20.0, SCREEN_W);
}

void display(double alpha)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(32.0);

	glBegin(GL_POINTS);
	glVertex2f(lerp(xposPrev, xpos, alpha), SCREEN_H / 2);
	glEnd();
}

void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, w, h, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

class Timer
{
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	bool m_running;

	int m_ticksPerSec;

public:
	Timer() :
		m_ticksPerSec(CLOCKS_PER_SEC),
		m_running(false)
	{
		QueryPerformanceFrequency(&m_frequency);
		m_start.QuadPart = m_end.QuadPart = 0;
	}

	long getTickCount() const
	{
		return clock();
	}

	void start()
	{
		m_running = true;
		QueryPerformanceCounter(&m_start);
	}

	void stop()
	{
		QueryPerformanceCounter(&m_end);
		m_running = false;
	}

	double getElapsedTime() const
	{
		LARGE_INTEGER end = m_end;
		if(m_running)
		{
			QueryPerformanceCounter(&end);
		}

		return (end.QuadPart - m_start.QuadPart) / double(m_frequency.QuadPart);
	}
};

int main(void)
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Simple example", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwSetFramebufferSizeCallback(window, reshape);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

	reshape(window, SCREEN_W, SCREEN_H);

    glfwSetKeyCallback(window, key_callback);


	Timer timer;

	// Setup game loop
	timer.start();
	const double dt = 1.0 / 30.0;
	double accumulator = 0.0;
	double prevTime = timer.getElapsedTime();

	// Lets make sure update is called once before draw
	update(dt);

	// Game loop
	while(!glfwWindowShouldClose(window))
	{
		// Process game events

		// Calculate time delta
		const double currentTime = timer.getElapsedTime();
		double deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		//LOG("%f", deltaTime);

		// Apply time delta to accumulator
		accumulator += deltaTime;
		while(accumulator >= dt)
		{
			// Update the game
			update(dt);
			accumulator -= dt;
		}

		// Draw the game
		display(accumulator / dt);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

//! [code]
