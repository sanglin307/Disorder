#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_

namespace Disorder
{
	class Renderer : public Component
	{
	public:
		Renderer(std::string const& name);
		virtual ~Renderer(){};

		virtual void PreDraw(CameraPtr const& camera){};
		virtual void Draw(RenderPathType pathType,int pass,CameraPtr const& camera) = 0;
		virtual void PostDraw(CameraPtr const& camera){};

		virtual void AddLight(LightPtr & light);
		virtual void ClearLight();
 
	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
		std::vector<LightPtr> _vLightArray;
	};



	class GeometryRenderer : public Renderer
	{
	private:
		  GeometryPtr _geometryObject;
	      MaterialPtr _material;

	private:
		  void BindRenderResource();

		    // for forward render
		  void SetBaseLightPass();
		  void SetDynamicLightPass(LightPtr const& light);

		  void _Draw();
	public:
		 
		  GeometryRenderer(std::string const& name);
          void SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat);
		

		  virtual void PreDraw(CameraPtr const& camera);
		  virtual void Draw(RenderPathType pathType,int pass,CameraPtr const& camera);
		  virtual void PostDraw(CameraPtr const& camera);
	};
}




#endif