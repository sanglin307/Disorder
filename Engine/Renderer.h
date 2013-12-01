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
		virtual void Draw(int pass,CameraPtr const& camera) = 0;
		virtual void PostDraw(CameraPtr const& camera){};

		virtual unsigned int GetPassNumber() = 0;

		virtual void AddLight(LightPtr & light);
		virtual void ClearLight();
 
	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
		std::vector<LightPtr> _vLightArray;
	};
 
	class BatchScreenTiles : public Renderer
	{
	   
	public:
		BatchScreenTiles(std::string const& name);

		void SetTexture(RenderSurfacePtr const& texture);
		 
		void AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord);
		virtual void Draw(int pass,CameraPtr const& camera);

		virtual unsigned int GetPassNumber()
		{
			return 1;
		}

		unsigned int GetCurrentDrawTriNumber();

	protected:
		std::vector<BatchTileVertex> _vertexs;
		std::vector<WORD> _indexs;
		unsigned int _savedVertexBufferSize;
		unsigned int _savedIndexBufferSize;
		RenderSurfacePtr _texture;
	};

	class BatchLines : public Renderer
	{
	public:
		BatchLines(std::string const& name);
		void AddLine(Vector3 const& beginPos,Vector4 const& beginColor,Vector3 const& endPos,Vector4 const& endColor);
		virtual void Draw(int pass,CameraPtr const& camera);

		virtual unsigned int GetPassNumber()
		{
			return 1;
		}
	private:
		std::vector<BatchLineVertex> _vertexs;
		unsigned int _savedVertexBufferSize;
	};

	class GeometryRenderer : public Renderer
	{
	private:
		  GeometryPtr _geometryObject;
	      SurfaceMaterialPtr _material;

	private:
		  void BindRenderResource();

		    // for forward render
		  void SetBaseLightPass();
		  void SetDynamicLightPass(LightPtr const& light);

		  void _Draw();

		  void DrawAxis(CameraPtr const& camera);
		  void DrawBoundingBox(CameraPtr const& camera);

		  GeometryRenderer(std::string const& name);

	public:
		 
		  static GeometryRendererPtr Create(std::string const& name);

          void SetGeometry(GeometryPtr const& geometry,SurfaceMaterialPtr const& mat);
		
		  virtual unsigned int GetPassNumber()
		  {
			  return _material->RenderPass.size();
		  }

		  virtual void PreDraw(CameraPtr const& camera);
		  virtual void Draw(int pass,CameraPtr const& camera);
		  virtual void PostDraw(CameraPtr const& camera);

	protected:
		ShaderPropertyManagerPtr _propertyManager;

		// light array
		ShaderPropertyPtr _LightNumberProperty;
		ShaderPropertyPtr _LightIntensityPackProperty;
		ShaderPropertyPtr _LightDirArrayProperty;
		ShaderPropertyPtr _LightColorArrayProperty;
 
		// for each light
		ShaderPropertyPtr _LightTypeProperty;
		ShaderPropertyPtr _LightIntensityProperty;
		ShaderPropertyPtr _LightPosProperty;
		ShaderPropertyPtr _LightColorProperty;
	};
}




#endif