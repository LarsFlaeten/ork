layout(location = 1) in vec3 normal;
layout(location = 3) in vec4 color;

out vec3 wpos;
out vec3 wnormal;
out vec4 fcolor;

void projection(vec3 normal, out vec3 worldPos, out vec3 worldNormal);

void main() {
    projection(normal, wpos, wnormal);
    fcolor = color;
}
