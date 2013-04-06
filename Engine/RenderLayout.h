#ifndef _DISORDER_RENDERLAYOUT_H_
#define _DISORDER_RENDERLAYOUT_H_


namespace Disorder
{
	enum VertexInputSematic
	{
		VIS_Position,
		VIS_Color,
		VIS_Normal,
		VIS_TexCoord0
	};

	typedef struct _VertexInputDes
	{
		VertexInputSematic Semantic;
		PixelFormat Format;
		unsigned int InstanceDataStepRate;
        bool InstanceData;
	} VertexInputDes;

	class RenderLayout
	{
	public:

		virtual bool CreateLayout(ShaderObjectPtr const& vertexShader,TopologyType topologyType) = 0;
 
        virtual void BindVertexBuffer(RenderBufferPtr const& renderBuffer)
        {
			_vertexBuffers.push_back(renderBuffer);
        }

         void BindIndexBuffer(RenderBufferPtr const& renderBuffer)
        {
             _indexBuffer = renderBuffer; 
        }
 
		void BindRasterizeState(RasterizeStatePtr const& rasterizeState)
		{
			_rasterizeState = rasterizeState;
		}

		void BindBlendState(BlendStatePtr const& blendState)
		{
			_blendState = blendState;
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

		BlendStatePtr const& GetBlendState()
		{
			return _blendState;
		}

		RasterizeStatePtr const& GetRasterizeState()
		{
			return _rasterizeState;
		}

		virtual void * GetLowInterface(){ return 0;}

	protected:
		std::vector<RenderBufferPtr> _vertexBuffers;
        RenderBufferPtr _indexBuffer;
        TopologyType _topologyType;
		RasterizeStatePtr _rasterizeState;
		BlendStatePtr _blendState;
	   
	};
}


#endif