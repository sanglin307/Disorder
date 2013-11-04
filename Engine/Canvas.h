#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
	//Batch element foreach texture.
	class CanvasBatchElement : public Renderer
	{
	    typedef struct _CanvasVertex
		{
			Vector3 position;
			Vector4 color;
			Vector2 texcoord;
		} CanvasVertex;
	public:
		CanvasBatchElement();
		void SetTexture(RenderSurfacePtr const& texture);
		 
		void AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord);
		virtual void Draw(RenderPathType pathType,int pass,CameraPtr const& camera);

		unsigned int GetCurrentDrawTriNumber();

	protected:
		std::vector<CanvasVertex> _vertexs;
		std::vector<WORD> _indexs;
		unsigned int _savedVertexBufferSize;
		unsigned int _savedIndexBufferSize;
		RenderSurfacePtr _texture;
	};

	class Canvas
	{
	public:

		Canvas(unsigned int width,unsigned int height);

		int GetStringLength(int hsize,std::string const& str);
		float GetStringLengthNormalize(float hsize,std::string const& str);
		void DrawString(int xPos,int yPos, int size, Vector4 const& color,std::string const& str);
		void DrawStringNormalize(float xPos,float yPos,float size,Vector4 const& color,std::string const& str);

		void Draw(CameraPtr const& camera);

		unsigned int GetCurrentDrawTriNumber();

		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

	protected:
		CanvasBatchElement _stringElement;
		CanvasBatchElement _tileElement;

		unsigned int _width;
		unsigned int _height;

		FontPtr _font;
	};
}

#endif