#version 460 core

layout (location = 0) in vec3 a_Position;
out vec2 fragCoord;

void main()
{
	gl_Position = vec4(a_Position, 1.0f);
	fragCoord = a_Position.xy;
}