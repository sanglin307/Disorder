#include "EngineInclude.h"
#include "Logger.h"

namespace Disorder
{
	InitialiseSingleton(ImageManager);

	ImagePtr ImageManager::Load(std::string const& fileName,bool reloadIfExist)
	{
		 if( !reloadIfExist )
		 {
			 if( _mapImages.find(fileName) != _mapImages.end() )
				 return _mapImages[fileName];
		 }

		 std::string imageFile = GConfig->sResourceTexPath + fileName;
		 ImageInput *imageInput = ImageInput::open(imageFile.c_str());
		 if( !imageInput )
		 {
			 BOOST_ASSERT(0);
			 std::string error = "load image file:" + imageFile + " failed!";
			 GLogger->Error(error);
			 return NULL;
		 }
		 const ImageSpec& spec = imageInput->spec();
		 std::vector<unsigned char> vPixels(spec.width*spec.height*spec.nchannels);
		 imageInput->read_image(TypeDesc::UINT8,&vPixels[0]);
		 imageInput->close();
		 delete imageInput;

		 ImagePtr imagePtr = Image::Create(spec,vPixels);
		 _mapImages[fileName] = imagePtr;

		 return imagePtr;
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

	void ImageManager::Save(std::string const& fileName,ImagePtr const& image)
	{
		std::string imageFile = GConfig->sResourceTexPath + fileName;
		ImageOutput *imageOut = ImageOutput::create(imageFile.c_str());
		if( !imageOut )
		{
			BOOST_ASSERT(0);
			std::string error = "save image file:" + imageFile + " failed!";
			GLogger->Error(error);
			return ;
		}

		imageOut->open(imageFile,image->GetSpec());
		imageOut->write_image(image->GetSpec().format,image->GetImageData());
		imageOut->close();
		delete imageOut;
	}

	ImagePtr Image::Create(ImageSpec const& spec,std::vector<unsigned char> const& pixels)
	{
		Image *pImage = new Image(spec,pixels);
		return ImagePtr(pImage);
	}

	ImagePtr Image::Create(int width,int height,int channels,unsigned char* pixels)
	{
		Image *pImage = new Image(width,height,channels,pixels);
		return ImagePtr(pImage);
	}


	Image::Image(int width,int height,int channels,unsigned char* pixels)
	{
		_imageSpec = ImageSpec(width,height,channels,TypeDesc::UINT8);
		int size = width*height*channels;
		_vPixels.resize(size);
		memcpy(&_vPixels[0],pixels,size*sizeof(unsigned char));
	}

	Image::Image(ImageSpec const& spec,std::vector<unsigned char> const& pixels)
		:_imageSpec(spec),_vPixels(pixels)
	{
	}
}