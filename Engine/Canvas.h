#ifndef _DISORDER_CANVAS_H_
#define _DISORDER_CANVAS_H_

namespace Disorder
{
 
	class Canvas
	{
	public:
       
		unsigned int GetStringLength(std::string const& str,float scale = 1.0f);
		void DrawString(unsigned int xPos, unsigned int yPos, std::string const& str, glm::vec4 const& color = glm::vec4(1.0f),float scale = 1.0f);	  // screen pos
		void DrawLine(glm::vec3 const& beginPos, glm::vec4 const& beginColor, glm::vec3 const& endPos, glm::vec4 const& endColor, bool bVolumeLines = true);

		void RenderLines(Camera* camera);
		void RenderStrings(Camera* camera);
 
		const unsigned int GetWidth() const
		{
			return _width;
		}

		const unsigned int GetHeight() const
		{
			return _height;
		}

		Canvas(unsigned int width, unsigned int height);
	protected:
 
		BatchScreenString _stringElement;
		
		BatchLines _lineElement;
		BatchVolumeLines _volumeLineElement;

		unsigned int _width;
		unsigned int _height;

		Font* _font;
		unsigned int _fontSize;
	};

	extern Canvas* GCanvas;
}

#endif