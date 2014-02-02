#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:
 
		float GetStringLength(float fsize,std::string const& str);
		void DrawString(float xPos,float yPos, float size, Eigen::Vector4f const& color,std::string const& str);	  // xPos,yPos in [ 0.f ,1.f]	 
		void DrawLine(Eigen::Vector3f const& beginPos,Eigen::Vector4f const& beginColor,Eigen::Vector3f const& endPos,Eigen::Vector4f const& endColor);
		void DrawTile(Eigen::Vector3f const& p00, Eigen::Vector3f const& c00, Eigen::Vector2f const& t00,Eigen::Vector3f const& p01, Eigen::Vector3f const& c01, Eigen::Vector2f const& t01,
			          Eigen::Vector3f const& p10, Eigen::Vector3f const& c10, Eigen::Vector2f const& t10,Eigen::Vector3f const& p11, Eigen::Vector3f const& c11, Eigen::Vector2f const& t11,TexturePtr tex);

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
		std::map<TexturePtr,BatchScreenTilesPtr> _tileElementMap;

		BatchLines _lineElement;

		unsigned int _width;
		unsigned int _height;

		FontPtr _font;

		 
	};
}

#endif