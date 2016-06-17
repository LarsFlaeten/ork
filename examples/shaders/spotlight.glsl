uniform light {
    vec3 worldLightPos;
    vec3 worldLightDir;
    vec2 spotlightAngle;
};

float illuminance(vec3 worldP, vec3 worldN, out vec3 worldL) {
    worldL = normalize(worldLightPos - worldP);
    float falloff = 1.0 - smoothstep(spotlightAngle.x, spotlightAngle.y, acos(dot(worldLightDir, -worldL)));
    return max(dot(worldN, worldL), 0.0) * falloff;
}

#ifdef _VERTEX_
#endif

#ifdef _FRAGMENT_
#endif
