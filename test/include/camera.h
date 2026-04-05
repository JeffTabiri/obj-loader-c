#ifndef GRASSY_GRASS_CAMERA_H
#define GRASSY_GRASS_CAMERA_H

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/vec3.hpp"

#endif // GRASSY_GRASS_CAMERA_H

class Camera {
  glm::vec3 m_position{};
  glm::vec3 m_target{};
  glm::vec3 m_forward{};
  glm::vec3 m_right{};
  glm::vec3 m_up{};
  glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

  // Euler Angles
  float m_yaw{};
  float m_pitch{};

  // Cam options
  float movement_speed{0.05};
  float mouse_sensitivity{0.05};

public:
  Camera();
  Camera(glm::vec3 position);
  glm::mat4 GetView() const;
  void SetPosition(glm::vec3 positon);
  void SetTarget(glm::vec3 target);

  glm::vec3 GetPosition() const;
  glm::vec3 GetForwardVector() const;
  glm::vec3 GetUpVector() const;
  glm::vec3 GetRightVector() const;
  glm::vec3 GetTarget() const;

  float GetYaw() const;
  void SetYaw(float yaw);

  float GetPitch() const;
  void SetPitch(float pitch);

  void MouseCallback(GLFWwindow *window, double xpos, double ypos);
  void UpdateCameraVectors();
};
