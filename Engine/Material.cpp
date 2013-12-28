#include "EngineInclude.h"

namespace Disorder
{
 
	//////////////////////////////////////////////////////////////////////////
	
	SurfaceMaterial::SurfaceMaterial(std::string const& name)
		:Name(name)
	{
		_propertyManager = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerMaterial);
	}
 
	//////////////////////////////////////////////////////////////////////////
	
	void SurfaceLambert::UpdateShaderProperty()
	{	
		_propertyManager->ClearShaderPropertyValue();
		DiffuseColorProperty->SetData(DiffuseColor);
		EmissiveColorProperty->SetData(EmissiveColor);
		_propertyManager->UpdateShaderProperty();
	}

	SurfaceLambert::SurfaceLambert(std::string const& name)
		:SurfaceMaterial(name)
	{
	}

	SurfaceLambertPtr SurfaceLambert::Create(std::string const& name)
	{
		SurfaceLambert* mat = new SurfaceLambert(name);
		mat->Type = MT_Lambert;
 
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
		ShaderPropertyManagerPtr parameterManager = mat->GetShaderPropertyManager();
 
		mat->DiffuseColorProperty = parameterManager->CreateProperty(ShaderPropertyManager::sDiffuseColor,eSP_Vector3);
		mat->EmissiveColorProperty = parameterManager->CreateProperty(ShaderPropertyManager::sEmissiveColor,eSP_Vector3);

		mat->DiffuseColor = Vector3(0.8f);
		mat->EmissiveColor = Vector3::ZERO;

		//RenderEffectPtr lightEffect =  resourceManager->CreateRenderEffect("LightPass.fx",SM_4_0,"VS","PS");
	 //
		//DepthStencilDesc dsDesc;
		//dsDesc.DepthEnable = false;
		//DepthStencilStatePtr noDepthState = resourceManager->CreateDepthStencilState(&dsDesc,0);
		//lightEffect->BindDepthStencilState(noDepthState);
		//BlendDesc bDesc;
		//bDesc.BlendEnable = true;
		//bDesc.BlendOp = BLEND_OP_ADD;
		//bDesc.SrcBlend = BLEND_ONE;
		//bDesc.DestBlend = BLEND_ONE;
		//BlendStatePtr blendState = resourceManager->CreateBlendState(&bDesc,1);
		//lightEffect->BindBlendState(blendState);

		//mat->LightType = parameterManager->CreateProperty("LightType",eSP_Int);
		//mat->LightIntensity = parameterManager->CreateProperty("LightIntensity",eSP_Float);
		//mat->LightPos =  parameterManager->CreateProperty("LightPos",eSP_Vector3);
	 //   mat->LightColor = parameterManager->CreateProperty("LightColor",eSP_Vector3);
		//lightEffect->InitShaderParameter();
		//mat->RenderPass.push_back(lightEffect);

		return SurfaceLambertPtr(mat);

	}

	//////////////////////////////////////////////////////////////////////////
	
	SurfacePhong::SurfacePhong(std::string const& name)
		:SurfaceLambert(name)
	{
	}

	void SurfacePhong::UpdateShaderProperty()
	{
		_propertyManager->ClearShaderPropertyValue();
		DiffuseColorProperty->SetData(DiffuseColor);
		SpecularColorProperty->SetData(SpecularColor);
		ShininessProperty->SetData(Shininess);
		_propertyManager->UpdateShaderProperty();
	}

	SurfacePhongPtr SurfacePhong::Create(std::string const& name)
	{
		SurfacePhong* mat = new SurfacePhong(name);
		mat->Type = MT_Phong;
		RenderResourceManagerPtr resourceManager  = GEngine->RenderResManager;
		ShaderPropertyManagerPtr parameterManager = mat->GetShaderPropertyManager();

		mat->DiffuseColorProperty = parameterManager->CreateProperty(ShaderPropertyManager::sDiffuseColor,eSP_Vector3);
		mat->SpecularColorProperty = parameterManager->CreateProperty(ShaderPropertyManager::sSpecularColor,eSP_Vector3);
		mat->ReflectionColorProperty = parameterManager->CreateProperty(ShaderPropertyManager::sReflectionColor,eSP_Vector3);
		mat->ShininessProperty = parameterManager->CreateProperty(ShaderPropertyManager::sShininess,eSP_Float);

		mat->Shininess = 8.f;
		mat->SpecularColor = Vector3(0.8f);
		mat->ReflectionColor = Vector3(0.f);
 
	
		/*DepthStencilDesc dsDesc;
		dsDesc.DepthEnable = false;
		DepthStencilStatePtr noDepthState = resourceManager->CreateDepthStencilState(&dsDesc,0);
		lightEffect->BindDepthStencilState(noDepthState);
		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		BlendStatePtr blendState = resourceManager->CreateBlendState(&bDesc,1);
		lightEffect->BindBlendState(blendState);*/

		return SurfacePhongPtr(mat);
	}
 
}
