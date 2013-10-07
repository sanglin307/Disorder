#ifndef _DISORDER_FONTMANAGER_H_
#define _DISORDER_FONTMANAGER_H_

 

namespace Disorder
{
	class FontManager : public Singleton<FontManager>
	{
		typedef boost::unordered_map<std::string,FontPtr> FontMap;

	public:
		FontManager();
		~FontManager();

		FontPtr CreateFontFromTrueTypeFile(std::string const& fontName,float fontSize,unsigned int fontResolution);

	protected:
		FontMap _fontMap;




	};

	#define GFontManager FontManager::GetSingleton()
}


#endif