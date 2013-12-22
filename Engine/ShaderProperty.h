#ifndef _DISORDER_SHADERPROPERTY_H_
#define _DISORDER_SHADERPROPERTY_H_

namespace Disorder
{

	enum EShaderProperty
	{
		eSP_Int,
		eSP_Float,
		eSP_Vector3,
		eSP_Vector4,
		eSP_Matrix3,
		eSP_Matrix4,
		eSP_ConstBuffer,
		eSP_ShaderResource,
		eSP_SampleState
	};

	class ShaderProperty
	{
	public:

		
		~ShaderProperty();

		int GetTotalSize();
		int GetElementNumber();

		void SetData(int data);
		void SetData(float data);
		void SetData(RenderBufferPtr constBuffer);
		void SetData(RenderSurfacePtr shaderResource);
		void SetData(SamplerStatePtr sample);
		void SetData(Vector3 const& vec);
		void SetData(Vector4 const& vec);
		void SetData(Matrix3 const& mat);
		void SetData(Matrix4 const& mat);

		void* GetData();
		int GetDataAsInt();
		float GetDataAsFloat();
		Vector3 GetDataAsVector3();
		Vector4 GetDataAsVector4();
		Matrix3 GetDataAsMatrix3();
		Matrix4 GetDataAsMatrix4();
		RenderBufferPtr GetDataAsConstBuffer();
		RenderSurfacePtr GetDataAsShaderResource();
		SamplerStatePtr GetDataAsSampler();

		EShaderProperty PropertyType;
		std::string PropertyName;
 
		static ShaderPropertyPtr Create(EShaderProperty type,std::string const& name);

	private:
		ShaderProperty(EShaderProperty type,std::string const& name);
		void* _data;

	};
 
	class ShaderPropertyManager
	{
		typedef boost::unordered_map<std::string,ShaderPropertyPtr> ShaderPropertyMap;
   public:    // for shader name mapping...................

        static const std::string sManagerCamera;
        static const std::string sManagerObject;
        static const std::string sManagerMaterial;
        static const std::string sManagerLight;
		static const std::string sManagerScene;
        static const std::string sManagerGlobal; // used to save global constant buffer,sampler,texture
		
        // for CameraTransforms
        static const std::string sCameraView;
        static const std::string sCameraProjection;
		static const std::string sCameraPosition;

        // for ObjectTransform
        static const std::string sObjectWorld;
        static const std::string sObjectNormal;

        // for Material Property
        static const std::string sAmbientColor;
        static const std::string sDiffuseColor;
		static const std::string sEmissiveColor;
		static const std::string sSpecularColor;
		static const std::string sReflectionColor;
		static const std::string sShininess;

        // LightProperty
        static const std::string sLightNumber;
        static const std::string sLightIntensityPack;
        static const std::string sLightDirArray;
        static const std::string sLightColorArray;

		//for scene property
		static const std::string sAmbientLowColor;
		static const std::string sAmbientUpperColor;

		//for global property
		static const std::string sFontTexture;
		static const std::string sFontSampler;
		static const std::string sScreenTileTexture;
	    static const std::string sScreenTileSampler;

	public:
		std::string Name;
 
		virtual void UpdateShaderProperty() = 0;
		 
		ShaderPropertyPtr GetProperty(std::string const& name);
		ShaderPropertyPtr CreateProperty(std::string const& name,EShaderProperty type);

		virtual void DumpContent();
	protected:
		ShaderPropertyManager(std::string name):Name(name){}
		ShaderPropertyMap _shaderPropertyMap;
       

	};
 

}




#endif