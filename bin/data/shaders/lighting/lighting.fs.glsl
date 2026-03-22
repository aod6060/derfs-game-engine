#version 400


uniform vec3 cameraPosition;

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


// Here is a test light

void main() {
    vec3 ambient = vec3(0.1);
    vec3 diffuse = vec3(1.0);
    vec3 specular = vec3(1.0);
    
    vec3 material = texture(albedoBuffer, v_TexCoords).rgb;

    vec3 p = texture(positionBuffer, v_TexCoords).xyz;

    vec3 l = normalize(vec3(1.0, 1.0, 0.0));
    vec3 n = texture(normalBuffer, v_TexCoords).xyz;
    vec3 v = normalize(cameraPosition - p);
    vec3 h = normalize(l + v);

    float ndotl = dot(n, l);
    float ndotv = dot(n, v);
    float ndoth = dot(n, h);

    float metal = texture(mrelBuffer, v_TexCoords).r;
    float roughness = texture(mrelBuffer, v_TexCoords).g;
    float emissive = texture(mrelBuffer, v_TexCoords).b;
    float lit = texture(mrelBuffer, v_TexCoords).a;

    float sv = pow(ndoth, (1.0 - roughness) * 256.0);

    vec3 a = ambient * material;
    vec3 d = mix(diffuse * material * ndotl, a, metal) * (1.0 - sv);
    vec3 s = specular * sv;
    s = mix(s, s * material, metal);

    vec3 e = material * emissive;

    vec3 color = mix(a + d + s + e, material, 1.0 - lit);
    
    //color = mix(s, material, 1.0 - lit) * 1.1;
    //vec3 color = ambient * material + diffuse * material * ndotl + specular * pow(ndoth, 128.0);

    out_Color = vec4(color, 1.0);
}