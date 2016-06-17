#ifdef _VERTEX_

layout(location = 1) in vec3 normal;
layout(location = 3) in vec4 color;

out vec3 wpos;
out vec3 wnormal;

void projection(vec3 normal, out vec3 worldPos, out vec3 worldNormal);

void main() {
    projection(normal, wpos, wnormal);
}

#endif

#ifdef _FRAGMENT_

uniform vec4 color;

in vec3 wpos;
in vec3 wnormal;

layout(location = 0) out vec4 data;

vec3 viewDir(vec3 worldP);

float illuminance(vec3 worldP, vec3 worldN, out vec3 worldL);

void main() {
    vec3 wl;
    vec3 wn = normalize(wnormal);
    float light = illuminance(wpos, wn, wl);
    data = color * (0.5 + 0.5 * dot(wn, wl));
}

#endif
