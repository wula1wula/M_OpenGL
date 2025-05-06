#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"vendor/stb_image/stb_image.h"
#include<iostream>

#include"shader.h"
#include"camera.h"

// settings
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

//窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 混合度值
float mixValue = 0.2f;

// 摄像机参数
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//时间参数
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

//光源位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//监视鼠标
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//隐藏光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	//开启隐藏
	glEnable(GL_DEPTH_TEST);

	//着色器
	Shader m_light_shader("res/Shader/ColorShader.shader");	
	Shader m_sunlight_shader("res/Shader/lightShader.shader");
	
	//顶点数据
	float vertices[] = {
	// positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//VAO VBO
	unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	if (VAO == 0) {
		std::cerr << "Failed to generate VAO" << std::endl;
		return -1;
	}
	glGenBuffers(1, &VBO);
	if (VBO == 0) {
		std::cerr << "Failed to generate VBO" << std::endl;
		return -1;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法线属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    //纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	if (lightVAO == 0) {
		std::cerr << "Failed to generate lightVAO" << std::endl;
		return -1;
	}
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	

	//加载纹理
	unsigned int diffuseMap = loadTexture("res/image/container2.png");
	unsigned int specularMap = loadTexture("res/image/lighting_maps_specular_color.png");
	unsigned int emissionMap = loadTexture("res/image/matrix.jpg");

    m_light_shader.use();
	m_light_shader.setInt("material.diffuse", 0);	
    m_light_shader.setInt("material.specular",1);
	//m_light_shader.setInt("material.emission", 2);

	//光源位置
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	//循环渲染
	while (!glfwWindowShouldClose(window))
	{
		//计算渲染时间
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//输入
		processInput(window);

		//渲染指令
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//绘制三角形
		m_light_shader.use();
		//m_light_shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

		//m_light_shader.setVec3("light.position", lightPos);
	
		//m_light_shader.setVec3("light.direction", -0.2f,-1.0f,-0.3f);
		
		//m_light_shader.setVec3("light.position", camera.Position);
		//m_light_shader.setVec3("light.direction", camera.Front);
		//m_light_shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		//m_light_shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		m_light_shader.setVec3("viewPos", camera.Position);
		m_light_shader.setFloat("material.shininess", 32.0f);

		// directional light
		m_light_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		m_light_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		m_light_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		m_light_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		m_light_shader.setVec3("pointLights[0].position", pointLightPositions[0]);
		m_light_shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		m_light_shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		m_light_shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		m_light_shader.setFloat("pointLights[0].constant", 1.0f);
		m_light_shader.setFloat("pointLights[0].linear", 0.09f);
		m_light_shader.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		m_light_shader.setVec3("pointLights[1].position", pointLightPositions[1]);
		m_light_shader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		m_light_shader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		m_light_shader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		m_light_shader.setFloat("pointLights[1].constant", 1.0f);
		m_light_shader.setFloat("pointLights[1].linear", 0.09f);
		m_light_shader.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		m_light_shader.setVec3("pointLights[2].position", pointLightPositions[2]);
		m_light_shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		m_light_shader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		m_light_shader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		m_light_shader.setFloat("pointLights[2].constant", 1.0f);
		m_light_shader.setFloat("pointLights[2].linear", 0.09f);
		m_light_shader.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		m_light_shader.setVec3("pointLights[3].position", pointLightPositions[3]);
		m_light_shader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		m_light_shader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		m_light_shader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		m_light_shader.setFloat("pointLights[3].constant", 1.0f);
		m_light_shader.setFloat("pointLights[3].linear", 0.09f);
		m_light_shader.setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		m_light_shader.setVec3("spotLight.position", camera.Position);
		m_light_shader.setVec3("spotLight.direction", camera.Front);
		m_light_shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_light_shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);	
		m_light_shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		m_light_shader.setFloat("spotLight.constant", 1.0f);
		m_light_shader.setFloat("spotLight.linear", 0.09f);
		m_light_shader.setFloat("spotLight.quadratic", 0.032f);
		m_light_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_light_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//光源颜色
		//glm::vec3 lightColor(1.0f,1.0f,1.0f);
		//lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0f));
		//lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7f));
		//lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3f));
		//glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f);
		//m_light_shader.setVec3("light.ambient", ambientColor);
		//m_light_shader.setVec3("light.diffuse", diffuseColor);
        //m_light_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//m_light_shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		//m_light_shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        //m_light_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		//m_light_shader.setFloat("light.constant", 1.0f);
		//m_light_shader.setFloat("light.linear", 0.09f);
		//m_light_shader.setFloat("light.quadratic", 0.032f);
		
		//材质颜色
		//m_light_shader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//m_light_shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//m_light_shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//m_light_shader.setFloat("material.shininess", 32.0f);
		//m_light_shader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
		//m_light_shader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		//m_light_shader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		//m_light_shader.setFloat("material.shininess", 32.0f);
		//m_light_shader.setVec3("material.specular", 0.5f,0.5f,0.5f);
        

		//相机
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		m_light_shader.setMat4("projection", projection);
		m_light_shader.setMat4("view", view);

		//模型
		glm::mat4 model = glm::mat4(1.0f);
		m_light_shader.setMat4("model", model);


		//纹理
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);

		//绘制
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//m_sunlight_shader.use();
		//m_sunlight_shader.setVec3("lightColor", lightColor);
		//m_sunlight_shader.setMat4("view", view);
		//m_sunlight_shader.setMat4("projection", projection);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f));
		//m_sunlight_shader.setMat4("model", model);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_light_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

        //绘制光源
		m_sunlight_shader.use();
		m_sunlight_shader.setMat4("projection", projection);
		m_sunlight_shader.setMat4("view", view);
		glBindVertexArray(lightVAO);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f));
		//m_sunlight_shader.setMat4("model", model);
		
		for (unsigned int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			m_sunlight_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//检查并调用事件，交换缓冲区
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//清理
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mixValue += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mixValue -= 0.001f;

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}