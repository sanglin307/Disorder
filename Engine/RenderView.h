#ifndef _DISORDER_RENDER_VIEW_
#define _DISORDER_RENDER_VIEW_


namespace Disorder
{
	class RenderView
	{
	public:
		virtual void* GetLowInterface(){ return 0;}
		virtual void Bind() {}
	protected:
		unsigned int _width;
		unsigned int _height;

	};
}


#endif