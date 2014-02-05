#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		float GetStringLength(float fsize,std::string const& str);
		void DrawString(float xPos,float yPos, float size, Eigen::Vector4f const& color,std::string const& str);	  // xPos,yPos in [ 0.f ,1.f]	 
		void DrawLine(Eigen::Vector3f const& beginPos,Eigen::Vector4f const& beginColor,Eigen::Vector3f const& endPos,Eigen::Vector4f const& endColor);

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