// This is the standard vertex shader for all 2D related stuff
// Because I don't need to move the screen around there will be
// No view matrix so it should simplify stuff.
#version 400


layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texCoords;

/*
uniform mat4 proj;
// We don't need a view matrix with these
// I do need a model matrix to scale them.
uniform mat4 model;
*/
// This should work across all 2D shaders
// Now the 2D render section will use a different 
layout(std140) uniform Standard2DTransform {
    mat4 proj;
    mat4 model;
};

out vec2 v_TexCoords;

void main() {
    gl_Position = proj * model * vec4(vertices, 1.0f);
    v_TexCoords = texCoords;
}