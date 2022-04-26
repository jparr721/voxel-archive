$input v_color0

#include "../common.sc"
#include "uniforms.sc"

void main()
{
    float thickness = 0.6;
    float opacity = 1.0;
    vec3 fw = abs(dFdx(v_color0)) + abs(dFdy(v_color0));
    vec3 val = smoothstep(vec3_splat(0.0), fw * thickness, v_color0);
    float edge = min(min(val.x, val.y), val.z); // Gets to 0.0 around the edges.

    vec4 rgba = vec4(v_color0.x, v_color0.y, v_color0.z, (1.0 - edge) * opacity);
    gl_FragColor = rgba;
}