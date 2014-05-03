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

	void FontManager::LoadFontImageData()
	{
		std::string Path = GConfig->sResourceFontPath + "png\\";
		boost::filesystem::path p(Path);
		if (!boost::filesystem::exists(p) || !boost::filesystem::is_directory(p))
		{
			return;
		}

		boost::filesystem::directory_iterator enditer;
		for (boost::filesystem::directory_iterator fileiter(p); fileiter != enditer; ++fileiter)
		{
			ImagePtr image = GImageManager->Load(fileiter->path().string(), PF_R8G8_UNORM);
			if (image)
			{
				FontPtr font = Font::Create(25, 96);
				font->CreateRenderResource(image);
				std::string fileName = fileiter->path().filename().string();
				int pos = fileName.find('.');
				fileName = fileName.substr(0, pos);
				_fontMap.insert(std::pair<std::string, FontPtr>(fileName, font));
			}
		}
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