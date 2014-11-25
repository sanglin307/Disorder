#ifndef _DISORDER_SHADERPROPERTY_H_
#define _DISORDER_SHADERPROPERTY_H_

namespace Disorder
{

	enum EShaderProperty
	{
		eSP_Bool,
		eSP_Int,
		eSP_Float,
		eSP_Double,
		eSP_ConstBuffer,
		eSP_ShaderResource,
		eSP_SampleState
	};

	class ShaderProperty
	{
	public:
		~ShaderProperty();

		unsigned int GetLength()
		{
			return _length;
		}

		void ClearData();

		void SetData(bool* data);
		void SetData(int* data);
		void SetData(float* data);
		void SetData(double* data);
		void SetData(RenderBuffer* constBuffer);
		void SetData(SurfaceView* shaderResource);
		void SetData(SamplerState* sample);

		void* GetData();
		int* GetDataAsInt();
		float* GetDataAsFloat();
		double* GetDataAsDouble();

		RenderBuffer* GetDataAsConstBuffer();
		SurfaceView* GetDataAsShaderResource();
		SamplerState* GetDataAsSampler();

		EShaderProperty PropertyType;
		std::string PropertyName;
 
		ShaderProperty(EShaderProperty type, unsigned int length, std::string const& name);

	private:
		void* _data;
		unsigned int _length;

	};
 
	class ShaderPropertyManager
	{
		typedef boost::unordered_map<std::string,ShaderProperty*> ShaderPropertyMap;
   public:    // for shader name mapping...................
	    virtual ~ShaderPropertyManager();

        static const std::string sManagerCamera;
        static const std::string sManagerObject;
        static const std::string sManagerMaterial;
        static const std::string sManagerDirectionLight;
		static const std::string sManagerPointLight;
		static const std::string sManagerSpotLight;
		static const std::string sManagerScene;
		static const std::string sManagerGBufferUnpack;
		static const std::string sManagerShadowMapGen;
        static const std::string sManagerGlobal; // used to save global constant buffer,sampler,texture
		
        // for CameraTransforms
        static const std::string sCameraView;
        static const std::string sCameraProjection;
		static const std::string sCameraViewProj;
		static const std::string sCameraViewInv;
		static const std::string sCameraProjInv;
		static const std::string sCameraViewProjInv;
		static const std::string sCameraPosition;

        // for ObjectTransform
        static const std::string sObjectWorld;
        static const std::string sObjectNormal;

        // for Material Property
        static const std::string sDiffuseColor;
		static const std::string sEmissiveColor;
		static const std::string sSpecularColor;
		static const std::string sSpecularExp;
		static const std::string sTransparency;

        // forward direction LightProperty
        static const std::string sDirectionLightIntensity;
        static const std::string sDirectionLightDir;
        static const std::string sDirectionLightColor;
		static const std::string sDirectionLightCastShadow;

		// point light
		static const std::string sPointLightPos;
		static const std::string sPointLightColor;
		static const std::string sPointLightIntensity;
		static const std::string sPointLightRangeRcp;
		static const std::string sPointLightCastShadow;
 
		// spot light
		static const std::string sSpotLightPos;
		static const std::string sSpotLightDir;
		static const std::string sSpotLightColor;
		static const std::string sSpotLightIntensity;
		static const std::string sSpotLightRangeRcp;
		static const std::string sSpotLightCosOuterCone;
		static const std::string sSpotLightCosInnerCone;
		static const std::string sSpotLightCastShadow;
 
		//for scene property
		static const std::string sScreenWidth;
		static const std::string sScreenHeight;
		static const std::string sShadowMapSize;
		static const std::string sAmbientLowColor;
		static const std::string sAmbientUpperColor;
		static const std::string sLineRadius;
 
		// ShadowMapGen
		static const std::string sShadowMapView;
		static const std::string sShadowMapViewArray;
		static const std::string sShadowMapProj;

		//for global property
		static const std::string sTextTexture;
		static const std::string sTextSampler;
		static const std::string sScreenTileTexture;
	    static const std::string sScreenTileSampler;

		static const std::string sGBufferDepthTexture;
		static const std::string sGBufferColorSpecIntTexture;
		static const std::string sGBufferNormalTexture;
		static const std::string sGBufferSpecPowTexture;

		static const std::string sSurfaceVisTex;
		static const std::string sSurfaceSampler;

		static const std::string sDiffuseTexture;
		static const std::string sDiffuseSampler;

		static const std::string sShadowSampler;
		static const std::string sShadowMapTexture2D;
		static const std::string sShadowMapTextureCube;

		static const std::string sSkyboxTexture;
		static const std::string sSkyboxSampler;

		static const std::string sLineTexture;
		static const std::string sLineSampler;

		static const std::string sScreenAATexture;
		static const std::string sScreenAASampler;

	public:
		std::string Name;
 
		virtual void ClearShaderPropertyValue();
		virtual void UpdateShaderProperty() = 0;
		 
		ShaderProperty* GetProperty(std::string const& name);
		ShaderProperty* CreateProperty(std::string const& name,EShaderProperty type,unsigned int length = 1);

		virtual void DumpContent();
	protected:
		ShaderPropertyManager(const std::string& name) :Name(name){}
		ShaderPropertyMap _shaderPropertyMap;

	};
 

}




#endif