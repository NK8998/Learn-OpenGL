#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>

#include <shader.h>
#include <functions.h>
#include <get_execution_dir.h>
#include <cube.h>
#include <camera.h>
#include <stb_image.h>
#include <model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 cameraUp);


unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

float mixRatio = 0.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, 5.0f);
glm::vec3 lightColor = glm::vec3(1.0f);



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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //using events
    glfwSetKeyCallback(window, key_callback);

    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    //buld and compile shader program
    //-------------------------------
    Shader ourShader(makeSourcePath("shaders/model_loading.vert"), makeSourcePath("shaders/model_loading.frag"));

    Model ourModel(makeSourcePath("resources/objects/camaro/scene.gltf"));

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window);

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();
        
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("lightColor", lightColor);

        //view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //render the model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_HEIGHT = static_cast<unsigned int>(height);
    SCR_WIDTH = static_cast<unsigned int>(width);
    firstMouse = true;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.05f;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) 
    {
        std::cout << "Space key callback - single press!" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixRatio += 0.2f;
        if (mixRatio > 1.0f)
            mixRatio = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixRatio -= 0.2f;
        if (mixRatio < 0.0f)
            mixRatio = 0;
    }

}

glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 cameraUp)
{
    glm::vec3 up = cameraUp;
    glm::vec3 forward = normalize(cameraPos - target);
    glm::vec3 right = normalize(cross(up, forward));
    up = cross(forward, right);

    glm::vec3 fpsCam = cameraPos;

    return glm::mat4(
        glm::vec4(right.x, up.x, forward.x, 0.0f),
        glm::vec4(right.y, up.y, forward.y, 0.0f),
        glm::vec4(right.z, up.z, forward.z, 0.0f),
        glm::vec4(
            -glm::dot(right, fpsCam),
            -glm::dot(up, fpsCam),
            -glm::dot(forward, fpsCam),
            1.0f
        )
    );
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

  
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
