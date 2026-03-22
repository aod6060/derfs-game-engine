#version 400

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
//out vec4 out_Color;
// Depth is implicatly handle by this shader
layout(location = 0) out vec4 out_Position;
layout(location = 1) out vec4 out_Normal;
layout(location = 2) out vec4 out_Albedo;
layout(location = 3) out vec4 out_MREL;

void main() {
    //out_Color = texture(albedoMaterial, v_AlbedoTexCoords);
    out_Position = vec4(v_WorldPosition, 1.0);
    out_Normal = vec4(normalize(v_Normal), 0.0);
    out_Albedo = texture(albedoMaterial, v_AlbedoTexCoords);
    out_MREL = vec4(
        texture(metalMaterial, v_MetalTexCoords).r,
        texture(roughnessMaterial, v_RoughnessTexCoords).r,
        texture(emissiveMaterial, v_EmissiveTexCoords).r,
        texture(litMaterial, v_LitTexCoords).r
    );
}