#pragma once

#include "rapier3d/window/framework/WindowFramework.hpp"
#include <bitset>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class ViewCamera {
private:
    constexpr static vec3 camera_up = vec3(0.0f, 1.0f, 0.0f);
    const float mouse_sensitivity, key_sensitivity;
    vec3 position, direction;
    vec2 angle;
    vec2 last_cursor_position;

    enum Direction {
        left, right, up, down, front, back
    };
    
    std::bitset<6> movement_bits;
public:
    ViewCamera(
        vec3 init_position = vec3(0), 
        vec2 init_angle = vec2(-90, 0),
        vec2 init_cursor_position = vec2(0),
        float mouse_sensitivity = 5 * 0.01f,
        float key_sensitivity = 5.0f
    );

    void InvokeCursorCallback(double xpos, double ypos);
    void InvokeKeyCallback(WindowFramework::Key key, WindowFramework::KeyAction action);

    mat4 ComputeViewMatrix(float delta_time);
    vec3 GetCameraPosition();
    vec3 GetCameraDirection();

    void Rotate(float yaw = 0, float pitch = 0);
};