#version 430 core

out vec4 fColor;

// Interpolated position in the object coordinate frame
in vec4 vPosition;

// The texture sampler for a cube
uniform samplerCube  sampler;

void main()
{
    // use the xyz coordinates of vPosition
    // this is interpreted as a vector from the origin to the point on the surface of the
    // untransformed shape.
    fColor = texture(sampler, vPosition.xyz);

    // There are two statements below that can be uncommented to set fColor.
    //  These were added for debugging purposes.

    // the first debugging statement visualizes the position as a color
    //      this is to see if the position is being sent properly to the fragment shader
    // The positions is normalized to bring the coordinates into range -1 to 1
    //    1 is added to each coordinate to bring them to range 0 to 2
    //    then divide by 2 to bring them to range 0 to 1
    // fColor = (vec4(normalize(vPosition).xyz,1.0) + vec4(1.0))/2.0;

    // this fColor just shows yellow, this is to see if the cube geometry is ok
    //    fColor = vec4(1,1,0,1);
}
