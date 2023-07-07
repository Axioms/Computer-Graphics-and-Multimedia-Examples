
#version 430 core

in vec4 bPosition;
in vec2 bTextureCoord;

uniform mat4 transform;

out vec2 vTextureCoord;
out vec4 vPosition;

void
main()
{
    vPosition = bPosition;
    vTextureCoord = bTextureCoord;
    gl_Position = transform * bPosition;
}
