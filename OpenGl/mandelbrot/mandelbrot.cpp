#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define STRINGFY(s) "#version 410 core\n"#s

static const char* vs = STRINGFY(
layout(location = 0) in vec2 pos;

void main(){
	gl_Position = vec4(pos, 0.0, 1.0);
}
);

static const char* fs = STRINGFY(

uniform int itr;
uniform double zoom;
uniform dvec2 screenSize;
uniform dvec2 offset;

double n = 0.0;
double threshold = 100.0;

double mandelbrot(dvec2 c){
	dvec2 z = vec2(0.0, 0.0);
	for(int i = 0; i < itr; i++){
		dvec2 znew;
		znew.x = (z.x * z.x) - (z.y * z.y) + c.x;
		znew.y = (2.0 * z.x * z.y) + c.y;
		z = znew;
		if((z.x * z.x) + (z.y * z.y) > threshold)
			break;
		n++;
	}
	return n / float(itr);
}

vec4 mapToColor(float t){
	float r = 9.0 * (1.0 - t) * t * t * t;
	float g = 15.0 * (1.0 - t) * (1.0 - t) * t * t;
	float b = 8.5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

	return vec4(r, g, b, 1.0);
}

void main(){
	dvec2 coord = dvec2(gl_FragCoord.xy);
	double t = mandelbrot(((coord - screenSize/2)/zoom) - offset);
	if(gl_FragCoord.x < 40){
		gl_FragColor = vec4(1.0);
	}
	gl_FragColor = mapToColor(float(t));
}
);

int WIDTH = 800,
	HEIGHT = 600;

int itr = 200;
double zoom = 100.0,
	   offsetX = 0.0,
	   offsetY = 0.0;

bool dragging = false;
double oldX, oldY;

unsigned int shaderProgram;

void keyCallback(GLFWwindow* window, int key, int scan, int action, int m);
void mouseCallback(GLFWwindow* window, int button, int action, int m);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void windowSizeCallback(GLFWwindow* window, int width, int height);

int main(){
	if(!glfwInit())
		LOG("GLFW: falha ao iniciar.");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Conjunto Mandelbrot", NULL, NULL);

	if(!window){
		LOG("GLFW: falha ao criar janela.");
		glfwTerminate();
	}

	glfwSetErrorCallback([](int e, const char* s) {std::cerr << s << std::endl;});
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	glfwMakeContextCurrent(window);

	if(glewInit() != 0)
		LOG("GLEW: falha ao iniciar.");

	float vertices[] = {
		-1.0f, 	-1.0f,
		1.0f,	-1.0f,
		1.0f,	1.0f,

		1.0f,	1.0f,
		-1.0f,	1.0f,
		-1.0f,	-1.0f,
	};

	unsigned int vao, vbo,
				 vertex, fragment;
	int sucess;
	char info[1024];

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &sucess);
	if(!sucess){
		glGetShaderInfoLog(vertex, 1024, NULL, info);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR do tipo: VERTEX\n" << info 
		<< "\n --------------------------------------------------------- " << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &sucess);
	if(!sucess){
		glGetShaderInfoLog(fragment, 1024, NULL, info);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR do tipo: FRAGMENT\n" << info
		<< "\n --------------------------------------------------------- " << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glUseProgram(shaderProgram);

	glUniform2d(glGetUniformLocation(shaderProgram, "screenSize"), (double)WIDTH, (double)HEIGHT);
	glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
	glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
	glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scan, int action, int m){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	if(key == GLFW_KEY_KP_0 && action == GLFW_PRESS) {
		itr = 200;
		zoom = 100.0;
		offsetX = 0.0;
		offsetY = 0.0;
	}

	if(key == GLFW_KEY_A && action == GLFW_PRESS)
		offsetX += 20 / zoom;
	else if(key == GLFW_KEY_D && action == GLFW_PRESS)
		offsetX -= 20 / zoom;
	else if(key == GLFW_KEY_W && action == GLFW_PRESS)
		offsetY -= 20 / zoom;
	else if(key == GLFW_KEY_S && action == GLFW_PRESS)
		offsetY += 20 / zoom;

	if(key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		zoom *= 2;
	else if(key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		zoom /= 2;

	if(key == GLFW_KEY_Q && action == GLFW_PRESS)
		itr += 50;
	else if(key == GLFW_KEY_E && action == GLFW_PRESS)
		(itr > 100)? itr -= 50 : itr = 50;

	glUniform1i(glGetUniformLocation(shaderProgram, "itr"), itr);
	glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
	glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
}

void mouseCallback(GLFWwindow* window, int button, int action, int m){
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		glfwGetCursorPos(window, &oldX, &oldY);
		dragging = true;
	} else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		dragging = false;
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos){
	if(dragging){
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		offsetX += (xPos - oldX) / zoom;
		offsetY += (yPos - oldY) / zoom;

		oldX = xPos;
		oldY = yPos;

		glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset){
	if(yOffset != 0){
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		double dx = (xPos - WIDTH / 2) / zoom - offsetX;
		double dy = (-yPos / 2) / zoom - offsetY;

		offsetX = -dx;
		offsetY = -dy;
		(yOffset < 0)? zoom /= 1.2 : zoom *= 1.2;

		dx = (xPos - WIDTH / 2) / zoom;
		dy = (-yPos / 2) / zoom;
		offsetX += dx;
		offsetY += dy;

		glUniform1d(glGetUniformLocation(shaderProgram, "zoom"), zoom);
		glUniform2d(glGetUniformLocation(shaderProgram, "offset"), offsetX, offsetY);
	}
}

void windowSizeCallback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
	glUniform2d(glGetUniformLocation(shaderProgram, "screenSize"), (double)WIDTH, (double)HEIGHT);
}