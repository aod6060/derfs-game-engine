// Modified Edge Detection -> Basically outputs a map were the edges 1.0 if its an edge and 0.0 if its not a edge.
/*
    This is looking like a great resource :)
   https://agatedragon.blog/2024/01/11/simple-edge-detection/

   I modified it a bit to output black and white values.
*/
#version 400

#define ARRAY_SIZE 9

uniform sampler2D tex0;
uniform float sampleDistance = 200.0f;

in vec2 v_TexCoords;

layout(location = 0) out vec4 out_Color;

void main() {
    float kernal[ARRAY_SIZE];
    kernal[0] = 0.0;
    kernal[1] = -1.0;
    kernal[2] = 0.0;

    kernal[3] = -1.0;
    kernal[4] = 4.0;
    kernal[5] = -1.0;

    kernal[6] = 0.0;
    kernal[7] = -1.0;
    kernal[8] = 0.0;

    for(int i = 0; i < ARRAY_SIZE; i++) {
        kernal[i] /= 8.0;
    }

    float s = 1.0 / sampleDistance;

    vec2 offsets[ARRAY_SIZE];

    offsets[0] = vec2(-s, -s);
    offsets[1] = vec2(0, -s);
    offsets[2] = vec2(s, -s);

    offsets[3] = vec2(-s, 0);
    offsets[4] = vec2(0, 0);
    offsets[5] = vec2(s, 0);

    offsets[6] = vec2(-s, s);
    offsets[7] = vec2(0, s);
    offsets[8] = vec2(s, s);

    vec3 result = vec3(0.0f);
    for(int i = 0; i < 9; i++) {
        result += texture(tex0, v_TexCoords + offsets[i]).rgb * kernal[i];
    }

    // Here is the modifiction. If the value is above
    // A certain value the result across all values will be 
    // 1.0 otherwise it will be 0.0
    result = (result.r > 0.0 || result.g > 0.0 || result.b > 0.0) ? vec3(1.0) : vec3(0.0);
    //result.g = (result.g > 0.0) ? 1.0 : 0.0;
    //result.b = (result.b > 0.0) ? 1.0 : 0.0;

    out_Color.rgb = result;
    out_Color.a = texture(tex0, v_TexCoords).a;
}