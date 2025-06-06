#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION

#include <textures.h>
#include <shader.h>
#include <box.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

float mixRatio = 0.0f;

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
0, 1, 3, // first triangle
1, 2, 3  // second triangle
};



int main() {
    //glfw: initialize and configure

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window:" << std::endl;
        glfwTerminate();
        return -1;


    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //using events
    glfwSetKeyCallback(window, key_callback);

	//glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //checking maximum vertex attribute supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "maximum nr of vertex attributes upported" << nrAttributes << std::endl;


    Shader shader("shaders/vshader2.vert", "shaders/fshader3.frag");
    BoxMesh cubeMesh(vertices, sizeof(vertices), indices, sizeof(indices));

    BoxInstance background(&cubeMesh, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(5.0f, 5.0f, 1.0f), "../../images/png/BG/BG.png");//bottom
    BoxInstance ground(&cubeMesh, glm::vec3(0.0f, -1.0f,0.0f), glm::vec3(8.0f, 1.0f, 1.0f), "../../images/png/Tiles/2.png");//bottom

    std::vector<BoxInstance> crates;

    //crate positions, eyeballing it
    glm::vec3 cratePositions[] = {
            glm::vec3(-1.0f,  -0.2f,  1.0f),
            glm::vec3(-0.7f,  -0.2f,  1.0f),
            glm::vec3(-0.4f,  -0.2f,  1.0f),
            glm::vec3(-0.7f,   0.1f,  1.0f),
    };

    for (int i = 0; i < (sizeof(cratePositions) / sizeof(glm::vec3)); i++)
    {
        crates.emplace_back(&cubeMesh, cratePositions[i], glm::vec3(0.3f, 0.3f, 1.0f), "../../images/wooden_box.jpg");
    }

    BoxInstance sign(&cubeMesh, glm::vec3(0.7f, -0.25f, 1.0f), glm::vec3(0.2f, 0.2f, 1.0f), "../../images/png/Object/Sign_2.png");

    //render loop
    while (!glfwWindowShouldClose(window)) 
    {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


        processInput(window);
   
        background.render(shader, 1.0f, 1.0f);
        ground.render(shader, 8.0, 1.0);
        for (const auto& crate : crates)
        {
            crate.render(shader, 1.0, 1.0);
        }

        sign.render(shader, 1.0, 1.0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //deallocate resources
    
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << std::to_string(width) << "px" << "\t" << std::to_string(height) << "px" << std::endl;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "Space key callback - single press!" << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixRatio += 0.2f;
        if (mixRatio > 1.0f)
            mixRatio = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixRatio -= 0.2f;
        if (mixRatio < 0.0f)
            mixRatio = 0;
    }
}

void processInput(GLFWwindow* window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    /*
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixRatio += 0.2f;
        if (mixRatio > 1.0f)
            mixRatio = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixRatio -= 0.2f;
        if (mixRatio < 0.0f);
            mixRatio = 0;
    }
    */
}
