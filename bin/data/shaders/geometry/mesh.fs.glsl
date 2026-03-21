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
out vec4 out_Color;

void main() {
    out_Color = texture(albedoMaterial, v_AlbedoTexCoords);
}