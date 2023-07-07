#version 430 core

out vec4 fColor;

uniform vec4 ambient_color;
uniform vec4 ambient_light;

void main()
{
    /**
     * The ambient light is mixed with the ambient color to get the ambient component.
     * For this example, this is the only component used to color the fragment.
     */
    fColor = ambient_color * ambient_light;
}
