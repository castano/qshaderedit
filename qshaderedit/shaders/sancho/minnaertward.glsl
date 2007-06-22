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
	
#version 110
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

uniform float k; // minnaert roughness  1.5
uniform float roughness; // Ward isotropic specular roughness 0.2
uniform float ior; // Schlick's fresnel approximation index of refraction 1.5

// Minnaert limb darkening diffuse term
vec3 minnaert( vec3 L, vec3 Nf, float k) {
	float ndotl = max( 0.0, dot(L, Nf));
	return gl_LightSource[0].diffuse.rgb * pow( ndotl, k);
}

// Ward isotropic specular term
vec3 wardiso( vec3 Nf, vec3 Ln, vec3 Hn, float roughness, float ndotv ) {
	float ndoth = dot( Nf, Hn);
	float ndotl = dot( Nf, Ln);
	float tandelta = tan( acos(ndoth));
	return gl_LightSource[0].specular.rgb
		* exp( -( pow( tandelta, 2.0) / pow( roughness, 2.0)))
		* (1.0 / sqrt( ndotl * ndotv ))
		* (1.0 / (4.0 * pow( roughness, 2.0)));
	}
	
float schlick( vec3 Nf, vec3 Vf, float ior, float ndotv ) {
	float kr = (ior-1.0)/(ior+1.0);
	kr *= kr;
	return kr + (1.0-kr)*pow( 1.0 - ndotv, 5.0);
}
	
void main() {
	vec3 N = normalize(v_N);
	vec3 V = normalize(v_V);
	vec3 L = normalize(vec3(gl_LightSource[0].position));
	vec3 Vf = -V;
	float ndotv = dot(N, Vf);
	vec3 H = normalize(L+Vf);

	vec3 ambient = gl_FrontMaterial.ambient.rgb;
	vec3 diffuse = gl_FrontMaterial.diffuse.rgb * minnaert( L, N, k);
	float fresnel = schlick( N, V, ior, ndotv);
	vec3 specular = gl_FrontMaterial.specular.rgb * wardiso( N, L, H, roughness, ndotv) * fresnel;

	gl_FragColor = vec4( ambient + diffuse + specular, 1.0);
}
[Parameters]
float ior = 1.9;
float k = 1.5;
float roughness = 0.15;
