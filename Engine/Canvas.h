#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		float GetStringLength(float fsize,std::string const& str);
		void DrawString(float xPos,float yPos, float size, Vector4f const& color,std::string const& str);	  // xPos,yPos in [ 0.f ,1.f]	 
		void DrawLine(Vector3f const& beginPos,Vector4f const& beginColor,Vector3f const& endPos,Vector4f const& endColor);

		void Render(CameraPtr const& camera);

		unsigned int GetCurrentDrawTriNumber();

		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

		static CanvasPtr Create(unsigned int width,unsigned int height);
	protected:

		Canvas(unsigned int width,unsigned int height);

		BatchScreenString _stringElement;
		
		BatchLines _lineElement;

		unsigned int _width;
		unsigned int _height;

		FontPtr _font;

		 
	};
}

#endif