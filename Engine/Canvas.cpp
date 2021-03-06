#include "EngineInclude.h"

namespace Disorder
{
	Canvas* GCanvas = NULL;

	Canvas::Canvas(unsigned int width,unsigned int height)
		:_width(width), _height(height), _stringElement("CanvasString"), _lineElement("BatchLines"), _volumeLineElement("BatchVolumeLines")
	{
		_fontSize = 25;
		_font = GFontManager->CreateFontFromTrueTypeFile("arial",_fontSize,96);
		_stringElement.SetTexture(_font->GetGlyphTexture());
 
	}

	void Canvas::DrawLine(glm::vec3 const& beginPos, glm::vec4 const& beginColor, glm::vec3 const& endPos, glm::vec4 const& endColor, bool bVolumeLines)
	{
		if (!bVolumeLines)
		{
			BatchLineVertex* pBuffer = _lineElement.PrepareAddVertex();
			pBuffer[0].color = beginColor;
			pBuffer[0].position = beginPos;

			pBuffer[1].color = endColor;
			pBuffer[1].position = endPos;

			_lineElement.EndAddVertex();
		}
		else
		{
			BatchLineVertex* pBuffer = _volumeLineElement.PrepareAddVertex();
			pBuffer[0].color = beginColor;
			pBuffer[0].position = beginPos;

			pBuffer[1].color = endColor;
			pBuffer[1].position = endPos;

			_volumeLineElement.EndAddVertex();
		}
	}

	void Canvas::RenderLines(Camera* camera)
	{
		_volumeLineElement.Render(camera);
		_lineElement.Render(camera);
	}

	void Canvas::RenderStrings(Camera* camera)
	{
		_stringElement.Render(camera);
	}
 
	unsigned int Canvas::GetStringLength(std::string const& str, float scale)
	{
		BOOST_ASSERT(scale <= 1.0f);
		float length = 0.f;
		float charHeight = scale * _fontSize * 1.0f / _height;
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];
			if( c == 0x20) // Space
			{
				length += charHeight / 10;
				continue;
			}
			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			length += charHeight * rect.aspectRatio;
		}

		// map to [-1,1] of rasterize space , so we should divide 2
		return (unsigned int)(length / 2 * _width);
	}
 
	void Canvas::DrawString(unsigned int xPos, unsigned int yPos,std::string const& str, glm::vec4 const& color,float scale)
	{
		float posX = xPos * 1.0f / _width;
		float posY = yPos * 1.0f / _height;

		//draw string x[-1.0,1.0] and y[-1.0,1.0] z = 0.0
		float charHeight = scale * _fontSize * 1.0f / _height;
		float ratio = _width * 1.0f / _height;
		float xTemp = (posX - 0.5f)*2.0f;
		float yTemp = (posY - 0.5f)*(-2.0f);
		// we draw char one by one
		for(unsigned int index=0;index<str.length();index++)
		{
			UINT c = str[index];

			if( c == 0x20) // Space
			{
				xTemp += charHeight / 10;
				continue;
			}

			const Font::GlyphInfo& rect = _font->GetGlyphInfo(c);
			 
			// clockwise
			float drawSizeX = charHeight * rect.aspectRatio;
			float drawSizeY = charHeight * ratio;

			BatchTileVertex* pVertexBuffer = _stringElement.PrepareAddVertex();

			pVertexBuffer[0].position = glm::vec3(xTemp,yTemp,0.f);
			pVertexBuffer[0].color = color;
			pVertexBuffer[0].texcoord = glm::vec2(rect.uvRect.left, rect.uvRect.top);

			pVertexBuffer[1].position = glm::vec3(xTemp + drawSizeX, yTemp, 0.f);
			pVertexBuffer[1].color = color;
			pVertexBuffer[1].texcoord = glm::vec2(rect.uvRect.right, rect.uvRect.top);

			pVertexBuffer[2].position = glm::vec3(xTemp, yTemp - drawSizeY, 0.f);
			pVertexBuffer[2].color = color;
			pVertexBuffer[2].texcoord = glm::vec2(rect.uvRect.left, rect.uvRect.bottom);

			pVertexBuffer[3].position = glm::vec3(xTemp + drawSizeX, yTemp - drawSizeY, 0.f);
			pVertexBuffer[3].color = color;
			pVertexBuffer[3].texcoord = glm::vec2(rect.uvRect.right, rect.uvRect.bottom);

			_stringElement.EndAddVertex();

			xTemp += drawSizeX;

		}
	}

 
 
}