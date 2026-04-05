// clang-format off
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <objloader.h>
#include "shader.h"
#include "camera.h"

// clang-format off
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Camera camera; 
double last_x;
double last_y;
bool first_mouse = true;
bool wireframe_mode = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

void Key_Input_Callback(GLFWwindow *window, int key, int scan_code, int action,
                        int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    float move_speed = 0.8;
    switch (key) {
    case GLFW_KEY_W: {
      auto pos = camera.GetPosition() + camera.GetForwardVector() * move_speed;
      camera.SetPosition(pos);
      return;
    }
    case GLFW_KEY_A: {
      auto pos = camera.GetPosition() -
                 glm::normalize(glm::cross(camera.GetForwardVector(),
                                           camera.GetUpVector())) *
                     move_speed;
      camera.SetPosition(pos);
      return;
    }
    case GLFW_KEY_S: {
      auto pos = camera.GetPosition() - camera.GetForwardVector() * move_speed;
      camera.SetPosition(pos);
      return;
    }
    case GLFW_KEY_D: {
      auto pos = camera.GetPosition() +
                 glm::normalize(glm::cross(camera.GetForwardVector(),
                                           camera.GetUpVector())) *
                     move_speed;
      camera.SetPosition(pos);
      return;
    }
    case GLFW_KEY_C: {
      if (!wireframe_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframe_mode = true;
      } else {
        wireframe_mode = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
      return;
    }
    default:;
    }
  }
}

void Mouse_Input_Callback(GLFWwindow *window, double xpos, double ypos) {
  if (first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_x;
  float yoffset =
      last_y - ypos; // reversed since y-coordinates go from bottom to top

  last_x = xpos;
  last_y = ypos;

  camera.MouseCallback(window, xoffset, yoffset);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OBJ_LOADER", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader shader("shader.vert", "shader.frag");

  object_attrib_t object;

  std::string path = "cube.obj";

  int result = read_obj_file(path.data(), &object);
  assert(result == 0);

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * object.num_vertices), object.vertices, GL_STATIC_DRAW);
  printf("%zu\n", object.num_vertices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (sizeof(int) * object.num_indices), object.indices,
               GL_STATIC_DRAW);

  printf("%zu\n", object.num_indices);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // Model View Projection
  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
  
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);

    // projection
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  shader.use();   
 
  while (!glfwWindowShouldClose(window)) {

     glfwSetKeyCallback(window, Key_Input_Callback);
    glfwSetCursorPosCallback(window, Mouse_Input_Callback); 
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view",camera.GetView());
    shader.setMat4("projection", projection);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, object.num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
