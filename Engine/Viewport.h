#ifndef _DISORDER_VIEWPORT_H_
#define _DISORDER_VIEWPORT_H_


namespace Disorder
{
	 class Viewport
	 {
	 public:
		 int X;
		 int Y;
		 int SizeX;
		 int SizeY;

	 protected:
		 bool _bActive;
		 RenderPathType _renderPath;

	 public:

		 Viewport(int x,int y,int sizeX,int sizeY);
		 
		 virtual void Draw();

		 virtual inline void Active(bool bActive)
		 {
			 _bActive = bActive;
		 }

		 virtual inline bool IsActive()
		 {
			 return _bActive;
		 }

		 virtual void* GetWindow()
		 {
			 return 0;
		 }

		 void SetRenderPathType(RenderPathType type)
		 {
			 _renderPath = type;
		 }

		 RenderPathType GetRenderPathType()
		 {
			 return _renderPath;
		 }
		 

	 };

	
}


#endif