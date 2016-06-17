#ifdef _VERTEX_

layout (location = 0) in vec3 vertex;
out vec3 dir;

uniform mat4 cameraToWorld;
uniform mat4 screenToCamera;

void main() {
    dir = (cameraToWorld * vec4((screenToCamera * vec4(vertex, 1.0)).xyz, 0.0)).xyz;
    gl_Position = vec4(vertex.xy, 0.9999999, 1.0);
}

#endif

#ifdef _FRAGMENT_

in vec3 dir;
layout (location = 0) out vec4 data;

#ifdef CUBEMAPARRAY
#extension GL_ARB_texture_cube_map_array : enable
uniform samplerCubeArray skymap;
uniform float map;
#else
uniform samplerCube skymap;
#endif

void main() {
#ifdef CUBEMAPARRAY
    data = texture(skymap, vec4(dir, map));
#else
    data = texture(skymap, dir);
#endif
}

#endif
