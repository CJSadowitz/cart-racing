#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoord = vec2(aTex.x, aTex.y);
    Normal = vec3(aNormal.x, aNormal.y, aNormal.z);

    gl_Position = projection * view * vec4(FragPos, 1.0f);
}