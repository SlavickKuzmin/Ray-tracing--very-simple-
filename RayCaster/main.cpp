#include <stdio.h>
#include <stdlib.h>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <gl/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Color.h"
#include "RayTracker.h"
#include "TimeMeasure.h"

const int screen_width = 800;
const int screen_height = 500;

void initGLEW();

int main(int argc, char **argv)
{
	initGLEW();

	// Open a window and create its OpenGL context
	GLFWwindow* window;
	window = glfwCreateWindow(screen_width, screen_height, "Simple ray tracker", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	RayTracker rayTracker(screen_width, screen_height);

	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	Material      ivory(1.0, Vec4f(0.6, 0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3), 50.);
	Material      glass(1.5, Vec4f(0.0, 0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8), 125.);
	Material      blue(1.0, Vec4f(0.7, 0.5, 0.3, 0.2), Vec3f(0.6, 0.7, 0.8), 500.);
	Material red_rubber(1.0, Vec4f(0.9, 0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1), 10.);
	Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

	rayTracker.AddSphere(Sphere(Vec3f(-3, 0, -16), 2, ivory));
	rayTracker.AddSphere(Sphere(Vec3f(-1.0, -1.5, -12), 2, glass));
	rayTracker.AddSphere(Sphere(Vec3f(1.5, -0.5, -18), 3, red_rubber));
	rayTracker.AddSphere(Sphere(Vec3f(7, 8, -28), 8, mirror));
	rayTracker.AddSphere(Sphere(Vec3f(-10, 10, -20), 1, mirror));
	rayTracker.AddSphere(Sphere(Vec3f(-2, 1, -7), 1, blue));

	rayTracker.AddLight(Light(Vec3f(-20, 20, 20), 1.5));
	rayTracker.AddLight(Light(Vec3f(30, 50, -25), 1.8));
	rayTracker.AddLight(Light(Vec3f(30, 20, 30), 1.7));

	rayTracker.SetDepth(4);

	Time::TimeMeasure perf;

	perf.StartMeasure();

	rayTracker.DoTracking();

	perf.StopMeasure();

	std::cout << "Render duration: " << perf.GetDifference().count() << " sec." << std::endl;

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClearColor(0, 0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(screen_width, screen_height, GL_RGBA, GL_UNSIGNED_BYTE, rayTracker.GetScreen().GetScreen());
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return 0;
}

/*!
 Initialize a GLSW library.
*/
void initGLEW()
{
	glewExperimental = GL_TRUE;
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		abort();
	}
}