#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    pos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0f);
}