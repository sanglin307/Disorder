#include "EngineInclude.h"

namespace Disorder
{
 
	Canvas::Canvas(unsigned int width,unsigned int height)
		:_width(width),_height(height),_stringElement("CanvasString"),_tileElement("CanvasTile"),_lineElement("BatchLines")
	{
		_font = GFontManager->CreateFontFromTrueTypeFile("calibri",20,96);
		_stringElement.SetTexture(_font->GetGlyphTexture());
 
	}

	void Canvas::DrawLine(Vector3 const& beginPos,Vector4 const& beginColor,Vector3 const& endPos,Vector4 const& endColor)
	{
		_lineElement.AddLine(beginPos,beginColor,endPos,endColor);
	}

	void Canvas::Draw(CameraPtr const& camera)
	{
		_lineElement.Draw(RPT_ForwardLighting,0,camera);
		_stringElement.Draw(RPT_ForwardLighting,0,camera);
		
	}

	unsigned int Canvas::GetCurrentDrawTriNumber()
	{
		return _stringElement.GetCurrentDrawTriNumber();
	}

	int Canvas::GetStringLength(int hsize,std::string const& str)
	{
		return (int)(GetStringLengthNormalize(hsize*1.0f/_height,str)*_width);
	}

	float Canvas::GetStringLengthNormalize(float hsize,std::string const& str)
	{
		float length = 0.f;
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];
			if( c == 0x20) // Space
			{
				length += hsize/10;
				continue;
			}
			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			length += hsize * rect.aspectRatio;
		}

		// map to [-1,1] of rasterize space , so we should divide 2
		return length/2;
	}

	void Canvas::DrawStringNormalize(Vector3 pos,float hsize,Vector4 const& color,std::string const& str)
	{
		DrawString( Vector3(pos.x * _width,pos.y *_height,pos.z),(int)(hsize*_height),color,str);
	}

	void Canvas::DrawStringDeviceSpace(Vector3  pos,int size,Vector4 const& color,std::string const& str)
	{
		//draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		float charSize = 1.0f * size / _height;

		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];

			if( c == 0x20) // Space
			{
				pos.x += charSize/10;
				continue;
			}

			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			 
			// clockwise
			float drawSizeX = charSize * rect.aspectRatio ;
			_stringElement.AddVertex(pos,color,Vector2(rect.uvRect.left,rect.uvRect.top));
			_stringElement.AddVertex(Vector3(pos.x+drawSizeX,pos.y,pos.z),color,Vector2(rect.uvRect.right,rect.uvRect.top));
			_stringElement.AddVertex(Vector3(pos.x,pos.y-charSize,pos.z),color,Vector2(rect.uvRect.left,rect.uvRect.bottom));
			_stringElement.AddVertex(Vector3(pos.x+drawSizeX,pos.y - charSize,pos.z),color,Vector2(rect.uvRect.right,rect.uvRect.bottom));

			pos.x += drawSizeX;

		}
	}

	void Canvas::DrawString(Vector3 pos,int hsize,Vector4 const& color,std::string const& str)
	{
		 //draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		pos.x = (pos.x - _width / 2.0f )*2.0f/_width;
		pos.y = (pos.y - _height / 2.0f )*(-2.0f)/_height;
		DrawStringDeviceSpace(pos,hsize,color,str);
	}
 
}