float3 ambient
<
	string UIName = "Ambient";
	string UIWidget = "color";
> = {0.2,0.2,0.2};

float3 diffuse
<
	string UIName = "Diffuse";
	string UIWidget = "color";
> = {1,1,1};

float3 specular
<
	string UIName = "Specular";
	string UIWidget = "color";
> = {0.75,0.75,0.75};

float shine
<
    string UIName = "Shine";
	string UIWidget = "slider";
	float UIMin = 8.0f;
	float UIStep = 8;
	float UIMax = 256.0f;
> = 32.0;

float tile
<
	string UIName = "Tile Factor";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIStep = 1.0;
	float UIMax = 32.0;
> = 1;

float depth
<
	string UIName = "Depth Factor";
	string UIWidget = "slider";
	float UIMin = 0.01f;
	float UIStep = 0.01f;
	float UIMax = 0.50f;
> = 0.1;

bool depth_bias
<
	string UIName = "Depth Bias";
	string UIWidget = "checkbox";
> = false;

bool border_clamp
<
	string UIName = "Border Clamp";
	string UIWidget = "checkbox";
> = false;

texture color_map_tex : DIFFUSE
<
    string ResourceName = "rockbump.jpg";
    string ResourceType = "2D";
>;

texture relief_map_tex : NORMAL
<
    string ResourceName = "rockbump.tga";
    string ResourceType = "2D";
>;

sampler2D color_map = sampler_state
{
	Texture = <color_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

sampler2D relief_map = sampler_state
{
	Texture = <relief_map_tex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

float3 lightpos : Position 
<
    string Object = "PointLight";
    string Space = "World";
    string UIWidget = "none";
> = {-10.0f, 10.0f, -10.0f};

float4x4 modelviewproj_matrix : WorldViewProjection <string UIWidget = "none";>;
float4x4 modelview_matrix : WorldView <string UIWidget = "none";>;
float4x4 view_matrix : View <string UIWidget = "none";>;

struct a2v 
{
    float4 pos       : POSITION;
    float3 normal    : NORMAL;
    float2 texcoord  : TEXCOORD0;
    float3 tangent   : TANGENT0;
    float3 binormal  : BINORMAL0;
};

struct v2f
{
	float4 hpos : POSITION;
	float3 eye : TEXCOORD0;
	float3 light : TEXCOORD1;
	float2 texcoord : TEXCOORD2;
};

v2f vertex_shader(a2v IN)
{
	v2f OUT;

	// vertex position in object space
	float4 pos=float4(IN.pos.x,IN.pos.y,IN.pos.z,1.0);

	// vertex position in clip space
	OUT.hpos=mul(pos,modelviewproj_matrix);

	// copy color and texture coordinates
	OUT.texcoord=IN.texcoord.xy*tile;

	// compute modelview rotation only part
	float3x3 modelviewrot;
	modelviewrot[0]=modelview_matrix[0].xyz;
	modelviewrot[1]=modelview_matrix[1].xyz;
	modelviewrot[2]=modelview_matrix[2].xyz;

	// tangent vectors in view space
	float3 tangent=mul(IN.tangent,modelviewrot);
	float3 binormal=mul(IN.binormal,modelviewrot);
	float3 normal=mul(IN.normal,modelviewrot);
	float3x3 tangentspace=float3x3(tangent,binormal,normal);

	// vertex position in view space (with model transformations)
	float3 vpos=mul(pos,modelview_matrix).xyz;

	// view in tangent space
	OUT.eye=mul(tangentspace,vpos);
	
	// light position in tangent space
	float4 light=float4(lightpos.x,lightpos.y,lightpos.z,1);
	light=mul(light,view_matrix);
	light.xyz=mul(tangentspace,light.xyz-vpos);
	OUT.light=light.xyz;

	return OUT;
}

// setup ray pos and dir based on view vector
// and apply depth bias and depth factor
void setup_ray(v2f IN,out float3 p,out float3 v)
{
	p = float3(IN.texcoord,0);
	v = normalize(IN.eye);
	
	v.z = abs(v.z);

	if (depth_bias)
	{
		float db = 1.0-v.z; db*=db; db=1.0-db*db;
		v.xy *= db;
	}
	
	v.xy *= depth;
}

// do normal mapping using given texture coordinate
// tangent space phong lighting with optional border clamp
// normal X and Y stored in red and green channels
float4 normal_mapping(v2f IN,float2 texcoord)
{
	// color map
	float4 color = tex2D(color_map,texcoord);
	
	// normal map
	float4 normal = tex2D(relief_map,texcoord);
	normal.xy = 2*normal.xy - 1;
	normal.y = -normal.y;
	normal.z = sqrt(1.0 - dot(normal.xy,normal.xy));

	// light and view in tangent space
	float3 l = normalize(IN.light);
	float3 v = normalize(IN.eye);

	// compute diffuse and specular terms
	float diff = saturate(dot(l,normal.xyz));
	float spec = saturate(dot(normalize(l-v),normal.xyz));

	// attenuation factor
	float att = 1.0 - max(0,l.z); 
	att = 1.0 - att*att;

	// border clamp
	float alpha=1;
	if (border_clamp)
	{
		if (texcoord.x<0) alpha=0;
		if (texcoord.y<0) alpha=0;
		if (texcoord.x>tile) alpha=0;
		if (texcoord.y>tile) alpha=0;
	}
	
	// compute final color
	float4 finalcolor;
	finalcolor.xyz = ambient*color.xyz +
		att*(color.xyz*diffuse*diff +
		specular*pow(spec,shine));
	finalcolor.w = alpha;
	return finalcolor;
}

// ray intersect depth map using cone space leaping
// depth value stored in alpha channel (black is at object surface)
// and sqrt of cone ratio stored in blue channel
void ray_intersect_cone(inout float3 p,inout float3 v)
{
	const int num_steps=15;
	
	float dist=length(v.xy);
	
	for( int i=0;i<num_steps;i++ )
	{
		float4 tex = tex2D(relief_map, p.xy);
		
		float height = max(0, tex.w - p.z);

		float cone_ratio = tex.z*tex.z;
		
		p += v * height * cone_ratio / 
			(cone_ratio + dist);
	}
}

// ray intersect depth map using linear and binary searches
// depth value stored in alpha channel (black at is object surface)
void ray_intersect_relief(inout float3 p,inout float3 v)
{
	const int num_steps_lin=10;
	const int num_steps_bin=5;
	
	v /= v.z*num_steps_lin;
	
	int i;
	for( i=0;i<num_steps_lin;i++ )
	{
		float4 tex = tex2D(relief_map, p.xy);
		if (p.z<tex.w)
			p+=v;
	}
	
	for( i=0;i<num_steps_bin;i++ )
	{
		v *= 0.5;
		float4 tex = tex2D(relief_map, p.xy);
		if (p.z<tex.w)
			p+=v;
		else
			p-=v;
	}
}

// ray intersect depth map using linear search then
// intersect view and line from last two search points
// depth value stored in alpha channel (black is at object surface)
void ray_intersect_pom(inout float3 p,inout float3 v)
{
	v/=v.z;
	
	float h0 = tex2D(relief_map, p.xy + v.xy * 1.000 ).w;
	float h1 = tex2D(relief_map, p.xy + v.xy * 0.875 ).w;
	float h2 = tex2D(relief_map, p.xy + v.xy * 0.750 ).w;
	float h3 = tex2D(relief_map, p.xy + v.xy * 0.625 ).w;
	float h4 = tex2D(relief_map, p.xy + v.xy * 0.500 ).w;
	float h5 = tex2D(relief_map, p.xy + v.xy * 0.375 ).w;
	float h6 = tex2D(relief_map, p.xy + v.xy * 0.250 ).w;
	float h7 = tex2D(relief_map, p.xy + v.xy * 0.125 ).w;
	
	float x,y,xh,yh;
	
	if      (h7 < 0.125) { x = 0.937; y = 0.938; xh = h7; yh = h7; }
	else if (h6 < 0.250) { x = 0.750; y = 0.875; xh = h6; yh = h7; }
	else if (h5 < 0.350) { x = 0.625; y = 0.750; xh = h5; yh = h6; }
	else if (h4 < 0.500) { x = 0.500; y = 0.625; xh = h4; yh = h5; }
	else if (h3 < 0.625) { x = 0.375; y = 0.500; xh = h3; yh = h4; }
	else if (h2 < 0.750) { x = 0.250; y = 0.375; xh = h2; yh = h3; }
	else if (h1 < 0.875) { x = 0.125; y = 0.250; xh = h1; yh = h2; }
	else                 { x = 0.000; y = 0.125; xh = h0; yh = h1; }
	
	float fParallaxEffect = (x*(y + yh) - y*(x + xh) - x + y)/(y + yh - x - xh);
	
	p.xyz += v*(1 - fParallaxEffect);
}

float4 pixel_shader_cone(v2f IN) : COLOR
{
	float3 p,v;
	
	setup_ray(IN,p,v);

	ray_intersect_cone(p,v);

	return normal_mapping(IN,p.xy);
}

float4 pixel_shader_relief(v2f IN) : COLOR
{
	float3 p,v;
	
	setup_ray(IN,p,v);

	ray_intersect_relief(p.xyz,v);
	
	return normal_mapping(IN,p.xy);
}

float4 pixel_shader_pom(v2f IN) : COLOR
{
	float3 p,v;
	
	setup_ray(IN,p.xyz,v);

	ray_intersect_pom(p,v);
	
	return normal_mapping(IN,p.xy);
}

float4 pixel_shader_normal(v2f IN) : COLOR
{
	return normal_mapping(IN,IN.texcoord);
}

technique cone_mapping
{
    pass p0 
    {
		CullMode = CCW;
   		AlphaTestEnable = True;
		AlphaFunc = GreaterEqual;
		AlphaRef = 16;
    	
		VertexShader = compile vs_1_1 vertex_shader();
		PixelShader  = compile ps_2_a pixel_shader_cone();
    }
}

technique relief_mapping
{
    pass p0 
    {
		CullMode = CCW;
   		AlphaTestEnable = True;
		AlphaFunc = GreaterEqual;
		AlphaRef = 16;
		
		VertexShader = compile vs_1_1 vertex_shader();
		PixelShader  = compile ps_2_a pixel_shader_relief();
    }
}

technique parallax_occlusion_mapping
{
    pass p0 
    {
		CullMode = CCW;
   		AlphaTestEnable = True;
		AlphaFunc = GreaterEqual;
		AlphaRef = 16;
		
		VertexShader = compile vs_1_1 vertex_shader();
		PixelShader  = compile ps_2_a pixel_shader_pom();
    }
}

technique normal_mapping
{
    pass p0 
    {
		CullMode = CCW;
    	
		VertexShader = compile vs_1_1 vertex_shader();
		PixelShader  = compile ps_2_0 pixel_shader_normal();
    }
}
