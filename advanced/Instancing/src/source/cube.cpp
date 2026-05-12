#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cube.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <textures.h>



Cube::Cube(float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    generateVertices();
    uploadToGPU();
}

void Cube::stretch(float newWidth, float newHeight, float newDepth)
{
    width = newWidth;
    height = newHeight;
    depth = newDepth;
    generateVertices();
}

const std::vector<float>& Cube::getVertices() const
{
	return vertices;
}

void Cube::generateVertices()
{

    float w = width / 2.0f;
    float h = height / 2.0f;
    float d = depth / 2.0f;

    vertices = {
        // Back face
        // positions // normals           // texture coords
        -w, -h, -d,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         w,  h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         w, -h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         w,  h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -w, -h, -d,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -w,  h, -d,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // Front face
        -w, -h,  d, 0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
         w, -h,  d, 0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
         w,  h,  d, 0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
         w,  h,  d, 0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
        -w,  h,  d, 0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
        -w, -h,  d, 0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

        // Left face
        -w,  h,  d, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -w,  h, -d, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -w, -h, -d, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -w, -h, -d, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -w, -h,  d, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -w,  h,  d, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        // Right face
         w,  h,  d, 1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         w, -h, -d, 1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         w,  h, -d, 1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         w, -h, -d, 1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         w,  h,  d, 1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         w, -h,  d, 1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

         // Bottom face
         -w, -h, -d, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
          w, -h, -d, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
          w, -h,  d, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
          w, -h,  d, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         -w, -h,  d, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         -w, -h, -d, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

         // Top face
         -w,  h, -d, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
          w,  h,  d, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          w,  h, -d, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
          w,  h,  d, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -w,  h, -d, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         -w,  h,  d, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f
    };

}


unsigned int Cube::getVAO() const
{
    return VAO;
}

void Cube::uploadToGPU() 
{
    if (VAO != 0) return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Cube::setTextures(const char* tex_path1, const char* tex_path2)
{
    if (tex_path1) {
        unsigned int texture = loadTexture(tex_path1);
        texture1 = texture;
    }

    if (tex_path2) {
        unsigned int texture = loadTexture(tex_path2);
        texture2 = texture;
    }

}


unsigned int Cube::checkIfTextureExists(const char* path) {

    auto it = textureMap.find(std::string(path));

    if (it != textureMap.end()) {
        return it->second;
    }
    else {
        return 0;
    }
}

void Cube::setFaceTexture(int faceIndex, const char* basePath, const char* emissionPath, const char* overlayPath) {

    if (faceIndex < 0 || faceIndex >= 6) {
        std::cerr << "Invalid face index\n";
        return;
    }
    
    if (basePath) {
        unsigned int base = checkIfTextureExists(basePath);

        if (!base) {
            base = loadTexture(basePath);
            textureMap[std::string(basePath)] = base;
        }

        faceTextures[faceIndex].base = base;
        
    }

    if (emissionPath) {
        unsigned int emission = checkIfTextureExists(emissionPath);

        if (!emission) {
            emission = loadTexture(emissionPath);
            textureMap[std::string(emissionPath)] = emission;
        }

        faceTextures[faceIndex].emmission = emission;
    }

    if (overlayPath) {
        unsigned int overlay = checkIfTextureExists(overlayPath);

        if (!overlay) {
            overlay = loadTexture(overlayPath);
            textureMap[std::string(overlayPath)] = overlay;
        }

        faceTextures[faceIndex].overlay = overlay;

    }
}


void Cube::render(glm::vec3 cubePosition, Shader &shader, unsigned int index) 
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePosition);
    float angle = 20.0f * index;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);

    
    for (int i = 0; i < 6; ++i) {
        if (faceTextures[i].base != 0)
        {
            shader.setInt("material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, faceTextures[i].base);

        }
        if (faceTextures[i].emmission != 0)
        {
            shader.setInt("material.emission", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, faceTextures[i].emmission);

        }
        if (faceTextures[i].overlay != 0) 
        {
            shader.setInt("material.specular", 2);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, faceTextures[i].overlay);

        }

        glDrawArrays(GL_TRIANGLES, i * 6, 6);

    }




}