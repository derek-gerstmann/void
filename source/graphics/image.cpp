// ============================================================================================== //
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#include "vd.h"
#include "core/core.h"
#include "core/asserts.h"
#include "graphics/image.h"

// ============================================================================================== //

#include <cctype>
#include <string>
#include <vector>

#if defined(VD_USE_HALF)
#include <half.h>
#endif

// ============================================================================================== //

typedef VD_NAMESPACE_SCOPE::f16 half;
#include <OpenImageIO/imageio.h>

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

namespace
{	
	typedef OIIO_NAMESPACE::ImageSpec 	OiioImageSpec;
	typedef OIIO_NAMESPACE::ImageInput 	OiioImageInput;
	typedef OIIO_NAMESPACE::ImageOutput OiioImageOutput;
	typedef OIIO_NAMESPACE::TypeDesc 	OiioTypeDesc;
	const OIIO_NAMESPACE::stride_t		OiioAutoStride = OIIO_NAMESPACE::AutoStride;
}

// ============================================================================================== //

ImageInput& 
ImageInput::GetInstance()
{
    static ImageInput* Instance = NULL;
    if(Instance == NULL) Instance = GetInstancePtr();
    return *Instance;
}

ImageInput* 
ImageInput::GetInstancePtr()
{
    static ImageInput Instance;
    return &Instance;
}

vd::status
ImageInput::Destroy()
{
	if(m_Handle != NULL)
	{
		Close();
	}

	m_Handle = NULL;
	m_IsOpen = false;
	m_Location = vd::string();
	m_Format = ImageFormat();
	return Status::Code::Success;
}

vd::status 
ImageInput::Open(
	const std::string& location, 
	const ImageFormat& format)
{	
	if(m_Handle != NULL)
	{
		Close();
	}
	
	OiioImageInput* input = OiioImageInput::create (location.c_str());
	if (input == NULL)
	{
		vdLogWarning("Failed to open image from file '%s'!", location.c_str());
		return Status::Code::OpenError;
	}
	
	OiioImageSpec spec;
	if(format.Width > 0)			spec.width = format.Width;
	if(format.Height > 0)			spec.height = format.Height;
	if(format.Depth > 0)			spec.depth = format.Depth;
	if(format.Tiles.Width > 0)		spec.tile_width = format.Tiles.Width;
	if(format.Tiles.Height > 0)		spec.tile_height = format.Tiles.Height;
	if(format.Tiles.Depth > 0)		spec.tile_depth = format.Tiles.Depth;
	if(format.Channels.Count > 0)	spec.nchannels = format.Channels.Count;
	
	if(!input->open(location.c_str(), spec))
	{
		vdLogWarning("Failed to open image from file '%s'!", location.c_str());
		return Status::Code::OpenError;
	}
	
	vdLogInfo("Opened image %d x %d (%d channels) from '%s'!", 
		format.Width, format.Height, format.Channels.Count, location.c_str());
	
	m_Format = format;
	m_Format.Width = spec.width;
	m_Format.Height = spec.height;
	m_Format.Channels.Count = spec.nchannels;
	m_Format.Tiles.Width = spec.tile_width;
	m_Format.Tiles.Height = spec.tile_height;
	m_Format.Tiles.Depth = spec.tile_depth;

	m_Location = location;
	m_IsOpen = true;
	m_Handle = reinterpret_cast<void*>(input);
	return Status::Code::Success;
}

vd::status
ImageInput::Close(void)
{
	if(m_Handle == NULL)
		return Status::Code::Success;
		
	OiioImageInput* input = reinterpret_cast<OiioImageInput*>(m_Handle);
	input->close();
	delete input;

	m_Handle = NULL;
	m_IsOpen = false;
	return Status::Code::Success;
}

vd::status
ImageInput::Read(
	ImageBuffer8u& pixels)
{
	OiioImageInput* input = reinterpret_cast<OiioImageInput*>(m_Handle);
	vdAssert(input != NULL);
	
	pixels.Setup(m_Format);
	
	if(!input->read_image(OiioTypeDesc::UINT8, pixels.Data))
	{
		vdLogWarning("Failed to load image from file '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogInfo("Loaded image %d x %d (%d channels - 8-bit unsigned) from '%s'!", 
		m_Format.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

vd::status
ImageInput::Read(
	ImageBuffer32f& pixels)
{
	OiioImageInput* input = reinterpret_cast<OiioImageInput*>(m_Handle);
	vdAssert(input != NULL);

	pixels.Setup(m_Format);
	
	if(!input->read_image(OiioTypeDesc::FLOAT, pixels.Data))
	{
		vdLogWarning("Failed to load image from file '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogInfo("Loaded image %d x %d (%d channels - 32 bit float) F32 from '%s'!", 
		m_Format.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

vd::status
ImageInput::ReadTile(
	vd::i32 x, vd::i32 y, vd::i32 z,
	PixelBuffer8u& pixels)
{
	OiioImageInput* input = reinterpret_cast<OiioImageInput*>(m_Handle);
	vdAssert(input != NULL);

	if(m_Format.Tiles.Width <= 0 || m_Format.Tiles.Height <= 0)
	{
		vdLogWarning("Failed to read tile from image -- invalid tile size used in image format!");
		return Status::Code::ReadError;
	}
	
	pixels.Setup(m_Format.Channels, m_Format.Tiles.Width * m_Format.Tiles.Height);
	
	if(!input->read_tile (x, y, z, OiioTypeDesc::UINT8, pixels.Data))
	{
		vdLogWarning("Failed to load image from file '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogDebug("Loaded tile [%d,%d,%d] %d x %d (%d channels - 8-bit unsigned) from '%s'!", 
		x,y,z, m_Format.Tiles.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

vd::status
ImageInput::ReadTile(
	vd::i32 x, vd::i32 y, vd::i32 z,
	PixelBuffer32f& pixels)
{
	OiioImageInput* input = reinterpret_cast<OiioImageInput*>(m_Handle);
	vdAssert(input != NULL);

	if(m_Format.Tiles.Width <= 0 || m_Format.Tiles.Height <= 0)
	{
		vdLogWarning("Failed to read tile from image -- invalid tile size used in image format!");
		return Status::Code::ReadError;
	}
	
	pixels.Setup(m_Format.Channels, m_Format.Tiles.Width * m_Format.Tiles.Height);
	
	if(!input->read_tile (x, y, z, OiioTypeDesc::FLOAT, pixels.Data))
	{
		vdLogWarning("Failed to load image from file '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogDebug("Loaded tile [%d,%d,%d] %d x %d (%d channels - 8-bit unsigned) from '%s'!", 
		x,y,z, m_Format.Tiles.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

// ============================================================================================== //

ImageOutput& ImageOutput::GetInstance()
{
    static ImageOutput* Instance = NULL;
    if(Instance == NULL) Instance = GetInstancePtr();
    return *Instance;
}

ImageOutput* 
ImageOutput::GetInstancePtr()
{
    static ImageOutput Instance;
    return &Instance;
}

vd::status
ImageOutput::Destroy()
{
	if(m_Handle != NULL)
	{
		Close();
	}

	m_Handle = NULL;
	m_IsOpen = false;
	m_Location = vd::string();
	m_Format = ImageFormat();
	return Status::Code::Success;
}

vd::status 
ImageOutput::Open(
	const std::string& location, 
	const ImageFormat& format)
{	
	if(m_Handle != NULL)
	{
		Close();
	}
	
	OiioImageOutput* output = OiioImageOutput::create (location.c_str());
	if (output == NULL)
	{
		vdLogWarning("Failed to open image for file '%s'!", location.c_str());
		return Status::Code::OpenError;
	}
	
	OiioImageSpec spec;
	if(format.Width > 0)			spec.width = format.Width;
	if(format.Height > 0)			spec.height = format.Height;
	if(format.Depth > 0)			spec.depth = format.Depth;
	if(format.Tiles.Width > 0)		spec.tile_width = format.Tiles.Width;
	if(format.Tiles.Height > 0)		spec.tile_height = format.Tiles.Height;
	if(format.Tiles.Depth > 0)		spec.tile_depth = format.Tiles.Depth;
	if(format.Channels.Count > 0)	spec.nchannels = format.Channels.Count;
	
	if(!output->open(location.c_str(), spec))
	{
		vdLogWarning("Failed to open image for file '%s'!", location.c_str());
		return Status::Code::OpenError;
	}
	
	vdLogInfo("Opened image %d x %d (%d channels) for '%s'!", 
		format.Width, format.Height, format.Channels.Count, location.c_str());

	m_Format = format;
	m_Format.Width = spec.width;
	m_Format.Height = spec.height;
	m_Format.Channels.Count = spec.nchannels;
	m_Format.Tiles.Width = spec.tile_width;
	m_Format.Tiles.Height = spec.tile_height;
	m_Format.Tiles.Depth = spec.tile_depth;

	m_Location = location;
	m_IsOpen = true;
	m_Handle = reinterpret_cast<void*>(output);

	return Status::Code::Success;
}

vd::status
ImageOutput::Close(void)
{
	if(m_Handle == NULL)
		return Status::Code::Success;
		
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	output->close();
	delete output;

	m_Handle = NULL;
	m_IsOpen = false;
	return Status::Code::Success;
}

vd::status
ImageOutput::Write(
	ImageBuffer8u& pixels)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);
	
	if(!output->write_image(OiioTypeDesc::UINT8, pixels.Data))
	{
		vdLogWarning("Failed to write image to '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogInfo("Wrote image %d x %d (%d channels - 8-bit unsigned) from '%s'!", 
		m_Format.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteScanLine(
	vd::i32 y, vd::i32 z,
	const PixelBuffer32f& pixels)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(!output->write_scanline(y, z, OiioTypeDesc::FLOAT, pixels.Data))
	{
		vdLogWarning("Failed to write scanline [%d %d] to image '%s'!", y, z, m_Location.c_str());
		return Status::Code::WriteError;
	}

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteScanLine(
	vd::i32 y, vd::i32 z,
	const PixelBuffer8u& pixels)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(!output->write_scanline(y, z, OiioTypeDesc::UINT8, pixels.Data))
	{
		vdLogWarning("Failed to write scanline [%d %d] to image '%s'!", y, z, m_Location.c_str());
		return Status::Code::WriteError;
	}

	return Status::Code::Success;
}

vd::status
ImageOutput::Write(
	ImageBuffer32f& pixels)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(!output->write_image(OiioTypeDesc::FLOAT, pixels.Data))
	{
		vdLogWarning("Failed to load image from file '%s'!", m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogInfo("Loaded image %d x %d (%d channels - 32 bit float) F32 from '%s'!", 
		m_Format.Width, m_Format.Height, m_Format.Channels.Count, m_Location.c_str());	

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteTile(
	vd::i32 x, vd::i32 y, vd::i32 z,
	const PixelBuffer8u& pixels, size_t offset,
	size_t xstride, size_t ystride, size_t zstride)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(m_Format.Tiles.Width <= 0 || m_Format.Tiles.Height <= 0)
	{
		vdLogWarning("Failed to write tile to image -- invalid tile size used in image format!");
		return Status::Code::ReadError;
	}
	
/*
	if(!output->write_tile (x, y, z, OiioTypeDesc::UINT8, ((char*)pixels.Data) + offset, 
							xstride ? xstride : OiioAutoStride,
							ystride ? ystride : OiioAutoStride,
							zstride ? zstride : OiioAutoStride))
*/
	if(!output->write_tile (x, y, z, OiioTypeDesc::UINT8, ((char*)pixels.Data)))
	{
		vdLogWarning("Failed to save tile [%d,%d,%d] to image '%s'!", x, y, z, m_Location.c_str());
		return Status::Code::ReadError;
	}

	vdLogDebug("Saved tile [%d,%d,%d] %d x %d (%d channels - 8-bit unsigned)!", 
		x,y,z, m_Format.Tiles.Width, m_Format.Height, m_Format.Channels.Count);	

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteTile(
	vd::i32 x, vd::i32 y, vd::i32 z,
	const PixelBuffer32f& pixels, size_t offset,
	size_t xstride, size_t ystride, size_t zstride)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(m_Format.Tiles.Width <= 0 || m_Format.Tiles.Height <= 0)
	{
		vdLogWarning("Failed to write tile to image -- invalid tile size used in image format!");
		return Status::Code::WriteError;
	}
/*	
	if(!output->write_tile (x, y, z, OiioTypeDesc::FLOAT, ((char*)pixels.Data) + offset, 
							xstride ? xstride : OiioAutoStride,
							ystride ? ystride : OiioAutoStride,
							zstride ? zstride : OiioAutoStride))
*/
	if(!output->write_tile (x, y, z, OiioTypeDesc::FLOAT, ((char*)pixels.Data)))
	{
		vdLogWarning("Failed to save tile [%d,%d,%d] to image '%s'!", x, y, z, m_Location.c_str());
		return Status::Code::WriteError;
	}

	vdLogDebug("Saved tile [%d,%d,%d] %d x %d (%d channels - 32bit float)!", 
		x,y,z, m_Format.Tiles.Width, m_Format.Tiles.Height, m_Format.Channels.Count);	

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteRectangle(
	vd::i32 x0, vd::i32 x1,
	vd::i32 y0, vd::i32 y1,
	vd::i32 z0, vd::i32 z1,
	const PixelBuffer8u& pixels, size_t offset,
	size_t xstride, size_t ystride, size_t zstride)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(!output->write_rectangle (x0, x1, y0, y1, z0, z1, 
						 		 OiioTypeDesc::UINT8, ((char*)pixels.Data)))
	{
		vdLogWarning("Failed to write rectangular region [%d,%d,%d] -> [%d,%d,%d] to image '%s'!", 
			x0, y0, z0, x1, y1, z1, m_Location.c_str());
		return Status::Code::WriteError;
	}

	vdLogDebug("Saved rectangular region [%d,%d,%d] -> [%d,%d,%d] (%d channels - 32bit float)!", 
		x0, y0, z0, x1, y1, z1, m_Format.Channels.Count);	

	return Status::Code::Success;
}

vd::status
ImageOutput::WriteRectangle(
	vd::i32 x0, vd::i32 x1,
	vd::i32 y0, vd::i32 y1,
	vd::i32 z0, vd::i32 z1,
	const PixelBuffer32f& pixels, size_t offset,
	size_t xstride, size_t ystride, size_t zstride)
{
	OiioImageOutput* output = reinterpret_cast<OiioImageOutput*>(m_Handle);
	vdAssert(output != NULL);

	if(!output->write_rectangle (x0, x1, y0, y1, z0, z1, 
						 		 OiioTypeDesc::FLOAT, ((char*)pixels.Data)))
	{
		vdLogWarning("Failed to write rectangular region [%d,%d,%d] -> [%d,%d,%d] to image '%s'!", 
			x0, y0, z0, x1, y1, z1, m_Location.c_str());
		return Status::Code::WriteError;
	}

	vdLogDebug("Saved rectangular region [%d,%d,%d] -> [%d,%d,%d] (%d channels - 32bit float)!", 
		x0, y0, z0, x1, y1, z1, m_Format.Channels.Count);	

	return Status::Code::Success;
}

// ============================================================================================== //

VD_IMPLEMENT_OBJECT(ImageInput, vd_sym(ImageInput), vd_sym(Object));
VD_IMPLEMENT_OBJECT(ImageOutput, vd_sym(ImageOutput), vd_sym(Object));

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //
