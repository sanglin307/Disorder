#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_



namespace Disorder
{
	class Renderer : public Component
	{
	public:
		virtual ~Renderer(){};

		virtual void Draw(MaterialViewType view) = 0;
		 
	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
	    
		MaterialParamMatrixPtr WorldViewProjMatrix;
		MaterialParamMatrixPtr WorldMatrix;
		MaterialParamMatrixPtr ViewMatrix;
		MaterialParamMatrixPtr ProjMatrix;
	};



	class GeometryRenderer : public Renderer
	{
	private:
		  GeometryPtr _geometryObject;
		  MaterialPtr _material;

	private:
		  void BuildRenderResource();
	public:
		  GeometryRenderer(GameObjectPtr const& base);
          void SetGeometry(GeometryPtr const& geometry,MaterialPtr const& mat); 
		  virtual void Draw(MaterialViewType view);
	};
}




#endif