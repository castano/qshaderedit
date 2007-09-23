[VertexShader]
//
// Vertex shader for testing the discard command
//
// Author: OGLSL implementation by Ian Nurse
//
// Copyright (C) 2002-2006  LightWork Design Ltd.
//          www.lightworkdesign.com
//
// See LightworkDesign-License.txt for license information
//

uniform vec3  LightPosition;
uniform vec3  LightColor;
uniform vec3  EyePosition;
uniform vec3  Specular;
uniform vec3  Ambient;
uniform float Kd;

varying vec3  DiffuseColor;
varying vec3  SpecularColor;

void main()
{
    vec3 ecPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 viewVec    = normalize(EyePosition - ecPosition);
    vec3 Hvec       = normalize(viewVec + lightVec);

    float spec = abs(dot(Hvec, tnorm));
    spec = pow(spec, 16.0);

    DiffuseColor    = LightColor * vec3 (Kd * abs(dot(lightVec, tnorm)));
    DiffuseColor    = clamp(Ambient + DiffuseColor, 0.0, 1.0);
    SpecularColor   = clamp((LightColor * Specular * spec), 0.0, 1.0);

    gl_TexCoord[0]  = gl_MultiTexCoord0;
    gl_Position     = ftransform();
}
[FragmentShader]
//
// Fragment shader for testing the discard command
//
// Author: Randi Rost
//
// Copyright (c) 2002-2006 3Dlabs Inc. Ltd. 
//
// See 3Dlabs-License.txt for license information
//

varying vec3  DiffuseColor;
varying vec3  SpecularColor;

uniform vec2  Scale;
uniform vec2  Threshold;
uniform vec3  SurfaceColor;

void main()
{
    float ss = fract(gl_TexCoord[0].s * Scale.s);
    float tt = fract(gl_TexCoord[0].t * Scale.t);

    if ((ss > Threshold.s) && (tt > Threshold.t)) discard;

    vec3 finalColor = SurfaceColor * DiffuseColor + SpecularColor;
    gl_FragColor = vec4(finalColor, 1.0);
}
[Parameters]
vec3 Ambient = vec3(0.1, 0.1, 0.1);
vec3 EyePosition = vec3(0, 0, 0);
float Kd = 1;
vec3 LightColor = vec3(1, 1, 1);
vec3 LightPosition = vec3(0, 10, 4);
vec2 Scale = vec2(5, 5);
vec3 Specular = vec3(0, 1, 0);
vec3 SurfaceColor = vec3(1, 1, 0);
vec2 Threshold = vec2(0.2, 0.2);
