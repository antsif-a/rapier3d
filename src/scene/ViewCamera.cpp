#include <glm/glm.hpp>

#include "window/framework/WindowFramework.hpp"
#include "scene/ViewCamera.hpp"

using namespace glm;
using namespace WindowFramework;

static vec3 compute_camera_direction(vec2 angle) {
    return vec3(
        cos(radians(angle.x)) * cos(radians(angle.y)),
        sin(radians(angle.y)),
        sin(radians(angle.x)) * cos(radians(angle.y))
    );
}

ViewCamera::ViewCamera(
    vec3 init_position,
    vec2 init_angle,
    vec2 init_cursor_position,
    float mouse_sensitivity,
    float key_sensitivity
)
    : position(init_position)
    , angle(init_angle)
    , last_cursor_position(init_cursor_position)
    , mouse_sensitivity(mouse_sensitivity)
    , key_sensitivity(key_sensitivity)
    , direction(compute_camera_direction(init_angle))
{}

void ViewCamera::InvokeCursorCallback(double xpos, double ypos) {
    vec2 cursor_offset(xpos - last_cursor_position.x, last_cursor_position.y - ypos);
    cursor_offset *= mouse_sensitivity;
    
    angle.x += cursor_offset.x;
    if (abs(angle.y + cursor_offset.y) <= 90.0f)
        angle.y += cursor_offset.y;

    direction = compute_camera_direction(angle);

    last_cursor_position = vec2(xpos, ypos);
}

void ViewCamera::InvokeKeyCallback(int key, int action) {
    if (!(action == KeyAction::Press or action == KeyAction::Release))
        return;
    bool is_press = action == GLFW_PRESS;
    if (key == Key::A)
        movement_bits.set(left, is_press);
    if (key == Key::D)
        movement_bits.set(right, is_press);
    if (key == Key::Space)
        movement_bits.set(up, is_press);
    if (key == Key::LeftShift)
        movement_bits.set(down, is_press);
    if (key == Key::W)
        movement_bits.set(front, is_press);
    if (key == Key::S)
        movement_bits.set(back, is_press);
}

mat4 ViewCamera::ComputeViewMatrix(float delta_time) {
    vec3 movement(
        movement_bits[right] - movement_bits[left],
        movement_bits[up] - movement_bits[down],
        movement_bits[front] - movement_bits[back]
    );

    movement *= key_sensitivity * delta_time;
    
    if (movement.x)
        position += movement.x * glm::cross(direction, camera_up);
    if (movement.y)
        position += movement.y * camera_up;
    if (movement.z)
        position += movement.z * direction;

    return glm::lookAt(position, position + direction, camera_up);
}

vec3 ViewCamera::GetCameraPosition() {
    return position;
}

vec3 ViewCamera::GetCameraDirection() {
    return direction;
}

void ViewCamera::Rotate(float yaw, float pitch) {
    this->angle.x += yaw;
    this->angle.y += pitch;
}