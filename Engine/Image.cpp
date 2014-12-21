#include "EngineInclude.h"
 

namespace Disorder
{
	ImageManager *GImageManager = NULL;

	ImageManager::~ImageManager()
	{
		std::map<std::string, Image*>::iterator iter = _mapImages.begin();
		while (iter != _mapImages.end())
		{
			delete iter->second;
		}
		_mapImages.clear();
	}

	Image* ImageManager::Load(std::string const& fileName, SamplerDesc* desc, bool reloadIfExist)
	{
		if (!reloadIfExist)
		{
			if (_mapImages.find(fileName) != _mapImages.end())
				return _mapImages[fileName];
		}

		int pos = fileName.find_last_of('.');
		if (pos == -1)
			return NULL;

		std::string suffix = fileName.substr(pos + 1);
		boost::to_lower(suffix);
		if (suffix == "png")
		{
			Image* img = LoadPng(fileName);
			if (img != NULL)
			{
				if (desc != NULL)
				{
					SamplerState* sampler = GRenderResourceMgr->CreateSamplerState(desc);
					img->SetResource(GRenderResourceMgr->CreateTexture2D(fileName,sampler, img->GetSpec().format, false, img));
				}
				else
				    img->SetResource(GRenderResourceMgr->CreateTexture2D(fileName,NULL, img->GetSpec().format, false, img));
			}

			return img;
		}
		else if (suffix == "jpg")
		{
			Image* img = LoadJpg(fileName);
			if (img != NULL)
			{
				if (desc != NULL)
				{
					SamplerState* sampler = GRenderResourceMgr->CreateSamplerState(desc);
					img->SetResource(GRenderResourceMgr->CreateTexture2D(fileName,sampler, img->GetSpec().format, false, img));
				}
				else
					img->SetResource(GRenderResourceMgr->CreateTexture2D(fileName,NULL, img->GetSpec().format, false, img));
			}

			return img;
		}

		return NULL;
	}

	Image* ImageManager::Load(std::string const& fileName,bool reloadIfExist)
	{
		return Load(fileName, NULL, reloadIfExist);
	}

	void ImageManager::Add(std::string const& imageName, Image* image)
	{
		_mapImages[imageName] = image;
	}
 
	Image* ImageManager::Find(std::string const& imageName)
	{
		std::map<std::string,Image*>::iterator iter = _mapImages.find(imageName);
		if( iter != _mapImages.end() )
			return iter->second;

		return NULL;
	}

	bool ImageManager::IsSupported(std::string& suffix)
	{
		boost::to_lower(suffix);
		if (suffix == "jpg" || suffix == "png")
			return true;

		return false;
	}

	Image* ImageManager::LoadPng(std::string const& fileName)
	{
		// only support png now.
		png_image image;

		/* Initialize the 'png_image' structure. */
		memset(&image, 0, (sizeof image));
		image.version = PNG_IMAGE_VERSION;

		/* The first argument is the file to read: */
		if (png_image_begin_read_from_file(&image, fileName.c_str()))
		{
			//image.format = Image::GetPngFormat(pixelFormat);
			int size = PNG_IMAGE_SIZE(image);
			png_bytep buffer = (png_bytep)malloc(size);

			if (buffer != NULL && png_image_finish_read(&image, NULL, buffer, 0, NULL))
			{
				Image* imagePtr = new Image(fileName,eIT_PNG, image.width, image.height, PF_R8G8B8A8_TYPELESS, buffer, size);
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

	void ImageManager::Convert_R8G8B8_R8G8B8A8(BYTE *pSrc, unsigned int srcLength, BYTE *pDst, unsigned int dstLength)
	{
		BOOST_ASSERT(srcLength / 3 == dstLength / 4);
		BYTE *pRead = pSrc;
		BYTE *pWrite = pDst;
		unsigned int count = 0;
		while (count < srcLength)
		{
			pWrite[0] = pRead[0];
			pWrite[1] = pRead[1];
			pWrite[2] = pRead[2];
			pWrite[3] = 0xff;  // alpha channel

			pRead += 3;
			pWrite += 4;
			count += 3;
		}

	}

	Image* ImageManager::LoadJpg(std::string const& fileName)
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;

		/* More stuff */
		FILE * infile;		/* source file */
		JSAMPARRAY buffer;		/* Output row buffer */
		int row_stride;		/* physical row width in output buffer */
 
		if (fopen_s(&infile,fileName.c_str(), "rb")) 
		{
			GLogger->Error("Can't open jpg file:" + fileName);
			return NULL;
		}

		/* Step 1: allocate and initialize JPEG decompression object */
		cinfo.err = jpeg_std_error(&jerr);
	 
		/* Now we can initialize the JPEG decompression object. */
		jpeg_create_decompress(&cinfo);

		/* Step 2: specify data source (eg, a file) */
		jpeg_stdio_src(&cinfo, infile);

		/* Step 3: read file parameters with jpeg_read_header() */
		(void)jpeg_read_header(&cinfo, TRUE);
 

		/* Step 4: set parameters for decompression */

		/* In this example, we don't need to change any of the defaults set by
		* jpeg_read_header(), so we do nothing here.
		*/

		/* Step 5: Start decompressor */
		(void)jpeg_start_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/
 
		/* We may need to do some setup of our own at this point before reading
		* the data.  After jpeg_start_decompress() we have the correct scaled
		* output image dimensions available, as well as the output colormap
		* if we asked for color quantization.
		* In this example, we need to make an output work buffer of the right size.
		*/
		/* JSAMPLEs per row in output buffer */
		row_stride = cinfo.output_width * cinfo.output_components;
		/* Make a one-row-high sample array that will go away when done with image */
		buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

		/* Step 6: while (scan lines remain to be read) */
		/*           jpeg_read_scanlines(...); */

		/* Here we use the library's state variable cinfo.output_scanline as the
		* loop counter, so that we don't have to keep track ourselves.
		*/
		unsigned int dataSize = cinfo.output_height * row_stride;
		BYTE *pData = (BYTE*)malloc(dataSize);
		BYTE *pDest = pData;

		while (cinfo.output_scanline < cinfo.output_height) 
		{
			/* jpeg_read_scanlines expects an array of pointers to scanlines.
			* Here the array is only one element long, but you could ask for
			* more than one scanline at a time if that's more convenient.
			*/
			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			/* Assume put_scanline_someplace wants a pointer and sample count. */
			memcpy(pDest, buffer[0], row_stride);
			pDest += row_stride;
		}

		Image* imagePtr = NULL;
		PixelFormat pixelFormat;
		if (cinfo.out_color_space == JCS_RGB && cinfo.output_components == 3 && cinfo.block_size == 8)
		{
			pixelFormat = PF_R8G8B8A8_TYPELESS;
			unsigned int imgDataSize = dataSize / 3 * 4;
			BYTE *pImageData = (BYTE*)malloc(imgDataSize);
			Convert_R8G8B8_R8G8B8A8(pData, dataSize, pImageData, imgDataSize);
			 imagePtr = new Image(fileName,eIT_JPG, cinfo.output_width, cinfo.output_height, pixelFormat, pImageData, imgDataSize);
			_mapImages[fileName] = imagePtr;
		}
		else
		{
			GLogger->Error("Not support this format jpg yet!");
		}
 
		free(pData);

		/* Step 7: Finish decompression */

		(void)jpeg_finish_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		* with the stdio data source.
		*/

		/* Step 8: Release JPEG decompression object */

		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_decompress(&cinfo);

		fclose(infile);
 
		return imagePtr;
	}

	bool ImageManager::SavePng(std::string const& fileName, const Image* image)
	{
		if (image == NULL)
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

	bool ImageManager::SaveJpg(std::string const& fileName, const Image* image)
	{ 
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		/* More stuff */
		FILE * outfile;		/* target file */
		JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
		int row_stride;		/* physical row width in image buffer */

		/* Step 1: allocate and initialize JPEG compression object */

		/* We have to set up the error handler first, in case the initialization
		* step fails.  (Unlikely, but it could happen if you are out of memory.)
		* This routine fills in the contents of struct jerr, and returns jerr's
		* address which we place into the link field in cinfo.
		*/
		cinfo.err = jpeg_std_error(&jerr);
		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);

		/* Step 2: specify data destination (eg, a file) */
		/* Note: steps 2 and 3 can be done in either order. */
 
		if (fopen_s(&outfile,fileName.c_str(), "wb"))
		{
			GLogger->Error("Can't Open file:" + fileName);
			return false;
		}

		jpeg_stdio_dest(&cinfo, outfile);

		/* Step 3: set parameters for compression */

		/* First we supply a description of the input image.
		* Four fields of the cinfo struct must be filled in:
		*/
		cinfo.image_width = image->GetSpec().width; 	/* image width and height, in pixels */
		cinfo.image_height = image->GetSpec().height;
		cinfo.input_components = RenderEngine::ComputePixelChannel(image->GetSpec().format);		/* # of color components per pixel */
		cinfo.in_color_space = (J_COLOR_SPACE)Image::GetJpgFormat(image->GetSpec().format); 	/* colorspace of input image */
		/* Now use the library's routine to set default compression parameters.
		* (You must set at least cinfo.in_color_space before calling this,
		* since the defaults depend on the source color space.)
		*/
		jpeg_set_defaults(&cinfo);
		/* Now you can set any non-default parameters you wish to.
		* Here we just illustrate the use of quality (quantization table) scaling:
		*/
		jpeg_set_quality(&cinfo, 75, TRUE /* limit to baseline-JPEG values */);

		/* Step 4: Start compressor */

		/* TRUE ensures that we will write a complete interchange-JPEG file.
		* Pass TRUE unless you are very sure of what you're doing.
		*/
		jpeg_start_compress(&cinfo, TRUE);

		/* Step 5: while (scan lines remain to be written) */
		/*           jpeg_write_scanlines(...); */

		/* Here we use the library's state variable cinfo.next_scanline as the
		* loop counter, so that we don't have to keep track ourselves.
		* To keep things simple, we pass one scanline per call; you can pass
		* more if you wish, though.
		*/
		row_stride = cinfo.image_width * cinfo.input_components;	/* JSAMPLEs per row in image_buffer */
		const BYTE *pData = image->GetImageData();

		while (cinfo.next_scanline < cinfo.image_height)
		{
			/* jpeg_write_scanlines expects an array of pointers to scanlines.
			* Here the array is only one element long, but you could pass
			* more than one scanline at a time if that's more convenient.
			*/
			BYTE *pDest = (BYTE*)(pData + cinfo.next_scanline * row_stride);
			row_pointer[0] = pDest;
			(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		/* Step 6: Finish compression */

		jpeg_finish_compress(&cinfo);
		/* After finish_compress, we can close the output file. */
		fclose(outfile);

		/* Step 7: release JPEG compression object */
		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);

		return true;
	}

	bool ImageManager::Save(std::string const& fileName,const Image* image)
	{
		if (image->GetSpec().type == eIT_PNG)
			return SavePng(fileName, image);
		else if (image->GetSpec().type == eIT_JPG)
			return SaveJpg(fileName, image);
 
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////

	Image::~Image()
	{
		if( _pPixelRawData )
			delete[] _pPixelRawData;

		_pPixelRawData = NULL;
	}
 
	Image::Image(const std::string& name,EImageType type,int width, int height, PixelFormat format, BYTE* pData, unsigned int dataSize)
	{
		_name = name;
		_imageSpec.type = type;
		_imageSpec.dataSize = dataSize;
		_imageSpec.format = format;
		_imageSpec.height = height;
		_imageSpec.width = width;

		if (pData != NULL && dataSize > 0)
		{
			_pPixelRawData = new BYTE[dataSize];
			memcpy(_pPixelRawData, pData, dataSize);
		}
	}
 
 

	unsigned int Image::GetJpgFormat(PixelFormat format)
	{
		switch (format)
		{
		 
		case PF_R8G8_TYPELESS:
		case PF_R8G8_UNORM:
		case PF_R8G8_UINT:
		case PF_R8G8_SNORM:
		case PF_R8G8_SINT:
			return JCS_GRAYSCALE;
		default:
			break;
		}

		return JCS_RGB;
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
 
}