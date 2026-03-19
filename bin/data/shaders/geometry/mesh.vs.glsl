#version 400


layout(location=0) in vec3 vertices;
layout(location=1) in vec3 normals;
layout(location=2) in vec2 albedoTexCoords;
layout(location=3) in vec2 metalTexCoords;
layout(location=4) in vec2 roughnessTexCoords;
layout(location=5) in vec2 emissiveTexCoords;
layout(location=6) in vec2 litTexCoords;
// 7, 8, 9, 10
layout(location=7) in mat4 model;

/*
uniform mat4 proj;
uniform mat4 view;
*/

layout(std140) uniform Camera {
    mat4 proj;
    mat4 view;
};

//uniform mat4 model;
//uniform mat4 normalMatrix;

// Varying Variables
out vec3 v_WorldPosition;
out vec3 v_Normal;
out vec2 v_AlbedoTexCoords;
out vec2 v_MetalTexCoords;
out vec2 v_RoughnessTexCoords;
out vec2 v_EmissiveTexCoords;
out vec2 v_LitTexCoords;

void main() {
    gl_Position = proj * view * model * vec4(vertices, 1.0);
    v_WorldPosition = (model * vec4(vertices, 1.0)).xyz;
    mat4 normalMatrix = inverse(transpose(model));
    v_Normal = normalize(mat3(normalMatrix) * normals);
    v_AlbedoTexCoords = albedoTexCoords;
    v_MetalTexCoords = metalTexCoords;
    v_RoughnessTexCoords = roughnessTexCoords;
    v_EmissiveTexCoords = emissiveTexCoords;
    v_LitTexCoords = litTexCoords;
}