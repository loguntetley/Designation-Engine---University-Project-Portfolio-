#version 450

layout(location = 0) in vec3 fragmentColour;
layout(location = 1) in vec2 fragmentTexture;
layout(set = 1, binding = 0) uniform sampler2D textureSampler;
layout(location = 0) out vec4 outColour;

void main() 
{
	outColour = texture(textureSampler, fragmentTexture);
}