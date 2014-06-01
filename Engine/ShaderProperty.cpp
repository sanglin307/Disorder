#include "EngineInclude.h"
 

namespace Disorder
{
		//////////////////////////////////////////////////////////////////////////

	ShaderProperty::ShaderProperty(EShaderProperty type,unsigned int length,std::string const& name)
		:PropertyType(type),PropertyName(name),_length(length)
	{
		if( PropertyType == eSP_Int )
			_data = new int[length];
		else if(PropertyType == eSP_Float )
			_data = new float[length];
		else if(PropertyType == eSP_Double )
			_data = new double[length];
		else if(PropertyType == eSP_ConstBuffer)
			_data = new RenderBufferPtr;
		else if( PropertyType == eSP_SampleState )
			_data = new SamplerStatePtr;
		else if( PropertyType == eSP_ShaderResource )
			_data = new SurfaceViewPtr;
		else
			BOOST_ASSERT(0);
	}

	ShaderProperty::~ShaderProperty()
	{
		if( PropertyType == eSP_Int || PropertyType == eSP_Float || PropertyType == eSP_Double )
			delete[] _data;
		else if(PropertyType == eSP_ConstBuffer)
			delete (RenderBufferPtr *)_data;
		else if( PropertyType == eSP_SampleState )
			delete (SamplerStatePtr *)_data;
		else if( PropertyType == eSP_ShaderResource )
			delete (SurfaceViewPtr *)_data;
		else
			BOOST_ASSERT(0);
	}
 
	void ShaderProperty::ClearData()
	{
		switch(PropertyType)
		{
			case eSP_Int:
				memset(_data,0,sizeof(int)*_length);
				break;
			case eSP_Float:
				memset(_data,0,sizeof(float)*_length);
				break;
			case eSP_Double:
				memset(_data,0,sizeof(double)*_length);
				break;
			case eSP_ConstBuffer:
			case eSP_ShaderResource:
			case eSP_SampleState:
				break;
			default:
				BOOST_ASSERT(0);
		}
	}

	void ShaderProperty::SetData(int* data)
	{
		BOOST_ASSERT(PropertyType == eSP_Int);
		memcpy(_data,data,sizeof(int)*_length);
	}

	void ShaderProperty::SetData(float* data)
	{
		BOOST_ASSERT(PropertyType == eSP_Float);
		memcpy(_data,data,sizeof(float)*_length);
	}

	void ShaderProperty::SetData(double* data)
	{
		BOOST_ASSERT(PropertyType == eSP_Double);
		memcpy(_data,data,sizeof(double)*_length);
	}

	void ShaderProperty::SetData(RenderBufferPtr constBuffer)
	{
		BOOST_ASSERT(PropertyType == eSP_ConstBuffer);
		*(RenderBufferPtr*)_data = constBuffer;
	}

	void ShaderProperty::SetData(SurfaceViewPtr shaderResource)
	{
		BOOST_ASSERT(PropertyType == eSP_ShaderResource);
		*(SurfaceViewPtr*)_data = shaderResource;
	}

	void ShaderProperty::SetData(SamplerStatePtr sample)
	{
		BOOST_ASSERT(PropertyType == eSP_SampleState);
		*(SamplerStatePtr*)_data = sample;
	}


	ShaderPropertyPtr ShaderProperty::Create(EShaderProperty type,unsigned int length,std::string const& name)
	{
		ShaderProperty* pProperty = new ShaderProperty(type,length,name);
		return ShaderPropertyPtr(pProperty);
	}

	void* ShaderProperty::GetData()
	{
		return _data;
	}

	int* ShaderProperty::GetDataAsInt()
	{
		BOOST_ASSERT(PropertyType == eSP_Int);
		return (int*)_data;
	}

	float* ShaderProperty::GetDataAsFloat()
	{
		BOOST_ASSERT(PropertyType == eSP_Float);
		return (float*)_data;
	}

	double* ShaderProperty::GetDataAsDouble()
	{
		BOOST_ASSERT(PropertyType == eSP_Double);
		return (double*)_data;
	}
 
	RenderBufferPtr ShaderProperty::GetDataAsConstBuffer()
	{
		BOOST_ASSERT(PropertyType == eSP_ConstBuffer );
		return *(RenderBufferPtr*)_data;
	}

	SurfaceViewPtr ShaderProperty::GetDataAsShaderResource()
	{
		BOOST_ASSERT(PropertyType == eSP_ShaderResource );
		return *(SurfaceViewPtr*)_data;
	}

	SamplerStatePtr ShaderProperty::GetDataAsSampler()
	{
		BOOST_ASSERT(PropertyType == eSP_SampleState );
		return *(SamplerStatePtr*)_data;
	}

	//////////////////////////////////////////////////////////////////////////

	const std::string ShaderPropertyManager::sManagerCamera = "CameraTransforms";
    const std::string ShaderPropertyManager::sManagerObject = "ObjectTransform";
    const std::string ShaderPropertyManager::sManagerMaterial = "MaterialProperty";
    const std::string ShaderPropertyManager::sManagerDirectionLight = "DirectionLightProperty";
	const std::string ShaderPropertyManager::sManagerPointLight = "PointLightProperty";
	const std::string ShaderPropertyManager::sManagerSpotLight = "SpotLightProperty";
	const std::string ShaderPropertyManager::sManagerScene = "SceneProperty";
	const std::string ShaderPropertyManager::sManagerShadowMapGen = "ShadowMapGen";
	const std::string ShaderPropertyManager::sManagerGlobal = "Global";

	// for CameraTransforms
    const std::string ShaderPropertyManager::sCameraView = "CameraView";
    const std::string ShaderPropertyManager::sCameraProjection = "CameraProjection";
	const std::string ShaderPropertyManager::sCameraViewProj = "CameraViewProj";
	const std::string ShaderPropertyManager::sCameraViewInv = "CameraViewInv";
	const std::string ShaderPropertyManager::sCameraProjInv = "CameraProjInv";
	const std::string ShaderPropertyManager::sCameraViewProjInv = "CameraViewProjInv";
	const std::string ShaderPropertyManager::sCameraPosition = "CameraPosition";

    // for ObjectTransform
    const std::string ShaderPropertyManager::sObjectWorld = "WorldTransform";
    const std::string ShaderPropertyManager::sObjectNormal = "WorldNormalTransform";

    // for Material Property
    const std::string ShaderPropertyManager::sDiffuseColor = "DiffuseColor";
	const std::string ShaderPropertyManager::sEmissiveColor = "EmissiveColor";
    const std::string ShaderPropertyManager::sSpecularColor = "SpecularColor";
	const std::string ShaderPropertyManager::sSpecularExp =  "SpecularExp";
	const std::string ShaderPropertyManager::sTransparency = "Transparency";
	
	// for Material Texture
	const std::string ShaderPropertyManager::sDiffuseTexture = "DiffuseTexture";
	const std::string ShaderPropertyManager::sDiffuseSampler = "DiffuseSampler";

	// for shadow map
	const std::string ShaderPropertyManager::sShadowSampler = "ShadowSampler";
	const std::string ShaderPropertyManager::sShadowMapTexture2D = "ShadowMapTexture2D";
	const std::string ShaderPropertyManager::sShadowMapTextureCube = "ShadowMapTextureCube";


    // Forward Dirction LightProperty
    const std::string ShaderPropertyManager::sDirectionLightIntensity = "DirectionLightIntensity";
    const std::string ShaderPropertyManager::sDirectionLightDir = "DirectionLightDir";
    const std::string ShaderPropertyManager::sDirectionLightColor = "DirectionLightColor";
 
	 
	// point light
	const std::string ShaderPropertyManager::sPointLightPos = "PointLightPos";
	const std::string ShaderPropertyManager::sPointLightColor = "PointLightColor";
	const std::string ShaderPropertyManager::sPointLightIntensity = "PointLightIntensity";
	const std::string ShaderPropertyManager::sPointLightRangeRcp = "PointLightRangeRcp";

	// spot light
	const std::string ShaderPropertyManager::sSpotLightPos = "SpotLightPos";
	const std::string ShaderPropertyManager::sSpotLightDir = "SpotLightDir";
	const std::string ShaderPropertyManager::sSpotLightColor = "SpotLightColor";
	const std::string ShaderPropertyManager::sSpotLightIntensity = "SpotLightIntensity";
	const std::string ShaderPropertyManager::sSpotLightRangeRcp = "SpotLightRangeRcp";
	const std::string ShaderPropertyManager::sSpotLightCosOuterCone = "SpotLightCosOuterCone";
	const std::string ShaderPropertyManager::sSpotLightCosInnerConeRcp = "SpotLightCosInnerConeRcp";

	// scene ambient
	const std::string ShaderPropertyManager::sAmbientLowColor = "AmbientLowColor";
	const std::string ShaderPropertyManager::sAmbientUpperColor = "AmbientUpperColor";
	 
	const std::string ShaderPropertyManager::sShadowMapView = "ShadowMapView";
	const std::string ShaderPropertyManager::sShadowMapViewArray = "ShadowMapViewArray";
	const std::string ShaderPropertyManager::sShadowMapProj = "ShadowMapProj";

	// Global 
	const std::string ShaderPropertyManager::sTextTexture = "TextTexture";
    const std::string ShaderPropertyManager::sTextSampler = "TextSampler";

	const std::string ShaderPropertyManager::sScreenTileTexture = "ScreenTileTexture";
	const std::string ShaderPropertyManager::sScreenTileSampler = "ScreenTileSampler";

	const std::string ShaderPropertyManager::sGBufferDepthTexture = "GBufferDepthTexture";
	const std::string ShaderPropertyManager::sGBufferColorSpecIntTexture = "GBufferColorSpecIntTexture";
	const std::string ShaderPropertyManager::sGBufferNormalTexture = "GBufferNormalTexture";
	const std::string ShaderPropertyManager::sGBufferSpecPowTexture = "GBufferSpecPowTexture";


	const std::string ShaderPropertyManager::sSurfaceVisTex = "SurfaceVisTex";
	const std::string ShaderPropertyManager::sSurfaceSampler = "SurfaceSampler";

	const std::string ShaderPropertyManager::sSkyboxTexture = "SkyboxTexture";
	const std::string ShaderPropertyManager::sSkyboxSampler = "SkyboxSampler";


	ShaderPropertyPtr ShaderPropertyManager::GetProperty(std::string const& name)
	{
		if( _shaderPropertyMap.find(name) != _shaderPropertyMap.end() )
			return _shaderPropertyMap.at(name);

		return NULL;
	}

	void ShaderPropertyManager::ClearShaderPropertyValue()
	{
		ShaderPropertyMap::const_iterator iter = _shaderPropertyMap.begin();
		while( iter != _shaderPropertyMap.end() )
		{
			iter->second->ClearData();
			++iter;
		}
	}

	ShaderPropertyManager::~ShaderPropertyManager()
	{
		if (_content != 0)
			delete[] _content;
		_content = 0;

	}

	ShaderPropertyPtr ShaderPropertyManager::CreateProperty(std::string const& name,EShaderProperty type,unsigned int length)
	{
		if( _shaderPropertyMap.find(name) != _shaderPropertyMap.end() )
			return _shaderPropertyMap.at(name);

		ShaderPropertyPtr shaderProperty = ShaderProperty::Create(type,length,name);

		_shaderPropertyMap.insert(std::pair<std::string,ShaderPropertyPtr>(name,shaderProperty));

		return shaderProperty;
	}

	void ShaderPropertyManager::DumpContent()
	{
		GLogger->Info("Shader Property Manager Name :" + Name);
		ShaderPropertyMap::const_iterator iter = _shaderPropertyMap.begin();
		while( iter != _shaderPropertyMap.end() )
		{
			GLogger->Info(" ****** Shader Property :" + iter->first);
			++iter;
		}
	}
 
}