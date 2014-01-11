#ifndef _DISORDER_IMAGE_H_
#define _DISORDER_IMAGE_H_

#include "OpenImageIO/ImageIO.h"
OIIO_NAMESPACE_USING

namespace Disorder
{

	class Image
	{
	public:
 
		const ImageSpec& GetSpec() const
		{
			return _imageSpec;
		}

		const void* GetImageData() const
		{
			return _pPixelRawData;
		}

		static ImagePtr Create(ImageSpec const& spec,void* pData);
		static ImagePtr Create(int width,int height,PixelFormat format,void* pData);
		static ImagePtr Create(int width,int height,int channels,unsigned char* pixels);
		static ImagePtr Create(int width,int height,int channels,unsigned short* pixels);
		static ImagePtr Create(int width,int height,int channels,unsigned int* pixels);
		static ImagePtr Create(int width,int height,int channels,float* pixels);

		~Image();

	protected:
		
		Image(ImageSpec const& spec,void* pPixels);
		void InitImage(unsigned int bytes,void* pData);

		ImageSpec _imageSpec;
		void* _pPixelRawData;

	};


	class ImageManager : public Singleton<ImageManager>
	{
		friend class Singleton<ImageManager>;
	public:
		ImagePtr Load(std::string const& fileName,bool reloadIfExist = false);
		void Save(std::string const& fileName,ImagePtr const& image);
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