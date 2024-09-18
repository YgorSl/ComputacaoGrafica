#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"

GLuint programa;
std::vector<Mesh*> listMesh;

//Vertex Shader
static const char* vShader = "                                \n\
#version 330                                                  \n\
                                                              \n\
layout(location=0) in vec3 pos;                               \n\
uniform mat4 model;                                           \n\
out vec4 vColor;                                              \n\
                                                              \n\
void main(){                                                  \n\
	gl_Position = model * vec4(pos, 1.0);                     \n\
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);              \n\
}";

static const char* fShader = "                                \n\
#version 330                                                  \n\
                                                              \n\
uniform vec3 triangleColor;                                   \n\
in vec4 vColor;                                               \n\
out vec4 color;                                               \n\
                                                              \n\
void main(){                                                  \n\
	color = vColor;                                           \n\
}";


void CriaTriangulos() {
	GLfloat vertices[] = {
		//x , y		
		-1.0f, 1.0f, 1.0f,         //Vertice 0 (Preto)
		-1.0f, -1.0f, 1.0f,           //Vertice 1 (Verde)
		1.0f, 1.0f, 1.0f,          //Vertice 2 (Vermelho)
		1.0f, -1.0f, 1.0f,            //Vertice 3 (Azul)
		-1.0f, 1.0f, -1.0f,            //Vertice 3 (Azul)
		-1.0f, -1.0f, -1.0f,            //Vertice 3 (Azul)
		1.0f, 1.0f, -1.0f,            //Vertice 3 (Azul)
		1.0f, -1.0f, -1.0f          //Vertice 2 (Vermelho)

	};

	GLuint indices[] = {
		0, 1, 5,
		0, 4, 5,

		1, 3, 5,
		3, 5, 7,

		4, 5, 7,
		4, 6, 7,

		0, 2, 6,
		0, 4, 6,

		2, 3, 6,
		3, 6, 7,

		0, 1, 2,
		1, 2, 3

	};


	Mesh* tri1 = new Mesh();
	Mesh* tri2 = new Mesh();

	tri1->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
	tri2->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));


	listMesh.push_back(tri1);
	listMesh.push_back(tri2);

}


void CompilaShader() {
	programa = glCreateProgram(); //Cria um programa
	GLuint _vShader = glCreateShader(GL_VERTEX_SHADER); //Cria um shader
	GLuint _fShader = glCreateShader(GL_FRAGMENT_SHADER); //Cria um shader

	//Gambiarra para converter Char em GLChar
	const GLchar* vCode[1];
	const GLchar* fCode[1];
	vCode[0] = vShader; //C�digo do vShader
	fCode[0] = fShader; //C�digo do fShader

	glShaderSource(_vShader, 1, vCode, NULL); //associa o shader ao codigo
	glCompileShader(_vShader); //Compila o shader
	glAttachShader(programa, _vShader); //Adiciona o shader ao programa

	glShaderSource(_fShader, 1, fCode, NULL); //associa o shader ao codigo
	glCompileShader(_fShader); //Compila o shader
	glAttachShader(programa, _fShader); //Adiciona o shader ao programa

	glLinkProgram(programa); //Adiciona o programa

}

int main() {
	if (!glfwInit()) {
		printf("GLFW: Nao pode ser iniciado");
		return 1;
	}

	GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Ola Mundo!", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW: Nao foi poss�vel criar a janela");
		glfwTerminate();
		return 1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	if (glewInit() != GLEW_OK) {
		printf("Glew: Nao pode ser iniciado!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);

	CriaTriangulos();
	CompilaShader();

	//Variaveis para controle da movimentação do triangulo
	bool direction = true, sizeDirection = true, angleDirection = true;
	//true=direita e false=esquerda
	float triOffset = 0.0f, maxOffset = 0.7f, minOffset = -0.7f, incOffset = 0.01f;
	float size = 0.4f, maxSize = 0.7f, minSize = -0.7f, incSize = 0.01f;
	float angle = 0.0f, maxAngle = 360.0f, minAngle = -1.0f, incAngle = 0.5f;

	while (!glfwWindowShouldClose(mainWindow)) {

		//Habilitar os eventos de usuario
		glfwPollEvents();

		glClearColor(1.0f, 0.75f, 0.79f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Desenha o triangulo
		glUseProgram(programa);
		listMesh[0]->RenderMesh();


		/*
		* Alterando a cor do triangulo
		*/
		GLuint uniColor = glGetUniformLocation(programa, "triangleColor");
		float r = (float)rand() / RAND_MAX;
		float g = (float)rand() / RAND_MAX;
		float b = (float)rand() / RAND_MAX;
		glUniform3f(uniColor, r, g, b);
		/*
		* Mover nosso triangulo
		*/
		//if (triOffset >= maxOffset || triOffset <= minOffset)
		//	direction = !direction;
		//triOffset += direction ? incOffset : incOffset * -1;

		//if (size >= maxSize || size <= minSize)
		//	sizeDirection = !sizeDirection;
		//size += sizeDirection ? incSize : incSize * -1;

		if (angle >= maxAngle || angle <= minAngle)
			angleDirection = !angleDirection;
		angle += angleDirection ? incAngle : incAngle * -1;

		//criar uma matriz 4x4 (1.0f)
		glm::mat4 model(1.0f);

		//Tamanho do triangulo
		model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));

		//Rotação
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 1.0f));

		GLuint uniModel = glGetUniformLocation(programa, "model");
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}