#ifndef _DISORDER_MATERIAL_H_
#define _DISORDER_MATERIAL_H_


namespace Disorder
{
 
	class SurfaceMaterial
	{
	public:
        std::string Name;
		MaterialType Type;
		std::string ShaderModel;
 
		virtual void UpdateShaderProperty(){};
		std::vector<RenderEffectPtr> RenderPass;
        
        const ShaderPropertyManagerPtr& GetShaderPropertyManager() const
        {
             return _propertyManager;
        }

    protected:

		SurfaceMaterial(std::string const& name);
        ShaderPropertyManagerPtr _propertyManager;
	};

    class SurfaceLambert : public SurfaceMaterial
	{
     public:
		 Vector3 AmbientColor;
		 Vector3 DiffuseColor;
		 Vector3 EmissiveColor;
		 float Transparency;

		 virtual void UpdateShaderProperty();

		 static SurfaceLambertPtr Create(std::string const& name);
	protected:

		 SurfaceLambert(std::string const& name);

         ShaderPropertyPtr  AmbientColorProperty;
         ShaderPropertyPtr  DiffuseColorProperty;
         ShaderPropertyPtr  EmissiveColorProperty;
         ShaderPropertyPtr  TransparentColorProperty;
         ShaderPropertyPtr  TransparencyFactorProperty;
    };

	class SurfacePhong : public SurfaceLambert
	{
	public:
		Vector3 SpecularColor;
		Vector3 ReflectionColor;
		float Shininess;

		virtual void UpdateShaderProperty();

		static SurfacePhongPtr Create(std::string const& name);
	protected:

		SurfacePhong(std::string const& name);

		ShaderPropertyPtr SpecularColorProperty;
		ShaderPropertyPtr ReflectionColorProperty;
		ShaderPropertyPtr ShininessProperty;

	};
 

}


#endif