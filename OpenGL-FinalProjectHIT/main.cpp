#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgur/imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "camera.h"
#include "UiManager.h"
#include "CubeManager.h"
#include "LightManager.h"
#include "FloorManager.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

// settings
const unsigned int screenWidth = 1280;
const unsigned int ScreenHeight = 720;

// camera
Camera *camera = new Camera(screenWidth, ScreenHeight);

// controls
float lastX = (float)screenWidth / 2.0;
float lastY = (float)ScreenHeight / 2.0;
bool firstMouse = true;
bool showMouse = true;
bool showMousePressed = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

ImGuiIO& io = ImGui::GetIO();

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(screenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	
	//camera init
	camera->Position = glm::vec3(3.0f, 5.0f, 8.0f);
	camera->Front = glm::vec3(-0.5f, -0.6f, -1.5f);

	//lights
	LightManager lights(camera);
	lights.InitLights();

	//cubes
	CubeManager cubes(camera);
	cubes.Lights = &lights;

	//floor
	FloorManager floor(camera);
	floor.Lights = &lights;

	//ui
	UiManager ui(window);
	ui.Camera = camera;
	ui.Cubes = &cubes;
	ui.Lights = &lights;
	ui.Floor = &floor;
	ui.ShowMouse = &showMouse;
	

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//ui
		ui.RenderBegin();

		// input
		processInput(window);

		// clear screen
		glClearColor(lights.ClearColor.x, lights.ClearColor.y, lights.ClearColor.z, lights.ClearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render shadow map
		lights.RenderShadwoMapToTextureBegin();

		floor.RenderFloorForShadowMap();
		cubes.RenderCubesForShadowMap();

		lights.RenderShadwoMapToTextureEnd();

		//clear screen
		glViewport(0, 0, screenWidth, ScreenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ui
		ui.RenderSet();

		//render scene with shadow map
		lights.RenderLights();
		cubes.RenderCubes(deltaTime);
		floor.RenderFloor();
		
		//render shadow map
		lights.RenderShadowMappDebug();
		//ui
		ui.RenderEnd();
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ui.CleanUp();
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	

	if (!showMouse) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(RIGHT, deltaTime);
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !showMousePressed)
	{
		showMouse = !showMouse;
		showMousePressed= true;
		glfwSetInputMode(window, GLFW_CURSOR, showMouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		showMousePressed = false;
	}

	if (showMouse && io.MouseWheel) {
		camera->ProcessMouseScroll(io.MouseWheel);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!showMouse) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera->ProcessMouseMovement(xoffset, yoffset);
	}
}

