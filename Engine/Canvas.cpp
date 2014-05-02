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

	void Canvas::DrawLine(glm::vec3 const& beginPos, glm::vec4 const& beginColor, glm::vec3 const& endPos, glm::vec4 const& endColor)
	{
		BatchLineVertex* pBuffer = _lineElement.PrepareAddVertex();
		pBuffer[0].color = beginColor;
		pBuffer[0].position = beginPos;

		pBuffer[1].color = endColor;
		pBuffer[1].position = endPos;

		_lineElement.EndAddVertex();
	}


	void Canvas::Render(CameraPtr const& camera)
	{
		_lineElement.Render(camera);
		_stringElement.Render(camera);
		
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
 
	void Canvas::DrawString(float xPos,float yPos, float size,glm::vec4 const& color,std::string const& str)
	{
		BOOST_ASSERT(xPos <=1.f && yPos <= 1.0f && size <= 1.0f);
		//draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		
		float xTemp = (xPos - 0.5f)*2.0f;
		float yTemp = (yPos - 0.5f)*(-2.0f);
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];

			if( c == 0x20) // Space
			{
				xTemp += size/10;
				continue;
			}

			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			 
			// clockwise
			float drawSizeX = size * rect.aspectRatio * 0.9f ;

			BatchTileVertex* pVertexBuffer = _stringElement.PrepareAddVertex();

			pVertexBuffer[0].position = glm::vec3(xTemp,yTemp,0.f);
			pVertexBuffer[0].color = color;
			pVertexBuffer[0].texcoord = glm::vec2(rect.uvRect.left, rect.uvRect.top);

			pVertexBuffer[1].position = glm::vec3(xTemp + drawSizeX, yTemp, 0.f);
			pVertexBuffer[1].color = color;
			pVertexBuffer[1].texcoord = glm::vec2(rect.uvRect.right, rect.uvRect.top);

			pVertexBuffer[2].position = glm::vec3(xTemp, yTemp - size, 0.f);
			pVertexBuffer[2].color = color;
			pVertexBuffer[2].texcoord = glm::vec2(rect.uvRect.left, rect.uvRect.bottom);

			pVertexBuffer[3].position = glm::vec3(xTemp + drawSizeX, yTemp - size, 0.f);
			pVertexBuffer[3].color = color;
			pVertexBuffer[3].texcoord = glm::vec2(rect.uvRect.right, rect.uvRect.bottom);

			_stringElement.EndAddVertex();

			xTemp += drawSizeX;

		}
	}

 
 
}