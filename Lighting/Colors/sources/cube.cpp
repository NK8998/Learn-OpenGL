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
         w, -h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         w,  h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         w,  h, -d,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -w,  h, -d,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -w, -h, -d,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

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
         w,  h, -d, 1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         w, -h, -d, 1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         w, -h, -d, 1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         w, -h,  d, 1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         w,  h,  d, 1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

         // Bottom face
         -w, -h, -d, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
          w, -h, -d, 0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
          w, -h,  d, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
          w, -h,  d, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         -w, -h,  d, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         -w, -h, -d, 0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

         // Top face
         -w,  h, -d, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
          w,  h, -d, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
          w,  h,  d, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          w,  h,  d, 0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -w,  h,  d, 0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -w,  h, -d, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
        Texture_Loader tex1(tex_path1);
        texture1 = tex1.texture;
    }

    if (tex_path2) {
        Texture_Loader tex2(tex_path2);
        texture2 = tex2.texture;
    }

}

void Cube::setFaceTexture(int faceIndex, const char* basePath, const char* overlayPath) {
    if (faceIndex < 0 || faceIndex >= 6) {
        std::cerr << "Invalid face index\n";
        return;
    }

    if (basePath) {
        Texture_Loader baseTex(basePath);
        faceTextures[faceIndex].base = baseTex.texture;
    }

    if (overlayPath) {
        Texture_Loader overlayTex(overlayPath);
        faceTextures[faceIndex].overlay = overlayTex.texture;
    }
}


void Cube::render(glm::vec3 cubePosition, Shader &shader) 
{
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePosition);
    model = glm::rotate(model, 0.0f, glm::vec3(0.5f, 1.0f, 0.0f));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);

    
    for (int i = 0; i < 6; ++i) {
        if (faceTextures[i].base != 0)
        {
            shader.setInt("material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, faceTextures[i].base);

        }
        if (faceTextures[i].overlay != 0) 
        {
            shader.setInt("material.specular", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, faceTextures[i].overlay);

        }
        glDrawArrays(GL_TRIANGLES, i * 6, 6);

    }




}