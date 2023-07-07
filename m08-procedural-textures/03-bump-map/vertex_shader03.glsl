
#version 430 core

in vec4 bPosition;
in vec4 bNormal;
in vec4 bTangent;
in vec2 bTextureCoord;

uniform mat4 mv_transform;
uniform mat4 v_transform;
uniform mat4 p_transform;
uniform mat4 normal_transform;
uniform vec4 light_position;  // in view coordinates


out vec4 vNormal;
out vec4 vPosition;


out vec3 LightDir;
out vec3 EyeDir;
out vec2 TexCoord;


void
main()
{
//    vNormal = normal_transform * bNormal;
//    vPosition = mv_transform * bPosition;
//    gl_Position =  p_transform * vPosition;

    mat4 mvp_transform = p_transform * mv_transform;

    EyeDir = vec3(mv_transform * bPosition);
    TexCoord = bTextureCoord.st;
    vec3 n = normalize((normal_transform * bNormal).xyz);
    vec3 t = normalize((normal_transform * bTangent).xyz);
    vec3 b = cross(n, t);
    vec3 v;
    vec4 LightPosition = light_position;
    v.x = dot(LightPosition.xyz, t);
    v.y = dot(LightPosition.xyz, b);
    v.z = dot(LightPosition.xyz, n);
    LightDir = normalize(v);
    v.x = dot(EyeDir, t);
    v.y = dot(EyeDir, b);
    v.z = dot(EyeDir, n);
    EyeDir = normalize(v);


    gl_Position = mvp_transform * bPosition;


}
