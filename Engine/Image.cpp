#include "EngineInclude.h"
 

namespace Disorder
{
	InitialiseSingleton(ImageManager);

	ImagePtr ImageManager::Load(std::string const& fileName,PixelFormat pixelFormat,bool reloadIfExist)
	{
		 if( !reloadIfExist )
		 {
			 if( _mapImages.find(fileName) != _mapImages.end() )
				 return _mapImages[fileName];
		 }

		 // only support png now.
		 png_image image; 

		 /* Initialize the 'png_image' structure. */
		 memset(&image, 0, (sizeof image));
		 image.version = PNG_IMAGE_VERSION;

		 /* The first argument is the file to read: */
		 if (png_image_begin_read_from_file(&image, fileName.c_str()))
		 {
			 image.format = Image::GetPngFormat(pixelFormat);
			 int size = PNG_IMAGE_SIZE(image);
			 png_bytep buffer = (png_bytep)malloc(size);

			 if (buffer != NULL && png_image_finish_read(&image, NULL, buffer, 0, NULL))
			 {
				 ImagePtr imagePtr = Image::Create(eIT_PNG, image.width, image.height, pixelFormat, buffer, size);
				 _mapImages[fileName] = imagePtr;
				 free(buffer);
				 return imagePtr;
			 }
			 else
			 {
				 if (buffer == NULL)
					 png_image_free(&image);

				 else
					 free(buffer);

				 return NULL;
			 }
		 }
	 
		 return NULL;
	}

	void ImageManager::Add(std::string const& imageName,ImagePtr const& image)
	{
		_mapImages[imageName] = image;
	}

	ImageManagerPtr ImageManager::Create()
	{
		ImageManager *pManager = new ImageManager;
		return ImageManagerPtr(pManager);
	}

	ImagePtr ImageManager::Find(std::string const& imageName)
	{
		std::map<std::string,ImagePtr>::iterator iter = _mapImages.find(imageName);
		if( iter != _mapImages.end() )
			return iter->second;

		return NULL;
	}

	bool ImageManager::Save(std::string const& fileName,ImagePtr const& image)
	{
		if( image == NULL )
			return false;

		const ImageSpec& spec = image->GetSpec();
		png_image pngSpec;
		memset(&pngSpec, 0, (sizeof pngSpec));
		pngSpec.version = PNG_IMAGE_VERSION;
		pngSpec.format = Image::GetPngFormat(spec.format);
		pngSpec.height = spec.height;
		pngSpec.width = spec.width;

		return png_image_write_to_file(&pngSpec, fileName.c_str(), 0, image->GetImageData(), 0, NULL) > 0;
 
	}

	////////////////////////////////////////////////////////////////////////////////////////

	Image::~Image()
	{
		if( _pPixelRawData )
			delete[] _pPixelRawData;

		_pPixelRawData = NULL;
	}
 
	ImagePtr Image::Create(EImageType type,int width, int height, PixelFormat format, BYTE* pData, unsigned int dataSize)
	{
		ImageSpec spec;
		spec.type = type;
		spec.dataSize = dataSize;
		spec.format = format;
		spec.height = height;
		spec.width = width;

		Image *pImage = new Image(spec,pData,dataSize);
		return ImagePtr(pImage);
	}
 
	unsigned int Image::GetPngFormat(PixelFormat format)
	{
		switch (format)
		{
		 
		case PF_R8G8_TYPELESS:
		case PF_R8G8_UNORM:
		case PF_R8G8_UINT:
		case PF_R8G8_SNORM:
		case PF_R8G8_SINT:
			return PNG_FORMAT_GA;
		default:
			break;
		}

		return PNG_FORMAT_RGBA;
	}
 
	Image::Image(ImageSpec const& spec,BYTE *pPixels,unsigned int size)
		:_imageSpec(spec)
	{
		if (pPixels != NULL && size > 0)
		{
			_pPixelRawData = new BYTE[size];
			memcpy(_pPixelRawData, pPixels, size);
		}
	}
 
}