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
		MaterialParameterManagerPtr parameterManager = GEngine->RenderEngine->ParameterManager;

		RenderEffectPtr baseEffect =  resourceManager->CreateRenderEffect("BaseLightPass.fx",SM_4_0,"VS","PS");
	 
		mat->WorldViewProjMatrix = parameterManager->GetMatrixParameter("WorldViewProjMatrix");
		mat->WorldMatrix = parameterManager->GetMatrixParameter("World");
		mat->ViewMatrix = parameterManager->GetMatrixParameter("View");
		mat->ProjMatrix = parameterManager->GetMatrixParameter("Projection");
		mat->WorldNormal = parameterManager->GetMatrixParameter("WorldNormal");

        MaterialParamVector4Ptr ambient = parameterManager->GetVector4Parameter("AmbientColor");
		ambient->SetValue(Vector4(ambientColor));

		MaterialParamVector4Ptr diffuse = parameterManager->GetVector4Parameter("DiffuseColor");
		diffuse->SetValue(Vector4(diffuseColor));
 
		mat->LightNumber = parameterManager->GetIntParameter("LightNumber");
		mat->LightIntensityPack = parameterManager->GetVector4Parameter("LightIntensityPack");
		mat->LightDirArray = parameterManager->GetVector3Parameter("LightDirArray");
		mat->LightColorArray = parameterManager->GetVector3Parameter("LightColorArray");

		baseEffect->PrepareRenderParam();
		mat->Effect[MVT_Perspective] = baseEffect;


		RenderEffectPtr lightEffect =  resourceManager->CreateRenderEffect("LightPass.fx",SM_4_0,"VS","PS");
	 
		mat->LightType = parameterManager->GetIntParameter("LightType");
		mat->LightIntensity = parameterManager->GetFloatParameter("LightIntensity");
		mat->LightPos =  parameterManager->GetVector3Parameter("LightPos");
	    mat->LightColor = parameterManager->GetVector3Parameter("LightColor");
		lightEffect->PrepareRenderParam();
		mat->Effect[MVT_Lights] = lightEffect;

		return mat;

	}
}
