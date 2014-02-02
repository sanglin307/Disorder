#include "EngineInclude.h"

namespace Disorder
{
 
	//////////////////////////////////////////////////////////////////////////
	
	SurfaceMaterial::SurfaceMaterial(std::string const& name)
		:Name(name)
	{
		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerMaterial);

		_DiffuseColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sDiffuseColor,eSP_Float,3);
		_EmissiveColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sEmissiveColor,eSP_Float,3);
		_SpecularColorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sSpecularColor,eSP_Float,3);
		_SpecularExpProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sSpecularExp,eSP_Float,1);
		_TransparencyFactorProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sTransparency,eSP_Float,1);

		DiffuseColor = Eigen::Vector3f::Constant(0.6f);
		EmissiveColor = Eigen::Vector3f::Constant(0.f);
		SpecularColor = Eigen::Vector3f::Constant(0.f);
		SpecularExp = 1.0f;
		Transparency = 0.0f;
	}

	SurfaceMaterialPtr SurfaceMaterial::Create(std::string const& name)
	{
		SurfaceMaterial *pMaterial = new SurfaceMaterial(name);
		return SurfaceMaterialPtr(pMaterial);
	}

	void SurfaceMaterial::UpdateShaderProperty()
	{	
		BOOST_ASSERT(SpecularExp <= 10.0f);  // because defer shading have the 8bit precision, so ...
		_propertyManager->ClearShaderPropertyValue();
		_DiffuseColorProperty->SetData(DiffuseColor.data());
		_EmissiveColorProperty->SetData(EmissiveColor.data());
		_SpecularColorProperty->SetData(SpecularColor.data());
		_SpecularExpProperty->SetData(&SpecularExp);
		_TransparencyFactorProperty->SetData(&Transparency);
		_propertyManager->UpdateShaderProperty();
	}
 
 
}
