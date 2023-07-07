
#version 430 core

in vec4 bPosition;
in vec4 bNormal;
in vec2 bTextureCoord;

uniform mat4 m_transform;
uniform mat4 vp_transform;
uniform mat4 normal_transform;


out vec4 wNormal;
out vec4 wPosition;
out vec2 vTextureCoord;


void
main()
{
    vTextureCoord = bTextureCoord;

    // The normal is transformed to world coordinates
    wNormal = normal_transform * bNormal;
    // The fragment position in the worldcoordinate frame is needed to compute shading.
    // So, the vertex position in world coordinates is sent to the fragment shader
    //      The corner positions will be interpolated to get the fragment position in world coordinates.
    wPosition = m_transform * bPosition;
    // Projecting the view coordinates finishes the transformation to the default coordinate frame.
    gl_Position =  vp_transform * wPosition;
}
