#version 400


uniform sampler2D tex0;

uniform float minValue = 0.0;
uniform float maxValue = 1.0;

in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
    vec4 color = texture(tex0, v_TexCoords);
    // I might have to change this because I don't remember
    // if glsl allows for "() ? [true] : [false]" construct.
    // Red
    color.r = (color.r >= minValue && color.r <= maxValue) ? color.r : 0.0;
    // Green
    color.g = (color.g >= minValue && color.g <= maxValue) ? color.g : 0.0;
    // Blue
    color.b = (color.b >= minValue && color.b <= maxValue) ? color.b : 0.0;
    out_Color = color;
}