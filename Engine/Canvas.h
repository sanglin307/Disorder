#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
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
		void SetTexture(RenderTexture2DPtr const& texture);
		 
		void AddVertex(Vector3 const& position,Vector4 const& color,Vector2 const& texcoord);
		virtual void Draw(MaterialViewType view);

		unsigned int GetCurrentDrawTriNumber();

	protected:
		std::vector<CanvasVertex> _vertexs;
		std::vector<WORD> _indexs;
		unsigned int _savedVertexBufferSize;
		unsigned int _savedIndexBufferSize;
		RenderTexture2DPtr _texture;
	};

	class Canvas
	{
	public:

		Canvas(unsigned int width,unsigned int height);

		void DrawString(float xPos,float yPos, float size, Vector4 const& color,std::string const& str);

		void Draw();

		unsigned int GetCurrentDrawTriNumber();

	protected:
		CanvasBatchElement _stringElement;

		unsigned int _width;
		unsigned int _height;

		FontPtr _font;
	};
}

#endif