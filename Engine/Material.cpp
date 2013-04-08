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
		RenderEffectPtr effect =  resourceManager->CreateRenderEffect("DiffuseTex.fx",SM_4_0,"VS","PS");
		//shader view
		RenderViewPtr renderView = resourceManager->CreateTexture2DViewFromFile("seafloor.dds");
		MaterialParamShaderResPtr shaderres = effect->GetShaderResourceParameter("DiffuseTexture");
		shaderres->SetValue(renderView);
	    SamplerStatePtr sampler = resourceManager->CreateSamplerState(SF_Linear,TAM_Wrap,0);
		MaterialParamSamplerStatePtr msampler = effect->GetSamplerStateParameter("LinearSampler");
		msampler->SetValue(sampler);

		effect->PrepareRenderParam();

		mat->Effect[MVT_Perspective] = effect;

		return mat;

	}
}
