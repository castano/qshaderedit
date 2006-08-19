[VertexShader]
varying vec3 v_rayOrigin;
varying vec3 v_rayDirection;

uniform vec2 viewportSize;

void main() {
	gl_Position = gl_Vertex;

	// Compute ray origin
	v_rayOrigin = gl_ModelViewMatrixInverse[3].xyz;

	// Compute ray direction
	vec3 dir;
	dir.xy = 2.0 * gl_MultiTexCoord0.xy - 1.0;
	dir.xy *= vec2(viewportSize.x / viewportSize.y, -1.0);	// flip y axis & adjust aspect ratio
	dir.z = -4.0; // focal length

	v_rayDirection = mat3(gl_ModelViewMatrixInverse) * dir;
}
[FragmentShader]
varying vec3 v_rayOrigin;
varying vec3 v_rayDirection;

uniform float time;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float shininess;

uniform vec3 backgroundColor;

uniform sampler2D checkSampler;

struct Ray {
	vec3 o;	// origin
	vec3 d;	// direction
};

struct Sphere {
	vec3 centre;
	float rad2;	// radius^2
	vec3 color;
	float Kd, Ks, Kr;
};

// Object database stored in constants
#define SQR(N) (N*N)
#define NOBJECTS 3
Sphere object[NOBJECTS] = Sphere[](
	Sphere(
		vec3(0.0, 0.0, 0.0),
		SQR(1.0),
		vec3(0.0, 0.5, 1.0),
		1.0, 1.0, 0.5,
	),
	Sphere(
		vec3(1.5, -0.5, 0.0),
		SQR(0.5),
		vec3(0.0, 1.0, 0.0),
		1.0, 1.0, 0.5,
	),	
	Sphere(
		vec3(0.0, -101.0, 0.0),
		SQR(100.0),
//		vec3(0.0, 0.0, 1.0),
		vec3(1.0, 1.0, 1.0),
		1.0, 1.0, 0.5,
	)
);
const float eps = 0.001;	// error epsilon

float SphereIntersect(Sphere s, Ray ray, out bool hit)
{
	vec3 v = s.centre - ray.o;
	float b = dot(v, ray.d);
	float disc = (b*b) - dot(v, v) + s.rad2;

	hit = true;
	// note - early returns not supported by HLSL compiler currently:
//	if (disc<=0) return -1.0; // ray misses
	if (disc <= 0.0) hit = false;;

	disc = sqrt(disc);
	float t2 = b + disc;

//	if (t2<=eps) return -1.0; // behind ray origin
	if (t2<=eps) hit = false; // behind ray origin

	float t1 = b - disc;

	if ((t1>eps) && (t1<t2))  // return nearest intersection
		return t1;
	else
		return t2;
}

vec3 SphereNormal(Sphere s, vec3 i)
{
	return normalize(i - s.centre);
}

// find nearest hit
// returns intersection point
vec3 NearestHit(Ray ray, out int hitobj, out bool anyhit)
{
	float mint = 1e10;
	hitobj = -1;
 	anyhit = false;
	for(int i=0; i<NOBJECTS; i++) {
		bool hit;
		float t = SphereIntersect(object[i], ray, hit);
		if (hit) {
			if (t < mint) {
				hitobj = i;
				mint = t;
				anyhit = true;
			}
		}
	}
	return ray.o + ray.d*mint;
}

// test for any hit (for shadow rays)
bool AnyHit(Ray ray)
{
	bool anyhit = false;
	for(int i=0; i<NOBJECTS; i++) {
		bool hit;
		float t = SphereIntersect(object[i], ray, hit);
		if (hit) {
//			return true;
			anyhit = true;
		}
	}
	return anyhit;
}

float saturate(float f)
{
	return max(0.0, min(1.0, f));
}

// Phong lighting model
vec3 Phong(vec3 n, vec3 l, vec3 v, float shininess, vec3 diffuseColor, vec3 specularColor)
{
	float ndotl = dot(n, l);
	float diff = saturate(ndotl);
	vec3 r = reflect(l, n);
	float spec = pow(saturate(dot(v, r)), shininess) * float(ndotl > 0.0);
	return diff * diffuseColor + spec * specularColor;
}

vec3 Shade(vec3 i, vec3 n, vec3 v, int hitobj)
{
	vec3 l = normalize(lightPosition - i);

	// check if shadowed
	Ray shadowray;
	shadowray.o = i;
	shadowray.d = l;
	bool shadowed = AnyHit(shadowray);

	// lighting
	vec3 diff = object[hitobj].color * object[hitobj].Kd;
	if (hitobj==2) {
		diff *= texture2D(checkSampler, i.xz);	// floor texture
	}
	vec3 spec = lightColor * object[hitobj].Ks;
	float shadowFactor = 0.25 + 0.75 * float(!shadowed);
	
	return Phong(n, l, v, shininess, diff, spec) * shadowFactor;
}


void animate_scene(float time)
{
	object[1].centre.x = sin(time)*1.5;
	object[1].centre.z = cos(time)*1.5;
}


void main() {
	animate_scene(time);
	
	Ray eyeray;
	eyeray.o = v_rayOrigin;
	eyeray.d = normalize(v_rayDirection);

	// find nearest hit
	int hitobj;
	bool hit;
	vec3 i = NearestHit(eyeray, hitobj, hit);
	vec3 c = vec3(0.0, 0.0, 0.0);
	if (hit) {
		// shade surface
		vec3 n = SphereNormal(object[hitobj], i);
		c = Shade(i, n, eyeray.d, hitobj);
	
		// shoot reflection ray
		vec3 r = reflect(eyeray.d, n);
		Ray reflray;
		reflray.o = i;
		reflray.d = r;
		int hitobj2;
		i = NearestHit(reflray, hitobj2, hit);
		if (hit) {
			n = SphereNormal(object[hitobj2], i);
			c += Shade(i, n, reflray.d, hitobj2) * object[hitobj].Kr;
		} else {
			c += backgroundColor;
		}
	} 
	else {
		c = backgroundColor;
	}

	gl_FragColor = vec4(c, 1.0);
}
