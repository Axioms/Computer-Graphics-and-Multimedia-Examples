
#version 430 core

in vec4 bPosition;


uniform mat4 transform;


void
main()
{
//    gl_PointSize = 400.0;
    vec4 pos = transform * bPosition;
    //gl_PointSize =(3.0 -  pos.z/pos.w) * 64;
    gl_Position = pos;
}
