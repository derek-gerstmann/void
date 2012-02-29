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

#ifndef VD_GRAPHICS_IMAGE_INCLUDED
#define VD_GRAPHICS_IMAGE_INCLUDED

#include "graphics/graphics.h"
#include "containers/containers.h"

#include <string>
#include <vector>

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_BEGIN();

// ============================================================================================== //

struct ChannelFormat
{
	VD_DECLARE_ENUM(Order,
		Invalid,
		Alpha,
		Luminance,
		R,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,
		Depth,
		Custom);
		
	vd::i32 Count;
	Order::Value Order;
	
	ChannelFormat() : 
		Count(0), 
		Order(ChannelFormat::Order::Invalid) 
	{
		// EMPTY!
	}
	
	ChannelFormat(
		ChannelFormat::Order::Value order,
		vd::i32 count = 0)
	{
		if(ChannelFormat::Order::Custom == order)
		{
			Count = count;
			Order = order;
		}
		else
		{
			Count = GetChannelCountForOrder(order);
			Order = order;
		}
	}
	
	static vd::i32
	GetChannelCountForOrder(
		ChannelFormat::Order::Value order)
	{
		switch (order)
		{
			case ChannelFormat::Order::Alpha:
			case ChannelFormat::Order::Luminance:
			case ChannelFormat::Order::R:
			case ChannelFormat::Order::Depth:
				return 1;
			case ChannelFormat::Order::RG:
				return 2;
			case ChannelFormat::Order::RGB:
			case ChannelFormat::Order::BGR:
				return 3;
			case ChannelFormat::Order::RGBA:
			case ChannelFormat::Order::BGRA:
				return 4;
			default:
				return 1;
		};
		return 1;
	};
};

// ============================================================================================== //

struct ColorSpaceFormat
{
	VD_DECLARE_ENUM(Hint,
		Invalid,
		Linear,
		GammaCorrected,
		sRGB,
		AdobeRGB,
		LogLuv,
		KodakLog,
		Custom);
	
	Hint::Value Linearity;
	
	ColorSpaceFormat() : Linearity(Hint::Linear) { }
};

// ============================================================================================== //

struct TileFormat
{
	vd::i32 Width;
	vd::i32 Height;
	vd::i32 Depth;
	
	TileFormat() : 
		Width(0), Height(0), Depth(0)
	{
		// EMPTY!
	}
};

struct ImageFormat
{
	vd::i32 Width;
	vd::i32 Height;
	vd::i32 Depth;
	TileFormat Tiles;
	ChannelFormat Channels;
	ColorSpaceFormat ColorSpace;
	
	ImageFormat() : 
		Width(0), Height(0), Depth(0), 
		Tiles(), Channels(), ColorSpace()
	{
		// EMPTY!
	}
};

// ============================================================================================== //

template<typename Type>
class PixelBuffer
{
public:
	Type* Data;
	ChannelFormat Channels;
	vd::u64 Size;

	PixelBuffer() :  Data(NULL), Channels(), Size(0) {}
	~PixelBuffer() { Destroy(); }
	
	bool Setup(const ChannelFormat& channels, vd::u64 size)
	{
		if(Data != NULL && ((Size * Channels.Count) == (size * channels.Count)))
		{
			Channels = channels;
			Size = size;
			return true;
		}
		
		Destroy();

		if(channels.Count)
			Data = VD_NEW_ARRAY( Type, size * channels.Count );
			
		if(Data == NULL)
			return false;

		Channels = channels;
		Size = size;
		return true;
	}
	
	void Destroy()
	{
		VD_SAFE_DELETE_ARRAY(Data);

		Data = NULL;
		Size = 0;
		Channels = ChannelFormat();
	}
	
	vd::u64 GetBytesPerPixel()
	{
		return Channels.Count * sizeof(Type);
	}
	
	VD_DISABLE_COPY_CONSTRUCTORS(PixelBuffer);
};

// ============================================================================================== //

template<typename Type>
class ImageBuffer
{
public:

	Type* Data;
	ImageFormat Format;
	vd::u64 BytesPerPixel;
	vd::u64 BytesPerScanline;
	
	ImageBuffer() : Data(NULL), Format(), BytesPerPixel(0), BytesPerScanline(0) {}
	~ImageBuffer() { Destroy(); }
	
	bool Setup(const ImageFormat& format)
	{
		Destroy();

		if(format.Width && format.Height && format.Channels.Count)
			Data = VD_NEW_ARRAY( Type, format.Width * format.Height * format.Channels.Count );
			
		if(Data == NULL)
			return false;

		Format = format;
		BytesPerPixel = Format.Channels.Count * sizeof(Type);	
		BytesPerScanline = Format.Width * BytesPerPixel;		
		return true;
	}
	
	void Destroy()
	{
		VD_SAFE_DELETE_ARRAY(Data);
		Data = NULL;
		Format = ImageFormat();
	}
	
	vd::u64 GetBytesPerPixel()
	{
		return BytesPerPixel;
	}

	vd::u64 GetBytesPerScanline() const
	{
		return BytesPerScanline;
	}

	vd::u64 GetIndex( size_t x, size_t y ) const
	{
		return y * BytesPerScanline + x * BytesPerPixel;
	}

	vd::u64 GetIndex( size_t x, size_t y, size_t z ) const
	{
		size_t p = y * BytesPerScanline + x * BytesPerPixel;
		if(z) p += z * BytesPerScanline * BytesPerPixel;
		return p;
	}
	
	void SetPixel(vd::u64 x, vd::u64 y, const Type& value) 
	{  
		Data[ GetIndex(x, y) ] = value;
	}

	void SetPixel(vd::u64 x, vd::u64 y, vd::u64 z, const Type& value) 
	{  
		Data[ GetIndex(x, y, z) ] = value;
	}

	const Type& GetPixel(vd::u64 x, vd::u64 y) const
	{  
		return Data[ GetIndex(x, y) ];
	}

	const Type& GetPixel(vd::u64 x, vd::u64 y, vd::u64 z) const 
	{  
		return Data[ GetIndex(x, y, z) ];
	}

	VD_DISABLE_COPY_CONSTRUCTORS(ImageBuffer);
};

// ============================================================================================== //

typedef PixelBuffer<vd::u8>  PixelBuffer8u;
typedef PixelBuffer<vd::u16> PixelBuffer16u;
typedef PixelBuffer<vd::u32> PixelBuffer32u;
typedef PixelBuffer<vd::f16> PixelBuffer16f;
typedef PixelBuffer<vd::f32> PixelBuffer32f;

// ============================================================================================== //

typedef ImageBuffer<vd::u8>  ImageBuffer8u;
typedef ImageBuffer<vd::u16> ImageBuffer16u;
typedef ImageBuffer<vd::u32> ImageBuffer32u;
typedef ImageBuffer<vd::f16> ImageBuffer16f;
typedef ImageBuffer<vd::f32> ImageBuffer32f;

// ============================================================================================== //

class ImageInput : public Object
{
public :
	ImageInput() : Object() { } 
	virtual ~ImageInput() { Destroy(); }
	
    static ImageInput& GetInstance();    
    
    vd::status Open(const std::string& location, const ImageFormat& format);
    vd::status Close();
    vd::status Destroy();
    
    vd::status Read(ImageBuffer8u& pixels);
    vd::status Read(ImageBuffer32f& pixels);
    
    vd::status ReadTile(vd::i32 x, vd::i32 y, vd::i32 z, PixelBuffer8u& pixels);
    vd::status ReadTile(vd::i32 x, vd::i32 y, vd::i32 z, PixelBuffer32f& pixels);
    
    const vd::string& GetLocation() const { return m_Location; }
	const ImageFormat& GetFormat() const { return m_Format; }    
	bool IsOpen() const { return m_IsOpen; }
	
	VD_DECLARE_OBJECT(ImageInput);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(ImageInput);

	void* m_Handle;
	bool m_IsOpen;
	ImageFormat m_Format;
	vd::string m_Location;
};

// ============================================================================================== //

class ImageOutput : public Object
{
public :
	ImageOutput() : Object() { } 
	
    static ImageOutput& GetInstance();
    
    vd::status Open(const std::string& location, const ImageFormat& format);
    vd::status Close();
    vd::status Destroy();

    vd::status Write(ImageBuffer8u& pixels);
    vd::status Write(ImageBuffer32f& pixels);

    vd::status WriteScanLine(vd::i32 y, vd::i32 z, const PixelBuffer8u& pixels);
    vd::status WriteScanLine(vd::i32 y, vd::i32 z, const PixelBuffer32f& pixels);
    
    vd::status WriteTile(vd::i32 x, vd::i32 y, vd::i32 z, const PixelBuffer8u& pixels, 
    					 size_t offset = 0, size_t xstride=0, size_t ystride=0, size_t zstride=0);

    vd::status WriteTile(vd::i32 x, vd::i32 y, vd::i32 z, const PixelBuffer32f& pixels, 
    					 size_t offset = 0, size_t xstride=0, size_t ystride=0, size_t zstride=0);
    
    const vd::string& GetLocation() const { return m_Location; }
	const ImageFormat& GetFormat() const { return m_Format; }    
	bool IsOpen() const { return m_IsOpen; }

	VD_DECLARE_OBJECT(ImageOutput);
	
private:

	VD_DISABLE_COPY_CONSTRUCTORS(ImageOutput);

	void* m_Handle;
	bool m_IsOpen;
	ImageFormat m_Format;
	vd::string m_Location;
};

// ============================================================================================== //

VD_GRAPHICS_NAMESPACE_END();

// ============================================================================================== //

#endif 
