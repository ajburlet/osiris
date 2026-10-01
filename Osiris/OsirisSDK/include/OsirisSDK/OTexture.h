#pragma once

#include <memory>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OGraphicsDefinitions.h"
#include "OsirisSDK/OGPUObject.h"
#include "OsirisSDK/OResource.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"

/**
 * @brief Represents the texture entity in GPUs. 
 */
class OTexture : public OMemoryManagedObject<OGraphicsAllocators::Default>, 
				 public OGPUObject, 
				 public OResource
{
public:
	/**
	 * @brief Class constructor.
	 */
	OTexture(OString&& name="");

	/**
	 * @brief Move constructor.
	 */
	OTexture(OTexture&& other);

	/**
	 * @brief Class destructor.
	 */
	~OTexture();

	/**
	 * @brief Move assignment operator.
	 */
	OTexture& operator=(OTexture&& aOther);

	/**
	 * @brief Filter type, to determine fragment color value between texture pixels.
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
	 * @brief Sets the minifying filter type.
	 */
	void setMinFilter(FilterType aFilter);

	/**
	 * @brief Sets the magnification filter type.
	 */
	void setMagFilter(FilterType aFilter);

	/**
	 * @brief Returns the minifying filter.
	 */
	FilterType minFilter() const;

	/**
	 * @brief Returns the magnification filter.
	 */
	FilterType magFilter() const;

	/**
	 * @brief Texture wrap mode.
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
	 * @brief Texture coordinate.
	 */
	enum Coordinate {
		S,
		T,
		R
	};

	/**
	 * @brief Sets the texture wrap paramter for a texture coordinate.
	 * @param coordinate Texture coordinate.
	 * @param wrapType The texture wrapping parameter.
	 */
	void setWrapType(Coordinate coordinate, WrapMode wrapType);

	/**
	 * @brief Returns the wrap parameter for a given texture coodinate.
	 * @param coordinate Texture coordinate.
	 */
	WrapMode wrapType(Coordinate coordinate) const;

	/**
	 * @brief Pixel format.
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
	 * @brief Pixel data type.
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
	 * @brief Sets the pixel format.
	 * @param srcPixelFormat Describes how the components are organized in the source texture data.
	 * @param pixelDataType Data type of the pixel components.
	 * @param dstPixelFormat Describes how the components should be organized when uploaded to the GPU.
	 * @note If <code>aDstPixelFormat</code> is left as undefined, it will assume the value of aSrcPixelFormat.
	 */
	void setPixelFormat(PixelFormat srcPixelFormat, PixelDataType pixelDataType, 
			    PixelFormat dstPixelFormat=PixelFormat::Undefined);

	/**
	 * @brief Returns the pixel format of the source texture data.
	 */
	PixelFormat sourcePixelFormat() const;

	/**
	 * @brief Returns the pixel format of the destination GPU format.
	 */
	PixelFormat destinationPixelFormat() const;

	/**
	 * @brief Returns the pixel data type.
	 */
	PixelDataType pixelDataType() const;

	/**
	 * @brief Sets the number of mipmap levels.
	 */
	void setMipmapLevelCount(uint32_t aMipmapLevelCount);

	/**
	 * @brief Returns the number of mipmap levels.
	 */
	uint32_t mipmapLevelCount() const;

	/**
	 * @brief The allowed byte alignment for the start of each pixel row. 
	 */
	enum class RowAlignment {
		Byte=1,
		EvenNumberedByte=2,
		Word=4,
		DoubleWord=8,
		Default=Word
	};

	/**
	 * @brief Sets the pack pixel row byte alignment.
	 */
	void setPackAlignment(RowAlignment alignment);

	/**
	 * @brief Sets the pack pixel row byte alignment.
	 */
	void setUnpackAlignment(RowAlignment alignment);

	/**
	 * @brief Returns the pack pixel row byte aligbment.
	 */
	RowAlignment packAlignment() const;

	/**
	 * @brief Returns the unpack pixel row byte aligbment.
	 */
	RowAlignment unpackAlignment() const;

	/**
	 * @brief Sets the mipmap level count.
	 */
	void setMipMapLevelCount(std::size_t levelCount);

	/**
	 * @brief Sets the texture content for a given mipmap level.
	 * @param mipmapLevel Mipmap level.
	 * @param width Texture width (number of rows).
	 * @param height Texture height (number of lines).
	 * @param data Texture content.
	 * @param size Size of the texture data.
	 */
	void setContent(uint32_t mipmapLevel, uint32_t width, uint32_t height, uint8_t* data, uint32_t size);

	/**
	 * @brief Retrieves the texture content for a given mipmap level.
	 * @param mipmapLevel Mipmap level.
	 * @param width A reference to an integer where the width will be written.
	 * @param height A reference to an integer where the height will be written.
	 * @param size A reference to an integer where the size of the mipmap content will be written.
	 * @return A pointer to the content buffer.
	 */
	uint8_t* content(uint32_t mipmapLevel, uint32_t& width, uint32_t& height, uint32_t& size) const;

private:
	/**
	 @cond HIDDEN
	 */
	struct Impl;
	std::unique_ptr<Impl> _impl;
	/**
	 @endcond
	 */
};

