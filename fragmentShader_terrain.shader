#version 330 core

out vec4 FragColor;

//in vec2 TexCoord;
in float vertColor;


//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); // texture sampler and the corresponding texture coordinates
    FragColor = vec4(vec3(vertColor / 255.0), 1.0);
   // 
    //FragColor = vec4(vec3(vertColor), 1.0);



}
