#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} vertex;

float map(float x, float a1, float b1, float a2, float b2) {
    return a2 + (x - a1) * (b2 - a2)/(b1 - a1);
}

uniform vec2 ScreenSize;
uniform vec2 TextureSize;

void main() {
    // 1. Calculate the ratios for width and height
    vec2 ratios = TextureSize / ScreenSize;

    // 2. Determine the final scale
    vec2 finalScale;

    if (TextureSize.x <= ScreenSize.x && TextureSize.y <= ScreenSize.y) {
        // CASE A: Image is smaller than screen - keep original pixel size
        finalScale = ratios;
    } else {
        // CASE B: Image is bigger than screen - "Aspect Fit"
        // We find which dimension is "more" out of bounds and scale by that
        float maxRatio = max(ratios.x, ratios.y);
        finalScale = TextureSize / (ScreenSize * maxRatio);
    }

    // 3. Apply the scale to our -1..1 position
    vec3 p = vec3(Position.xy * finalScale, Position.z);

    vertex.TexCoords = TexCoords;
    gl_Position = vec4(p, 1.0);
}