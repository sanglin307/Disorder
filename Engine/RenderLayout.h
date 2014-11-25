#ifndef _DISORDER_RENDERLAYOUT_H_
#define _DISORDER_RENDERLAYOUT_H_


namespace Disorder
{
 
	class RenderLayout
	{
	public:
 
        virtual void BindVertexBuffer(RenderBuffer* renderBuffer)
        {
			_vertexBuffers.push_back(renderBuffer);
        }

		void UnBindVertexBuffer()
		{
			_vertexBuffers.clear();
		}

         void BindIndexBuffer(RenderBuffer* renderBuffer)
        {
             _indexBuffer = renderBuffer; 
        }

		std::vector<RenderBuffer*> const& GetVertexBuffers() const
		{
			return _vertexBuffers;
		}

		RenderBuffer* GetIndexBuffer() const
		{
			return _indexBuffer;
		}
 
		TopologyType GetTopology()
		{
			return _topologyType;
		}
 
		virtual void * GetHandle(){ return 0;}

		virtual void FinishBufferBinding(RenderEffect* renderEffect){};

	protected:
		RenderLayout(){}

		std::vector<RenderBuffer*> _vertexBuffers;
        RenderBuffer* _indexBuffer;
        TopologyType _topologyType;
	};
}


#endif