#ifdef _VERTEX_

layout (location = 0) in vec4 vertex;
out vec2 uv;

void main() {
    gl_Position = vertex;
    uv = vertex.xy * 0.5 + vec2(0.5);
}

#endif

#ifdef _FRAGMENT_

uniform sampler2D colorSampler;
uniform sampler2D depthSampler;

in vec2 uv;
layout (location = 0) out vec4 data;

void main() {
    data = textureLod(colorSampler, uv, 0.0);
    data += textureLod(colorSampler, uv, 1.0) * 0.25;
    data += textureLod(colorSampler, uv, 2.0) * 0.25;
    data += textureLod(colorSampler, uv, 3.0) * 0.25;
    data += textureLod(colorSampler, uv, 4.0) * 0.25;
    data += textureLod(colorSampler, uv, 5.0) * 0.25;
    data += textureLod(colorSampler, uv, 6.0) * 0.25;
    data += textureLod(colorSampler, uv, 7.0) * 0.25;
    data += textureLod(colorSampler, uv, 8.0) * 0.25;
    gl_FragDepth = texture(depthSampler, uv).x;
}

#endif
