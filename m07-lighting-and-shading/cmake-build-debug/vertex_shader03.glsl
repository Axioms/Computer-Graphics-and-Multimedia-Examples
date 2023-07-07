
#version 430 core

// position from the underlying shape
in vec4 bPosition;
// normal vector at this vertex
in vec4 bNormal;

//  model-view-projection transformation
uniform mat4 transform;

// send the normal vector to the fragment shader
out vec4 vNormal;

void
main()
{
    // from the buffer to the out variable to the fragment shader
    vNormal = bNormal;
    // Position of this vertex computed in default coordinate system
    gl_Position = transform * bPosition;
}
