#include "EngineInclude.h"
 

namespace Disorder
{
	std::map<std::string,std::string> ShaderObject::sMapIncludeFiles;
	std::map<std::string,std::string> ShaderObject::_mapShaderMacro;

	void ShaderObject::SetShaderMacro(std::string const& name,std::string const& value)
	{
		 _mapShaderMacro[name] = value;
	}
	
	void ShaderObject::RemoveShaderMacro(std::string const& name)
	{
		if( _mapShaderMacro.find(name) != _mapShaderMacro.end() )
			_mapShaderMacro.erase(name);
	}

	void ShaderObject::DumpContent()
	{
		GLogger->Info("Shader Object :" + GetShaderName() );
	}

	void RenderEffect::UpdateShaderParameter()
	{
		if( _vertexShader != NULL )
			_vertexShader->UpdateShaderParameter();
		if (_geometryShader != NULL)
			_geometryShader->UpdateShaderParameter();
		if( _pixelShader != NULL )
			_pixelShader->UpdateShaderParameter();
	}

 
}