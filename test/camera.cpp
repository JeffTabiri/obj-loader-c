#include "camera.h"

#include <cstdio>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/vec3.hpp"

Camera::Camera() {}

Camera::Camera(glm::vec3 position) {
  m_position = position;
  m_up = glm::vec3(0.0f, 1.0f, 0.0f);
  m_target = glm::vec3(0.0f, 0.0f, 1.0f);
  m_forward = glm::normalize(m_position - m_target);
  m_right = glm::normalize(glm::cross(m_up, m_forward));
  m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
  m_up = glm::cross(m_forward, m_right);
  m_yaw = -90.f;
  m_pitch = 0.0f;
  UpdateCameraVectors();
}

glm::mat4 Camera::GetView() const {
  glm::mat4 inverse_eye_pos(1.0f);
  inverse_eye_pos = glm::translate(inverse_eye_pos, -m_position);

  // own implementation of look_at
  glm::vec3 forward_vector =
      glm::normalize(m_position - (m_position + m_forward));
  glm::vec3 left = glm::normalize(glm::cross(m_up, forward_vector));
  glm::vec3 up = glm::cross(forward_vector, left);
  glm::mat4 rotation = glm::transpose(
      glm::mat4(glm::vec4(left, 0.0f), glm::vec4(up, 0.0f),
                glm::vec4(forward_vector, 0.0f), glm::vec4(0, 0, 0, 1)));

  // return glm::transpose(rotation) * inverse_eye_pos;
  return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::MouseCallback(GLFWwindow *window, double x_offset,
                           double y_offset) {
  x_offset *= mouse_sensitivity;
  y_offset *= mouse_sensitivity;
  m_yaw += x_offset;
  m_pitch += y_offset;

  // when pitch is getting out of bounds, screen dont flip.
  if (m_pitch > 89.0f)
    m_pitch = 89.0f;
  if (m_pitch < -89.0f) {
    m_pitch = -89.0f;
  }

  UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  front.y = sin(glm::radians(m_pitch));
  front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_forward = glm::normalize(front);
  m_right = glm::normalize(glm::cross(m_forward, m_world_up));
  m_up = glm::normalize(glm::cross(m_right, m_forward));
}

float Camera::GetYaw() const { return m_yaw; }

void Camera::SetYaw(float yaw) { m_yaw = yaw; }

float Camera::GetPitch() const { return m_pitch; }

void Camera::SetPitch(float pitch) { m_pitch = pitch; }

void Camera::SetPosition(const glm::vec3 positon) { m_position = positon; }

glm::vec3 Camera::GetForwardVector() const { return m_forward; }

glm::vec3 Camera::GetUpVector() const { return m_up; }

glm::vec3 Camera::GetRightVector() const { return m_right; }

void Camera::SetTarget(glm::vec3 target) { m_target = target; }

glm::vec3 Camera::GetPosition() const { return m_position; }

glm::vec3 Camera::GetTarget() const { return m_target; }
