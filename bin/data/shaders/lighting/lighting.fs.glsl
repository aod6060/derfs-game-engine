#version 400



layout(std140) uniform SunLight {
    vec3 direction;
    float unused1;
    vec3 albedo;
    float unused2;
    float ambient;
    float diffuse;
    float specular;
    int isOn;
} sunLight;

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
float sunLightAmbient() {
    return sunLight.ambient;
}

float sunLightDiffuse(float ndotl) {
    //return ndotl;
    float ret = sunLight.diffuse;
    if(ndotl <= 0.1) {
        ret = ret * 0.1;
    } else if(ndotl > 0.1 && ndotl < 0.8 ) {
        ret = ret * 0.5;
    }
    return ret;
}

float sunLightSpecular(float ndoth, float power) {
    return pow(ndoth, power) > sunLight.ambient ? sunLight.specular : 0.0;
}

float emissive(float e) {
    return e > 0.1 ? 1.0 : 0.0;
}

float computeRoughness(float roughness) {
    return (1.0 - roughness) * 256.0;
}

vec3 getSunLight(
    vec3 p,
    vec3 n,
    vec3 v,
    vec3 material,
    float metal,
    float roughness
) {
    if(sunLight.isOn == 1) {
        vec3 l = normalize(sunLight.direction);
        vec3 h = normalize(l + v);

        float ndotl = max(dot(n, l), 0.0);
        float ndoth = max(dot(n, h), 0.0);

        float A = sunLightAmbient();
        float D = sunLightDiffuse(ndotl);
        float S = sunLightSpecular(ndoth, computeRoughness(roughness));

        vec3 lightColor = sunLight.albedo;

        vec3 Ac = A * lightColor * material;
        vec3 Dc = mix((D * lightColor * material) * (1.0 - S), Ac * (1.0 - S), metal);
        vec3 Sc = (S * mix(lightColor, lightColor * material, metal));

        return Ac + Dc + Sc;
    } else {
        return vec3(0.0);
    }
}

void main() {
    vec3 p = texture(positionBuffer, v_TexCoords).xyz;
    vec3 n = texture(normalBuffer, v_TexCoords).xyz;
    vec3 v = normalize(cameraPosition - p);

    vec3 material = texture(albedoBuffer, v_TexCoords).rgb;
    float metal = texture(mrelBuffer, v_TexCoords).r;
    float roughness = texture(mrelBuffer, v_TexCoords).g;
    float emi = texture(mrelBuffer, v_TexCoords).b;
    float lit = texture(mrelBuffer, v_TexCoords).a;
    
    // SunLight
    vec3 sun = getSunLight(
        p,
        n,
        v,
        material,
        metal,
        roughness
    );

    float E = emissive(emi);
    vec3 Ec = E * material;

    vec3 finalColor = mix(Ec, sun + Ec, lit);
    //vec3 finalColor = Ec;

    out_Color = vec4(finalColor, 1.0);
}