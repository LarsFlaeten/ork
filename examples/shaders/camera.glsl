uniform camera {
    vec3 worldCameraPos;
};

uniform mat4 localToScreen;
uniform mat4 localToWorld;

#ifdef _VERTEX_

layout(location = 0) in vec3 POS;

void projection() {
    gl_Position = localToScreen * vec4(POS, 1.0);
}

void projection(out vec3 worldVertex) {
    gl_Position = localToScreen * vec4(POS, 1.0);
    worldVertex = (localToWorld * vec4(POS, 1.0)).xyz;
}

void projection(vec3 normal, out vec3 worldVertex, out vec3 worldNormal) {
    gl_Position = localToScreen * vec4(POS, 1.0);
    worldVertex = (localToWorld * vec4(POS, 1.0)).xyz;
    worldNormal = (localToWorld * vec4(normal, 0.0)).xyz;
}

#endif

#ifdef _FRAGMENT_

vec3 viewDir(vec3 worldP) {
    return normalize(worldP - worldCameraPos);
}

#endif
