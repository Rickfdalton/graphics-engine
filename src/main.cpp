#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb/stb_image.h>
extern "C" {
#include <tinyfiledialogs/tinyfiledialogs.h>
}

using namespace std;

// Forward declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);
unsigned int load_texture(const char *path);

// Global variables
bool cameraMode = true;
Camera cam = Camera(glm::vec3(0.0f, 0.0f, 2.5f));
Camera_Movement cam_mov;
float dt, lastX, lastY, pitch, yaw, fov;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (!cameraMode) return;
    
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    cam.process_mouse_move(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    if (!cameraMode) return;
    cam.process_mouse_scroll(yoffset);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.process_keyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.process_keyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.process_keyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.process_keyboard(RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam.process_keyboard(UP, dt);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam.process_keyboard(DOWN, dt);
}

unsigned int load_texture(const char *path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                                 GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
    } else {
        cout << "TEXTURE LOAD FAILED: " << path << endl;
        glDeleteTextures(1, &texture);
        return 0;
    }
}

int main() {
    stbi_set_flip_vertically_on_load(true);

    // Initialize GLFW
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(800, 400, "Hello Window", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize glad" << endl;
        return -1;
    }

    // Initialize variables
    lastX = 400;
    lastY = 300;
    pitch = 0.0f;
    yaw = 0.0f;
    fov = 45.0f;

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_STENCIL_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup vertex data
    float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    float planeVertices[] = {
            5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, 5.0f,
            0.0f, 0.0f,  -5.0f, -0.5f, -5.0f, 0.0f,  2.0f,
            5.0f, -0.5f, 5.0f,  2.0f,  0.0f,  -5.0f, -0.5f, -5.0f,
            0.0f, 2.0f,  5.0f,  -0.5f, -5.0f, 2.0f,  2.0f};

    // Load shader
    Shader ourShader("../shaders/depth_testing.vs", "../shaders/depth_testing.fs");
    Shader borderShader("../shaders/depth_testing.vs", "../shaders/border_shader.fs");

    // Setup cube VAO/VBO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Setup plane VAO/VBO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    // Load textures
    char cubeTexturePath[260] = "../assets/textures/wall-egypt.png";
    char planeTexturePath[260] = "../assets/textures/metal.png";
    unsigned int cube_texture = load_texture(cubeTexturePath);
    unsigned int plane_texture = load_texture(planeTexturePath);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    borderShader.use();
    borderShader.setInt("texture1", 0);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Timing
    dt = 0.0f;
    float last_time = 0.0f;
    glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Handle camera mode toggle
        bool currentMode = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (currentMode != cameraMode) {
            cameraMode = currentMode;
            firstMouse = true;
            int cursorMode = cameraMode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
            glfwSetInputMode(window, GLFW_CURSOR, cursorMode);
        }

        // Update framebuffer and delta time
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);

        float current_time = glfwGetTime();
        dt = current_time - last_time;
        last_time = current_time;

        process_input(window);

        // Render
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);  

        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00); 

        ourShader.use();
        ourShader.setVec3("viewPos", cam.cam_pos);

        glm::mat4 view = cam.get_view_mat();
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE,
                                             glm::value_ptr(view));

        glm::mat4 projection = glm::perspective(glm::radians(cam.zoom),
                                                                                     (float)fbWidth / (float)fbHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE,
                                             glm::value_ptr(projection));
 

        // Render plane
        glBindVertexArray(planeVAO);
        ourShader.setInt("texture1", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, plane_texture);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE,
                                             glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        

        // Render cubes
        glBindVertexArray(cubeVAO);
        ourShader.setInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        model = glm::mat4(1.0f);

        glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        glStencilMask(0xFF);
        // Render cube 0
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE,
                                             glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render cube 1
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE,
                                             glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //==================================
        //Drawing border 
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        
        borderShader.use();
        borderShader.setVec3("viewPos", cam.cam_pos);

        view = cam.get_view_mat();
        glUniformMatrix4fv(glGetUniformLocation(borderShader.ID, "view"), 1, GL_FALSE,
                                             glm::value_ptr(view));

        projection = glm::perspective(glm::radians(cam.zoom),(float)fbWidth / (float)fbHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(borderShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(cubeVAO);
        borderShader.setInt("texture1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_texture);
        model = glm::mat4(1.0f);
        
        // Render cube 0 upscaled
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.2f,1.2f,1.2f));
        glUniformMatrix4fv(glGetUniformLocation(borderShader.ID, "model"), 1, GL_FALSE,
                                             glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Render cube 1 upscaled
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.2f,1.2f,1.2f));
        glUniformMatrix4fv(glGetUniformLocation(borderShader.ID, "model"), 1, GL_FALSE,
                                             glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  
        glEnable(GL_DEPTH_TEST); 


        // ImGui UI
        ImGui::Begin("Engine Debug");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::ColorEdit3("Background Color", &clearColor[0]);

        ImGui::Separator();
        ImGui::Text("Cube Texture");
        if (ImGui::Button("Browse Cube Texture"))
        {
            const char* path = tinyfd_openFileDialog(
                "Select Cube Texture",
                "",
                0,
                NULL,
                NULL,
                0
            );

            if (path)
            {
                unsigned int newTexture = load_texture(path);
                if (newTexture != 0)
                {
                    if (cube_texture != 0)
                        glDeleteTextures(1, &cube_texture);

                    cube_texture = newTexture;
                    strcpy(cubeTexturePath, path);
                }
            }
        }
        ImGui::TextWrapped("%s", cubeTexturePath);

        ImGui::Separator();
         if (ImGui::Button("Browse Floor Texture"))
        {
            const char* path = tinyfd_openFileDialog(
                "Select Floor Texture",
                "",
                0,
                NULL,
                NULL,
                0
            );

            if (path)
            {
                unsigned int newTexture = load_texture(path);
                if (newTexture != 0)
                {
                    if (plane_texture != 0)
                        glDeleteTextures(1, &plane_texture);

                    plane_texture = newTexture;
                    strcpy(planeTexturePath, path);
                }
            }
        }
        ImGui::TextWrapped("%s", planeTexturePath);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}
