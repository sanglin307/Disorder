#ifndef _DISORDER_IMAGE_H_
#define _DISORDER_IMAGE_H_

#include <png.h>
#include <zlib.h>

namespace Disorder
{
	enum EImageType
	{
		eIT_PNG,
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
 
		static ImagePtr Create(EImageType type,int width, int height, PixelFormat format, BYTE* pData, unsigned int dataSize);
	 
		~Image();

		static unsigned int GetPngFormat(PixelFormat format);

	protected:
		

		Image(ImageSpec const& spec, BYTE *pPixels, unsigned int size);
	
		ImageSpec _imageSpec;
		BYTE* _pPixelRawData; 
	};


	class ImageManager : public Singleton<ImageManager>
	{
		friend class Singleton<ImageManager>;
	public:
		ImagePtr Load(std::string const& fileName, PixelFormat pixelFormat, bool reloadIfExist = false);
		bool Save(std::string const& fileName,ImagePtr const& image);
		void Add(std::string const& imageName,ImagePtr const& image);
		ImagePtr Find(std::string const& imageName);
		
	private:
		ImageManager(){};
		static ImageManagerPtr Create();
		std::map<std::string,ImagePtr> _mapImages;

	};

	#define GImageManager ImageManager::GetSingleton()
}




#endif