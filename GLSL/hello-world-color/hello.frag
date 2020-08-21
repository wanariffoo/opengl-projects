
#ifdef GL_ES
precision mediump float;
#endif

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;


// Function that plots a line on the y-axis using a value between 0.0-1.0
float plot(vec2 st) {    
    return smoothstep(0.02, 0.0, abs(st.y - st.x));
}


void main() {

	vec2 st = gl_FragCoord.xy / u_resolution;

	// //// Strobing
	// 	vec4 color = vec4( abs(sin(u_time)), 0.0, 0.0, 1.0);
	// 	gl_FragColor = color;




	//// Linear interpolation
		// Linear function, similar to y = x
		float y = st.x;

		vec3 color = vec3(y);

		// Plotting a line
		float pct = plot(st);
		color = (1.0-pct)*color+pct*vec3(0.0,1.0,0.0);

		gl_FragColor = vec4(color,1.0);


}