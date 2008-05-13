[VertexShader]
//
// Vertex shader for drawing the Mandelbrot set
//
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Michael Rivero
//
// Copyright (c) 2002-2005: 3Dlabs, Inc.
//
// See 3Dlabs-License.txt for license information
//

uniform vec3 LightPosition;
uniform float SpecularContribution;
uniform float DiffuseContribution;
uniform float Shininess;

varying float LightIntensity;
varying vec3  Position;

void main()
{
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);
    float spec      = max(dot(reflectVec, viewVec), 0.0);
    spec            = pow(spec, Shininess);
    LightIntensity  = DiffuseContribution * 
                          max(dot(lightVec, tnorm), 0.0) +
                          SpecularContribution * spec;
    Position        = vec3(gl_MultiTexCoord0 - 0.5) * 5.0;
    gl_Position     = ftransform();

}
[FragmentShader]
//
// Fragment shader for drawing the Mandelbrot set
//
// Authors: Dave Baldwin, Steve Koren, Randi Rost
//          based on a shader by Michael Rivero
//
// Copyright (c) 2002-2005: 3Dlabs, Inc.
//
// See 3Dlabs-License.txt for license information
//

varying vec3  Position;
varying float LightIntensity;

uniform float MaxIterations;
uniform float Zoom;
uniform float Xcenter;
uniform float Ycenter;
uniform vec3  InnerColor;
uniform vec3  OuterColor1;
uniform vec3  OuterColor2;

void main()
{
    float   real  = Position.y * Zoom + Xcenter;
    float   imag  = Position.x * Zoom + Ycenter;
    float   Creal = real;   // Change this line...
    float   Cimag = imag;   // ...and this one to get a Julia set

    float r2 = 0.0;
    float iter;

    for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
    {
        float tempreal = real;

        real = (tempreal * tempreal) - (imag * imag) + Creal;
        imag = 2.0 * tempreal * imag + Cimag;
        r2   = (real * real) + (imag * imag);
    }

    // Base the color on the number of iterations

    vec3 color;

    if (r2 < 4.0)
        color = InnerColor;
    else
        color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));

    color *= LightIntensity;

    gl_FragColor = vec4(color, 1.0);
}
[Parameters]
float DiffuseContribution = 0.8;
vec3 InnerColor = vec3(0, 0, 0);
vec3 LightPosition = vec3(0, 0, 4);
float MaxIterations = 24;
vec3 OuterColor1 = vec3(1, 0, 0);
vec3 OuterColor2 = vec3(0, 1, 0);
float Shininess = 8;
float SpecularContribution = 0.4;
float Xcenter = -0.4;
float Ycenter = 0;
float Zoom = 0.5;
