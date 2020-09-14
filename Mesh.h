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
	// λ��
	glm::vec3 Position;
	// ������
	glm::vec3 Normal;
	// ��������
	glm::vec2 TexCoords;
	//tangent
	//glm::vec3 Tangent;
	//// bitangent
	//glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;			//����id
	string type;				//����
	string path;				//·��
};


//�����ࣺ�����洢���㡢���ʲ�����Ⱦ������ֻʵ����һ���������Ӧһ���������
class Mesh {
public:
	/*���㣬����������VAO*/
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

		// ���ö��㻺������������ָ��
		setupMesh();
	}

	Mesh(vector<Vertex> vertices, Mtl* mtl)
	{
		this->vertices = vertices;
		this->mtl = mtl;
		// ���ö��㻺������������ָ��
		setupMesh();
	}

	// render the mesh
	void Draw(Shader shader)
	{
		// ��Ⱦ����
		shader.use();
		shader.setVec3("material.ambient", mtl->ambient);
		shader.setVec3("material.diffuse", mtl->diffuse);
		shader.setVec3("material.specular", mtl->specular); // �߹�����������Ĳ���û����ȫ��Ӱ��
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

		// �����ûع�Ĭ��
		//	glActiveTexture(GL_TEXTURE0);
	}

private:

	unsigned int VBO, EBO;

	/*  Functions    */
	// ��ʼ����������
	void setupMesh()
	{
		// �������������
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ������Ŀ�Ľṹ����ڴ沼���������ġ�
		// ���ǿ��Լ򵥵ش���һ��ָ��ṹ���ָ�룬������������ת��Ϊһ��glm::vec3/2����
		//�ٴ�ת��Ϊ3/2���������ֽ�����
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//������
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// ���ö�������
		// ��������
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨����
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		//// vertex tangent �Ȳ���
		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		//// vertex bitangent �Ȳ���
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};
#endif
