#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_

namespace Disorder
{
	class Renderer : public Component
	{
	public:
	 
		Renderer(std::string const& name);
		virtual ~Renderer(){};

		virtual void Render(CameraPtr const& camera) = 0;

		virtual bool Overlaps(const Frustrum& frustrum)
		{
			return true;
		}
 
	 

		const RenderLayoutPtr& GetRenderLayout() const
		{
			return _renderLayout;
		}

		const RenderEffectPtr& GetRenderEffect() const
		{
			return _renderEffect;
		}

		virtual void BuildRenderLayout(RenderEffectPtr const& effect,bool releaseOld){};
		 

		void SetRenderEffect(RenderEffectPtr const& effect)
		{
			_renderEffect = effect;
		}
 
		virtual void DebugDraw(){};

	protected:
		RenderEffectPtr _renderEffect;
		RenderLayoutPtr _renderLayout;
	};
 
	class BatchScreenString : public Renderer
	{
	   
	public:
		BatchScreenString(std::string const& name);
		~BatchScreenString();

		void SetTexture(SurfaceViewPtr const& texture);
		
		BatchTileVertex* PrepareAddVertex();
		void EndAddVertex();

		virtual void Render(CameraPtr const& camera);
  
	protected:
		BatchTileVertex* _vertexs;
	    unsigned int _vertexNum;

		unsigned short* _indexs;
	    unsigned int _indexNum;

		unsigned int _savedVertexBufferSize;
		unsigned int _savedIndexBufferSize;
		SurfaceViewPtr _texture;
	};

	 
	class BatchLines : public Renderer
	{
	public:
		BatchLines(std::string const& name);
		virtual void Render(CameraPtr const& camera);
		~BatchLines();

		BatchLineVertex* PrepareAddVertex();
		void EndAddVertex();
	 
	private:
		BatchLineVertex* _vertexs;
		unsigned int _vertexNum;
		unsigned int _savedVertexBufferSize;
	};

	class SimpleTile : public Renderer
	{
	public:
		SimpleTile();
		SimpleTile(std::string const& name,const TileTexVertex* positions,const RenderEffectPtr& renderEffect);
		 
		virtual void Render(CameraPtr const& camera);
	};

	class GeometryRenderer : public Renderer
	{
	private:
		  GeometryPtr _geometryObject;
	      SurfaceMaterialPtr _material;
		 
	private:

		  void DrawAxis(CameraPtr const& camera);
		  void DrawBoundingBox(CameraPtr const& camera);

		  GeometryRenderer(std::string const& name);

	public:
		 
		  static GeometryRendererPtr Create(std::string const& name);

          void SetGeometry(GeometryPtr const& geometry,SurfaceMaterialPtr const& mat);
		  virtual void BuildRenderLayout(RenderEffectPtr const& effect,bool releaseOld);

		  virtual bool Overlaps(const Frustrum& frustrum);		 
		  void UpdateShaderProperty();
		  virtual void Render(CameraPtr const& camera);
		  
		  virtual void DebugDraw();

		  const GeometryPtr GetGeometry() const
		  {
			  return _geometryObject;
		  }

	protected:
 
		// for each light
		ShaderPropertyPtr _LightTypeProperty;
		ShaderPropertyPtr _LightIntensityProperty;
		ShaderPropertyPtr _LightPosProperty;
		ShaderPropertyPtr _LightColorProperty;
	};
}




#endif