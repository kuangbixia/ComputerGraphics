#pragma once
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<string>
using namespace std;

//�����洢mtl�ļ�����ÿ�����ʵ���Ϣ
class Mtl
{
public:
	string name;
	glm::vec3 ambient;//������Ĳ���
	glm::vec3 diffuse;//�������Ĳ���
	glm::vec3 specular;//�����Ĳ���
	float shininess;

	//3�������id
	unsigned int texture_ambient_id;
	unsigned int texture_diffuse_id;
	unsigned int texture_specular_id;

	//3������ͼƬ��·��
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

