#version 330 core
out vec4 FragColor;  

in vec3 pos;
in vec3 normal;
in vec2 tex; // I have NO TEXTURES :D
  
void main()
{
    FragColor = vec4(pos, 1.0);
}