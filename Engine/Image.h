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

		const BYTE* GetImageData() const
		{
			return _pPixelRawData;
		}

		void SetResource(RenderTexture2DPtr resource)
		{
			_resource = resource;
		}

		const RenderTexture2DPtr GetResource() const
		{
			return _resource;
		}
 
		static ImagePtr Create(EImageType type,int width, int height, PixelFormat format, BYTE* pData, unsigned int dataSize);
	 
		~Image();

		static unsigned int GetPngFormat(PixelFormat format);
		static unsigned int GetJpgFormat(PixelFormat format);

	protected:
		

		Image(ImageSpec const& spec, BYTE *pPixels, unsigned int size);
	
		ImageSpec _imageSpec;
		BYTE* _pPixelRawData; 
		RenderTexture2DPtr _resource;
	};


	class ImageManager : public Singleton<ImageManager>
	{
		friend class Singleton<ImageManager>;
	public:
		ImagePtr Load(std::string const& fileName, bool reloadIfExist = false);
		ImagePtr Load(std::string const& fileName, SamplerDesc* desc, bool reloadIfExist = false);
		bool Save(std::string const& fileName,ImagePtr const& image);
		void Add(std::string const& imageName,ImagePtr const& image);
		ImagePtr Find(std::string const& imageName);

		bool IsSupported(std::string& suffix);
		
	private:
		ImageManager(){};
		static ImageManagerPtr Create();
		std::map<std::string,ImagePtr> _mapImages;

		ImagePtr LoadPng(std::string const& fileName);
		ImagePtr LoadJpg(std::string const& fileName);

		bool SavePng(std::string const& fileName, ImagePtr const& image);
		bool SaveJpg(std::string const& fileName, ImagePtr const& image);

		void Convert_R8G8B8_R8G8B8A8(BYTE *pSrc,unsigned int srcLength, BYTE *pDst,unsigned int dstLength);

	};

	#define GImageManager ImageManager::GetSingleton()
}




#endif