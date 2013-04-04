#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_



namespace Disorder
{
	class Renderer : public Component
	{
	public:
		virtual ~Renderer(){};

		virtual void Draw() = 0;

	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
	    

	};

 

	class CubeRenderer: public Renderer
	{
		typedef struct _SimpleVertex
		{
			Vector3 position;
			Vector3 normal;
			Vector2 texcoord;
		} SimpleVertex;
	public:
		CubeRenderer(GameObjectPtr const& base, float length);

		virtual void Draw();

	
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
		  virtual void Draw();
	};
}




#endif