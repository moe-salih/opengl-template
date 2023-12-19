#version 460 core

layout (location = 0) out vec4 fragColor;
in vec2 fragCoord;

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

vec3 palette(float t) {

	vec3 a = vec3(0.5, 0.5, 0.5);
	vec3 b = vec3(0.5, 0.5, 0.5);
	vec3 c = vec3(1.0, 1.0, 1.0);
	vec3 d = vec3(0.263, 0.416, 0.557);

	return a + b*cos(6.28318*(c*t+d));
}

void main() {
	vec2 uv = fragCoord.xy;
	uv.x *= u_resolution.x /u_resolution.y;
	vec2 uv0 = uv;

	vec3 finalColor = vec3(0.0);

	for(float i=0.0; i<6.0; i++) {

		uv = fract(uv * 1.5) - 0.5;
		float d = length(uv) * exp(-length(uv0));
		vec3 col = palette(length(uv0) + i*.4 + u_time*.4);

		d = sin(d*8. + u_time)/8.;
		d = abs(d);
		d = pow(.009/d, 1.5);

		finalColor += col * d;
	}
	fragColor = vec4(finalColor, 1.0);
}