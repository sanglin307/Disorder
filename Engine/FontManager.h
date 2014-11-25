#ifndef _DISORDER_FONTMANAGER_H_
#define _DISORDER_FONTMANAGER_H_
 
namespace Disorder
{
	class FontManager
	{
		typedef boost::unordered_map<std::string,Font*> FontMap;

	public:
		FontManager();
		~FontManager();

		Font* CreateFontFromTrueTypeFile(std::string const& fontName,unsigned int fontSize,unsigned int fontResolution);

		void LoadFontImageData();

	protected:
		
		FontMap _fontMap;
 
	};

	extern FontManager* GFontManager;
}


#endif