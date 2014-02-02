#include "EngineInclude.h"

namespace Disorder
{
	CanvasPtr Canvas::Create(unsigned int width,unsigned int height)
	{
		Canvas* pCanvas = new Canvas(width,height);
		return CanvasPtr(pCanvas);
	}

	Canvas::Canvas(unsigned int width,unsigned int height)
		:_width(width),_height(height),_stringElement("CanvasString"),_lineElement("BatchLines")
	{
		_font = GFontManager->CreateFontFromTrueTypeFile("calibri",20,96);
		_stringElement.SetTexture(_font->GetGlyphTexture());
 
	}

	void Canvas::DrawLine(Eigen::Vector3f const& beginPos,Eigen::Vector4f const& beginColor,Eigen::Vector3f const& endPos,Eigen::Vector4f const& endColor)
	{
		_lineElement.AddLine(beginPos,beginColor,endPos,endColor);
	}

	void Canvas::DrawTile(Eigen::Vector3f const& p00, Eigen::Vector3f const& c00, Eigen::Vector2f const& t00,Eigen::Vector3f const& p01, Eigen::Vector3f const& c01, Eigen::Vector2f const& t01,
			      Eigen::Vector3f const& p10, Eigen::Vector3f const& c10, Eigen::Vector2f const& t10,Eigen::Vector3f const& p11, Eigen::Vector3f const& c11, Eigen::Vector2f const& t11,TexturePtr tex)
	{
		if(_tileElementMap.find(tex) == _tileElementMap.end() )
		{
			BatchScreenTilesPtr tile = BatchScreenTiles::Create("CanvasTiles");

		}
	}

	void Canvas::Render(CameraPtr const& camera)
	{
		_lineElement.Render(camera);
		_stringElement.Render(camera);
		
	}

	unsigned int Canvas::GetCurrentDrawTriNumber()
	{
		return _stringElement.GetCurrentDrawTriNumber();
	}

	 

	float Canvas::GetStringLength(float fontSize,std::string const& str)
	{
		BOOST_ASSERT(fontSize<= 1.0f);
		float length = 0.f;
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];
			if( c == 0x20) // Space
			{
				length += fontSize/10;
				continue;
			}
			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			length += fontSize * rect.aspectRatio * 0.9f;
		}

		// map to [-1,1] of rasterize space , so we should divide 2
		return length/2;
	}
 
	void Canvas::DrawString(float xPos,float yPos, float size, Eigen::Vector4f const& color,std::string const& str)
	{
		BOOST_ASSERT(xPos <=1.f && yPos <= 1.0f && size <= 1.0f);
		//draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		
		Eigen::Vector2f pos = Eigen::Vector2f((xPos - 0.5f)*2.0f,(yPos - 0.5f)*(-2.0f));
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];

			if( c == 0x20) // Space
			{
				pos.x() += size/10;
				continue;
			}

			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			 
			// clockwise
			float drawSizeX = size * rect.aspectRatio * 0.9f ;
			_stringElement.AddVertex(Eigen::Vector3f(pos.x(),pos.y(),0.f),color,Eigen::Vector2f(rect.uvRect.left,rect.uvRect.top));
			_stringElement.AddVertex(Eigen::Vector3f(pos.x()+drawSizeX,pos.y(),0.f),color,Eigen::Vector2f(rect.uvRect.right,rect.uvRect.top));
			_stringElement.AddVertex(Eigen::Vector3f(pos.x(),pos.y()-size,0.f),color,Eigen::Vector2f(rect.uvRect.left,rect.uvRect.bottom));
			_stringElement.AddVertex(Eigen::Vector3f(pos.x()+drawSizeX,pos.y() - size,0.f),color,Eigen::Vector2f(rect.uvRect.right,rect.uvRect.bottom));

			pos.x() += drawSizeX;

		}
	}

 
 
}