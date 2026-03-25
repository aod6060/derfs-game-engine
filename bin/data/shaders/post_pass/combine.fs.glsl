#version 400


// This will take in to texture and combine them
// using different methods

#define COMBINE_OP_ADD 0
#define COMBINE_OP_SUB 1
#define COMBINE_OP_MUL 2
#define COMBINE_OP_DIV 3
#define COMBINE_OP_MIX 4


uniform int op = COMBINE_OP_ADD;
uniform sampler2D a;
uniform sampler2D b;
uniform float mixValue = 0.5; 
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

void load_sampler(out vec3 valueA, out vec3 valueB) {
    valueA = texture(a, v_TexCoords).rgb;
    valueB = texture(b, v_TexCoords).rgb;
}