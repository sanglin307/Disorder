//shader common
cbuffer SceneProperty
{
	float3 AmbientLowColor;
	float3 AmbientUpperColor;
}

cbuffer CameraTransforms
{
	matrix CameraView;
	matrix CameraProjection;	
	matrix CameraViewProj;
    matrix CameraViewInv;
	matrix CameraProjInv;
	matrix CameraViewProjInv;
	float3 CameraPosition;
}

cbuffer ObjectTransform
{
	matrix WorldTransform;
	matrix WorldNormalTransform;  // translate for normal.
}

cbuffer MaterialProperty
{
	float3 DiffuseColor;
	float3 SpecularColor;
	float3 EmissiveColor;
	float  SpecularExp;	
}

cbuffer ShadowMapGen
{
	matrix ShadowMapView; 
	matrix ShadowMapViewArray[6];
	matrix ShadowMapProj;
}

SamplerComparisonState ShadowSampler;
Texture2D<float>  ShadowMapTexture2D;
TextureCube<float>       ShadowMapTextureCube;

SamplerState DiffuseSampler;
Texture2D DiffuseTexture;


cbuffer DirectionLightProperty
{
	float3 DirectionLightColor;
	float3 DirectionLightDir;	
	float  DirectionLightIntensity;
} 

 

cbuffer PointLightProperty
{
    float3 PointLightPos;
	float3 PointLightColor;
	float PointLightIntensity;
	float PointLightRangeRcp;
}

cbuffer SpotLightProperty
{
   float3 SpotLightPos;
   float3 SpotLightDir;
   float3 SpotLightColor;
   float SpotLightIntensity;
   float SpotLightRangeRcp;
   float SpotLightCosOuterCone;
   float SpotLightCosInnerConeRcp;
}
 
struct Material
{
   float3 Normal;
   float3 DiffuseColor;
   float3 SpecularColor;
   float  SpecularExp;
};

float3 CalculateAmbient(float3 normal, float3 color)
{
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;

	// Calculate the ambient value
	float3 ambient = AmbientLowColor + up * (AmbientUpperColor - AmbientLowColor);

	// Apply the ambient value to the color
	return ambient * color;
}

float3 CalculateDirectionLight(float3 position, Material material)
{
   //diffuse
   float3 lightColor = DirectionLightIntensity * DirectionLightColor.rgb;
	   float NDotL = saturate(dot(-DirectionLightDir, material.Normal));
   float3 finalColor = lightColor * NDotL * material.DiffuseColor;
   
   //specular
   float3 ToEye = normalize(CameraPosition - position);
   float3 reflectVector = reflect(DirectionLightDir,material.Normal);
   float NDotH = saturate(dot(reflectVector, ToEye));
   finalColor += lightColor * saturate(pow(NDotH, material.SpecularExp)) * material.SpecularColor;
   
   return finalColor;
}

float PCFShadowCubeMap(float3 position)
{
	matrix selectShadowMat;
	float3 lightDir = normalize(position - PointLightPos);
	float3 dirValue = abs(lightDir);
	if (dirValue.x > dirValue.y && dirValue.x > dirValue.z) // x is max component
	{
		lightDir.z = -lightDir.z;   // right hand coordinate to left hand....
		if (lightDir.x < 0)
			selectShadowMat = ShadowMapViewArray[1];
		else
			selectShadowMat = ShadowMapViewArray[0];
	}
	else if (dirValue.y > dirValue.x && dirValue.y > dirValue.z) // y is max component
	{
		lightDir.z = -lightDir.z;   // right hand coordinate to left hand....
		//lightDir.x = -lightDir.x;
		if (lightDir.y < 0)
			selectShadowMat = ShadowMapViewArray[3];
		else
			selectShadowMat = ShadowMapViewArray[2];
	}
	else
	{
		lightDir.x = -lightDir.x;
		if (lightDir.z < 0)
		{
			selectShadowMat = ShadowMapViewArray[5];
		}
		else
		{
			selectShadowMat = ShadowMapViewArray[4];
		}
	}

	float4 posShadowMap = mul(selectShadowMat, float4(position, 1.0));
		posShadowMap = mul(ShadowMapProj, posShadowMap);

	float3 UVD = posShadowMap.xyz / posShadowMap.w;

	return ShadowMapTextureCube.SampleCmp(ShadowSampler, lightDir, UVD.z).r;
	//float4 vshadow;
	////float offset = 0.01;
	//vshadow.x =  ShadowMapTextureCube.SampleCmp(ShadowSampler, lightDir,UVD.z).r;
	//vshadow.y = ShadowMapTextureCube.SampleCmp(ShadowSampler, lightDir + float3(offset,0,0), UVD.z).r;
	//vshadow.z = ShadowMapTextureCube.SampleCmp(ShadowSampler, lightDir + float3(0,offset, 0), UVD.z).r;
	//vshadow.w = ShadowMapTextureCube.SampleCmp(ShadowSampler, lightDir + float3(offset, offset,0), UVD.z).r;
	//
	//return lerp(lerp(vshadow.x, vshadow.y, 0.5),
	//	lerp(vshadow.z, vshadow.w, 0.5),
	//	0.5);

}

 float PCFShadow( float3 position )
{
	// Transform the world position to shadow projected space
	float4 posShadowMap = mul(ShadowMapView,float4(position,1.0));
    posShadowMap = mul(ShadowMapProj,posShadowMap);

	// Transform the position to shadow clip space
	float3 UVD = posShadowMap.xyz / posShadowMap.w;

	// Convert to shadow map UV values
	UVD.xy = 0.5 * UVD.xy + 0.5;
	UVD.y = 1.0 - UVD.y;

	//UVD.z -= 0.03f;
 
	//return ShadowMapTexture2D.SampleCmp(ShadowSampler, UVD.xy, UVD.z).r;

	float offset = 1.0f / SHADOWMAPSIZE;
	// Compute the hardware PCF value
	float4 vshadow;
	vshadow.x = ShadowMapTexture2D.SampleCmp(ShadowSampler, UVD.xy, UVD.z).r;
	vshadow.y = ShadowMapTexture2D.SampleCmp(ShadowSampler, UVD.xy + float2(offset, 0), UVD.z).r;
	vshadow.z = ShadowMapTexture2D.SampleCmp(ShadowSampler, UVD.xy + float2(0, offset), UVD.z).r;
	vshadow.w = ShadowMapTexture2D.SampleCmp(ShadowSampler, UVD.xy + float2(offset, offset), UVD.z).r;
	
	return lerp(lerp(vshadow.x, vshadow.y, 0.5),
		                     lerp(vshadow.z, vshadow.w, 0.5),
		                     0.5);
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shaders
/////////////////////////////////////////////////////////////////////////////
 
 
float3 CalculatePointLight(float3 position, Material material)
{
	float3 ToEye = normalize(CameraPosition.xyz - position);
    float3 ToLight = PointLightPos - position;
 
	// range Attenuation
	float DistToLightNorm = 1.0 - saturate(length(ToLight) * PointLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;

	//diffuse
	float diffuse = saturate(dot(ToLight, material.Normal));
    float3 diffuseColor = PointLightColor * diffuse * material.DiffuseColor;
   
    //specular
    float3 reflectVector = reflect(-ToLight,material.Normal);
	float specular = saturate(dot(reflectVector, ToEye));
    float3 specularColor = PointLightColor * saturate(pow(specular, material.SpecularExp)) * material.SpecularColor;

	return PointLightIntensity *Attn * (diffuseColor + specularColor);
}


float3 CalculateSpotLight(float3 position, Material material)
{
	float3 ToEye = normalize(CameraPosition.xyz - position);
	float3 ToLight = SpotLightPos - position;

    // Cone attenuation
	float cosAng = dot(-SpotLightDir, ToLight);
	float conAtt = saturate((cosAng - SpotLightCosOuterCone) * SpotLightCosInnerConeRcp);
	conAtt *= conAtt;

	// range Attenuation
	float DistToLightNorm = 1.0 - saturate(length(ToLight) * SpotLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	Attn *= conAtt;
   
    //diffuse
	float diffuse = saturate(dot(ToLight, material.Normal));
    float3 diffuseColor = SpotLightColor * diffuse * material.DiffuseColor;
   
    //specular
    float3 reflectVector = reflect(-ToLight,material.Normal);
	float specular = saturate(dot(reflectVector, ToEye));
	float3 specularColor = SpotLightColor * saturate(pow(specular, material.SpecularExp)) * material.SpecularColor;

	return SpotLightIntensity *Attn * (diffuseColor + specularColor);
}