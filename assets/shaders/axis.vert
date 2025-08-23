#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const vec3 axisColor[3] = vec3[](
    vec3(1, 0 ,0),
    vec3(0, 1 ,0),
    vec3(0, 0 ,1)
);


vec3 makePos(int id) {
    int axis = id / 2; // 0, 1, 2
    int end  = id % 2; // 0, 1
    float t = (end == 0) ? -1.0 : +1.0;
    return axis == 0 ? vec3(t, 0, 0)
         : axis == 1 ? vec3(0, t, 0)
         :             vec3(0, 0, t);
}

out vec3 vColor;

void main() {
    vec3 pos = makePos(gl_VertexID);
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vColor = axisColor[gl_VertexID / 2];
}
