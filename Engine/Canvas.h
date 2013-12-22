#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:
 
		int GetStringLength(int hsize,std::string const& str);
		float GetStringLengthNormalize(float hsize,std::string const& str);
		void DrawString(Vector2 pos, int size, Vector4 const& color,std::string const& str);
		void DrawStringNormalize(Vector2 pos,float size,Vector4 const& color,std::string const& str);
		
		void DrawStringDeviceSpace(Vector2 pos,int size,Vector4 const& color,std::string const& str); // ( -1,-1,-1) --- (1,1,1)

		void DrawLine(Vector3 const& beginPos,Vector4 const& beginColor,Vector3 const& endPos,Vector4 const& endColor);
		void DrawTile(Vector3 const& p00, Vector3 const& c00, Vector2 const& t00,Vector3 const& p01, Vector3 const& c01, Vector2 const& t01,
			          Vector3 const& p10, Vector3 const& c10, Vector2 const& t10,Vector3 const& p11, Vector3 const& c11, Vector2 const& t11,TexturePtr tex);

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