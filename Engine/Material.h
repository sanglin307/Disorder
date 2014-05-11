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
		 SamplerStatePtr DiffuseSampler;
		 SurfaceViewPtr DiffuseTexture;
		 

		 std::map<std::string, RenderTexture2DPtr> TextureChannelMap;

		void UpdateShaderProperty();
	
		void UpdateTextureResource();  // create texture resource view according to TextureCannelMap

        const ShaderPropertyManagerPtr& GetShaderPropertyManager() const
        {
             return _propertyManager;
        }

		static SurfaceMaterialPtr Create(std::string const& name);

    protected:

		SurfaceMaterial(std::string const& name);
        ShaderPropertyManagerPtr _propertyManager;

		// tex data
		ShaderPropertyPtr _DiffuseTexProperty;
		ShaderPropertyPtr _DiffuseSampler;


		

		// non-tex data
		ShaderPropertyPtr  _DiffuseColorProperty;
        ShaderPropertyPtr  _EmissiveColorProperty;
		ShaderPropertyPtr  _SpecularColorProperty;
		ShaderPropertyPtr  _SpecularExpProperty;
        ShaderPropertyPtr  _TransparencyFactorProperty;


	};
 
}


#endif