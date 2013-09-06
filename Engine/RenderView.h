#ifndef _DISORDER_RENDER_VIEW_
#define _DISORDER_RENDER_VIEW_


namespace Disorder
{
	class RenderView
	{
	public:
		virtual void* GetLowInterface(){ return 0;}

		void SetTexture(RenderTexture2DPtr texture)
		{
			_targetTexture = texture;
		}

		const RenderTexture2DPtr GetTexture() const
		{
			return _targetTexture;
		}

	protected:
		RenderTexture2DPtr _targetTexture;

	};
}


#endif