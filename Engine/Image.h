#ifndef _DISORDER_IMAGE_H_
#define _DISORDER_IMAGE_H_

#include "OpenImageIO/ImageIO.h"
OIIO_NAMESPACE_USING

namespace Disorder
{

	class Image
	{
	public:

		Image(ImageSpec const& spec,std::vector<unsigned char> const& pixels);
		Image(int width,int height,int channels,unsigned char* pixels);
		

		const ImageSpec& GetSpec() const
		{
			return _imageSpec;
		}

		const unsigned char* GetImageData() const
		{
			return _vPixels.data();
		}

	private:
		ImageSpec _imageSpec;
		std::vector<unsigned char> _vPixels;

	};


	class ImageManager : public Singleton<ImageManager>
	{
	public:
		ImagePtr Load(std::string const& fileName,bool reloadIfExist = false);
		void Save(std::string const& fileName,ImagePtr const& image);
		void Add(std::string const& imageName,ImagePtr const& image);
		ImagePtr Find(std::string const& imageName);

	private:
		std::map<std::string,ImagePtr> _mapImages;

	};

	#define GImageManager ImageManager::GetSingleton()
}




#endif