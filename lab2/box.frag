#version 330 core

in vec2 uv; // Keep UV for texture mapping

// Texture sampler
uniform sampler2D textureSampler; 

out vec3 finalColor;

void main()
{
    // Apply texture only, without color
    finalColor = texture(textureSampler, uv).rgb;
}
