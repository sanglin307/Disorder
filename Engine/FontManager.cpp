#include "EngineInclude.h"

namespace Disorder
{
	 
	FontManager *GFontManager = NULL;

	FontManager::FontManager()
	{
		
	}

	FontManager::~FontManager()
	{
	
	}

	void FontManager::LoadFontImageData()
	{
		std::string Path = GConfig->ResourceFontPath + "png\\";
		boost::filesystem::path p(Path);
		if (!boost::filesystem::exists(p) || !boost::filesystem::is_directory(p))
		{
			return;
		}

		boost::filesystem::directory_iterator enditer;
		for (boost::filesystem::directory_iterator fileiter(p); fileiter != enditer; ++fileiter)
		{
			Image *image = GImageManager->Load(fileiter->path().string());
			if (image)
			{
				Font* font = new Font(25, 96);
				font->CreateRenderResource(image);
				std::string fileName = fileiter->path().filename().string();
				int pos = fileName.find('.');
				fileName = fileName.substr(0, pos);
				_fontMap.insert(std::pair<std::string, Font*>(fileName, font));
			}
		}
	}
 
	Font* FontManager::CreateFontFromTrueTypeFile(std::string const& fontName,unsigned int fontSize,unsigned int fontResolution)
	{
		if( _fontMap.find(fontName) != _fontMap.end() )
			return _fontMap.at(fontName);
 
		Font* font = new Font(fontSize,fontResolution);
		font->LoadFontFromTrueTypeFile(fontName);

		_fontMap.insert(std::pair<std::string,Font*>(fontName,font));

		return font;

	}
}