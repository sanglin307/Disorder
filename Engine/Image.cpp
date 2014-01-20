#include "EngineInclude.h"
 

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
		 std::vector<unsigned char> vPixels((size_t)spec.image_bytes());
		 imageInput->read_image(TypeDesc::UINT8,&vPixels[0]);
		 imageInput->close();
		 delete imageInput;

		 ImagePtr imagePtr = Image::Create(spec,vPixels.data());
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
		if( image == NULL )
			return;

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

	////////////////////////////////////////////////////////////////////////////////////////

	Image::~Image()
	{
		if( _pPixelRawData )
			delete _pPixelRawData;
	}
 
	ImagePtr Image::Create(ImageSpec const& spec,void* pData)
	{
		Image *pImage = new Image(spec);
		return ImagePtr(pImage);
	}

	ImagePtr Image::Create(int width,int height,int channels,unsigned char* pixels)
	{
		Image *pImage = new Image(ImageSpec(width,height,channels,TypeDesc::UINT8),pixels);
		return ImagePtr(pImage);
	}

	ImagePtr Image::Create(int width,int height,int channels,unsigned short* pixels)
	{
		Image *pImage = new Image(ImageSpec(width,height,channels,TypeDesc::UINT16),pixels);
		return ImagePtr(pImage);
	}

    ImagePtr Image::Create(int width,int height,int channels,unsigned int* pixels)
	{
		Image *pImage = new Image(ImageSpec(width,height,channels,TypeDesc::UINT32),pixels);
		return ImagePtr(pImage);
	}

	ImagePtr Image::Create(int width,int height,int channels,float* pixels)
	{
		Image *pImage = new Image(ImageSpec(width,height,channels,TypeDesc::FLOAT),pixels);
		return ImagePtr(pImage);
	}

	void Image::InitRawImage(unsigned int bytes,void* pData)
	{
		if( !_pPixelRawData )
		{
		   _pPixelRawData = new unsigned char[bytes];
		   memcpy(_pPixelRawData,pData,bytes);
		}
	}
 
	Image::Image(ImageSpec const& spec)
		:_imageSpec(spec),_pPixelRawData(NULL)
	{
	}

	Image::Image(ImageSpec const& spec,void *pPixels)
		:_imageSpec(spec),_pPixelRawData(NULL)
	{
		InitRawImage((unsigned int)spec.image_bytes(),pPixels);
	}

	// not debug yet
	void Image::ConvertData_B5G5R5A1_UNORM(int width,int height,void *pData)
	{
		BOOST_ASSERT(_pPixelRawData == 0);
		_pPixelRawData = new unsigned char[width*height*4];

		for(int i=0;i<width;i++ )
			for(int j=0;j<height;j++ )
			{
				int index = j*width + i;
				unsigned short* pSrc = ((unsigned short*)pData) + index;
				unsigned char* pDest = ((unsigned char*)_pPixelRawData) + 4 * index;
				pDest[0] = (*pSrc) >> 1 & 0x1F; // R
				pDest[1] = (*pSrc) >> 6 & 0x1F; // G
				pDest[2] = (*pSrc) >> 11 & 0x1F; // B;
				pDest[3] = (*pSrc) & 0x01;  // A
			}
	}

	// include PF_D24_UNORM_S8_UINT,PF_R24_UNORM_X8_TYPELESS
	void Image::ConvertData_PF_R24G8_TYPELESS(int width,int height,void *pData)
	{
		BOOST_ASSERT(_pPixelRawData == 0);
		_pPixelRawData = new unsigned char[width*height*3];

		for(int i=0;i<width;i++ )
			for(int j=0;j<height;j++ )
			{
				int index = j*width + i;
				unsigned int* pSrc = ((unsigned int*)pData) + index;
				unsigned char* pDest = ((unsigned char*)_pPixelRawData) + 3 * index;
				pDest[0] = (unsigned char)(((*pSrc) >> 8 & 0x0FFFFFF) / 65536.f);  
				pDest[1] = 0;//(*pSrc) & 0x00FF;  
				pDest[2] = 0;
			}
	}

	ImagePtr Image::Create(int width,int height,PixelFormat format,void* pData)
	{
		// only support some format
		ImageSpec spec;
		Image *pImage = NULL;

		switch (format)
		{
		  case PF_A8_UNORM:
		  case PF_R8_SINT:
		  case PF_R8_SNORM:
		  case PF_R8_TYPELESS:
		  case PF_R8_UINT:
		  case PF_R8_UNORM:
			  spec = ImageSpec(width,height,1,TypeDesc::UINT8);
			  break;
		  case PF_B5G5R5A1_UNORM:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT8);
			  pImage = new Image(spec);
			  pImage->ConvertData_B5G5R5A1_UNORM(width,height,pData);
			  break;
		  case PF_B5G6R5_UNORM:
		  case PF_D16_UNORM:
		  case PF_R16_FLOAT:
		  case PF_R16_SINT:
		  case PF_R16_SNORM:
		  case PF_R16_TYPELESS:
		  case PF_R16_UINT:
		  case PF_R16_UNORM:
		  case PF_R8G8_SINT:
		  case PF_R8G8_SNORM:
		  case PF_R8G8_TYPELESS:
		  case PF_R8G8_UINT:
		  case PF_R8G8_UNORM:
			  spec = ImageSpec(width,height,2,TypeDesc::UINT8);
			  break;
		  case PF_D24_UNORM_S8_UINT:
		  case PF_R24_UNORM_X8_TYPELESS:
		  case PF_R24G8_TYPELESS:
			   spec = ImageSpec(width,height,3,TypeDesc::UINT8);
			   pImage = new Image(spec);
			   pImage->ConvertData_PF_R24G8_TYPELESS(width,height,pData);
			   break;
		  case PF_D32_FLOAT:
		  case PF_D32_FLOAT_S8X24_UINT:
		  case PF_R32_FLOAT:
		  case PF_R32_FLOAT_X8X24_TYPELESS:
		  case PF_R32_SINT:
		  case PF_R32_TYPELESS:
		  case PF_R32_UINT:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT8);
			  break;
	
		  case PF_R10G10B10_XR_BIAS_A2_UNORM:
		  case PF_R10G10B10A2_TYPELESS:
		  case PF_R10G10B10A2_UINT:
		  case PF_R10G10B10A2_UNORM:
		  case PF_R11G11B10_FLOAT:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT8);
			  break;
		  case PF_R16G16_FLOAT:
		  case PF_R16G16_SINT:
		  case PF_R16G16_SNORM:
		  case PF_R16G16_TYPELESS:
		  case PF_R16G16_UINT:
		  case PF_R16G16_UNORM:
		      spec = ImageSpec(width,height,2,TypeDesc::UINT16);
			  break;

		  case PF_B8G8R8X8_TYPELESS:
		  case PF_B8G8R8X8_UNORM:
		  case PF_B8G8R8X8_UNORM_SRGB:	 
		  case PF_G8R8_G8B8_UNORM: 
		  case PF_R8G8_B8G8_UNORM:
		  case PF_R8G8B8A8_SINT:
		  case PF_R8G8B8A8_SNORM:
		  case PF_R8G8B8A8_TYPELESS:
		  case PF_R8G8B8A8_UINT:
		  case PF_R8G8B8A8_UNORM:
		  case PF_R8G8B8A8_UNORM_SRGB:
		  case PF_B8G8R8A8_TYPELESS:
		  case PF_B8G8R8A8_UNORM:
		  case PF_B8G8R8A8_UNORM_SRGB:
		  case PF_R9G9B9E5_SHAREDEXP:
		  case PF_X24_TYPELESS_G8_UINT:
		  case PF_X32_TYPELESS_G8X24_UINT:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT8);
			  break;
		  case PF_R16G16B16A16_FLOAT:
		  case PF_R16G16B16A16_SINT:
		  case PF_R16G16B16A16_SNORM:
		  case PF_R16G16B16A16_TYPELESS:
		  case PF_R16G16B16A16_UINT:
		  case PF_R16G16B16A16_UNORM:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT16);
			  break;
		  case PF_R32G32_FLOAT:
		  case PF_R32G32_SINT:
		  case PF_R32G32_TYPELESS:
		  case PF_R32G32_UINT:
		  case PF_R32G8X24_TYPELESS:
			  spec = ImageSpec(width,height,2,TypeDesc::UINT32);
			  break;

		  case PF_R32G32B32_FLOAT:
		  case PF_R32G32B32_SINT:
		  case PF_R32G32B32_TYPELESS:
		  case PF_R32G32B32_UINT:
			  spec = ImageSpec(width,height,3,TypeDesc::UINT32);
			  break;

		  case PF_R32G32B32A32_FLOAT:
		  case PF_R32G32B32A32_SINT:
		  case PF_R32G32B32A32_TYPELESS:
		  case PF_R32G32B32A32_UINT:
			  spec = ImageSpec(width,height,4,TypeDesc::UINT32);
			  break;

		 // case PF_BC1_TYPELESS:
		 // case PF_BC1_UNORM:
		 // case PF_BC1_UNORM_SRGB:
		 // case PF_BC4_SNORM:
		 // case PF_BC4_TYPELESS:
		 // case PF_BC4_UNORM:
			//return 4;

		 // case PF_BC2_TYPELESS:
		 // case PF_BC2_UNORM:
		 // case PF_BC2_UNORM_SRGB:
		 // case PF_BC3_TYPELESS:
		 // case PF_BC3_UNORM:
		 // case PF_BC3_UNORM_SRGB:
		 // case PF_BC5_SNORM:
		 // case PF_BC5_TYPELESS:
		 // case PF_BC5_UNORM:
		 // case PF_BC6H_SF16:
		 // case PF_BC6H_TYPELESS:
		 // case PF_BC6H_UF16:
		 // case PF_BC7_TYPELESS:
		 // case PF_BC7_UNORM:
		 // case PF_BC7_UNORM_SRGB:
			//return 8;

		  default:
			  std::stringstream str;
			  str<< "Not support pixel format:"<< format << "  to save to image!";
			  GLogger->Error(str.str());
			  return NULL;
	    }

		if( pImage == NULL ) // non convert data
		{
			 pImage = new Image(spec);
			 pImage->InitRawImage((size_t)spec.image_bytes(),pData);
		}

		return ImagePtr(pImage);
	}
}