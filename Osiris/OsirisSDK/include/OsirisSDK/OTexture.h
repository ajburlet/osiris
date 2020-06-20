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
		Nearest=0,		/**< Uses nearest pixel value. */
		Linear,			/**< Uses a linear interpolation of the four nearest pixels. */
		NearestMipmapNearest,	/**< Uses the nearest mipmap level and nearst pixel value. */
		LinearMipmapNearest,	/**< Uses the nearest mipmap level and linear interpolation pixel value. */
		NearestMipmapLinear,	/**< Uses an interpolation of nearest pixel values of two mipmap levels. */
		LinearMipmapLinear,	/**< Uses an interpolation of linear pixel values of two mipmap levels. */
		Default=Linear		/**< Default value [Linear]. */
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
		ClampToEdge=0,
		ClampToBorder,
		MirroredRepeat,
		Repeat,
		MirroredClampedToEdge,
		Default=ClampToEdge
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
		BGRA,
		IntegerR,
		IntegerRG,
		IntegerRGB,
		IntegerBGR,
		IntegerRGBA,
		IntegerBGRA,
		CompressedR,
		CompressedRG,
		CompressedRGB,
		CompressedRGBA
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
		Float,
		UnsignedByte332,
		UnsignedByte233Reversed,
		UnsignedShort565,
		UnsignedShort565Reversed,
		UnsignedShort4444,
		UnsignedShort4444Reversed,
		UnsignedShort5551,
		UnsignedShort1555Reversed,
		UnsignedInteger8888,
		UnsignedInteger8888Reversed,
		UnsignedInteger1010102,
		UnsignedInteger2101010Reversed
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
	 @brief Returns the pixel format of the source texture data.
	 */
	PixelFormat sourcePixelFormat() const;

	/**
	 @brief Returns the pixel format of the destination GPU format.
	 */
	PixelFormat destinationPixelFormat() const;

	/**
	 @brief Returns the pixel data type.
	 */
	PixelDataType pixelDataType() const;

	/**
	 @brief Sets the number of mipmap levels.
	 */
	void setMipmapLevelCount(uint32_t aMipmapLevelCount);

	/**
	 @brief Returns the number of mipmap levels.
	 */
	uint32_t mipmapLevelCount() const;

	/**
	 @brief The allowed byte alignment for the start of each pixel row. 
	 */
	enum class RowAlignment {
		Byte=1,
		EvenNumberedByte=2,
		Word=4,
		DoubleWord=8,
		Default=Word
	};

	/**
	 @brief Sets the pack pixel row byte alignment.
	 */
	void setPackAlignment(RowAlignment aAlignment);

	/**
	 @brief Sets the pack pixel row byte alignment.
	 */
	void setUnpackAlignment(RowAlignment aAlignment);

	/**
	 @brief Returns the pack pixel row byte aligbment.
	 */
	RowAlignment packAlignment() const;

	/**
	 @brief Returns the unpack pixel row byte aligbment.
	 */
	RowAlignment unpackAlignment() const;

	/**
	 @brief Sets the texture content for a given mipmap level.
	 @param aMipmapLevel Mipmap level.
	 @param aRows Number of rows.
	 @param aLines Number of lines.
	 @param aData Texture content.
	 @param aSize Size of the texture data.
	 */
	void setContent(uint32_t aMipmapLevel, uint32_t aRows, uint32_t aLines, uint8_t* aData, uint32_t aSize);

	/**
	 @brief Retrieves the texture content for a given mipmap level.
	 @param aMipmapLevel Mipmap level.
	 @param aRows A reference to an integer where the number of rows will be written.
	 @param aLines A reference to an integer where the number of line will be written.
	 @param aSize A reference to an integer where the size of the mipmap content will be written.
	 @return A pointer to the content buffer.
	 */
	uint8_t* content(uint32_t aMipmapLevel, uint32_t& aRows, uint32_t& aLines, uint32_t& aSize) const;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	Impl*			_impl			= nullptr;
	/**
	 @endcond
	 */

	FilterType		_minFilter		= FilterType::Default;
	FilterType		_magFilter		= FilterType::Default;
	WrapMode		_wrapTypeS		= WrapMode::Default;
	WrapMode		_wrapTypeT		= WrapMode::Default;
	WrapMode		_wrapTypeR		= WrapMode::Default;
	PixelFormat		_srcPixelFormat		= PixelFormat::Undefined;
	PixelFormat		_dstPixelFormat		= PixelFormat::Undefined;
	PixelDataType		_pixelDataType		= PixelDataType::Undefined;
	RowAlignment		_packAlignment		= RowAlignment::Default;
	RowAlignment		_unpackAlignment	= RowAlignment::Default;
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
	default:		throw OException("Invalid coordinate.");
	}
}

inline OTexture::WrapMode OTexture::wrapType(Coordinate aCoordinate) const
{
	switch (aCoordinate) {
	case Coordinate::S:	return _wrapTypeS;
	case Coordinate::R:	return _wrapTypeR;
	case Coordinate::T:	return _wrapTypeT;
	default:		throw OException("Invalid coordinate.");
	}
}

inline void OTexture::setPixelFormat(OTexture::PixelFormat aSrcPixelFormat, OTexture::PixelDataType aPixelDataType,
				     OTexture::PixelFormat aDstPixelFormat)
{
	_srcPixelFormat = aSrcPixelFormat;
	_pixelDataType = aPixelDataType;
	_dstPixelFormat = (aDstPixelFormat == PixelFormat::Undefined) ? aSrcPixelFormat : aDstPixelFormat;
}

inline OTexture::PixelFormat OTexture::sourcePixelFormat() const
{
	return _srcPixelFormat;
}

inline OTexture::PixelFormat OTexture::destinationPixelFormat() const
{
	return _dstPixelFormat;
}

inline OTexture::PixelDataType OTexture::pixelDataType() const
{
	return _pixelDataType;
}

inline void OTexture::setPackAlignment(OTexture::RowAlignment aAlignment)
{
	_packAlignment = aAlignment;
}

inline void OTexture::setUnpackAlignment(RowAlignment aAlignment)
{
	_unpackAlignment = aAlignment;
}

inline OTexture::RowAlignment OTexture::packAlignment() const
{
	return _packAlignment;
}

inline OTexture::RowAlignment OTexture::unpackAlignment() const
{
	return _unpackAlignment;
}
