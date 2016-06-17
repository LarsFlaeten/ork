#ifdef _VERTEX_

layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 wpos;
out vec3 wnormal;
out vec2 fuv;

void projection(vec3 normal, out vec3 worldPos, out vec3 worldNormal);

void main() {
    projection(normal, wpos, wnormal);
    fuv = uv;
}

#endif

#ifdef _FRAGMENT_

uniform sampler2D tex;

in vec3 wpos;
in vec3 wnormal;
in vec2 fuv;

layout(location = 0) out vec4 data;

vec3 viewDir(vec3 worldP);

float illuminance(vec3 worldP, vec3 worldN, out vec3 worldL);

void main() {
    vec3 wl;
    vec3 wn = normalize(wnormal);
    float light = illuminance(wpos, wn, wl);

    float angle = dot(viewDir(wpos), reflect(wl, wn));
    float specular = light * pow(max(angle, 0.0), 5.0);

    vec4 fcolor = texture(tex, fuv);

    data = fcolor * (light + 0.2) + vec4(specular);
}

#endif
