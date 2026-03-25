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
float ambient() {
    return 0.1;
}

float diffuse(float ndotl) {
    //return ndotl;
    float ret = 1.0;
    if(ndotl <= 0.1) {
        ret = 0.1;
    } else if(ndotl > 0.1 && ndotl < 0.8 ) {
        ret = 0.5;
    } else {
        ret = 1.0;
    }
    return ret;
}

float specular(float ndoth, float power) {
    return pow(ndoth, power) > 0.1 ? 1.0 : 0.0;
}

float emissive(float e) {
    return e > 0.1 ? 1.0 : 0.0;
}

void main() {
    vec3 p = texture(positionBuffer, v_TexCoords).xyz;
    vec3 l = normalize(vec3(1.0, 1.0, 0.0));
    vec3 n = texture(normalBuffer, v_TexCoords).xyz;
    vec3 v = normalize(cameraPosition - p);
    vec3 h = normalize(l + v);

    float ndotl = dot(n, l);
    float ndotv = dot(n, v);
    float ndoth = dot(n, h);

    vec3 material = texture(albedoBuffer, v_TexCoords).rgb;
    float metal = texture(mrelBuffer, v_TexCoords).r;
    float roughness = texture(mrelBuffer, v_TexCoords).g;
    float emi = texture(mrelBuffer, v_TexCoords).b;
    float lit = texture(mrelBuffer, v_TexCoords).a;

    vec3 lightColor = vec3(1.0f);

    float A = ambient();
    float D = diffuse(ndotl);
    float S = specular(ndoth, (1.0 - roughness) * 256.0);
    float E = emissive(emi);

    vec3 Ac = A * lightColor * material;
    vec3 Dc = mix((D * lightColor * material) * (1.0 - S), Ac * (1.0 - S), metal);
    vec3 Sc = (S * mix(lightColor, lightColor * material, metal));
    vec3 Ec = E * material;

    vec3 finalColor = mix(Ec, Ac + Dc + Sc + Ec, lit);
    vec3 specularColor = mix(Ec, Sc + Ec, lit);

    
    out_Color = vec4(finalColor, 1.0);
}