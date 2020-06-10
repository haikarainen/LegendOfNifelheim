#version 450 core

#if defined(VR)
#extension GL_EXT_multiview : enable
#endif

in vec3 vertexRoot;
in vec3 vertexOffset;
in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out float fragmentHeight;

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


void main()
{
    float vertexScale = texture(testMap, vec2(vertexRoot.x / 5.0, 1.0 - vertexRoot.z / 5.0)).r;
    vec4 position = vec4(vertexRoot + vertexOffset * vertexScale, 1.0);
    vec3 normal = normalize(vertexNormal);
    fragmentHeight = clamp(-position.y, 0.0, 10.0);

#if defined(VR)
    if(gl_ViewIndex == 0)
    {

        gl_Position = mvpMatrixLeft * position;
        fragmentPosition = vec3(mvMatrixLeft * position);
        fragmentNormal = vec3(normalMatrixLeft * normal);
    }
    else 
    {
        gl_Position = mvpMatrixRight * position;
        fragmentPosition = vec3(mvMatrixRight * position);
        fragmentNormal = vec3(normalMatrixRight * normal);
    }
#else 
    gl_Position = mvpMatrix * position;
    fragmentPosition = vec3(mvMatrix * position);
    fragmentNormal = vec3(normalMatrix * normal);
#endif
}
