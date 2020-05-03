#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGPUObject.h"

/**
 @brief Represents the texture entity in GPUs. 
 */
class OTexture : public OGPUObject
{
public:
	/**
	 @brief Class constructor.
	 */
	OTexture();

	/**
	 @brief Class destructor.
	 */
	~OTexture();

	/**
	 @brief Filter type, to determine fragment color value between texture pixels.
	 */
	enum class FilterType {
		Nearest,		/**< Uses nearest pixel value. */
		Linear,			/**< Uses a linear interpolation of the four nearest pixels. */
		NearestMipmapNearest,	/**< Uses the nearest mipmap level and nearst pixel value. */
		LinearMipmapNearest,	/**< Uses the nearest mipmap level and linear interpolation pixel value. */
		NearestMipmapLinear,	/**< Uses an interpolation of nearest pixel values of two mipmap levels. */
		LinearMipmapLinear	/**< Uses an interpolation of linear pixel values of two mipmap levels. */
	};

	/**
	 @brief Sets the minifying filter type.
	 */
	void setMinFilter(FilterType aFilter);

	/**
	 @brief Sets the magnification filter type.
	 */
	void setMagFilter(FilterType aFilter);

	/**
	 @brief Returns the minifying filter.
	 */
	FilterType minFilter() const;

	/**
	 @brief Returns the magnification filter.
	 */
	FilterType magFilter() const;

	/**
	 @brief Texture wrap mode.
	 */
	enum WrapMode {
		ClampToEdge,
		ClampToBorder,
		MirroredRepeat,
		Repeat,
		MirroredClampedToEdge
	};

	/**
	 @brief Texture coordinate.
	 */
	enum Coordinate {
		S,
		T,
		R
	};

	/**
	 @brief Sets the texture wrap paramter for a texture coordinate.
	 @param aCoordinate Texture coordinate.
	 @param aWrapType The texture wrapping parameter.
	 */
	void setWrapType(Coordinate aCoordinate, WrapMode aWrapType);

	/**
	 @brief Returns the wrap parameter for a given texture coodinate.
	 @param aCoordinate Texture coordinate.
	 */
	WrapMode wrapType(Coordinate aCoordinate) const;

	/**
	 @brief Pixel format.
	 */
	enum class PixelFormat {
		Undefined,
		R,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA
	};

	/**
	 @brief Pixel data type.
	 */
	enum class PixelDataType {
		Undefined,
		Byte,
		UnsignedByte,
		Short,
		UnsignedShort,
		Integer,
		UnsignedInteger,
		HalfFloat,
		Float
	};

	/**
	 @brief Sets the pixel format.
	 @param aSrcPixelFormat Describes how the components are organized in the source texture data.
	 @param aPixelDataType Data type of the pixel components.
	 @param aDstPixelFormat Describes how the components should be organized when uploaded to the GPU.
	 @note If <code>aDstPixelFormat</code> is left as undefined, it will assume the value of aSrcPixelFormat.
	 */
	void setPixelFormat(PixelFormat aSrcPixelFormat, PixelDataType aPixelDataType, 
			    PixelFormat aDstPixelFormat=PixelFormat::Undefined);

	/**
	 @brief Sets the number of mipmap levels.
	 */
	void setMipmapLevelCount(uint32_t aMipmapLevelCount);

	/**
	 @brief Sets the texture content for a given mipmap level.
	 @param aMipmapLevel Mipmap level.
	 @param aRows Number of rows.
	 @param aLines Number of lines.
	 @param aData Texture content.
	 @param aSize Size of the texture data.
	 */
	void setContent(uint32_t aMipmapLevel, uint32_t aRows, uint32_t aLines, uint8_t* aData, uint32_t aSize);

private:
	struct Impl;
	
	Impl*			_impl		= nullptr;
	FilterType		_minFilter	= FilterType::Nearest;
	FilterType		_magFilter	= FilterType::Nearest;
	WrapMode		_wrapTypeS	= WrapMode::ClampToBorder;
	WrapMode		_wrapTypeT	= WrapMode::ClampToBorder;
	WrapMode		_wrapTypeR	= WrapMode::ClampToBorder;
	PixelFormat		_srcPixelFormat = PixelFormat::Undefined;
	PixelFormat		_dstPixelFormat = PixelFormat::Undefined;
	PixelDataType		_pixelDataType	= PixelDataType::Undefined;
	
};

inline void OTexture::setMinFilter(OTexture::FilterType aFilter)
{
	_minFilter = aFilter;
}

inline void OTexture::setMagFilter(OTexture::FilterType aFilter)
{
	_magFilter = aFilter;
}

inline OTexture::FilterType OTexture::minFilter() const
{
	return _minFilter;
}

inline OTexture::FilterType OTexture::magFilter() const
{
	return _magFilter;
}

inline void OTexture::setWrapType(Coordinate aCoordinate, OTexture::WrapMode aWrapType)
{
	switch (aCoordinate) {
	case Coordinate::S:	_wrapTypeS = aWrapType;		break;
	case Coordinate::R:	_wrapTypeR = aWrapType;		break;
	case Coordinate::T:	_wrapTypeT = aWrapType;		break;
	}
}

inline OTexture::WrapMode OTexture::wrapType(Coordinate aCoordinate) const
{
	switch (aCoordinate) {
	case Coordinate::S:	return _wrapTypeS;
	case Coordinate::R:	return _wrapTypeR;
	case Coordinate::T:	return _wrapTypeT;
	}
}

inline void OTexture::setPixelFormat(OTexture::PixelFormat aSrcPixelFormat, OTexture::PixelDataType aPixelDataType,
				     OTexture::PixelFormat aDstPixelFormat)
{
	_srcPixelFormat = aSrcPixelFormat;
	_pixelDataType = aPixelDataType;
	_dstPixelFormat = (aDstPixelFormat == PixelFormat::Undefined) ? aSrcPixelFormat : aDstPixelFormat;
}
