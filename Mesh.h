#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"Shader.h"
#include"Mtl.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
	// 位置
	glm::vec3 Position;
	// 法向量
	glm::vec3 Normal;
	// 纹理坐标
	glm::vec2 TexCoords;
	//tangent
	//glm::vec3 Tangent;
	//// bitangent
	//glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;			//纹理id
	string type;				//类型
	string path;				//路径
};


//网格类：用来存储顶点、材质并且渲染，这里只实现了一个网格类对应一种纹理材质
class Mesh {
public:
	/*顶点，索引，纹理，VAO*/
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;
	Mtl* mtl;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// 设置顶点缓冲区及其属性指针
		setupMesh();
	}

	Mesh(vector<Vertex> vertices, Mtl* mtl)
	{
		this->vertices = vertices;
		this->mtl = mtl;
		// 设置顶点缓冲区及其属性指针
		setupMesh();
	}

	// render the mesh
	void Draw(Shader shader)
	{
		// 渲染网格
		shader.use();
		shader.setVec3("material.ambient", mtl->ambient);
		shader.setVec3("material.diffuse", mtl->diffuse);
		shader.setVec3("material.specular", mtl->specular); // 高光照明对物体的材质没有完全的影响
		shader.setFloat("material.shininess", mtl->shininess);
		shader.setInt("material.specularId", mtl->texture_specular_id);
		shader.setInt("material.ambientId", mtl->texture_ambient_id);

		glActiveTexture(GL_TEXTURE0);
		if (mtl->texture_diffuse_id != 0) {
			glUniform1i(glGetUniformLocation(shader.ID, "material.diffuseTexture"), 0);
			glBindTexture(GL_TEXTURE_2D, mtl->texture_diffuse_id);
		}
		if (mtl->texture_specular_id != 0) {
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(shader.ID, "material.specularTexture"), 1);
			glBindTexture(GL_TEXTURE_2D, mtl->texture_specular_id);
		}
		if (mtl->texture_ambient_id != 0) {
			glActiveTexture(GL_TEXTURE2);
			glUniform1i(glGetUniformLocation(shader.ID, "material.ambientTexture"), 2);
			glBindTexture(GL_TEXTURE_2D, mtl->texture_ambient_id);
		}
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

		// 让设置回归默认
		//	glActiveTexture(GL_TEXTURE0);
	}

private:

	unsigned int VBO, EBO;

	/*  Functions    */
	// 初始化网格数据
	void setupMesh()
	{
		// 创建缓存和数组
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 所有项目的结构体的内存布局是连续的。
		// 我们可以简单地传递一个指向结构体的指针，它可以完美地转换为一个glm::vec3/2数组
		//再次转换为3/2浮点数的字节数组
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//绑定索引
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// 设置顶点数据
		// 顶点坐标
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// 顶点法向量
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// 顶点纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		//// vertex tangent 先不管
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//// vertex bitangent 先不管
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};
#endif
