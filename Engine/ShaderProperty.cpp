#include "EngineInclude.h"
 

namespace Disorder
{
		//////////////////////////////////////////////////////////////////////////
	ShaderProperty::ShaderProperty(EShaderProperty type,std::string const& name)
		:PropertyType(type),PropertyName(name)
	{
		if( PropertyType == eSP_Int )
			_data = new int;
		else if(PropertyType == eSP_Float )
			_data = new float;
		else if(PropertyType == eSP_ConstBuffer)
			_data = new RenderBufferPtr;
		else if( PropertyType == eSP_SampleState )
			_data = new SamplerStatePtr;
		else if( PropertyType == eSP_ShaderResource )
			_data = new RenderSurfacePtr;
		else if( PropertyType == eSP_Matrix3 )
			_data = new Matrix3;
		else if( PropertyType == eSP_Matrix4 )
			_data = new Matrix4;
		else if( PropertyType == eSP_Vector3 )
			_data = new Vector3;
		else if( PropertyType == eSP_Vector4 )
			_data = new Vector4;
		else
			BOOST_ASSERT(0);
	}

	ShaderProperty::~ShaderProperty()
	{
		if( PropertyType == eSP_Int )
			delete (int *)_data;
		else if(PropertyType == eSP_Float )
			delete (float *)_data;
		else if(PropertyType == eSP_ConstBuffer)
			delete (RenderBufferPtr *)_data;
		else if( PropertyType == eSP_SampleState )
			delete (SamplerStatePtr *)_data;
		else if( PropertyType == eSP_ShaderResource )
			delete (RenderSurfacePtr *)_data;
		else if ( PropertyType == eSP_Matrix3 )
			delete (Matrix3 *)_data;
		else if( PropertyType == eSP_Matrix4 )
			delete (Matrix4 *)_data;
		else if( PropertyType == eSP_Vector3 )
			delete (Vector3 *)_data;
		else if( PropertyType == eSP_Vector4 )
			delete (Vector4 *)_data;
		else
			BOOST_ASSERT(0);
	}

	int ShaderProperty::GetTotalSize()
	{
		switch(PropertyType)
		{
			case eSP_Int:
			case eSP_Float:
				return 4;
			case eSP_ConstBuffer:
			case eSP_ShaderResource:
			case eSP_SampleState:
				return 6;  // share pointer
			case eSP_Vector3:
				return 3 * sizeof(float);
			case eSP_Vector4:
				return 4 * sizeof(float);
			case eSP_Matrix4:
				return 16 * sizeof(float);
			case eSP_Matrix3:
				return 9 * sizeof(float);
			default:
				return 0;
		}
	}

	int ShaderProperty::GetElementNumber()
	{
		switch(PropertyType)
		{
			case eSP_Int:
			case eSP_Float:
			case eSP_ConstBuffer:
			case eSP_ShaderResource:
			case eSP_SampleState:
				return 1;
			case eSP_Vector3:
				return 3;
			case eSP_Vector4:
				return 4;
			case eSP_Matrix4:
				return 16;
			case eSP_Matrix3:
				return 9;
			default:
				return 0;
		}
	}
 
	void ShaderProperty::ClearData()
	{
		switch(PropertyType)
		{
			case eSP_Int:
				SetData(0);
				break;
			case eSP_Float:
				SetData(0.f);
				break;
			case eSP_ConstBuffer:
			case eSP_ShaderResource:
			case eSP_SampleState:
				break;
			case eSP_Vector3:
				SetData(Vector3::ZERO);
				break;
			case eSP_Vector4:
				SetData(Vector4::ZERO);
				break;
			case eSP_Matrix4:
				SetData(Matrix4::IDENTITY);
				break;
			case eSP_Matrix3:
				SetData(Matrix3::IDENTITY);
				break;
			default:
				BOOST_ASSERT(0);
		}
	}

	void ShaderProperty::SetData(int data)
	{
		BOOST_ASSERT(PropertyType == eSP_Int);
		*(int*)_data = data;
	}

	void ShaderProperty::SetData(float data)
	{
		BOOST_ASSERT(PropertyType == eSP_Float);
		*(float*)_data = data;
	}

	void ShaderProperty::SetData(RenderBufferPtr constBuffer)
	{
		BOOST_ASSERT(PropertyType == eSP_ConstBuffer);
		*(RenderBufferPtr*)_data = constBuffer;
	}

	void ShaderProperty::SetData(RenderSurfacePtr shaderResource)
	{
		BOOST_ASSERT(PropertyType == eSP_ShaderResource);
		*(RenderSurfacePtr*)_data = shaderResource;
	}

	void ShaderProperty::SetData(SamplerStatePtr sample)
	{
		BOOST_ASSERT(PropertyType == eSP_SampleState);
		*(SamplerStatePtr*)_data = sample;
	}

	void ShaderProperty::SetData(Vector3 const& vec)
	{
		BOOST_ASSERT(PropertyType == eSP_Vector3);
		*(Vector3*)_data = vec;
	}

	void ShaderProperty::SetData(Vector4 const& vec)
	{
		BOOST_ASSERT(PropertyType == eSP_Vector4);
		*(Vector4*)_data = vec;
	}

	void ShaderProperty::SetData(Matrix4 const& mat)
	{
		BOOST_ASSERT(PropertyType == eSP_Matrix4);
		*(Matrix4*)_data = mat;
	}

	void ShaderProperty::SetData(Matrix3 const& mat)
	{
		BOOST_ASSERT(PropertyType == eSP_Matrix3);
		*(Matrix3*)_data = mat;
	}

	ShaderPropertyPtr ShaderProperty::Create(EShaderProperty type,std::string const& name)
	{
		ShaderProperty* pProperty = new ShaderProperty(type,name);
		return ShaderPropertyPtr(pProperty);
	}

	void* ShaderProperty::GetData()
	{
		return _data;
	}

	int ShaderProperty::GetDataAsInt()
	{
		BOOST_ASSERT(PropertyType == eSP_Int);
		return *(int*)_data;
	}

	float ShaderProperty::GetDataAsFloat()
	{
		BOOST_ASSERT(PropertyType == eSP_Float);
		return *(float*)_data;
	}

	Vector3 ShaderProperty::GetDataAsVector3()
	{
		BOOST_ASSERT(PropertyType == eSP_Vector3 );
		return *(Vector3*)_data;
	}

	Vector4 ShaderProperty::GetDataAsVector4()
	{
		BOOST_ASSERT(PropertyType == eSP_Vector4 );
		return *(Vector4*)_data;
	}

	Matrix3 ShaderProperty::GetDataAsMatrix3()
	{
		BOOST_ASSERT(PropertyType == eSP_Matrix3 );
		return *(Matrix3*)_data;
	}

	Matrix4 ShaderProperty::GetDataAsMatrix4()
	{
		BOOST_ASSERT(PropertyType == eSP_Matrix4 );
		return *(Matrix4*)_data;
	}

	RenderBufferPtr ShaderProperty::GetDataAsConstBuffer()
	{
		BOOST_ASSERT(PropertyType == eSP_ConstBuffer );
		return *(RenderBufferPtr*)_data;
	}

	RenderSurfacePtr ShaderProperty::GetDataAsShaderResource()
	{
		BOOST_ASSERT(PropertyType == eSP_ShaderResource );
		return *(RenderSurfacePtr*)_data;
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
	const std::string ShaderPropertyManager::sManagerScene = "SceneProperty";
	const std::string ShaderPropertyManager::sManagerGlobal = "Global";

	// for CameraTransforms
    const std::string ShaderPropertyManager::sCameraView = "CameraView";
    const std::string ShaderPropertyManager::sCameraProjection = "CameraProjection";
	const std::string ShaderPropertyManager::sCameraPosition = "CameraPosition";

    // for ObjectTransform
    const std::string ShaderPropertyManager::sObjectWorld = "WorldTransform";
    const std::string ShaderPropertyManager::sObjectNormal = "WorldNormalTransform";

    // for Material Property
    const std::string ShaderPropertyManager::sDiffuseColor = "DiffuseColor";
	const std::string ShaderPropertyManager::sEmissiveColor = "EmissiveColor";
    const std::string ShaderPropertyManager::sSpecularColor = "SpecularColor";
	const std::string ShaderPropertyManager::sReflectionColor = "ReflectionColor";
	const std::string ShaderPropertyManager::sShininess = "Shininess";

    // LightProperty
    const std::string ShaderPropertyManager::sDirectionLightIntensity = "DirectionLightIntensity";
    const std::string ShaderPropertyManager::sDirectionLightDir = "DirectionLightDir";
    const std::string ShaderPropertyManager::sDirectionLightColor = "DirectionLightColor";
 

	// scene ambient
	const std::string ShaderPropertyManager::sAmbientLowColor = "AmbientLowColor";
	const std::string ShaderPropertyManager::sAmbientUpperColor = "AmbientUpperColor";

	// Global 
	const std::string ShaderPropertyManager::sFontTexture = "FontTexture";
    const std::string ShaderPropertyManager::sFontSampler = "FontSampler";

	const std::string ShaderPropertyManager::sScreenTileTexture = "ScreenTileTexture";
	const std::string ShaderPropertyManager::sScreenTileSampler = "ScreenTileSampler";

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

	ShaderPropertyPtr ShaderPropertyManager::CreateProperty(std::string const& name,EShaderProperty type)
	{
		if( _shaderPropertyMap.find(name) != _shaderPropertyMap.end() )
			return _shaderPropertyMap.at(name);

		ShaderPropertyPtr shaderProperty = ShaderProperty::Create(type,name);

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