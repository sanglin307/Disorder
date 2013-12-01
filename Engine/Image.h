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

		const unsigned char* GetImageData() const
		{
			return _vPixels.data();
		}

		static ImagePtr Create(ImageSpec const& spec,std::vector<unsigned char> const& pixels);
		static ImagePtr Create(int width,int height,int channels,unsigned char* pixels);

	protected:
		
		Image(ImageSpec const& spec,std::vector<unsigned char> const& pixels);
		Image(int width,int height,int channels,unsigned char* pixels);
		
		ImageSpec _imageSpec;
		std::vector<unsigned char> _vPixels;

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