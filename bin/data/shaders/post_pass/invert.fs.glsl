// A very simple shader that inverts the input
#version 400


uniform sampler2D tex0;

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    vec4 color = texture(tex0, v_TexCoords);
    out_Color = vec4(1.0 - color.rgb, color.a);
}