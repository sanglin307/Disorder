#ifndef _DISORDER_RENDERPATH_H_
#define _DISORDER_RENDERPATH_H_


namespace Disorder
{

	class RenderPath
	{
	public:
		static void Render(RenderPathType type);


	private:
		static void _ForwardLighting();

	};
}


#endif