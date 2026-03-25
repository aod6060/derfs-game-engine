/*
    This is a simple copy shader

    Basically it will make a copy of a texture using 
    framebuffers.

    This could be used to for example to apply 
    gaussian_blur over and over again.
*/
#version 400

uniform sampler2D tex0;

in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
    out_Color = texture(tex0, v_TexCoords);
}