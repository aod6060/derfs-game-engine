#version 400


#define MAX_LIGHTS 128

#define DIRECTION_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Light 
{
    int type;
    float ambient;
    float diffuse;
    float specular;
    // All  light type will use this
    // Direction ~ Basically a Direction
    // Point ~ The Position of the point light
    // Spot ~ The Position of the spot light
    vec3 position; 
    float unused1;
    // The color of the light
    vec3 albedo;
    float unused2;
    // Point Light
    vec3 unused3;
    float radius;
    // Spot Light Section
    vec3 spotDirection;
    float spotCutOff;
};

layout(std140) uniform LightSystem 
{
    Light lights[MAX_LIGHTS];
    int lightSize;
    ivec3 unused;
} lights;

uniform vec3 cameraPosition;
//uniform int spotSize;

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

float getAmbient(int index) 
{
    return lights.lights[index].ambient;
}

float getDiffuse(int index, float ndotl) {
    return (ndotl > 0.01) ? lights.lights[index].diffuse : lights.lights[index].ambient;
}

float getSpecular(int index, float ndoth, float power) 
{
    return pow(ndoth, power) > 0.01 ? lights.lights[index].specular : lights.lights[index].ambient;
}

float emissive(float e) 
{
    return e > 0.1 ? 1.0 : 0.0;
}

float computeRoughness(float roughness) 
{
    return (1.0 - roughness) * 256.0;
}

vec3 getLight(
    vec3 p,
    vec3 n,
    vec3 v,
    vec3 material,
    float metal,
    float roughness
)
{
    vec3 amount = vec3(0.0);

    for(int i = 0; i < lights.lightSize; i++) {
        if(lights.lights[i].type == DIRECTION_LIGHT) {
            vec3 l = normalize(lights.lights[i].position);
            vec3 h = normalize(l + v);

            float ndotl = max(dot(n, l), 0.0);
            float ndoth = max(dot(n, h), 0.0);

            float A = getAmbient(i);
            float D = getDiffuse(i, ndotl);
            float S = getSpecular(i, ndoth, computeRoughness(roughness));

            vec3 lightColor = lights.lights[i].albedo;

            vec3 Ac = A * lightColor * material;
            vec3 Dc = mix((D * lightColor * material) * (1.0 - S), Ac * (1.0 - S), metal);
            vec3 Sc = (S * mix(lightColor, lightColor * material * 2.0, metal));

            amount += (Ac + Dc + Sc);
        } else if(lights.lights[i].type == POINT_LIGHT) {
            vec3 l = normalize(lights.lights[i].position - p);
            vec3 h = normalize(l + v);

            // Calculate Attenuation
            float dist = length(lights.lights[i].position - p);
            float rad = lights.lights[i].radius;

            if(dist <= rad) {
                float ndotl = max(dot(n, l), 0.0);
                float ndoth = max(dot(n, h), 0.0);

                float A = getAmbient(i);
                float D = getDiffuse(i, ndotl);
                float S = getSpecular(i, ndoth, computeRoughness(roughness));

                vec3 lightColor = lights.lights[i].albedo;

                vec3 Ac = A * lightColor * material;
                vec3 Dc = mix((D * lightColor * material) * (1.0 - S), Ac * (1.0 - S), metal);
                vec3 Sc = (S * mix(lightColor, lightColor * material * 2.0, metal));

                amount += (Ac + Dc + Sc) * mix(1.0, 0.0, clamp(dist / rad, 0.0, 1.0));
            }
        } else if(lights.lights[i].type == SPOT_LIGHT) {
            vec3 l = normalize(lights.lights[i].position - p);
            vec3 h = normalize(l + v);
            vec3 d = normalize(lights.lights[i].spotDirection);
            
            float spotCutOff = cos(lights.lights[i].spotCutOff * (3.14 / 180.0));
            float spotCutOffOut = (spotCutOff * 1.5);
            float theta = dot(l, d);
            float epsilon = spotCutOff - spotCutOffOut;

            if(theta > spotCutOff) {
                float ndotl = max(dot(n, l), 0.0);
                float ndoth = max(dot(n, h), 0.0);

                float A = getAmbient(i);
                float D = getDiffuse(i, ndotl);
                float S = getSpecular(i, ndoth, computeRoughness(roughness));

                vec3 lightColor = lights.lights[i].albedo;

                vec3 Ac = A * lightColor * material;
                vec3 Dc = mix((D * lightColor * material) * (1.0 - S), Ac * (1.0 - S), metal);
                vec3 Sc = (S * mix(lightColor, lightColor * material * 2.0, metal));

                amount += (Ac + Dc + Sc) * clamp((theta - spotCutOffOut) / epsilon, 0.0, 1.0);
            }
        }
    }

    return amount;
}

void main() 
{
    vec3 p = texture(positionBuffer, v_TexCoords).xyz;
    vec3 n = texture(normalBuffer, v_TexCoords).xyz;
    vec3 v = normalize(cameraPosition - p);

    vec3 material = texture(albedoBuffer, v_TexCoords).rgb;
    float metal = texture(mrelBuffer, v_TexCoords).r;
    float roughness = texture(mrelBuffer, v_TexCoords).g;
    float emi = texture(mrelBuffer, v_TexCoords).b;
    float lit = texture(mrelBuffer, v_TexCoords).a;

    // Light
    vec3 lights = getLight(
        p,
        n,
        v,
        material,
        metal,
        roughness
    );

    float E = emissive(emi);
    vec3 Ec = E * material;

    vec3 finalColor = mix(Ec, lights + Ec, lit);
    //vec3 finalColor = Ec;

    out_Color = vec4(finalColor, 1.0);
}