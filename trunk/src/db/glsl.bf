radians(degrees) //Converts degrees to radians
degrees(radians) //Converts radians to degrees
sin(angle) //The standard trigonometric sine function.
cos(angle)
tan(angle)
asin(x)
acos(x)
atan(y, x)
atan( y_over_x)
pow(x, y) //Returns x raised to the y power
exp(x)
log(x)
exp2(x)
log2(x)
sqrt(x)
inversesqrt(x) //1/sqrt(x)
abs(x) //Returns x if x >= 0, otherwise it returns –x.
sign(x) //Returns 1.0 if x > 0, 0.0 if x = 0, or –1.0 if x < 0
floor(x) //Returns a value equal to the nearest integer that is less than or equal to x
ceil(x) //Returns a value equal to the nearest integer that is greater than or equal to x
fract(x) //Returns x – floor (x)
mod(x,y) //Modulus. Returns x – y ∗ floor (x/y)
min(x,y) //Returns y if y < x, otherwise it returns x
max(x,y) //Returns y if x < y, otherwise it returns x
clamp(x,minVal,maxVal) //Returns min (max (x, minVal), maxVal)
mix(x,y,a) //Returns the linear blend of x and y
step(edge, x) //Returns 0.0 if x < edge, otherwise it returns 1.0
smoothstep(edge0, edge1,x) //Returns 0.0 if x <= edge0 and 1.0 if x >= edge1 and performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1
length(x) //Returns the length of vector x
distance(p0, p1) //Returns the distance between p0 and p1
dot(x, y) //Returns the dot product of x and y, i.e. x[0]y[0]+x[1]y[1]+...
cross(x, y) //Returns the cross product of x and y
normalize(x) //Returns a vector in the same direction as x but with a length of 1.
ftransform() //For vertex shaders only
faceforward(N, I, Nref) //If dot(Nref, I) < 0 return N, otherwise return –N.
reflect(I,N)
refract(I,N,eta)
matrixCompMult(x, y)
outerProduct(c, r)
transpose(m)
lessThan(x,y) //Returns the component-wise compare of x < y.
lessThanEqual(x,y) //Returns the component-wise compare of x <= y.
greaterThan(x, y) //Returns the component-wise compare of x > y.
greaterThanEqual(x,y) //Returns the component-wise compare of x >= y.
equal(x, y) //Returns the component-wise compare of x == y.
notEqual(x,y) //Returns the component-wise compare of x != y.
any(x) //Returns true if any component of x is true.
all(x) //Returns true only if all components of x are true.
not(x) //Returns the component-wise logical complement of x.
texture1D(sampler1D sampler, float coord [, float bias])
texture1DProj (sampler1D sampler, vec2 coord [, float bias] )
texture1DLod (sampler1D sampler, float coord, float lod)
texture1DProjLod (sampler1D sampler, vec2 coord, float lod)
texture2D (sampler2D sampler, vec2 coord [, float bias] )
texture2DProj (sampler2D sampler, vec3 coord [, float bias] )
texture2DProj (sampler2D sampler, vec4 coord [, float bias] )
texture2DLod (sampler2D sampler, vec2 coord, float lod)
texture2DProjLod (sampler2D sampler, vec3 coord, float lod)
texture2DProjLod (sampler2D sampler, vec4 coord, float lod)
texture3D (sampler3D sampler, vec3 coord [, float bias] )
texture3DProj (sampler3D sampler, vec4 coord [, float bias] )
texture3DLod (sampler3D sampler, vec3 coord, float lod)
texture3DProjLod (sampler3D sampler, vec4 coord, float lod)
textureCube (samplerCube sampler, vec3 coord [, float bias] )
textureCubeLod (samplerCube sampler, vec3 coord, float lod)
shadow1D (sampler1DShadow sampler, vec3 coord [, float bias] )
shadow2D (sampler2DShadow sampler, vec3 coord [, float bias] )
shadow1DProj (sampler1DShadow sampler, vec4 coord [, float bias] )
shadow2DProj (sampler2DShadow sampler, vec4 coord [, float bias] )
shadow1DLod (sampler1DShadow sampler, vec3 coord, float lod)
shadow2DLod (sampler2DShadow sampler, vec3 coord, float lod)
shadow1DProjLod(sampler1DShadow sampler, vec4 coord, float lod)
shadow2DProjLod(sampler2DShadow sampler, vec4 coord, float lod)
dFdx(p) //Returns the derivative in x using local differencing for the input argument p.
dFdy(p) //Returns the derivative in y using local differencing for the input argument p.
fwidth(p) // Returns the sum of the absolute derivative in x and y
noise1(x) //Returns a 1D noise value based on the input value x.
noise2(x) //Returns a 2D noise value based on the input value x.
noise3(x) //Returns a 3D noise value based on the input value x.
noise4(x) //Returns a 4D noise value based on the input value x.
