#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in float aColor;
//layout(location = 1) in vec2 aTexCoord;

//out vec2 TexCoord;
out float vertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertColor = aColor;
   // TexCoord = aTexCoord;
}
