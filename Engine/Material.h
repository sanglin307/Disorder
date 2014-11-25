#ifndef _DISORDER_MATERIAL_H_
#define _DISORDER_MATERIAL_H_


namespace Disorder
{
	class SurfaceMaterial
	{
	public:
	 
         std::string Name;
		 std::string ShaderModel;
 
		 // input 
		 glm::vec3 DiffuseColor;
		 glm::vec3 EmissiveColor;
		 glm::vec3 SpecularColor;
		 glm::vec3 ReflectionColor;

		 float SpecularExp;
		 float Transparency;

		 // input texture related
		 SamplerState* DiffuseSampler;
		 SurfaceView* DiffuseTexture;
		 

		 std::map<std::string, RenderTexture2D*> TextureChannelMap;

		void UpdateShaderProperty();
	
		void UpdateTextureResource();  // create texture resource view according to TextureCannelMap

        const ShaderPropertyManager* GetShaderPropertyManager() const
        {
             return _propertyManager;
        }

		SurfaceMaterial(std::string const& name);
    protected:

		
        ShaderPropertyManager* _propertyManager;

		// tex data
		ShaderProperty* _DiffuseTexProperty;
		ShaderProperty* _DiffuseSampler;

		// non-tex data
		ShaderProperty*  _DiffuseColorProperty;
		ShaderProperty*  _EmissiveColorProperty;
		ShaderProperty*  _SpecularColorProperty;
		ShaderProperty*  _SpecularExpProperty;
		ShaderProperty*  _TransparencyFactorProperty;


	};
 
}


#endif