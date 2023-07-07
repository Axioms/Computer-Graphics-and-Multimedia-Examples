#version 430 core

out vec4 fColor;

in vec2 vTextureCoord;

uniform sampler2D  sampler;

void main()
{
    fColor = texture(sampler, vTextureCoord);
}
