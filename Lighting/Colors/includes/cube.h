#pragma once

#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>

struct FaceTexture {
	unsigned int base = 0;   
	unsigned int overlay = 0; 
};

class Cube {
public:
	Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

	void stretch(float newWidth, float newHeight, float newDepth);

	const std::vector<float>& getVertices() const;

	unsigned int getVAO() const;

	void uploadToGPU();

	void render(glm::vec3 cubePosition, Shader& shader);

	void setTextures(const char* tex_path1, const char* tex_path2);

	void setFaceTexture(int faceIndex, const char* basePath, const char* overlayPath = nullptr);

private:
	unsigned int texture1, texture2;

	float width, height, depth;

	std::vector<float> vertices;

	unsigned int VAO, VBO;

	FaceTexture faceTextures[6];

	std::unordered_map<std::string, unsigned int> textureMap;

	unsigned int checkIfTextureExists(const char* path);

	void generateVertices();
};