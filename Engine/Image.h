#ifndef _DISORDER_IMAGE_H_
#define _DISORDER_IMAGE_H_

#include "png.h"
#include "zlib.h"

#include "jpeglib.h"

#include "RenderState.h"

namespace Disorder
{
	enum EImageType
	{
		eIT_PNG,
		eIT_JPG
	};

	struct ImageSpec
	{
		EImageType type;
		unsigned int width;      
		unsigned int height;    
		PixelFormat format;
		unsigned int dataSize;
	};

	class Image
	{
	public:
 
		const ImageSpec& GetSpec() const
		{
			return _imageSpec;
		}

		const std::string& GetName() const
		{
			return _name;
		}

		const BYTE* GetImageData() const
		{
			return _pPixelRawData;
		}

		void SetResource(RenderTexture2D* resource)
		{
			_resource = resource;
		}

		RenderTexture2D* GetResource() const
		{
			return _resource;
		}
 
		Image(const std::string& name,EImageType type,int width, int height, PixelFormat format, BYTE* pData, unsigned int dataSize);
	 
		~Image();

		static unsigned int GetPngFormat(PixelFormat format);
		static unsigned int GetJpgFormat(PixelFormat format);

	protected:
	
		ImageSpec _imageSpec;
		BYTE* _pPixelRawData; 
		RenderTexture2D* _resource;
		std::string _name;
	};

	class ImageManager  
	{
	public:
		Image* Load(std::string const& fileName, bool reloadIfExist = false);
		Image* Load(std::string const& fileName, SamplerDesc* desc, bool reloadIfExist = false);
		bool Save(std::string const& fileName,const Image* image);
		void Add(std::string const& imageName,Image* image);
		Image* Find(std::string const& imageName);

		bool IsSupported(std::string& suffix);
		~ImageManager();
	private:
		std::map<std::string,Image*> _mapImages;

		Image* LoadPng(std::string const& fileName);
		Image* LoadJpg(std::string const& fileName);

		bool SavePng(std::string const& fileName, const Image* image);
		bool SaveJpg(std::string const& fileName, const Image* image);

		void Convert_R8G8B8_R8G8B8A8(BYTE *pSrc,unsigned int srcLength, BYTE *pDst,unsigned int dstLength);

	};

	extern ImageManager* GImageManager;
}




#endif