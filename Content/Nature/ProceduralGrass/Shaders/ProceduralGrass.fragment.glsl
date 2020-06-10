#version 450 core

#if defined(VR)
#extension GL_EXT_multiview : enable
#endif

in vec3 fragmentPosition;
in vec3 fragmentNormal;
in float fragmentHeight;

out vec4 outColor;

layout(binding=0) uniform parameters
{
#if defined(VR)
    mat4 mvpMatrixLeft;
    mat4 mvMatrixLeft;
    mat3 normalMatrixLeft;
    mat4 invViewMatrixLeft;

    mat4 mvpMatrixRight;
    mat4 mvMatrixRight;
    mat3 normalMatrixRight;
    mat4 invViewMatrixRight;
#else 
    mat4 mvpMatrix;
    mat4 mvMatrix;
    mat3 normalMatrix;
    mat4 invViewMatrix;
#endif
};

layout(binding=1) uniform sampler2D irradiance;

layout(binding=2) uniform sampler2D testMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 equirectangularUv(vec3 direction)
{
    vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    mat4 e_invViewMatrix;
#if defined(VR)
    if(gl_ViewIndex == 0)
    {
        e_invViewMatrix = invViewMatrixLeft;
    }
    else 
    {
        e_invViewMatrix = invViewMatrixRight;
    }
#else 
    e_invViewMatrix = invViewMatrix;
#endif

    vec3 normal = normalize(fragmentNormal);

    vec3 viewDir = normalize(-fragmentPosition);

    vec4 rotatedNormal = normalize(e_invViewMatrix * vec4(normal.xyz, 0.0));
    vec4 rotatedViewDir = normalize(e_invViewMatrix * vec4(viewDir, 0.0));
    vec3 refVec = normalize(reflect(-rotatedViewDir.xyz, rotatedNormal.xyz));

    vec2 refUv = equirectangularUv(refVec);

    float albedoScale = 1.0;
    vec3 rootColor = vec3(0.454, 0.670, 0.050);
    vec3 tipColor = vec3(00.117, 0.964, 0.141);
    float albedoHeightDelta = clamp(fragmentHeight * albedoScale, 0.0, 1.0);
    vec3 albedo = mix(rootColor, tipColor, albedoHeightDelta);

    // Calculate diffuse term
    float diffuseStrength = 1.0;
    vec3 diffuse = texture(irradiance, refUv).xyz * diffuseStrength;
    float glow = 0.8;
    vec3 lightDiffuse = mix(albedo * diffuse, albedo, glow);

    float occlusionStrength = 1.0;
    float occlusionScale = 1.0;
    float occlusion = mix(clamp(fragmentHeight * occlusionScale, 0.0, 1.0), 1.0, 1.0 - occlusionStrength);

    outColor.rgb = lightDiffuse * occlusion;
    outColor.a = 1.0;


}
