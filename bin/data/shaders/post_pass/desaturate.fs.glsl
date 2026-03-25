#version 400


uniform sampler2D tex0;
uniform float value; // A Value between 0.0 and 1.0


in vec2 v_TexCoords;
out vec4 out_Color;


void main() {
    vec4 color = texture(tex0, v_TexCoords);
    vec3 bw = vec3((color.r + color.g + color.b) / 3.0);
    out_Color = vec4(mix(color.rgb, bw, value), color.a);
}