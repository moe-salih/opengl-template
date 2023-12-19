#version 460 core

layout (location = 0) out vec4 fragColor;
in vec2 fragCoord;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;


void main() {
    vec4 O=vec4(1);

    vec2 uv = fragCoord.xy;
	uv.x *= u_resolution.x /u_resolution.y;
    vec2 u = uv;
    
    u = abs(u+u-(O.xy=u_resolution.xy))/O.y; O *= 0.;

    for (float i=0, t = u_time * 0.5; i<46.; O += .002/ 
        abs(abs(u.x - sin(t + i * .17) * .6) + u.y
        - sin(t + i * .1) * .6) * (cos(i+++vec4(0, 1, 2, 0)) + 1.2)
    ) u *= mat2(cos(i*.001+vec4(0, 33, 11, 0)));

    fragColor = O;
}