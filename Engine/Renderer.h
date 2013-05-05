#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_



namespace Disorder
{
	class Renderer : public Component
	{
	public:
		virtual ~Renderer(){};

		virtual void Draw(MaterialViewType view,CameraPtr const& camera) = 0;

		virtual void SetLightParam(LightPtr const& light){;}
	 
		 
	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
	    
		MaterialParamMatrixPtr WorldViewProjMatrix;
		MaterialParamMatrixPtr WorldMatrix;
		MaterialParamMatrixPtr ViewMatrix;
		MaterialParamMatrixPtr ProjMatrix;
		MaterialParamMatrixPtr WorldNormal;

		MaterialParamIntPtr    LightType;
	    MaterialParamFloatPtr  LightIntensity;
	    MaterialParamVector3Ptr LightPos;
	    MaterialParamVector3Ptr LightDir;
	    MaterialParamVector3Ptr LightColor;
	};



	class GeometryRenderer : public Renderer
	{
	private:
		  GeometryPtr _geometryObject;
		  MaterialPtr _material;

	private:
		  void BuildRenderResource();
	public:
		 
          void SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat); 
		  void SetLightParam(LightPtr const& light);
		  virtual void Draw(MaterialViewType view,CameraPtr const& camera);
	};
}




#endif