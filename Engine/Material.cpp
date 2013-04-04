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

	MaterialPtr MaterialGenerator::GenerateLambert()
	{
		MaterialPtr mat = boost::make_shared<Material>();
		mat->Type = MT_Lambert;

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		RenderEffectPtr effect =  resourceManager->CreateRenderEffect("SimpleFX_PT.fx",SM_4_0,"VS","PS");
		mat->Effect[MVT_Perspective] = effect;

		return mat;

	}
}
