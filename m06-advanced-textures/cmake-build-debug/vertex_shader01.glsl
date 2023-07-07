
#version 430 core

in vec4 bPosition;

uniform mat4 transform;

//  we use the vertex position, object coordinates, to sample the texture
// in this way, the texture will follow stay fixed on the object.
out vec4 vPosition;

void
main()
{
    gl_Position = transform * bPosition;

    // pass coordinates relative to the shape so that the texture doesn't slide across the surface
    vPosition = bPosition;

    // Try the following to see what happens when the texture is sampled using world coordinates
//    vPosition = transform * bPosition;
}
