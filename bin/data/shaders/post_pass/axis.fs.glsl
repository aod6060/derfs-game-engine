#version 400

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
#define AXIS_W 3


uniform sampler2D tex0;
uniform int axis = AXIS_X;

in vec2 v_TexCoords;
out vec4 out_Color;

void main() {
    vec4 color = texture(tex0, v_TexCoords);

    switch(axis) {
        case AXIS_X:
            out_Color = vec4(vec3(color.x), 1.0);
            break;
        case AXIS_Y:
            out_Color = vec4(vec3(color.y), 1.0);
            break;
        case AXIS_Z:
            out_Color = vec4(vec3(color.z), 1.0);
            break;
        case AXIS_W:
            out_Color = vec4(vec3(color.w), 1.0);
            break;
    }
}