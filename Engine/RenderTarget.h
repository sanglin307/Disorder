#ifndef _DISORDER_RENDERTARGET_H_
#define _DISORDER_RENDERTARGET_H_

namespace Disorder
{
	class RenderTarget
	{
	public:
		inline void SetRenderTargetView(RenderViewPtr const& renderTargetView)
		{
			_renderTargets.push_back(renderTargetView);
		}

		RenderViewPtr const& GetRenderTargetView(unsigned int index)
		{
			BOOST_ASSERT(index<_renderTargets.size());
			return _renderTargets[index];
		}

		inline void SetDepthStencilView(RenderViewPtr const& depthStencilView)
		{
			_depthStencilView = depthStencilView;
		}

		RenderViewPtr const& GetDepthStencilView()
		{
			return _depthStencilView;
		}
 
	protected:
		std::vector<RenderViewPtr> _renderTargets;
		RenderViewPtr _depthStencilView;
	};
}


#endif