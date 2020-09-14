#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
#include "Mesh.h"
#include "Shader.h"
#include"Mtl.h"
//
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
//#include <map>
#include <vector>
using namespace std;


struct fIndex {
	int positionIndex;
	int texCoordIndex;
	int normalIndex;
};

//模型类：读取obj mtl文件，加载模型，管理组成模型的网格，渲染整个模型
class Model {
public:

	vector<Mesh> meshes; //模型的所有网格
	vector<glm::vec3> positions;//obj文件中的所有位置坐标
	vector<glm::vec3> normals;//obj文件中的所有法向量
	vector<glm::vec2> texCoords;//obj文件中的所有纹理坐标
	vector<Mtl*> mtls;//从mtl文件里面读取的各材质信息
	string directory;
	string mtlDirectory;
	string absoluteDirectory;

	//初始化的时候加载网格以及材质
	Model(const char* path, const char* mtl, const char* absolute) {
		directory = path;
		mtlDirectory = mtl;
		absoluteDirectory = absolute;
		//先加载材质包
		if (!setUpMtls()) {
			cout << "set up mtls wrong" << endl;
		}
		//加载网格
		if (!setUpMeshs()) {
			cout << "set up meshs wrong" << endl;
		}

	}

	bool setUpMtls() {
		//读入文件
		ifstream file;
		file.open(mtlDirectory);
		if (file.fail()) {
			cout << "fail to read the file" << endl;
			return false;
		}
		string line;
		int count = 0;
		Mtl* mid = new Mtl();
		//一行一行读入，直到文件结尾
		while (getline(file, line)) {
			if (line.substr(0, 6) == "newmtl") {
				if (count != 0) {
					int width, height, nrChannels;
					unsigned char* data;
					//加载漫反射和镜面反射、环境纹理
					if (mid->texture_diffuse_path != "") {
						glGenTextures(1, &mid->texture_diffuse_id);
						glBindTexture(GL_TEXTURE_2D, mid->texture_diffuse_id);
						// 为当前绑定的纹理对象设置环绕、过滤方式
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						stbi_set_flip_vertically_on_load(true);
						data = stbi_load(mid->texture_diffuse_path.c_str(), &width, &height, &nrChannels, 0);
						//通过通道数判断图片类型
						if (data) {
							if (nrChannels == 3) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
							else if (nrChannels == 4) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
						}
						else {
							std::cout << "Failed to load texture" << std::endl;
							return false;
						}
						stbi_image_free(data);
					}
					if (mid->texture_specular_path != "") {
						glGenTextures(1, &mid->texture_specular_id);
						glBindTexture(GL_TEXTURE_2D, mid->texture_specular_id);
						// 为当前绑定的纹理对象设置环绕、过滤方式
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						stbi_set_flip_vertically_on_load(true);
						data = stbi_load(mid->texture_specular_path.c_str(), &width, &height, &nrChannels, 0);
						if (data) {
							if (nrChannels == 3) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
							else if (nrChannels == 4) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
						}
						else {
							std::cout << "Failed to load texture" << std::endl;
							return false;
						}
						stbi_image_free(data);
					}

					if (mid->texture_ambient_path != "") {
						glGenTextures(1, &mid->texture_ambient_id);
						glBindTexture(GL_TEXTURE_2D, mid->texture_ambient_id);
						// 为当前绑定的纹理对象设置环绕、过滤方式
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						stbi_set_flip_vertically_on_load(true);
						data = stbi_load(mid->texture_ambient_path.c_str(), &width, &height, &nrChannels, 0);
						if (data) {
							if (nrChannels == 3) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
							else if (nrChannels == 4) {
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);
							}
						}
						else {
							std::cout << "Failed to load texture" << std::endl;
							return false;
						}
						stbi_image_free(data);
					}
					mtls.push_back(mid);
				}
				istringstream s(line.substr(6));
				mid = new Mtl();
				s >> mid->name;
				count++;
			}
			else if (line.substr(0, 2) == "Ns") {
				istringstream s(line.substr(2));
				s >> mid->shininess;
			}
			else if (line.substr(0, 2) == "Ka") {
				istringstream s(line.substr(2));
				s >> mid->ambient.x;
				s >> mid->ambient.y;
				s >> mid->ambient.z;
			}
			else if (line.substr(0, 2) == "Kd") {
				istringstream s(line.substr(2));
				s >> mid->diffuse.x;
				s >> mid->diffuse.y;
				s >> mid->diffuse.z;
			}
			else if (line.substr(0, 2) == "Ks") {
				istringstream s(line.substr(2));
				s >> mid->specular.x;
				s >> mid->specular.y;
				s >> mid->specular.z;
			}
			else if (line.substr(0, 6) == "map_Kd") {
				istringstream s(line.substr(6));
				string filename;
				s >> filename;
				filename = absoluteDirectory + filename;
				mid->texture_diffuse_path = filename;
			}
			else if (line.substr(0, 6) == "map_Ks") {
				istringstream s(line.substr(6));
				string filename;
				s >> filename;
				filename = absoluteDirectory + filename;
				mid->texture_specular_path = filename;
			}
			else if (line.substr(0, 6) == "map_Bump") {
				istringstream s(line.substr(6));
				string filename;
				s >> filename;
				filename = absoluteDirectory + filename;
				mid->texture_ambient_path = filename;
			}

		}
		int width, height, nrChannels;
		unsigned char* data = 0;

		if (mid->texture_diffuse_path != "") {
			glGenTextures(1, &mid->texture_diffuse_id);
			glBindTexture(GL_TEXTURE_2D, mid->texture_diffuse_id);
			// 为当前绑定的纹理对象设置环绕、过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_set_flip_vertically_on_load(true);
			data = stbi_load(mid->texture_diffuse_path.c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				if (nrChannels == 3) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else if (nrChannels == 4) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
			else {
				std::cout << "Failed to load texture" << std::endl;

				return false;
			}
			stbi_image_free(data);
		}
		if (mid->texture_specular_path != "") {
			glGenTextures(1, &mid->texture_specular_id);
			glBindTexture(GL_TEXTURE_2D, mid->texture_specular_id);
			// 为当前绑定的纹理对象设置环绕、过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_set_flip_vertically_on_load(true);
			data = stbi_load(mid->texture_specular_path.c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				if (nrChannels == 3) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else if (nrChannels == 4) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
			else {
				std::cout << "Failed to load texture" << std::endl;
				return false;
			}
			stbi_image_free(data);
		}

		if (mid->texture_ambient_path != "") {
			glGenTextures(1, &mid->texture_ambient_id);
			glBindTexture(GL_TEXTURE_2D, mid->texture_ambient_id);
			// 为当前绑定的纹理对象设置环绕、过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_set_flip_vertically_on_load(true);
			data = stbi_load(mid->texture_ambient_path.c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				if (nrChannels == 3) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else if (nrChannels == 4) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
			else {
				std::cout << "Failed to load texture" << std::endl;
				return false;
			}
			stbi_image_free(data);
		}



		mtls.push_back(mid);
		return true;
	}



	bool setUpMeshs() {
		ifstream file;
		file.open(directory);
		if (file.fail()) {
			cout << "fail to read the file" << endl;
			return false;
		}
		string line;
		vector<Vertex> midVertex;
		vector<fIndex> midIndex;
		int meshAmount = 0;
		string mtlName;
		int mtlIndex = 0;
		while (getline(file, line)) {
			//以关键字o区分不同的网格
			if (line.substr(0, 1) == "o") {
				if (meshAmount != 0) {
					for (int i = 0; i < mtls.size(); i++) {
						if (mtls[i]->name == mtlName)
							mtlIndex = i;
					}
					meshes.push_back(Mesh(midVertex, mtls[mtlIndex]));
					midVertex.clear();
				}
				meshAmount++;
			}
			else if (line.substr(0, 1) == "v") {
				if (line.substr(0, 2) == "vn") {
					istringstream s(line.substr(2));
					glm::vec3 vn;
					s >> vn.x;
					s >> vn.y;
					s >> vn.z;
					normals.push_back(vn);
				}
				else if (line.substr(0, 2) == "vt") {
					istringstream s(line.substr(2));
					glm::vec2 vt;
					s >> vt.x;
					s >> vt.y;
					texCoords.push_back(vt);
				}
				else {
					istringstream s(line.substr(2));
					glm::vec3 v;
					s >> v.x;
					s >> v.y;
					s >> v.z;
					positions.push_back(v);
				}
			}
			else if (line.substr(0, 1) == "f") {
				istringstream s(line.substr(2));
				string index;
				while (s >> index) {
					if (index.find("\\") != string::npos) {
						cout << "没有纹理坐标？？？" << endl;
						return false;
					}
					//cout << index;
					fIndex f;

					replace(index.begin(), index.end(), '/', ' ');
					//cout << index;
					istringstream indexstream(index);
					indexstream >> f.positionIndex >> f.texCoordIndex >> f.normalIndex;

					f.positionIndex--;
					f.texCoordIndex--;
					f.normalIndex--;
					midIndex.push_back(f);

				}

				if (midIndex.size() == 4) {
					for (int i = 0; i < 3; i++) {
						Vertex v;
						//cout << midIndex.at(i).positionIndex << endl;;
						//cout << positions.at(0).x;
						v.Position = positions.at(midIndex.at(i).positionIndex);
						v.Normal = normals.at(midIndex.at(i).normalIndex);
						v.TexCoords = texCoords.at(midIndex.at(i).texCoordIndex);
						midVertex.push_back(v);
					}
					for (int i = 0; i < 4; i++)
					{
						if (i != 1)
						{
							Vertex v;
							v.Position = positions.at(midIndex.at(i).positionIndex);
							v.Normal = normals.at(midIndex.at(i).normalIndex);
							v.TexCoords = texCoords.at(midIndex.at(i).texCoordIndex);
							midVertex.push_back(v);
						}
					}
				}
				else if (midIndex.size() == 3)
					for (int i = 0; i < 3; i++) {

						Vertex v;
						v.Position = positions.at(midIndex.at(i).positionIndex);
						v.Normal = normals.at(midIndex.at(i).normalIndex);
						v.TexCoords = texCoords.at(midIndex.at(i).texCoordIndex);
						midVertex.push_back(v);
					}
				midIndex.clear();
			}
			else if (line.substr(0, 6) == "usemtl") {
				istringstream s(line.substr(6));
				s >> mtlName;
			}

		}
		for (int i = 0; i < mtls.size(); i++) {
			if (mtls[i]->name == mtlName)
				mtlIndex = i;
		}
		meshes.push_back(Mesh(midVertex, mtls[mtlIndex]));
		midVertex.clear();
		return true;
	}
	//渲染模型、一个一个网格类渲染，注意一个网格类只有一种纹理，
	//建议加载前检查obj文件，看一下每个以o分块的网格是否只有一个usemtl
	//我的网格类是以o区分的
	void draw(Shader shader) {
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
	~Model();


};


Model::~Model()
{
}


#endif
