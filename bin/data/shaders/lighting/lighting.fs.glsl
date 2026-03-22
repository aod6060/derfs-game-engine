#version 400

#define DEPTH 0
#define POSITION 1
#define NORMAL 2
#define ALBEDO 3
#define METAL 4
#define ROUGHNESS 5
#define EMISSIVE 6
#define LIT 7

uniform int test;

// DepthBuffer
uniform sampler2D depthBuffer; // 0
// PositionBuffer
uniform sampler2D positionBuffer; // 1
// NormalBuffer
uniform sampler2D normalBuffer; // 2
// AlbedoBuffer
uniform sampler2D albedoBuffer; // 3
// MRELBuffer
uniform sampler2D mrelBuffer; // 4

in vec2 v_TexCoords;

out vec4 out_Color;

void main() {
    // Testing to see if this data is being passed to the lighting shader
    //out_Color = vec4(v_TexCoords, 0.0, 1.0);
    switch(test) {
        case DEPTH:
            out_Color = vec4(vec3(texture(depthBuffer, v_TexCoords).r), 1.0);
            break;
        case POSITION:
            out_Color = vec4(texture(positionBuffer, v_TexCoords).rgb, 1.0);
            break;
        case NORMAL:
            out_Color = vec4(texture(normalBuffer, v_TexCoords).rgb, 1.0);
            break;
        case ALBEDO:
            out_Color = vec4(texture(albedoBuffer, v_TexCoords).rgb, 1.0);
            break;
        case METAL:
            out_Color = vec4(vec3(texture(mrelBuffer, v_TexCoords).r), 1.0);
            break;
        case ROUGHNESS:
            out_Color = vec4(vec3(texture(mrelBuffer, v_TexCoords).g), 1.0);
            break;
        case EMISSIVE:
            out_Color = vec4(vec3(texture(mrelBuffer, v_TexCoords).b), 1.0);
            break;
        case LIT:
            out_Color = vec4(vec3(texture(mrelBuffer, v_TexCoords).a), 1.0);
            break;
        default:
            out_Color = vec4(v_TexCoords, 0.0, 1.0);
            break;
    }

}