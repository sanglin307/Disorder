#include "EngineInclude.h"

namespace Disorder
{
 
	Font::Font(unsigned int fontWidSize,unsigned int fontRevolution)
		:_fontSize(fontWidSize), _fontRevolution(fontRevolution), _fontMaxBearingY(0)
		,_characterSpacer(5),_antialiasColor(false)
	{
		_codePointRangeList.push_back(CodePointRange(33,166));

	}
	
	void Font::SaveFontLayout(const std::string& fontName)
	{
		//unsigned int _fontSize; // in pixel.
		//unsigned int _fontRevolution;
		//int   _fontMaxBearingY;
		//UINT _characterSpacer;
		//CodePointMap _codePointMap;



	}

	void Font::LoadFontLayout(const std::string& fontName)
	{

	}

	void Font::CreateRenderResource(Image* image)
	{
		SamplerDesc desc;
		desc.Filter = SF_Min_Mag_Linear_Mip_Point;
		SamplerState* sampler = GEngine->RenderResourceMgr->CreateSamplerState(&desc);
		RenderTexture2D* fontTexture = GEngine->RenderResourceMgr->CreateTexture2D(sampler, image->GetSpec().format, false, image);
		_glyphsTexture = GEngine->RenderResourceMgr->CreateSurfaceView(SV_ShaderResource, fontTexture, image->GetSpec().format);
	}

	void Font::LoadFontFromTrueTypeFile(std::string const& fontName)
	{
		FT_Library  ft_library;
		FT_Error error = FT_Init_FreeType( &ft_library );
		BOOST_ASSERT(!error);

		std::string strFilePath = GConfig->ResourceFontPath + "ttf\\" + fontName + ".ttf";
		FT_Face face;
	    error = FT_New_Face( ft_library,strFilePath.c_str(),0,&face );
		BOOST_ASSERT(!error);
		
	    // Convert our point size to freetype 26.6 fixed point format
        FT_F26Dot6 ftSize = (FT_F26Dot6)(_fontSize * (1 << 6));
        FT_Set_Char_Size( face, ftSize, 0, _fontRevolution, _fontRevolution );
 
        int max_height = 0, max_width = 0;
 
		// Calculate maximum width, height and bearing
		size_t glyphCount = 0;
		for (std::vector<CodePointRange>::const_iterator r = _codePointRangeList.begin();
			r != _codePointRangeList.end(); ++r)
		{
			const CodePointRange& range = *r;
			for(UINT cp = range.first; cp <= range.second; ++cp, ++glyphCount)
			{
				FT_Load_Char( face, cp, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP );

				if( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
					max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );
				if( face->glyph->metrics.horiBearingY > _fontMaxBearingY )
					_fontMaxBearingY = face->glyph->metrics.horiBearingY;

				if( (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 ) > max_width)
					max_width = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );
			}

		}

		// Now work out how big our texture needs to be
		size_t rawSize = (max_width + _characterSpacer) *
							((max_height >> 6) + _characterSpacer) * glyphCount;

		UINT tex_side = static_cast<UINT>(Math::Sqrtf((float)rawSize));
		// just in case the size might chop a glyph in half, add another glyph width/height
		tex_side += std::max(max_width, (max_height>>6));
		// Now round up to nearest power of two
		UINT roundUpSize = Math::FirstPOW2From(tex_side);

		// Would we benefit from using a non-square texture (2X width(
		size_t finalWidth, finalHeight;
		if (roundUpSize*roundUpSize*0.5 >= rawSize)
		{
			finalHeight = static_cast<size_t>(roundUpSize * 0.5);
		}
		else
		{
			finalHeight = roundUpSize;
		}
		finalWidth = roundUpSize;

		float textureAspect = (float)finalWidth / (float)finalHeight;

		const size_t pixel_bytes = 2; //LA
		size_t data_width = finalWidth * pixel_bytes;
		size_t data_size = finalWidth * finalHeight * pixel_bytes;

        unsigned char* imageData = new unsigned char[data_size];
        for (size_t i = 0; i < data_size; i += pixel_bytes)
        {
            imageData[i + 0] = 0x00; // L
			imageData[i + 1] = 0x00; // A
		 
        }

        size_t l = 0, m = 0;
		for (std::vector<CodePointRange>::const_iterator r = _codePointRangeList.begin();
			r != _codePointRangeList.end(); ++r)
		{
			const CodePointRange& range = *r;
			for(UINT cp = range.first; cp <= range.second; ++cp )
			{
				FT_Error ftResult;

				// Load & render glyph
				ftResult = FT_Load_Char( face, cp, FT_LOAD_RENDER | FT_LOAD_NO_BITMAP );
				if (ftResult)
				{
					continue;
				}

				FT_Int advance = face->glyph->advance.x >> 6;

				unsigned char* buffer = face->glyph->bitmap.buffer;

				if (!buffer)
				{
					// Yuck, FT didn't detect this but generated a null pointer!
					continue;
				}

				int y_bearing = ( _fontMaxBearingY >> 6 ) - ( face->glyph->metrics.horiBearingY >> 6 );
				int x_bearing = face->glyph->metrics.horiBearingX >> 6;

				for(int j = 0; j < face->glyph->bitmap.rows; j++ )
				{
					size_t row = j + m + y_bearing;
					unsigned char* pDest = &imageData[(row * data_width) + (l + x_bearing) * pixel_bytes];
					for(int k = 0; k < face->glyph->bitmap.width; k++ )
					{
						if (_antialiasColor)
						{
							// Use the same greyscale pixel for all components RGBA
							*pDest++= *buffer;
					 
						}
						else
						{
							// Always white whether 'on' or 'off' pixel, since alpha
							// will turn off
							*pDest++= 0xFF;
						 
						}
						// Always use the greyscale value for alpha
						*pDest++= *buffer++; 
					}
				}

				SetGlyphTexCoords(cp,
					(float)l / (float)finalWidth,  // u1
					(float)m / (float)finalHeight,  // v1
					(float)( l + ( face->glyph->advance.x >> 6 ) ) / (float)finalWidth, // u2
					( m + ( max_height >> 6 ) ) / (float)finalHeight, // v2
					textureAspect
					);

				// Advance a column
				l += (advance + _characterSpacer);

				// If at end of row
				if( finalWidth - 1 < l + ( advance ) )
				{
					m += ( max_height >> 6 ) + _characterSpacer;
					l = 0;
				}
			}
		}

		if( ft_library )
			FT_Done_FreeType(ft_library);

		// save it to image 
		PixelFormat pixelFormat = PF_R8G8_UNORM;
		Image* fontImage = Image::Create(eIT_PNG, finalWidth, finalHeight, pixelFormat, imageData, data_size);
		GImageManager->Add(fontName,fontImage);
		std::string strImageFileName = GConfig->ResourceFontPath + "png\\" + fontName + ".png";
		GImageManager->Save(strImageFileName,fontImage);

		delete imageData;

		CreateRenderResource(fontImage);
	}


}