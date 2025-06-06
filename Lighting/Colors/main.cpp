#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>

#include <shader.h>
#include <functions.h>

#define STB_IMAGE_IMPLEMENTATION

#include <cube.h>
#include <textures.h>

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

//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
bool droneCam = false;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0f / 2.0;
float fov   =  45.0f;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.5f, 3.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

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

    glEnable(GL_DEPTH_TEST);

    //checking maximum vertex attribute supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "maximum nr of vertex attributes upported" << nrAttributes << std::endl;

    //buld and compile shader program
    //-------------------------------

    Shader lightingShader("shaders/colors.vert", "shaders/colors.frag");
    Shader lightCubeShader("shaders/light_cube.vert", "shaders/light_cube.frag");

    Cube cube(1.0f, 1.0f, 1.0f);
    cube.uploadToGPU();
    Cube lightCube(0.5f, 0.5f, 0.5f);
    lightCube.uploadToGPU();


    //cube positions 
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f, 3.2f),
            glm::vec3(0.0f,  0.0f, -3.2f),
            glm::vec3(4.0f,  0.0f, -3.2f),
    };

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = LookAt(cameraPos, cameraPos + cameraFront, cameraUp);//Look at


        float changingX = sin(glfwGetTime() * lightPos.x) * 2;

        glm::vec3 movingLight = glm::vec3(changingX, lightPos.y, lightPos.z);

        lightingShader.use();
        lightingShader.setVec3("lightPos", movingLight);
        lightingShader.setVec3("viewPos", cameraPos);
        lightingShader.setVec3("objectColor", objectColor);
        lightingShader.setVec3("lightColor", lightColor);
        lightingShader.setFloat("mixRatio", mixRatio);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        cube.render(cubePositions[0], lightingShader);

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        lightCube.render(movingLight, lightCubeShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << std::to_string(width) << "px" << "\t" << std::to_string(height) << "px" << std::endl;
    SCR_HEIGHT = static_cast<unsigned int>(height);
    SCR_WIDTH = static_cast<unsigned int>(width);
    firstMouse = true;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const float cameraSpeed = 0.05f;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        droneCam = !droneCam;
    }

}

glm::mat4 LookAt(glm::vec3 cameraPos, glm::vec3 target, glm::vec3 cameraUp)
{
    glm::vec3 forward = normalize(cameraPos - target);
    glm::vec3 right = normalize(cross(cameraUp, forward));
    glm::vec3 up = cross(forward, right);
    float cameraYpos = droneCam ? cameraPos.y : 0.0f;
    glm::vec3 fpsCam = glm::vec3(cameraPos.x, cameraYpos, cameraPos.z);

    return glm::mat4(
        glm::vec4(right.x, up.x, forward.x, 0.0),
        glm::vec4(right.y, up.y, forward.y, 0.0),
        glm::vec4(right.z, up.z, forward.z, 0.0),
        glm::vec4(
            -glm::dot(right, fpsCam), 
            -glm::dot(up, fpsCam), 
            -glm::dot(forward, fpsCam), 
            1.0
        )
    );
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = static_cast<float>(2.5f * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;

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

    float sensitivity = 0.1f; // 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // ensuring when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// Create 3d world where player can move around. 
// See if u can detect collision and avoid clipping into things. bruh fuck no