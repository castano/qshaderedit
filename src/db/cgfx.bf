abs( x ) //Absolute value of x .
acos( x ) //Arccosine of x in range [0, p], x in [–1, 1].
all( x ) //Returns true if every component of x is not equal to 0.
any( x ) //Returns true if any component of x is not equal to 0.
asin( x ) //Arcsine of x in range [–p/2, p/2]; x should be in [–1, 1].
atan( x ) //Arctangent of x in range [–p/2, p/2].
atan2( y , x ) //Arctangent of y / x in range [–p, p].
ceil( x ) //Smallest integer not less than x .
clamp( x , a , b ) //x clamped to the range [ a , b ] as follows:Returns a if x is less than a .Returns b if x is greater than b .Returns x otherwise.
cos( x ) //Cosine of x .
cosh( x ) //Hyperbolic cosine of x .
cross( A , B ) //Cross product of vectors A and B ; A and B must be three-component vectors.
degrees( x ) //Radian-to-degree conversion.
determinant( M ) //Determinant of matrix M .
dot( A , B ) //Dot product of vectors A and B .
exp( x ) //Exponential function e x .
exp2( x ) //Exponential function 2 x .
floor( x ) //Largest integer not greater than x .
fmod( x , y ) //Remainder of x / y , with the same sign as x .
frac( x ) //Fractional part of x .
frexp( x , out exp ) //Splits x into a normalized fraction in the interval [½, 1), which is returned, and a power of 2, which is stored in exp .
isfinite( x ) //Returns true if x is finite.
isinf( x ) //Returns true if x is infinite.
isnan( x ) //Returns true if x is NaN (Not a Number).
ldexp( x , n ) //x x 2 n .
lerp( a , b , f ) //Linear interpolation:(1 – f )* a + b * f where a and b are matching vector or scalar types. f can be either a scalar or a vector of the same type as a and b .
lit( NdotL , NdotH , m ) //Computes lighting coefficients for ambient, diffuse, and specular light contributions.
log( x ) //Natural logarithm ln( x ) ; x must be greater than 0.
log2( x ) //Base 2 logarithm of x ; x must be greater than 0.
log10( x ) //Base 10 logarithm of x ; x must be greater than 0.
max( a , b ) //Maximum of a and b .
min( a , b ) //Minimum of a and b .
modf( x , out ip ) //Splits x into integral and fractional parts, each with the same sign as x .
mul( M , N ) //Matrix product of matrix M and matrix N .
mul( M , v ) //Product of matrix M and column vector v .
mul( v , M ) //Product of row vector v and matrix M , as shown below:
noise( x ) //Either a one-, two-, or three-dimensional noise function, depending on the type of its argument.
pow( x , y ) //
radians( x ) //Degree-to-radian conversion.
round( x ) //Closest integer to x .
rsqrt( x ) //Reciprocal square root of x ; x must be greater than 0.
saturate( x ) //Clamps x to the [0, 1] range.
sign( x ) //1 if x > 0; –1 if x < 0; 0 otherwise.
sin( x ) //Sine of x .
sincos(float x , out s , out c ) //s is set to the sine of x , and c is set to the cosine of x .
sinh( x ) //Hyperbolic sine of x .
smoothstep( min , max , x ) //For values of x between min and max , returns a smoothly varying value that ranges from 0 at x = min to 1 at x = max .
step( a , x ) //0 if x < a ;1 if x >= a .
sqrt( x ) //Square root of x ;
tan( x ) //Tangent of x .
tanh( x ) //Hyperbolic tangent of x .
transpose( M ) //Matrix transpose of matrix M .
distance( pt1 , pt2 ) //Euclidean distance between points pt1 and pt2 .
faceforward( N , I , Ng ) //N if dot( Ng , I ) < 0; - N otherwise.
length( v ) //Euclidean length of a vector.
normalize( v ) //Returns a vector of length 1 that points in the same direction as vector v .
reflect( I , N ) //Computes reflection vector from entering ray direction I and surface normal N .
refract( I , N , eta ) //Given entering ray direction I , surface normal N , and relative index of refraction eta , computes refraction vector.
tex1D(sampler1D tex , float s ) //1D nonprojective texture query
tex1D(sampler1D tex , float s , float dsdx , float dsdy ) //1D nonprojective texture query with derivatives
tex1D(sampler1D tex , float2 sz ) //1D nonprojective depth compare texture query
tex1D(sampler1D tex , float2 sz , float dsdx , float dsdy ) //1D nonprojective depth compare texture query with derivatives
tex1Dproj(sampler1D tex , float2 sq ) //1D projective texture query
tex1Dproj(sampler1D tex , float3 szq ) //1D projective depth compare texture query
tex2D(sampler2D tex , float2 s ) //2D nonprojective texture query
tex2D(sampler2D tex , float2 s , float2 dsdx , float2 dsdy ) //2D nonprojective texture query with derivatives
tex2D(sampler2D tex , float3 sz ) //2D nonprojective depth compare texture query
tex2D(sampler2D tex , float3 sz , float2 dsdx , float2 dsdy ) //2D nonprojective depth compare texture query with derivatives
tex2Dproj(sampler2D tex , float3 sq ) //2D projective texture query
tex2Dproj(sampler2D tex , float4 szq ) //2D projective depth compare texture query
texRECT(samplerRECT tex , float2 s ) //2D nonprojective texture rectangle texture query (OpenGL only)
texRECT(samplerRECT tex , float2 s , float2 dsdx , float2 dsdy ) //2D nonprojective texture rectangle texture query with derivatives (OpenGL only)
texRECT(samplerRECT tex , float3 sz ) //2D nonprojective texture rectangle depth compare texture query (OpenGL only)
texRECT(samplerRECT tex , float3 sz , float2 dsdx , float2 dsdy ) //2D nonprojective depth compare texture query with derivatives (OpenGL only)
texRECTproj(samplerRECT tex , float3 sq ) //2D texture rectangle projective texture query (OpenGL only)
texRECTproj(samplerRECT tex , float3 szq ) //2D texture rectangle projective depth compare texture query (OpenGL only)
tex3D(sampler3D tex , float3 s ) //3D nonprojective texture query
tex3D(sampler3D tex , float3 s , float3 dsdx , float3 dsdy ) //3D nonprojective texture query with derivatives
tex3Dproj(sampler3D tex , float4 sq ) //3D projective texture query
texCUBE(samplerCUBE tex , float3 s ) //Cube map nonprojective texture query
texCUBE(samplerCUBE tex , float3 s , float3 dsdx , float3 dsdy ) //Cube map nonprojective texture query with derivatives
texCUBEproj(samplerCUBE tex , float4 sq ) //Cube map projective texture query (ignores q)
ddx( a ) //Approximate partial derivative of a with respect to screen-space x coordinate
ddy( a ) //Approximate partial derivative of a with respect to screen-space y coordinate
debug //
lerp //
offsettex2D //
offsettexRECT //
offsettex2DScaleBias //
offsettexRECTScaleBias //
tex1D_dp3 //
tex2D_dp3x2 //
texRECT_dp3x2 //
tex3D_dp3x3 //
texCUBE_dp3x3 //
texCUBE_reflect_dp3x3 //
texCUBE_reflect_eye_dp3x3 //
tex_dp3x2_depth //
pack_4byte //
unpack_4ubyte //
pack_4ubyte //
unpack_4byte //
pack_2ushort //
unpack_2ushort //
pack_2half //
unpack_2half //