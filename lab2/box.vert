#version 330 core

// Input
layout(location = 0) in vec3 vertexPosition;
// layout(location = 1) in vec3 vertexColor;  // No need for color
layout(location = 2) in vec2 vertexUV;

// Output data, to be interpolated for each fragment
out vec2 uv; // Keep UV for texture mapping

// Matrix for vertex transformation
uniform mat4 MVP;

void main() {
    // Transform vertex
    gl_Position =  MVP * vec4(vertexPosition, 1);

    // Pass UV to the fragment shader
    uv = vertexUV;
}
