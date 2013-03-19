#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_



namespace Disorder
{
	class Renderer : public Component
	{
	public:
		virtual ~Renderer(){};

		virtual void Draw() = 0;

	public:
		RenderTechniquePtr _renderTechnique;
		RenderLayoutPtr _renderLayout;
	    

	};


	class BrushRenderer : public Renderer
	{
	};

	class CubeRenderer: public BrushRenderer
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

	public:

          void SetGeometry(GeometryPtr const& geometry); 
		  virtual void Draw();
	};
}




#endif