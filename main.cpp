#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<stdio.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"Model.h"
#include"Shader.h"
#include"stb_image.h"
#include"Camera.h"
#include<iostream>
using namespace std;
#include<vector>

enum Boat_Movement {
	U,
	D,
	L,
	R,
	UP,
	DOWN,
	BIG,
	SMALL
};

GLFWwindow* window;

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1200;
Camera camera(glm::vec3(0.96242f, 0.05f, 4.40402f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float modelx = 0.0f;
float modely = 0.0f;
float modelz = 0.0f;

int modelGroup1[] = { 0,3 };
int modelGroup2[] = { 1,2,4,5 };

//模型位置
glm::vec3 BoatPosition[7] = {
	glm::vec3(6.11984f, 3.06532f, -12.5518f), //飞行器
	glm::vec3(0.0f, 0.0f, 0.0f), //船
	glm::vec3(-0.0141791f, 0.126055f, 0.253061f), //机器人
	glm::vec3(6.11984f, 3.43746f, -12.9165f), //警察
	glm::vec3(-0.0329339f, 0.371418f, 0.00922995f), //树
	glm::vec3(0.0995694f, 0.374369f, -0.0464961f), //少女
	glm::vec3(-8.12897f, -1.0f, -16.8456f)//房子
};
//模型大小
glm::vec3 modelSize[7] = {
	glm::vec3(0.302283f,0.302283f,0.302283f),  //飞行器
	glm::vec3(0.5f,0.5f,0.5f), //船
	glm::vec3(0.0365756f,0.0365756f,0.0365756f),	//机器人
	glm::vec3(0.00620062f,0.00620062f,0.00620062f),	//警察
	glm::vec3(0.0219672f,0.0219672f,0.0219672f),		//树
	glm::vec3(0.002f,0.002f,0.002f),//少女
	glm::vec3(0.03f,0.03f,0.03f)	//房子
};
int currTarget = 0;
//模型方向
glm::vec3 Front[7] = {
	glm::vec3(0.36238f, 0.0f, -0.93203f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.36238f, 0.0f, -0.93203f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f)
};
glm::vec3 BoatRight = glm::vec3(1.0f, 0.0f, 0.0f);


//录像功能
vector<int> action[7];
int currAction = -1;
int isStart = 0;
int isVideoing = 0;
int isReviewing[7];
int now[7];



const char path[] = "model/飞行器/Intergalactic_Spaceship-(Wavefront).obj";
const char mtl[] = "model/飞行器/Intergalactic_Spaceship-(Wavefront).mtl";
const char absolute[] = "model/飞行器/";

const char path1[] = "model/船/jiaotongchuang.obj";
const char mtl1[] = "model/船/jiaotongchuang.mtl";
const char absolute1[] = "model/船/";

const char path2[] = "model/nanosuit/nanosuit.obj";
const char mtl2[] = "model/nanosuit/nanosuit.mtl";
const char absolute2[] = "model/nanosuit/";

const char path3[] = "model/police/untitled.obj";
const char mtl3[] = "model/police/untitled.mtl";
const char absolute3[] = "model/police/";

const char path4[] = "model/tree/A01992.obj";
const char mtl4[] = "model/tree/A01992.mtl";
const char absolute4[] = "model/tree/";

const char path5[] = "model/骑驴少女/骑驴少女.obj";
const char mtl5[] = "model/骑驴少女/1.mtl";
const char absolute5[] = "model/骑驴少女/";

const char path6[] = "model/房子/house.obj";
const char mtl6[] = "model/房子/1.mtl";
const char absolute6[] = "model/房子/";

vector<string> skyboxFaces{	//天空盒路径
	"skybox/right.jpg",
	"skybox/left.jpg",
	"skybox/top.jpg",
	"skybox/bottom.jpg",
	"skybox/front.jpg",
	"skybox/back.jpg"
};

//天空盒参数
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};



//点光源位置
glm::vec3 lightPos[4] = {
	glm::vec3(10.0,2.0,3.0),
	glm::vec3(14.0,-10.0,3.0),
	glm::vec3(2.0,2.0,10.0),
	glm::vec3(-10.0,3.0,-12.0)
};

//各种回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadCubemap(vector<std::string> faces);
void BoatProcessKeyboard(Boat_Movement Boat_ketboard, float deltaTime);


void Init();
void setLights(Shader shader);


int main() {

	Init();

	Shader shader("vertex.vs", "fragment.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");

	// 天空盒 VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//加载天空盒
	unsigned int cubemapTexture = loadCubemap(skyboxFaces);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	//读取模型
	shader.use();
	Model aircraft(path, mtl, absolute);
	Model ship(path1, mtl1, absolute1);
	Model nanosuit(path2, mtl2, absolute2);
	Model police(path3, mtl3, absolute3);
	Model tree(path4, mtl4, absolute4);
	Model gril(path5, mtl5, absolute5);
	Model house(path6, mtl6, absolute6);

	//渲染引擎
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//GL_LINE GL_FILL
	glEnable(GL_DEPTH_TEST);


	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//录像功能:用数组记录操作，然后播放
		if (isStart == 1) {
			action[currTarget].clear();
			isStart = 0;
		}
		if (isVideoing == 1) {
			action[currTarget].push_back(currAction);
		}
		int changeTarget = 0;
		for (int i = 0; i < 7; i++) {
			if (isReviewing[i] == 1) {
				if (now[i] < action[i].size()) {
					if (action[i].at(now[i]) != -1) {
						changeTarget = currTarget;
						currTarget = i;
						BoatProcessKeyboard((Boat_Movement)action[i].at(now[i]), deltaTime);
						currTarget = changeTarget;
					}
					now[i]++;
				}
				else {
					now[i] = 0;
					isReviewing[i] = 0;
				}
			}
		}





		shader.use();
		//传入摄像机位置
		shader.setVec3("viewPos", camera.Position);
		//设置光照，封装在最下面的
		setLights(shader);
		//调节投影和可视空间
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// 控制模型位置，仅通过model矩阵传入就可以改变模型大小位置和旋转
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[0]); // 位置
		model = glm::scale(model, modelSize[0]);	// 大小
		float a = glm::dot(Front[0], glm::vec3(0.0f, 0.0f, -1.0f));
		float b = glm::dot(Front[0], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		aircraft.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[1]); // 位置
		model = glm::scale(model, modelSize[1]);	// 大小
													//flaot a = glm::angle(Front, glm::vec3(0.0f, 0.0f, -1.0f));
		a = glm::dot(Front[1], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[1], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		ship.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[2]); // 位置
		model = glm::scale(model, modelSize[2]);	// 大小
		a = glm::dot(Front[2], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[2], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		nanosuit.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[3]); // 位置
		model = glm::scale(model, modelSize[3]);	// 大小
		a = glm::dot(Front[3], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[3], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		police.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[4]); // 位置
		model = glm::scale(model, modelSize[4]);	// 大小
		a = glm::dot(Front[4], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[4], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		tree.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[5]); // 位置
		model = glm::scale(model, modelSize[5]);	// 大小
		a = glm::dot(Front[5], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[5], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		gril.draw(shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, BoatPosition[6]); // 位置
		model = glm::scale(model, modelSize[6]);	// 大小
		a = glm::dot(Front[6], glm::vec3(0.0f, 0.0f, -1.0f));
		b = glm::dot(Front[6], glm::vec3(-1.0f, 0.0f, 0.0f));
		if (b > 0)
			b = 1;
		else
			b = -1;
		model = glm::rotate(model, b * (float)acos(a), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);

		house.draw(shader);

		// 渲染天空盒
		glDepthFunc(GL_LEQUAL);  // 改变深度函数，当值等于深度缓冲区的内容时，深度测试通过
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 从视图矩阵中删除平移
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// 天空盒方块
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // 将深度函数设置回默认值

		currAction = -1;


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//退出
	glfwTerminate();

	return 0;
}





void Init() {
	//初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPENGL", NULL, NULL);

	//没有创建成功
	if (window == NULL) {
		printf("err:windows is NULL");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return;
	}

}

//键盘输入的回调函数
void processInput(GLFWwindow* window) {
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		BoatProcessKeyboard(U, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		BoatProcessKeyboard(D, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		BoatProcessKeyboard(L, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		BoatProcessKeyboard(R, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		currTarget = 0;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		currTarget = 1;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		currTarget = 2;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		currTarget = 3;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		currTarget = 4;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		currTarget = 5;
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		currTarget = 6;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		BoatProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		BoatProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		BoatProcessKeyboard(BIG, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		BoatProcessKeyboard(SMALL, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		isVideoing = 1;
		isStart = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		isVideoing = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		isReviewing[0] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		isReviewing[1] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		isReviewing[2] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) {
		isReviewing[3] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
		isReviewing[4] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS) {
		isReviewing[5] = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS) {
		isReviewing[6] = 1;
	}

}

// glfw:每当窗口大小改变(通过操作系统或用户调整大小)时，执行这个回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// 确保viewport与新的窗口尺寸匹配;注意宽度和高度将明显大于指定的视网膜显示器。
	glViewport(0, 0, width, height);
}

// glfw:每当鼠标移动时，都会调用这个回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 颠倒了，因为y坐标是从下到上

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: 每当鼠标滚轮滚动时，都会调用此回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

//加载天空盒
unsigned int loadCubemap(vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
//控制船的前进方向
void BoatProcessKeyboard(Boat_Movement Boat_ketboard, float deltaTime)
{

	float velocity = 0.5f * deltaTime;
	if (currTarget == 0) {
		glm::vec3 preFront = Front[0];
		for (int i = 0; i < sizeof(modelGroup1) / sizeof(int); i++) {
			currTarget = modelGroup1[i];

			if (Boat_ketboard == U)
				BoatPosition[currTarget] -= Front[0] * velocity;
			if (Boat_ketboard == D)
				BoatPosition[currTarget] += Front[0] * velocity;
			if (Boat_ketboard == L) {
				if (i != 0) {
					glm::vec3 prePosition = BoatPosition[currTarget] - BoatPosition[0];
					glm::vec3 nomalfront = normalize(prePosition);
					float transRate = 0;
					if (prePosition.x != 0)
						transRate = prePosition.x / nomalfront.x;
					else if (prePosition.y != 0)
						transRate = prePosition.y / nomalfront.y;
					else if (prePosition.z != 0)
						transRate = prePosition.z / nomalfront.z;
					else
						transRate = 0;
					nomalfront = normalize(nomalfront - glm::cross(nomalfront, glm::vec3(0.0f, 1.0f, 0.0f)) * velocity);
					nomalfront = nomalfront * transRate;
					BoatPosition[currTarget] = BoatPosition[0] + nomalfront;
				}
				Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, 1.0f, 0.0f)) * velocity);
			}
			if (Boat_ketboard == R) {
				if (i != 0) {
					glm::vec3 prePosition = BoatPosition[currTarget] - BoatPosition[0];
					glm::vec3 nomalfront = normalize(prePosition);
					float transRate = 0;
					if (prePosition.x != 0)
						transRate = prePosition.x / nomalfront.x;
					else if (prePosition.y != 0)
						transRate = prePosition.y / nomalfront.y;
					else if (prePosition.z != 0)
						transRate = prePosition.z / nomalfront.z;
					else
						transRate = 0;
					nomalfront = normalize(nomalfront - glm::cross(nomalfront, glm::vec3(0.0f, -1.0f, 0.0f)) * velocity);
					nomalfront = nomalfront * transRate;
					BoatPosition[currTarget] = BoatPosition[0] + nomalfront;
				}
				Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, -1.0f, 0.0f)) * velocity);
			}
			if (Boat_ketboard == UP)
				BoatPosition[currTarget] += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
			if (Boat_ketboard == DOWN)
				BoatPosition[currTarget] -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
			if (Boat_ketboard == BIG)
				modelSize[currTarget] = modelSize[currTarget] * (float)1.001;
			if (Boat_ketboard == SMALL)
				modelSize[currTarget] = modelSize[currTarget] * (float)0.999;
		}
		currTarget = 0;
	}
	else if (currTarget == 1) {
		for (int i = 0; i < sizeof(modelGroup2) / sizeof(int); i++) {
			currTarget = modelGroup2[i];

			if (Boat_ketboard == U)
				BoatPosition[currTarget] -= Front[1] * velocity;
			if (Boat_ketboard == D)
				BoatPosition[currTarget] += Front[1] * velocity;
			if (Boat_ketboard == L) {
				if (i != 0) {
					glm::vec3 prePosition = BoatPosition[currTarget] - BoatPosition[1];
					glm::vec3 nomalfront = normalize(prePosition);
					float transRate = 0;
					if (prePosition.x != 0)
						transRate = prePosition.x / nomalfront.x;
					else if (prePosition.y != 0)
						transRate = prePosition.y / nomalfront.y;
					else if (prePosition.z != 0)
						transRate = prePosition.z / nomalfront.z;
					else
						transRate = 0;
					nomalfront = normalize(nomalfront - glm::cross(nomalfront, glm::vec3(0.0f, 1.0f, 0.0f)) * velocity);
					nomalfront = nomalfront * transRate;
					BoatPosition[currTarget] = BoatPosition[1] + nomalfront;
				}
				Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, 1.0f, 0.0f)) * velocity);
			}
			if (Boat_ketboard == R) {
				if (i != 0) {
					glm::vec3 prePosition = BoatPosition[currTarget] - BoatPosition[1];
					glm::vec3 nomalfront = normalize(prePosition);
					float transRate = 0;
					if (prePosition.x != 0)
						transRate = prePosition.x / nomalfront.x;
					else if (prePosition.y != 0)
						transRate = prePosition.y / nomalfront.y;
					else if (prePosition.z != 0)
						transRate = prePosition.z / nomalfront.z;
					else
						transRate = 0;
					nomalfront = normalize(nomalfront - glm::cross(nomalfront, glm::vec3(0.0f, -1.0f, 0.0f)) * velocity);
					nomalfront = nomalfront * transRate;
					BoatPosition[currTarget] = BoatPosition[1] + nomalfront;
				}
				Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, -1.0f, 0.0f)) * velocity);
			}
			if (Boat_ketboard == UP)
				BoatPosition[currTarget] += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
			if (Boat_ketboard == DOWN)
				BoatPosition[currTarget] -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
			if (Boat_ketboard == BIG)
				modelSize[currTarget] = modelSize[currTarget] * (float)1.001;
			if (Boat_ketboard == SMALL)
				modelSize[currTarget] = modelSize[currTarget] * (float)0.999;
		}
		currTarget = 1;
	}
	else {
		if (Boat_ketboard == U)
			BoatPosition[currTarget] -= Front[currTarget] * velocity;
		if (Boat_ketboard == D)
			BoatPosition[currTarget] += Front[currTarget] * velocity;
		if (Boat_ketboard == L)
			Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, 1.0f, 0.0f)) * velocity);
		if (Boat_ketboard == R)
			Front[currTarget] = normalize(Front[currTarget] - glm::cross(Front[currTarget], glm::vec3(0.0f, -1.0f, 0.0f)) * velocity);
		if (Boat_ketboard == UP)
			BoatPosition[currTarget] += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (Boat_ketboard == DOWN)
			BoatPosition[currTarget] -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
		if (Boat_ketboard == BIG)
			modelSize[currTarget] = modelSize[currTarget] * (float)1.001;
		if (Boat_ketboard == SMALL)
			modelSize[currTarget] = modelSize[currTarget] * (float)0.999;
	}

	// 这里可能要调整？
	if (isVideoing == 1) {
		currAction = Boat_ketboard;
	}
}

/*设置所有光照，注意修改点光源之后要记得在fragment里面修改点光源的数组大小*/
void setLights(Shader shader) {
	glm::vec3 pointLightColor = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 diffuseColor = pointLightColor * glm::vec3(3.0f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence

	glm::vec3 parallelLightColor = glm::vec3(1.0f, 0.72f, 0.06f);
	glm::vec3 spotLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	shader.setVec3("parallelLight.direction", glm::vec3(-1.0f, -1.0f, 0.0f));//z=0平面上45°角右上指向左下
	shader.setVec3("parallelLight.ambient", parallelLightColor);
	shader.setVec3("parallelLight.diffuse", parallelLightColor);
	shader.setVec3("parallelLight.specular", parallelLightColor);

	shader.setVec3("pointLight[0].position", lightPos[0]);
	shader.setVec3("pointLight[0].ambient", ambientColor);
	shader.setVec3("pointLight[0].diffuse", diffuseColor);
	shader.setVec3("pointLight[0].specular", pointLightColor);
	shader.setFloat("pointLight[0].constant", 1.0f);
	shader.setFloat("pointLight[0].linear", 0.09);
	shader.setFloat("pointLight[0].quadratic", 0.032);

	shader.setVec3("pointLight[1].position", lightPos[1]);
	shader.setVec3("pointLight[1].ambient", ambientColor);
	shader.setVec3("pointLight[1].diffuse", diffuseColor);
	shader.setVec3("pointLight[1].specular", pointLightColor);
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.09);
	shader.setFloat("pointLight[1].quadratic", 0.032);

	shader.setVec3("pointLight[2].position", lightPos[2]);
	shader.setVec3("pointLight[2].ambient", ambientColor);
	shader.setVec3("pointLight[2].diffuse", diffuseColor);
	shader.setVec3("pointLight[2].specular", pointLightColor);
	shader.setFloat("pointLight[2].constant", 1.0f);
	shader.setFloat("pointLight[2].linear", 0.09);
	shader.setFloat("pointLight[2].quadratic", 0.032);

	shader.setVec3("pointLight[3].position", lightPos[3]);
	shader.setVec3("pointLight[3].ambient", ambientColor);
	shader.setVec3("pointLight[3].diffuse", diffuseColor);
	shader.setVec3("pointLight[3].specular", pointLightColor);
	shader.setFloat("pointLight[3].constant", 1.0f);
	shader.setFloat("pointLight[3].linear", 0.09);
	shader.setFloat("pointLight[3].quadratic", 0.032);

	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", glm::vec3(0.0, 0.0, 0.0));
	shader.setVec3("spotLight.diffuse", spotLightColor);
	shader.setVec3("spotLight.specular", spotLightColor);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.1);
	shader.setFloat("spotLight.quadratic", 0.032);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

}