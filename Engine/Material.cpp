#include "EngineInclude.h"

namespace Disorder
{
 
	//////////////////////////////////////////////////////////////////////////
	
	SurfaceMaterial::SurfaceMaterial(std::string const& name)
		:Name(name)
	{
		_propertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerMaterial);

		_DiffuseColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sDiffuseColor,eSP_Float,3);
		_EmissiveColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sEmissiveColor,eSP_Float,3);
		_SpecularColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sSpecularColor,eSP_Float,3);
		_SpecularExpProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sSpecularExp,eSP_Float,1);
		_TransparencyFactorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sTransparency,eSP_Float,1);

		ShaderPropertyManager* globalProperty = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_DiffuseTexProperty = globalProperty->CreateProperty(ShaderPropertyManager::sDiffuseTexture, eSP_ShaderResource, 1);
		_DiffuseSampler = globalProperty->CreateProperty(ShaderPropertyManager::sDiffuseSampler, eSP_SampleState, 1);
 

		DiffuseColor = glm::vec3(0.6f);
		EmissiveColor = glm::vec3(0.f);
		SpecularColor = glm::vec3(0.f);
		SpecularExp = 8.0f;
		Transparency = 0.0f;
	}
 
	void SurfaceMaterial::UpdateShaderProperty()
	{	
		BOOST_ASSERT(SpecularExp <= 10.0f);  // because defer shading have the 8bit precision, so ...
		_propertyManager->ClearShaderPropertyValue();

		if (DiffuseTexture != NULL)
		{
			_DiffuseTexProperty->SetData(DiffuseTexture);
			_DiffuseSampler->SetData(DiffuseSampler);
		}
		else
		{
			_DiffuseTexProperty->SetData(RenderResourceManager::DefaultWhiteTexture2D);
			_DiffuseSampler->SetData(RenderResourceManager::DefaultSamplerState);
		}

		_DiffuseColorProperty->SetData(glm::value_ptr(DiffuseColor));
		_EmissiveColorProperty->SetData(glm::value_ptr(EmissiveColor));
		_SpecularColorProperty->SetData(glm::value_ptr(SpecularColor));
		_SpecularExpProperty->SetData(&SpecularExp);
		_TransparencyFactorProperty->SetData(&Transparency);
		_propertyManager->UpdateShaderProperty();
	}
 
 
	void SurfaceMaterial::UpdateTextureResource()
	{
		if (TextureChannelMap.size() == 0)
			return;

		std::map<std::string, RenderTexture2D*>::const_iterator iter = TextureChannelMap.cbegin();
		while (iter != TextureChannelMap.end())
		{
			std::string key = iter->first;
			boost::to_lower(key);
			if (key == "color" || key == "diffusecolor")
			{
				DiffuseTexture = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, iter->second, PF_R8G8B8A8_UNORM);
				DiffuseSampler = iter->second->Sampler;
			}
			++iter;
		}
	}
}
