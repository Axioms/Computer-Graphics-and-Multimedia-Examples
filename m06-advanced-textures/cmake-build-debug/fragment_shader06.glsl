#version 430 core

out vec4 fColor;

uniform sampler2D  sampler;

void main()
{
    // gl_PointCoord provides the coordinates of this fragment inside the point
    //     The coordinates are in the range from 0 to 1.
    fColor = texture(sampler, gl_PointCoord);
}
