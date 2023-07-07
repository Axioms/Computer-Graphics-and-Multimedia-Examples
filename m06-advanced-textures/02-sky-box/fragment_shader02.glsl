#version 430 core

out vec4 fColor;

in vec2 vTextureCoord;
in vec4 vPosition;

uniform sampler2D  sampler2;
uniform samplerCube  samplerC;
uniform int surface_effect;

void main()
{
    /*
     * Surface_effect is used to distinguish between the skybox and the scene.
     */
    if(surface_effect == 0) {
        fColor = texture(sampler2, vTextureCoord);
    } else {
        fColor = texture(samplerC, vPosition.xyz);
    }
}
