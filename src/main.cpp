#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb/stb_image.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);
unsigned int load_texture(const char *path);
bool cameraMode = false;

Camera cam = Camera(glm::vec3(0.0f, 0.0f, 2.5f));
Camera_Movement cam_mov;

float dt, lastX, lastY, pitch, yaw, fov;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (!cameraMode)
    return;
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
  if (!cameraMode)
    return;
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
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

  if (data) {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "TEXTURE LOAD FAILED: " << path << endl;
  }
  stbi_image_free(data);
  return texture;
}

int main() {
  stbi_set_flip_vertically_on_load(true);

  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW\n";
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 400, "Hello Window", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize glad" << std::endl;
    return -1;
  }

  lastX = 400;
  lastY = 300;
  pitch = 0.0f;
  yaw = 0.0f;
  fov = 45.0f;

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  dt = 0.0f;
  float last_time = 0.0f;
  glm::vec4 clearColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
  glm::vec3 lightPos(2.2f, 1.0f, 1.0f);

  //  Setup Dear ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  Shader ourShader("../shaders/model_loading.vs",
                   "../shaders/model_loading.fs");
  Model ourModel("../assets/"
                 "bone_swords/scene.gltf");

  // render loop
  while (!glfwWindowShouldClose(window)) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool currentMode = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (currentMode != cameraMode) {
      cameraMode = currentMode;

      if (cameraMode) {
        firstMouse = true;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    float current_time = glfwGetTime();
    dt = current_time - last_time;
    last_time = current_time;

    process_input(window);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();
    ourShader.setVec3("lightPos", lightPos);
    ourShader.setVec3("viewPos", cam.cam_pos);
    ourShader.setVec3("lightColor", glm::vec3(1.0f));

    glm::mat4 view;
    view = cam.get_view_mat();
    int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(
        glm::radians(cam.zoom), (float)fbWidth / (float)fbHeight, 0.1f, 100.0f);

    int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    ourModel.Draw(ourShader);

    ImGui::Begin("Engine Debug");

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("Background Color", &clearColor[0]);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}