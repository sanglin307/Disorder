#include "EngineInclude.h"

namespace Disorder
{
	Material::Material()
	{
	    memset(Effect,0,sizeof(Effect)); 
	}

 
	MaterialParamVector3Ptr MaterialParameterManager::GetVector3Parameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamVector3>(_materialParamMap.at(name));

		MaterialParamVector3Ptr vector = boost::make_shared<MaterialParamVector3>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,vector));

		return vector;
	}

	MaterialParamVector4Ptr MaterialParameterManager::GetVector4Parameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamVector4>(_materialParamMap.at(name));

		MaterialParamVector4Ptr vector = boost::make_shared<MaterialParamVector4>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,vector));

		return vector;
	}

	MaterialParamIntPtr MaterialParameterManager::GetIntParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamInt>(_materialParamMap.at(name));

		MaterialParamIntPtr vector = boost::make_shared<MaterialParamInt>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,vector));

		return vector;
	}

	MaterialParamFloatPtr MaterialParameterManager::GetFloatParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamFloat>(_materialParamMap.at(name));

		MaterialParamFloatPtr vector = boost::make_shared<MaterialParamFloat>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,vector));

		return vector;
	}


	MaterialParamMatrixPtr MaterialParameterManager::GetMatrixParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamMatrix>(_materialParamMap.at(name));

		MaterialParamMatrixPtr matrix = boost::make_shared<MaterialParamMatrix>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,matrix));

		return matrix;
	}

	void Material::UpdateMaterialParameters(GameObjectPtr const& gameObject,CameraPtr const& camera)
	{
		Matrix4 worldMat = gameObject->GetTransform()->GetWorldMatrix();
		Matrix4 viewMat = camera->ViewMatrix;
		Matrix4 projMat = camera->ProjectMatrix;
		Matrix4 wvpMat = projMat*viewMat*worldMat;
		
		WorldMatrix->SetValue(worldMat);
		WorldNormal->SetValue(worldMat.GetNormalMatrix());
		ViewMatrix->SetValue(viewMat);
		ProjMatrix->SetValue(projMat);
		WorldViewProjMatrix->SetValue(wvpMat);


		if(CameraPosition != NULL)
		{
			GameObjectPtr goCamera = camera->GetBase();
			CameraPosition->SetValue(goCamera->GetTransform()->GetWorldPosition());
		}

		//material colors parameters
		if( AmbientColorParam != NULL )
		{
			AmbientColorParam->SetValue(Vector4(AmbientColor));
		}
		if( DiffuseColorParam != NULL )
		{
			DiffuseColorParam->SetValue(Vector4(DiffuseColor));
		}
		if( SpecularColorParam != NULL )
		{
			SpecularColorParam->SetValue(Vector4(SpecularColor));
		}
		if( EmissiveColorParam != NULL )
		{
			EmissiveColorParam->SetValue(Vector4(EmissiveColor));
		}
		if( OpacityParam != NULL )
		{
			OpacityParam->SetValue(Opacity);
		}
		if( ShininessParam != NULL )
		{
			ShininessParam->SetValue(Shininess);
		}
		if( RelectivityParam != NULL )
		{
			RelectivityParam->SetValue(Reflectivity);
		}
	}

	MaterialPtr MaterialGenerator::GeneratePhong(Vector3 const& ambientColor,Vector3 const& diffuseColor,Vector3 const& specularColor,float shininess)
	{
		MaterialPtr mat = boost::make_shared<Material>();
		mat->Type = MT_Phong;
		mat->AmbientColor = ambientColor;
		mat->DiffuseColor = diffuseColor;
		mat->SpecularColor = specularColor;
		mat->EmissiveColor = Vector3(0.0f);
		mat->Opacity = 1.0;
		mat->Reflectivity = 0.0;
		mat->Shininess = shininess;

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		MaterialParameterManagerPtr parameterManager = GEngine->RenderEngine->ParameterManager;

		RenderEffectPtr baseEffect =  resourceManager->CreateRenderEffect("BasePassPhong.fx",SM_4_0,"VS","PS");
	 
		mat->WorldViewProjMatrix = parameterManager->GetMatrixParameter("WorldViewProjMatrix");
		mat->WorldMatrix = parameterManager->GetMatrixParameter("World");
		mat->ViewMatrix = parameterManager->GetMatrixParameter("View");
		mat->ProjMatrix = parameterManager->GetMatrixParameter("Projection");
		mat->WorldNormal = parameterManager->GetMatrixParameter("WorldNormal");

		mat->AmbientColorParam = parameterManager->GetVector4Parameter("AmbientColor");
		mat->DiffuseColorParam = parameterManager->GetVector4Parameter("DiffuseColor");
		mat->SpecularColorParam = parameterManager->GetVector4Parameter("SpecularColor");
		mat->ShininessParam = parameterManager->GetFloatParameter("Shininess");
		mat->CameraPosition = parameterManager->GetVector3Parameter("CameraPosition");
 
		mat->LightNumber = parameterManager->GetIntParameter("LightNumber");
		mat->LightIntensityPack = parameterManager->GetVector4Parameter("LightIntensityPack");
		mat->LightDirArray = parameterManager->GetVector3Parameter("LightDirArray");
		mat->LightColorArray = parameterManager->GetVector3Parameter("LightColorArray");

		baseEffect->PrepareRenderParam();
		mat->Effect[RPT_ForwardMultiPassLight][FRP_BaseLight] = baseEffect;
 
		RenderEffectPtr lightEffect =  resourceManager->CreateRenderEffect("LightPass.fx",SM_4_0,"VS","PS");
	 
		DepthStencilDesc dsDesc;
		dsDesc.DepthEnable = false;
		DepthStencilStatePtr noDepthState = resourceManager->CreateDepthStencilState(&dsDesc,0);
		lightEffect->BindDepthStencilState(noDepthState);
		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		BlendStatePtr blendState = resourceManager->CreateBlendState(&bDesc,1);
		lightEffect->BindBlendState(blendState);

		mat->LightType = parameterManager->GetIntParameter("LightType");
		mat->LightIntensity = parameterManager->GetFloatParameter("LightIntensity");
		mat->LightPos =  parameterManager->GetVector3Parameter("LightPos");
	    mat->LightColor = parameterManager->GetVector3Parameter("LightColor");
		lightEffect->PrepareRenderParam();
		mat->Effect[RPT_ForwardMultiPassLight][FRP_DynamicLight] = lightEffect;

		return mat;
	}
 
	MaterialPtr MaterialGenerator::GenerateLambert(Vector3 const& ambientColor,Vector3 const& diffuseColor)
	{
		MaterialPtr mat = boost::make_shared<Material>();
		mat->Type = MT_Lambert;
		mat->AmbientColor = ambientColor;
		mat->DiffuseColor = diffuseColor;
		mat->SpecularColor = Vector3(1.0f);
		mat->EmissiveColor = Vector3(0.0f);
		mat->Opacity = 1.0;
		mat->Reflectivity = 0.0;
		mat->Shininess = 1.0;

		RenderResourceManagerPtr resourceManager  = GEngine->RenderEngine->ResourceManager;
		MaterialParameterManagerPtr parameterManager = GEngine->RenderEngine->ParameterManager;

		RenderEffectPtr baseEffect =  resourceManager->CreateRenderEffect("BaseLightPass.fx",SM_4_0,"VS","PS");
	 
		mat->WorldViewProjMatrix = parameterManager->GetMatrixParameter("WorldViewProjMatrix");
		mat->WorldMatrix = parameterManager->GetMatrixParameter("World");
		mat->ViewMatrix = parameterManager->GetMatrixParameter("View");
		mat->ProjMatrix = parameterManager->GetMatrixParameter("Projection");
		mat->WorldNormal = parameterManager->GetMatrixParameter("WorldNormal");

		mat->AmbientColorParam = parameterManager->GetVector4Parameter("AmbientColor");
		mat->DiffuseColorParam = parameterManager->GetVector4Parameter("DiffuseColor");
	 
		mat->LightNumber = parameterManager->GetIntParameter("LightNumber");
		mat->LightIntensityPack = parameterManager->GetVector4Parameter("LightIntensityPack");
		mat->LightDirArray = parameterManager->GetVector3Parameter("LightDirArray");
		mat->LightColorArray = parameterManager->GetVector3Parameter("LightColorArray");

		baseEffect->PrepareRenderParam();
		mat->Effect[RPT_ForwardMultiPassLight][FRP_BaseLight] = baseEffect;


		RenderEffectPtr lightEffect =  resourceManager->CreateRenderEffect("LightPass.fx",SM_4_0,"VS","PS");
	 
		DepthStencilDesc dsDesc;
		dsDesc.DepthEnable = false;
		DepthStencilStatePtr noDepthState = resourceManager->CreateDepthStencilState(&dsDesc,0);
		lightEffect->BindDepthStencilState(noDepthState);
		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		BlendStatePtr blendState = resourceManager->CreateBlendState(&bDesc,1);
		lightEffect->BindBlendState(blendState);

		mat->LightType = parameterManager->GetIntParameter("LightType");
		mat->LightIntensity = parameterManager->GetFloatParameter("LightIntensity");
		mat->LightPos =  parameterManager->GetVector3Parameter("LightPos");
	    mat->LightColor = parameterManager->GetVector3Parameter("LightColor");
		lightEffect->PrepareRenderParam();
		mat->Effect[RPT_ForwardMultiPassLight][FRP_DynamicLight] = lightEffect;

		return mat;

	}
}
