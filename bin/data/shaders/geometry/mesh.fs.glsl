#version 400

// These are going to be removed... Its for testing
#define REGULAR 0
#define POSITION 1
#define NORMALS 2
#define ALBEDO_MATERIAL 3
#define METAL_MATERIAL 4
#define ROUGHNESS_MATERIAL 5
#define EMISSIVE_MATERIAL 6
#define LIT_MATERIAL 7
#define ALBEDO_TC 8
#define METAL_TC 9
#define ROUGHNESS_TC 10
#define EMISSIVE_TC 11
#define LIT_TC 12

uniform int test;

/*
uniform sampler2D tex0;
uniform float uvScale;
in vec2 v_TexCoords;
out vec4 out_Color;
*/
// Uniforms
uniform sampler2D albedoMaterial;
uniform sampler2D metalMaterial;
uniform sampler2D roughnessMaterial;
uniform sampler2D emissiveMaterial;
uniform sampler2D litMaterial;

// Varying Variables
in vec3 v_WorldPosition;
in vec3 v_Normal;
in vec2 v_AlbedoTexCoords;
in vec2 v_MetalTexCoords;
in vec2 v_RoughnessTexCoords;
in vec2 v_EmissiveTexCoords;
in vec2 v_LitTexCoords;

// Output Variable
out vec4 out_Color;

void main() {
    //out_Color = texture(tex0, v_TexCoords * uvScale);
    switch(test) {
        case REGULAR:
            out_Color = texture(albedoMaterial, v_AlbedoTexCoords);
            break;
        case POSITION:
            out_Color = vec4(v_WorldPosition, 1.0);
            break;
        case NORMALS:
            out_Color = vec4(v_Normal, 1.0);
            break;
        case ALBEDO_MATERIAL:
            out_Color = texture(albedoMaterial, v_AlbedoTexCoords);
            break;
        case METAL_MATERIAL:
            out_Color = texture(metalMaterial, v_MetalTexCoords);
            break;
        case ROUGHNESS_MATERIAL:
            out_Color = texture(roughnessMaterial, v_RoughnessTexCoords);
            break;
        case EMISSIVE_MATERIAL:
            out_Color = texture(emissiveMaterial, v_EmissiveTexCoords);
            break;
        case LIT_MATERIAL:
            out_Color = texture(litMaterial, v_LitTexCoords);
            break;
        case ALBEDO_TC:
            out_Color = vec4(v_AlbedoTexCoords, 0.0, 1.0);
            break;
        case METAL_TC:
            out_Color = vec4(v_MetalTexCoords, 0.0, 1.0);
            break;
        case ROUGHNESS_TC:
            out_Color = vec4(v_RoughnessTexCoords, 0.0, 1.0);
            break;
        case EMISSIVE_TC:
            out_Color = vec4(v_EmissiveTexCoords, 0.0, 1.0);
            break;
        case LIT_TC:
            out_Color = vec4(v_LitTexCoords, 0.0, 1.0);
            break;
    }
}