cbuffer CameraTransforms
{
	matrix CameraView;
	matrix CameraProjection;	
	float3 CameraPosition;
}

cbuffer ObjectTransform
{
	matrix World;
	matrix WorldNormal;  // translate for normal.
}

cbuffer MaterialProperty
{
	float4 AmbientColor;
	float4 DiffuseColor;
}

// support max 4 direct lights now
cbuffer LightProperty
{
	int    LightNumber;
	float4 LightIntensityPack;
	float3 LightDirArray;
	float3 LightColorArray;	
}