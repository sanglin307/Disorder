#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:

		Canvas(unsigned int width,unsigned int height);

		int GetStringLength(int hsize,std::string const& str);
		float GetStringLengthNormalize(float hsize,std::string const& str);
		void DrawString(Vector3 pos, int size, Vector4 const& color,std::string const& str);
		void DrawStringNormalize(Vector3 pos,float size,Vector4 const& color,std::string const& str);
		
		void DrawStringDeviceSpace(Vector3 pos,int size,Vector4 const& color,std::string const& str); // ( -1,-1,-1) --- (1,1,1)

		void DrawLine(Vector3 const& beginPos,Vector4 const& beginColor,Vector3 const& endPos,Vector4 const& endColor);
	 
		void Draw(CameraPtr const& camera);

		unsigned int GetCurrentDrawTriNumber();

		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

	protected:
		BatchScreenTiles _stringElement;
		BatchScreenTiles _tileElement;

		BatchLines _lineElement;

		unsigned int _width;
		unsigned int _height;

		FontPtr _font;

		 
	};
}

#endif