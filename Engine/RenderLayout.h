#ifndef _DISORDER_RENDERLAYOUT_H_
#define _DISORDER_RENDERLAYOUT_H_


namespace Disorder
{
 
	class RenderLayout
	{
	public:
 
        virtual void BindVertexBuffer(RenderBufferPtr const& renderBuffer)
        {
			_vertexBuffers.push_back(renderBuffer);
        }

         void BindIndexBuffer(RenderBufferPtr const& renderBuffer)
        {
             _indexBuffer = renderBuffer; 
        }

		std::vector<RenderBufferPtr> const& GetVertexBuffers()
		{
			return _vertexBuffers;
		}

		RenderBufferPtr const& GetIndexBuffer()
		{
			return _indexBuffer;
		}
 
		TopologyType GetTopology()
		{
			return _topologyType;
		}
 
		virtual void * GetHandle(){ return 0;}

		virtual void FinishBufferBinding(RenderEffectPtr const& renderEffect){};

	protected:
		RenderLayout(){}

		std::vector<RenderBufferPtr> _vertexBuffers;
        RenderBufferPtr _indexBuffer;
        TopologyType _topologyType;
	};
}


#endif