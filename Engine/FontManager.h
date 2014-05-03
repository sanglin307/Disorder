#ifndef _DISORDER_FONTMANAGER_H_
#define _DISORDER_FONTMANAGER_H_

 

namespace Disorder
{
	class FontManager : public Singleton<FontManager>
	{
		friend class Singleton<FontManager>;

		typedef boost::unordered_map<std::string,FontPtr> FontMap;

	public:
	
		~FontManager();

		FontPtr CreateFontFromTrueTypeFile(std::string const& fontName,unsigned int fontSize,unsigned int fontResolution);

		void LoadFontImageData();

	protected:
		static FontManagerPtr Create();

		FontManager();
		FontMap _fontMap;




	};

	#define GFontManager FontManager::GetSingleton()
}


#endif