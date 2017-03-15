﻿#include "Application.h"
#include "Model/Scene.h"
#include "System/system.hpp"

#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace std;

// static fields
std::unique_ptr<Models::Scene> Application::scene = std::make_unique<Models::Scene>();
GLFWwindow* Application::window = nullptr;

void glfwErrorCallback(int errorCode, const char* errorDescription) {
    std::cout << "glfw error (" << errorCode << ") " << errorDescription << std::endl;
}

void Application::initialize(int* argc, char ** argv) {
	glfwInit();

	// Set all the required options for GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    
    //glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	Screen::width = 640;
	Screen::height = 480;

	window = glfwCreateWindow(Screen::width, Screen::height, "Rudy", nullptr, nullptr);
	if (window == nullptr) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// init glad
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << glGetString(GL_VERSION) << std::endl;
    }
    else {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return;
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glfwGetFramebufferSize(window, &Screen::width, &Screen::height);
	glViewport(0, 0, Screen::width, Screen::height);

	glPolygonMode(GL_FRONT, GL_FILL);

	// keyboard callback
	glfwSetKeyCallback(window, processKeyboard);
	// mouse move callback
	glfwSetCursorPosCallback(window, processMouseFreeMove);
	// hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// hide cursor and lock to window
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::runMainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
		renderScene();
	}

	glfwTerminate();
}

void Application::renderScene() {
	Time::updateClock();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto mainCamera = Camera::getMainCamera();
    // todo: Clear method must be defined within Camera itself (with solid color, skybox or no clear)
	//if(mainCamera != nullptr)
	//	mainCamera->clearWithSkybox();

	//
	for(auto it = scene->begin(); it != scene->end(); ++it)
	{
		drawGameObject(*it->second);
		// todo: there maybe must be base Update call for Component
		// Component::Update();
		it->second->Update();
	}
}

void Application::drawGameObject(GameObject& gameObject)
{
	if (!gameObject.renderer)
		return;

    gameObject.renderer->update();
	gameObject.renderer->render();
}

void Application::processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << " scancode: " << scancode << " action: " << " " << action << " mode: " << mode << endl;

	if(action == GLFW_PRESS)
		Input::registerKeyPressed(key);
	if (action == GLFW_RELEASE)
		Input::resetKeyPressed(key);

	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::processMousePress(int button, int state, int x, int y)
{
	//std::cout << "button: " << button << " state:" << state << " x: "<<  x << " y: " << y;
}

void Application::processMouseFreeMove(GLFWwindow* window, double x, double y)
{
	Input::mousePosition.x = static_cast<float>(x);
	Input::mousePosition.y = static_cast<float>(y);

	//std::cout << x << ", " << y << std::endl;
}

void Application::processMousePressedMove(int x, int y)
{
}

void Application::leaveMainLoop()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

void Application::setMouseCursorToCenter()
{
	glfwSetCursorPos(window, Screen::width / 2, Screen::height / 2);
}

void Application::exit()
{
	// fixme
	// todo: create destroyable scene api
	// Good enough will for GameObject to use something like this:
	// Engine::scene.AddModel();
	// rename Application to Engine
	scene.reset();
}
