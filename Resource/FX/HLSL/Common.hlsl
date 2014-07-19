//shader common
cbuffer SceneProperty
{
	float3 AmbientLowColor;
	float3 AmbientUpperColor;
	int    ScreenWidth;
	int    ScreenHeight;
	int    ShadowMapSize;
	float    LineRadius;
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
	bool   DirectionLightCastShadow;
} 

 

cbuffer PointLightProperty
{
    float3 PointLightPos;
	float3 PointLightColor;
	float PointLightIntensity;
	float PointLightRangeRcp;
	bool  PointLightCastShadow;
}

cbuffer SpotLightProperty
{
   float3 SpotLightPos;
   float3 SpotLightDir;
   float3 SpotLightColor;
   float SpotLightIntensity;
   float SpotLightRangeRcp;
   float SpotLightCosOuterCone;
   float SpotLightCosInnerCone;
   bool  SpotLightCastShadow;
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
		if (lightDir.y < 0)
		{
			selectShadowMat = ShadowMapViewArray[3];
			lightDir.x = -lightDir.x;
		}
		else
		{
			selectShadowMat = ShadowMapViewArray[2];
			lightDir.z = -lightDir.z;
		}
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

	float offset = 1.0f / ShadowMapSize;
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
 
float3 CalculatePointLight(float3 position, Material material)
{
	float3 ToEye = normalize(CameraPosition.xyz - position);
    float3 ToLight = PointLightPos - position;
 
	// range Attenuation
	float DistToLightNorm = 1.0 - saturate(length(ToLight) * PointLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;

	ToLight = normalize(ToLight);

	//diffuse
	float diffuse = saturate(dot(ToLight, material.Normal));
	float3 diffuseColor = PointLightColor * diffuse * material.DiffuseColor;

	float3 H = normalize(ToEye + ToLight);
	float3 specularColor = PointLightColor * pow(max(0, dot(H, material.Normal)), material.SpecularExp) * material.SpecularColor;

	return PointLightIntensity *Attn * (diffuseColor + specularColor);
}


float3 CalculateSpotLight(float3 position, Material material)
{
	float3 ToEye = normalize(CameraPosition.xyz - position);
	float3 ToLight = SpotLightPos - position;
	float LightLength = length(ToLight);
	ToLight = normalize(ToLight);

    // Cone attenuation
	float cosAng = dot(SpotLightDir, -ToLight);
	float conAtt = saturate(cosAng - SpotLightCosOuterCone);
	
	conAtt *= conAtt;

	// range Attenuation
	float DistToLightNorm = 1.0 - saturate(LightLength * SpotLightRangeRcp);
	float Attn = DistToLightNorm * DistToLightNorm;
	Attn *= conAtt;

	float diffuse = saturate(dot(ToLight, material.Normal));
    float3 diffuseColor = SpotLightColor * diffuse * material.DiffuseColor;

	float3 H = normalize(ToEye + ToLight);
	float3 specularColor = PointLightColor * pow(max(0, dot(H, material.Normal)), material.SpecularExp) * material.SpecularColor;

	return SpotLightIntensity *Attn * (diffuseColor + specularColor);
}