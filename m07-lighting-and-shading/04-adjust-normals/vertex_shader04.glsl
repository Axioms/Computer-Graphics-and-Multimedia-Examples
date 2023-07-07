
#version 430 core

in vec4 bPosition;
in vec4 bNormal;

// what we have been calling 'transform' before is called 'mvp_transform' here
// this indicates that the transform is projection times view times animation times world
uniform mat4 mvp_transform;
// the normal transform computed in the driver program
uniform mat4 normal_transform;

out vec4 vNormal;


void
main()
{
    // transform the normal before sending to the fragment shader
    vNormal = normal_transform * bNormal;
    gl_Position =  mvp_transform * bPosition;
}
