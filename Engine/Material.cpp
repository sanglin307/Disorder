#include "EngineInclude.h"

namespace Disorder
{
	Material::Material()
	{
		for(int i=0;i<MVT_NUM_VIEW_TYPES;i++)
		{
			Effect[i] = 0;
		}
	}

	MaterialPtr MaterialGenerator::GenerateLambert(Vector3 const& ambientColor,Vector3 const& diffuseColor)
	{
		MaterialPtr mat = boost::make_shared<Material>();
		mat->Type = MT_Lambert;
		mat->AmbientColor = ambientColor;
		mat->DiffuseColor = diffuseColor;
		mat->SpecularColor = Vector3(1.0f);
		mat->EmissiveColor = Vector3(1.0f);
		mat->Opacity = 1.0;
		mat->Reflectivity = 0.0;
		mat->Shininess = 1.0;

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		RenderEffectPtr baseEffect =  resourceManager->CreateRenderEffect("BaseLightPass.fx",SM_4_0,"VS","PS");
	 
		mat->WorldViewProjMatrix = baseEffect->GetMatrixParameter("WorldViewProjMatrix");
		mat->WorldMatrix = baseEffect->GetMatrixParameter("World");
		mat->ViewMatrix = baseEffect->GetMatrixParameter("View");
		mat->ProjMatrix = baseEffect->GetMatrixParameter("Projection");
		mat->WorldNormal = baseEffect->GetMatrixParameter("WorldNormal");

        MaterialParamVector4Ptr ambient = baseEffect->GetVector4Parameter("AmbientColor");
		ambient->SetValue(Vector4(ambientColor));

		MaterialParamVector4Ptr diffuse = baseEffect->GetVector4Parameter("DiffuseColor");
		diffuse->SetValue(Vector4(diffuseColor));
 
		mat->LightNumber = baseEffect->GetIntParameter("LightNumber");
		mat->LightIntensityArray = baseEffect->GetFloatParameter("LightIntensityArray");
		mat->LightDirArray = baseEffect->GetVector3Parameter("LightDirArray");
		mat->LightColorArray = baseEffect->GetVector3Parameter("LightColorArray");

		baseEffect->PrepareRenderParam();
		mat->Effect[MVT_Perspective] = baseEffect;


		RenderEffectPtr lightEffect =  resourceManager->CreateRenderEffect("LightPass.fx",SM_4_0,"VS","PS");
	 
		mat->LightType = lightEffect->GetIntParameter("LightType");
		mat->LightIntensity = lightEffect->GetFloatParameter("LightIntensity");
		mat->LightPos =  lightEffect->GetVector3Parameter("LightPos");
	    mat->LightDir =  lightEffect->GetVector3Parameter("LightDir");
	    mat->LightColor = lightEffect->GetVector3Parameter("LightColor");
		lightEffect->PrepareRenderParam();
		mat->Effect[MVT_Lights] = lightEffect;

		return mat;

	}
}
