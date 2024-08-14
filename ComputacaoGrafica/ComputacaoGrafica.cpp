#include <stdio.h>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const GLint WIDTH = 800, HEIGHT = 600;

int main(){
	if (!glfwInit()) {
		printf("Não funcionou o GLFW");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computacao Grafica", NULL,NULL);
	if (!window)
	{
		printf("A janela não pode ser criada");
		glfwTerminate();
		return 1;
	}
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("Não foi iniciado");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	while (!glfwWindowShouldClose(window)) {
		//cria um evento
		glfwPollEvents();
		//define a cor do fundo
		glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	
}

