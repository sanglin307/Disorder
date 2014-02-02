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
		 Eigen::Vector3f DiffuseColor;
		 Eigen::Vector3f EmissiveColor;
		 Eigen::Vector3f SpecularColor;
		 Eigen::Vector3f ReflectionColor;

		 float SpecularExp;
		 float Transparency;

		void UpdateShaderProperty();
	
        const ShaderPropertyManagerPtr& GetShaderPropertyManager() const
        {
             return _propertyManager;
        }

		static SurfaceMaterialPtr Create(std::string const& name);

    protected:

		SurfaceMaterial(std::string const& name);
        ShaderPropertyManagerPtr _propertyManager;

		ShaderPropertyPtr  _DiffuseColorProperty;
        ShaderPropertyPtr  _EmissiveColorProperty;
		ShaderPropertyPtr  _SpecularColorProperty;

		ShaderPropertyPtr  _SpecularExpProperty;
        ShaderPropertyPtr  _TransparencyFactorProperty;
	};
 
}


#endif