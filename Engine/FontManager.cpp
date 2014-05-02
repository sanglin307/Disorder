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

	FontManagerPtr FontManager::Create()
	{
		FontManager *pManager = new FontManager;
		return FontManagerPtr(pManager);
	}

	FontPtr FontManager::CreateFontFromTrueTypeFile(std::string const& fontName,unsigned int fontSize,unsigned int fontResolution)
	{
		if( _fontMap.find(fontName) != _fontMap.end() )
			return _fontMap.at(fontName);
 
		FontPtr font = Font::Create(fontSize,fontResolution);
		font->LoadFontFromTrueTypeFile(fontName);

		_fontMap.insert(std::pair<std::string,FontPtr>(fontName,font));

		return font;

	}
}