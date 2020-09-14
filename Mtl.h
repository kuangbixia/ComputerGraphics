#pragma once
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<string>
using namespace std;

//用来存储mtl文件里面每个材质的信息
class Mtl
{
public:
	string name;
	glm::vec3 ambient;//环境光的材质
	glm::vec3 diffuse;//漫反射光的材质
	glm::vec3 specular;//反射光的材质
	float shininess;

	//3种纹理的id
	unsigned int texture_ambient_id;
	unsigned int texture_diffuse_id;
	unsigned int texture_specular_id;

	//3种纹理图片的路径
	string texture_ambient_path;
	string texture_diffuse_path;
	string texture_specular_path;



	Mtl() {
		texture_ambient_id = 0;
		texture_diffuse_id = 0;
		texture_specular_id = 0;
		texture_diffuse_path = "";
		texture_specular_path = "";
		texture_ambient_path = "";
	}
	~Mtl() {}

};

