#version 450 		//Using GLSL 4.5

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec2 texture;

layout(set = 0, binding = 0) uniform UniformBufferObjectViewProjection 
{
	mat4 m_Projection;
	mat4 m_View;
} UniformBufferObjectViewProjection;

layout(push_constant) uniform PushModel 
{
	mat4 m_Model;
} pushModel;

layout(location = 0) out vec3 fragmentColour;
layout(location = 1) out vec2 fragmentTexture;

void main() 
{
	gl_Position = UniformBufferObjectViewProjection.m_Projection * UniformBufferObjectViewProjection.m_View
	 * pushModel.m_Model * vec4(position, 1.0);
	
	fragmentColour = colour;
	fragmentTexture = texture;
}