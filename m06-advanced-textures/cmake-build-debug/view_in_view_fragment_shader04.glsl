#version 430 core

out vec4 fColor;

in vec2 vTextureCoord;

uniform sampler2D  sampler;
uniform vec4 texture_scale_translate;

void main()
{
    // transform the model coordinates to select part of the texture.
    vec2 tc = texture_scale_translate.xy * vTextureCoord
        + texture_scale_translate.zw;
    
    fColor = texture(sampler, tc);
   
}
