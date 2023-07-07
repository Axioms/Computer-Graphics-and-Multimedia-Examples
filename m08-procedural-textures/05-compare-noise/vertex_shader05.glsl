
#version 450 core

in vec4 bPosition;
//in vec4 bColor;
in vec2 bTextureCoord;

uniform mat4 transform;

//out vec4 vColor;
out vec2 vTextureCoord;

void
main()
{
    //vColor = bColor;
    vTextureCoord = bTextureCoord;
    //gl_PointSize = 25.0;
    gl_Position = transform * bPosition;
}
