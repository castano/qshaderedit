[VertexShader]
// Copyright 2007 Sancho Pereira
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

varying vec3 v_V;
varying vec3 v_N;

void main() {
	gl_Position = ftransform();
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
// Copyright 2007 Sancho Pereira
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#version 110
varying vec3 v_V;
varying vec3 v_N;

const float roughness = 0.05;

void main() {
	vec3 N = normalize(v_N);
	vec3 V = normalize(v_V);
	vec3 R = reflect(V, N);
	vec3 L = normalize(vec3(gl_LightSource[0].position));
	vec3 Nf = faceforward(N, V, N);
	vec3 Vf = -V;
	vec3 H = normalize(L+Vf);
	float ndoth = dot(Nf, H);
	float ndotl = dot(Nf, L);
	float ndotv = dot(Nf, Vf);
	float delta = acos(ndoth);
	float tandelta = tan(delta);
	float secta = exp( -( pow( tandelta, 2.0) / pow( roughness, 2.0)));
	float sectb = 1.0 / sqrt( ndotl * ndotv );
	float sectc = 1.0 / ( 4.0 * pow( roughness, 2.0) );

	vec4 ambient = gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * max(dot(L, N), 0.0);
	vec4 specular = gl_FrontMaterial.specular * (sectc * secta * sectb);

	gl_FragColor = ambient + diffuse + specular;
}
