[VertexShader]
varying vec3 v_V;
varying vec3 v_N;

uniform float time;

uniform vec2 freq;
uniform vec2 scale;

void main () {
	float wiggleX = sin(gl_Vertex.x * freq.x + time) * scale.x;
	float wiggleY = cos(gl_Vertex.y * freq.y + time) * scale.y;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.x + wiggleY, gl_Vertex.y + wiggleX, gl_Vertex.z, gl_Vertex.w);
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
varying vec3 v_V;
varying vec3 v_N;

void main () {
	vec3 N = normalize(v_N);
	vec3 V = normalize(v_V);
	vec3 R = reflect(V, N);
	vec3 L = normalize(vec3(gl_LightSource[0].position));

	vec3 ambient = vec3(0.1, 0.0, 0.0);
	vec3 diffuse = vec3(1.0, 0.0, 0.0) * max(dot(L, N), 0.0);
	vec3 specular = vec3(1.0, 1.0, 1.0) * pow(max(dot(R, L), 0.0), 8.0);

	gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
[Parameters]
vec2 freq = vec2(2, 2.5);
vec2 scale = vec2(0.5, 0.2);
