/*
    This is looking like a great resource :)
    https://agatedragon.blog/2024/01/09/simple-gaussian-blur/
*/
#version 400

#define ARRAY_SIZE 9

uniform sampler2D tex0;
uniform float widthDistance = 640.0f;
uniform float heightDistance = 320.0f;
in vec2 v_TexCoords;

layout(location = 0) out vec4 out_Color;

void main() {
    float kernal[ARRAY_SIZE];
    kernal[0] = 1.0;
    kernal[1] = 4.0;
    kernal[2] = 1.0;

    kernal[3] = 4.0;
    kernal[4] = 8.0;
    kernal[5] = 4.0;

    kernal[6] = 1.0;
    kernal[7] = 4.0;
    kernal[8] = 1.0;

    for(int i = 0; i < ARRAY_SIZE; i++) {
        kernal[i] /= 28.0;
    }

    float w = 1.0 / widthDistance;
    float h = 1.0 / heightDistance;

    vec2 offsets[ARRAY_SIZE];

    offsets[0] = vec2(-w, -h);
    offsets[1] = vec2(0, -h);
    offsets[2] = vec2(w, -h);

    offsets[3] = vec2(-w, 0);
    offsets[4] = vec2(0, 0);
    offsets[5] = vec2(w, 0);

    offsets[6] = vec2(-w, h);
    offsets[7] = vec2(0, h);
    offsets[8] = vec2(w, h);

    vec3 result = vec3(0.0f);
    for(int i = 0; i < 9; i++) {
        result += texture(tex0, v_TexCoords + offsets[i]).rgb * kernal[i];
    }

    out_Color.rgb = result;
    out_Color.a = texture(tex0, v_TexCoords).a;
}