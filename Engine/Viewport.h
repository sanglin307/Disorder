#ifndef _DISORDER_VIEWPORT_H_
#define _DISORDER_VIEWPORT_H_


namespace Disorder
{
	 class Viewport
	 {
	 public:

		 int SizeX;
		 int SizeY;

	 protected:
		 bool _bActive;
		 RenderPath* _renderPath;

	 public:

		 Viewport(int sizeX,int sizeY);
		 ~Viewport();

		 virtual void Render();

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

		 void SetRenderPath(RenderPathType type);
		 void ToggleRenderPath();
		 RenderPath* GetRenderPath() const;
	 };

	
}


#endif