#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_



namespace Disorder
{
	class Renderer : public Component
	{
	public:
		virtual ~Renderer(){};

		virtual void PreDraw(CameraPtr const& camera){};
		virtual void Draw(MaterialViewType view,CameraPtr const& camera) = 0;
		virtual void PostDraw(CameraPtr const& camera){};


		virtual void SetDirectLightParam(std::vector<LightPtr> const& lightArray){;}
		virtual void SetLightParam(LightPtr const& light){;}
	 
		 
	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;

 
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
		  virtual void SetLightParam(LightPtr const& light);
		  virtual void SetDirectLightParam(std::vector<LightPtr> const& lightArray);
		  virtual void PreDraw(CameraPtr const& camera);
		  virtual void Draw(MaterialViewType view,CameraPtr const& camera);
		  virtual void PostDraw(CameraPtr const& camera);
	};
}




#endif