#include "EngineInclude.h"

namespace Disorder
{
	MaterialParamCBufferPtr RenderEffect::GetConstantBufferParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamCBuffer>(_materialParamMap.at(name));

		MaterialParamCBufferPtr constBuffer = boost::make_shared<MaterialParamCBuffer>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,constBuffer));

		return constBuffer;
	}

		MaterialParamShaderResPtr RenderEffect::GetShaderResourceParameter(std::string const& name)
	{
		// texture and structed.
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamShaderRes>(_materialParamMap.at(name));

		MaterialParamShaderResPtr shaderres = boost::make_shared<MaterialParamShaderRes>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,shaderres));

		return shaderres;
	}

	MaterialParamSamplerStatePtr RenderEffect::GetSamplerStateParameter(std::string const& name)
	{
		if( _materialParamMap.find(name) != _materialParamMap.end() )
			return boost::dynamic_pointer_cast<MaterialParamSamplerState>(_materialParamMap.at(name));

		MaterialParamSamplerStatePtr ss = boost::make_shared<MaterialParamSamplerState>();
		_materialParamMap.insert(std::pair<std::string,MaterialParamPtr>(name,ss));

		return ss;
	}


}