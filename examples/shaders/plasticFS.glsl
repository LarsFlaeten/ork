in vec3 wpos;
in vec3 wnormal;
in vec4 fcolor;

layout(location = 0) out vec4 data;

vec3 viewDir(vec3 worldP);

float illuminance(vec3 worldP, vec3 worldN, out vec3 worldL);

void main() {
    vec3 wl;
    vec3 wn = normalize(wnormal);
    float light = illuminance(wpos, wn, wl);

    float angle = dot(viewDir(wpos), reflect(wl, wn));
    float specular = light * pow(max(angle, 0.0), 5.0);

    data = fcolor * (light + 0.2) + vec4(specular);
}
