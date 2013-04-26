#ifndef _DISORDER_RENDERLAYOUT_H_
#define _DISORDER_RENDERLAYOUT_H_


namespace Disorder
{
 
	class RenderLayout
	{
	public:

		static std::string POSITION;
		static std::string COLOR;
		static std::string NORMAL;
		static std::string TEXCOORD;

		virtual bool CreateLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType,bool soloBuffer) = 0;
 
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
 
		virtual void * GetLowInterface(){ return 0;}

	protected:
		std::vector<RenderBufferPtr> _vertexBuffers;
        RenderBufferPtr _indexBuffer;
        TopologyType _topologyType;
	};
}


#endif