#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include"Shader.h"

struct Color
{
	float r, g, b, a;
};

Color background = { 0.0f, 0.0f, 0.0f, 1.0f };

void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);
	if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
		background = { 1.0f, 0.0f, 0.0f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
		background = { 0.0f, 1.0f, 0.0f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS)
		background = { 0.0f, 0.0f, 1.0f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS)
		background = { 0.0f, 0.8f, 0.7f, 1.0f };
	if (glfwGetKey(win, GLFW_KEY_5) == GLFW_PRESS)
		background = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void onResize(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
#pragma region WINDOW INITIALIZATION
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(900, 900, "My OpenGL Window", NULL, NULL);
	if (win == NULL)
	{
		std::cout << "Error.Couldn't create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error.Couldn't load GLAD!" << std::endl;
		glfwTerminate();
		return -1;
	}

	std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;
	std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glfwSetFramebufferSizeCallback(win, onResize);
#pragma endregion

	glViewport(0, 0, 900, 900);

#pragma region POLIGON INITIALIZATION

	const int verts = 4;

	float polygon[verts * (3 + 3)] = {
		   0.0f, 0.5f, 0.0f,    1.0f, 0.0f, 1.0f,
			0.5f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,
		   -0.5f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
		  0.0f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f

	};

	unsigned int vertexId[] = {
		0, 1, 2,
		1, 2, 3

	};

	unsigned int VBO_polygon, VAO_polygon, EBO_polygon;
	glGenBuffers(1, &VBO_polygon);
	glGenBuffers(1, &EBO_polygon);
	glGenVertexArrays(1, &VAO_polygon);

	glBindVertexArray(VAO_polygon);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glBufferData(GL_ARRAY_BUFFER, verts * 6 * sizeof(float), polygon, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), vertexId, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader* polygon_shader = new Shader("Shaders\\v_sh.vert", "Shaders\\f_sh.frag");

#pragma endregion

	float position[3] = { 1.0f, 0.0f, 1.0f };

	int stage = 0;

	while (!glfwWindowShouldClose(win))
	{
		position[0] = sin(glfwGetTime() * 2);
		position[1] = cos(glfwGetTime() * 2);
		position[2] = 0;

		processInput(win);

		glClearColor(background.r, background.g, background.b, background.a);
		glClear(GL_COLOR_BUFFER_BIT);

		polygon_shader->use();
		polygon_shader->setFloatVec("uniformPos", position, 3);

		glBindVertexArray(VAO_polygon);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	delete polygon_shader;

	glfwTerminate();
	return 0;
}