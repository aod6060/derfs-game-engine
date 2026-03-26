#version 400


// This will take in to texture and combine them
// using different methods

#define COMBINE_OP_ADD 0
#define COMBINE_OP_SUB 1
#define COMBINE_OP_MUL 2
#define COMBINE_OP_DIV 3
#define COMBINE_OP_MIX 4
#define COMBINE_OP_TEXTURE 5
// cameraPosition - texture
#define COMBINE_OP_CAMERA_TEXTURE 6
// texture - cameraPosition
#define COMBINE_OP_TEXTURE_CAMERA 7
// Normalized
#define COMBINE_OP_CAMERA_TEXTURE_NORMALIZED 8
#define COMBINE_OP_TEXTURE_CAMERA_NORMALIZED 9

uniform int op = COMBINE_OP_ADD;
uniform sampler2D a;
uniform sampler2D b;
uniform float mixValue = 0.5;
uniform vec3 cameraPosition;

// If COMBINE_OP_MIX is used it will be 
// a Value between 0 -> 1 using the mix function


in vec2 v_TexCoords;

out vec4 out_Color;

void load_sampler(out vec3 valueA, out vec3 valueB);

void add_sampler();
void sub_sampler();
void mul_sampler();
void div_sampler();
void mix_sampler();
void texture_sampler();
void camera_texture_sampler();
void texture_camera_sampler();
void camera_texture_normalized_sampler();
void texture_camera_normalized_sampler();

void main() {
    switch(op) {
        case COMBINE_OP_ADD:
            add_sampler();
            break;
        case COMBINE_OP_SUB:
            sub_sampler();
            break;
        case COMBINE_OP_MUL:
            mul_sampler();
            break;
        case COMBINE_OP_DIV:
            div_sampler();
            break;
        case COMBINE_OP_MIX:
            mix_sampler();
            break;
        case COMBINE_OP_TEXTURE:
            texture_sampler();
            break;
        case COMBINE_OP_CAMERA_TEXTURE:
            camera_texture_sampler();
            break;
        case COMBINE_OP_TEXTURE_CAMERA:
            texture_camera_sampler();
            break;
        case COMBINE_OP_CAMERA_TEXTURE_NORMALIZED:
            camera_texture_normalized_sampler();
            break;
        case COMBINE_OP_TEXTURE_CAMERA_NORMALIZED:
            texture_camera_normalized_sampler();
            break;
    }
}

void add_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    out_Color = vec4(va + vb, 1.0);
}

void sub_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    out_Color = vec4(va - vb, 1.0);
}

void mul_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    out_Color = vec4(va * vb, 1.0);
}

void div_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    out_Color = vec4(va / vb, 1.0);
}

void mix_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    out_Color = vec4(mix(va, vb, mixValue), 1.0);
}

void texture_sampler() {
    vec3 va, vb;
    load_sampler(va, vb);
    float value = length(vb);
    out_Color = vec4(mix(va, vb, value), 1.0);
}

void load_sampler(out vec3 valueA, out vec3 valueB) {
    valueA = texture(a, v_TexCoords).rgb;
    valueB = texture(b, v_TexCoords).rgb;
}

void camera_texture_sampler() {
    vec4 color = texture(a, v_TexCoords);
    vec3 
    out_Color = 
}

void texture_camera_sampler() {

}

void camera_texture_normalized_sampler() {

}

void texture_camera_normalized_sampler() {

}
