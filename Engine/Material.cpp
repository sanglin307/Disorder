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
		RenderEffectPtr effect =  resourceManager->CreateRenderEffect("Default.fx",SM_4_0,"VS","PS");
	 
		MaterialParamVector4Ptr ambient = effect->GetVector4Parameter("AmbientColor");
		ambient->SetValue(Vector4(ambientColor));

		MaterialParamVector4Ptr diffuse = effect->GetVector4Parameter("DiffuseColor");
		diffuse->SetValue(Vector4(diffuseColor));
 
		effect->PrepareRenderParam();

		mat->Effect[MVT_Perspective] = effect;

		return mat;

	}
}
