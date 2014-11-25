#ifndef _DISORDER_RENDERER_H_
#define _DISORDER_RENDERER_H_

 

namespace Disorder
{
	class Renderer : public Component
	{
	public:
		Renderer(std::string const& name);
		virtual ~Renderer(){};

		virtual void Render(Camera* camera) = 0;

		virtual bool Overlaps(const Frustrum& frustrum)
		{
			return true;
		}
 
		const RenderLayout* GetRenderLayout() const
		{
			return _renderLayout;
		}

		const RenderEffect* GetRenderEffect() const
		{
			return _renderEffect;
		}

		virtual void BuildRenderLayout(RenderEffect* effect,bool releaseOld){};
		 

		void SetRenderEffect(RenderEffect* effect)
		{
			_renderEffect = effect;
		}
 
		virtual void DebugDraw(){};

	protected:
		RenderEffect* _renderEffect;
		RenderLayout* _renderLayout;
	};
 
	class BatchScreenString : public Renderer
	{
	   
	public:
		BatchScreenString(std::string const& name);
		~BatchScreenString();

		void SetTexture(SurfaceView* texture);
		
		BatchTileVertex* PrepareAddVertex();
		void EndAddVertex();

		virtual void Render(Camera* camera);
  
	protected:
		BatchTileVertex* _vertexs;
	    unsigned int _vertexNum;

		unsigned short* _indexs;
	    unsigned int _indexNum;

		unsigned int _savedVertexBufferSize;
		unsigned int _savedIndexBufferSize;
		SurfaceView* _texture;
	};

	 
	class BatchLines : public Renderer
	{
	public:
		BatchLines(std::string const& name);
		virtual void Render(Camera* camera);
		~BatchLines();

		BatchLineVertex* PrepareAddVertex();
		void EndAddVertex();
	 
	private:
		BatchLineVertex* _vertexs;
		unsigned int _vertexNum;
		unsigned int _savedVertexBufferSize;
	};

	class BatchVolumeLines : public Renderer
	{
	public:
		BatchVolumeLines(std::string const& name);
		virtual void Render(Camera* camera);
		~BatchVolumeLines();
 
		BatchLineVertex* PrepareAddVertex();
		void EndAddVertex();

	private:

		void BuildBuffer(int maxLineNumber);

		BatchLineVertex* _vertexs;
		unsigned int _vertexNum;
		unsigned int _savedVertexBufferSize;

		float *_positions;
		float *_otherPositions;
		float *_colors;

		ShaderProperty* _sLineTextureProperty;
		ShaderProperty* _sLineSamplerProperty;
	};

	class SimpleTile : public Renderer
	{
	public:
		SimpleTile();
		SimpleTile(std::string const& name,const TileTexVertex* positions,RenderEffect* renderEffect);
		 
		virtual void Render(Camera* camera);
	};

	class GeometryRenderer : public Renderer
	{
	private:
		  Geometry* _geometryObject;
	      SurfaceMaterial* _material;
		  RenderLayout* _shadowLayout;
		  BoxBounds    _boxBounds;
	private:

		  void DrawAxis(Camera* camera);
		  void DrawBoundingBox(Camera* camera);

		  

	public:
		GeometryRenderer(std::string const& name);
		const BoxBounds& GetBoxBounds() const
		{
			return _boxBounds;
		}

        void SetGeometry(Geometry* geometry,SurfaceMaterial* mat);
		virtual void BuildRenderLayout(RenderEffect* effect,bool releaseOld);

		void UpdateBoundingBox();

		virtual bool Overlaps(const Frustrum& frustrum);		 
		void UpdateShaderProperty();
		virtual void Render(Camera* camera);
		virtual void RenderShadow(Camera* camera, RenderEffect* shadowEffect);

		virtual void DebugDraw();

		const Geometry* GetGeometry() const
		{
			return _geometryObject;
		}
 
	};
}




#endif