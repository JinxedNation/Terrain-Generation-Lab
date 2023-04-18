#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;




void main()
{
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5); // texture sampler and the corresponding texture coordinates

    
    // previous code
       FragColor = texture(texture1, TexCoord);
    
    //--------------------

    /*
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;
   // FragColor = vec4(lightColor * objectColor, 1.0);

   // FragColor = (texture(texture1, TexCoord) * vec4(lightColor * objectColor, 1.0));
    FragColor = (texture(texture1, TexCoord) * vec4(result, 1.0));
    */
    
}
