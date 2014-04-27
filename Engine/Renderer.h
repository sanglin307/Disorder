#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_

namespace Disorder
{
	class Renderer : public Component
	{
	public:
	 
		Renderer(std::string const& name);
		virtual ~Renderer(){};

		virtual void PreRender(CameraPtr const& camera){};
		virtual void Render(CameraPtr const& camera) = 0;
		virtual void PostRender(CameraPtr const& camera){};

		virtual bool Overlaps(const Frustrum& frustrum)
		{
			return true;
		}
 
		virtual void AddLight(LightPtr & light);
		virtual void ClearLight();
		
		const std::vector<LightPtr>& GetNonDirectionLights() const
		{
			return _vNonDirectionLights;
		}

		const std::vector<DirectionLightPtr>& GetDirectionLights() const
		{
			return _vDirectionLights;
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
		std::vector<DirectionLightPtr> _vDirectionLights;
		std::vector<LightPtr> _vNonDirectionLights;
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
		  virtual void PreRender(CameraPtr const& camera);
		  virtual void Render(CameraPtr const& camera);
		  virtual void PostRender(CameraPtr const& camera);
		  virtual void DebugDraw();
	protected:
 
		// for each light
		ShaderPropertyPtr _LightTypeProperty;
		ShaderPropertyPtr _LightIntensityProperty;
		ShaderPropertyPtr _LightPosProperty;
		ShaderPropertyPtr _LightColorProperty;
	};
}




#endif