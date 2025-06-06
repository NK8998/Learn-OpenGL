#ifndef BOX_INSTANCE_H
#define BOX_INSTANCE_H

#include <textures.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoxMesh {
public:
    unsigned int VAO, VBO, EBO;

    BoxMesh(const float* vertices, size_t vertSize, const unsigned int* indices, size_t indexSize) 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // tex coords
        glEnableVertexAttribArray(2);
    }

    void bind() const {
        glBindVertexArray(VAO);
    }
};


class BoxInstance {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation; 
    unsigned int texture;
    BoxMesh* mesh;

    BoxInstance(BoxMesh* meshRef, glm::vec3 pos, glm::vec3 scale, const char* texture_path)
        : mesh(meshRef), position(pos), scale(scale), rotation(0.0f)
    {
        Texture_Loader tex(texture_path);
        texture = tex.texture;
    }

    void render(Shader& shader, float texRepeatX, float texRepeatY) const {

        mesh->bind();
        glBindTexture(GL_TEXTURE_2D, texture);
        //scaling -> rotation -> translation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        //rotate here 
        model = glm::scale(model, scale);
        // transformations
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


        shader.use();
        shader.setFloat("repeatX", texRepeatX);
        shader.setFloat("repeatY", texRepeatY);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    }
};


#endif