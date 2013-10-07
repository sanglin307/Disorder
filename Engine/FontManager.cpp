#include "EngineInclude.h"

namespace Disorder
{
	InitialiseSingleton(FontManager);

	FontManager::FontManager()
	{
		
	}

	FontManager::~FontManager()
	{
	
	}

	FontPtr FontManager::CreateFontFromTrueTypeFile(std::string const& fontName,float fontSize,unsigned int fontResolution)
	{
		if( _fontMap.find(fontName) != _fontMap.end() )
			return _fontMap.at(fontName);

		
		FontPtr font = boost::make_shared<Font>(fontSize,fontResolution);
		font->LoadFontFromTrueTypeFile(fontName);

		_fontMap.insert(std::pair<std::string,FontPtr>(fontName,font));

		return font;

	}
}