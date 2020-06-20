#include <functional>

#include "OsirisSDK/OException.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OShaderProgram.h"
#include "OsirisSDK/OOpenGLVertexBufferHandle.h"
#include "OsirisSDK/OOpenGLResourceCommandEncoder.h"


// ****************************************************************************
// utils
// ****************************************************************************
GLenum getUsageType(OGPUObject* aObject)
{
	GLenum usage;
	switch (aObject->usage()) {
	case OGPUObject::Usage::GPU:
		usage = GL_STATIC_DRAW;
		break;
	case OGPUObject::Usage::Mixed:
		usage = GL_DYNAMIC_DRAW;
		break;
	default:
		throw OException("Invalid buffer usage value.");
	}

	return usage;
}

GLenum wrapModeConvert(OTexture::WrapMode aWrapMode)
{
	switch (aWrapMode) {
	case OTexture::WrapMode::ClampToEdge:		return GL_CLAMP_TO_EDGE;
	case OTexture::WrapMode::ClampToBorder:		return GL_CLAMP_TO_BORDER;
	case OTexture::WrapMode::MirroredRepeat:	return GL_MIRRORED_REPEAT;
	case OTexture::WrapMode::Repeat:		return GL_REPEAT;
	case OTexture::WrapMode::MirroredClampedToEdge:	return GL_MIRROR_CLAMP_TO_EDGE;
	default:
		throw OException("Invalid texture wrap mode.");
	}

	return 0;
}

GLenum filterConvert(OTexture::FilterType aFilterType)
{
	switch (aFilterType) {
	case OTexture::FilterType::Nearest:			return GL_NEAREST;
	case OTexture::FilterType::Linear:			return GL_LINEAR;
	case OTexture::FilterType::NearestMipmapNearest:	return GL_NEAREST_MIPMAP_NEAREST;
	case OTexture::FilterType::NearestMipmapLinear:		return GL_NEAREST_MIPMAP_LINEAR;
	case OTexture::FilterType::LinearMipmapNearest:		return GL_LINEAR_MIPMAP_NEAREST;
	case OTexture::FilterType::LinearMipmapLinear:		return GL_LINEAR_MIPMAP_LINEAR;
	default:
		throw OException("Invalid texture filter type.");
	}

	return 0;
}

GLenum pixelFormatConvert(OTexture::PixelFormat aPixelFormat, bool& aIsCompressed)
{
	switch (aPixelFormat) {
	case OTexture::PixelFormat::R:			aIsCompressed = false;	return GL_RED;
	case OTexture::PixelFormat::RG:			aIsCompressed = false;	return GL_RG;
	case OTexture::PixelFormat::RGB:		aIsCompressed = false;	return GL_RGB;
	case OTexture::PixelFormat::BGR:		aIsCompressed = false;	return GL_BGR;
	case OTexture::PixelFormat::RGBA:		aIsCompressed = false;	return GL_RGBA;
	case OTexture::PixelFormat::BGRA:		aIsCompressed = false;	return GL_BGRA;
	case OTexture::PixelFormat::IntegerR:		aIsCompressed = false;	return GL_RED_INTEGER;
	case OTexture::PixelFormat::IntegerRG:		aIsCompressed = false;	return GL_RG_INTEGER;
	case OTexture::PixelFormat::IntegerRGB:		aIsCompressed = false;	return GL_RGB_INTEGER;
	case OTexture::PixelFormat::IntegerBGR:		aIsCompressed = false;	return GL_BGRA_INTEGER;
	case OTexture::PixelFormat::IntegerRGBA:	aIsCompressed = false;	return GL_RGBA_INTEGER;
	case OTexture::PixelFormat::IntegerBGRA:	aIsCompressed = false;	return GL_BGRA_INTEGER;
	case OTexture::PixelFormat::CompressedR:	aIsCompressed = true;	return GL_COMPRESSED_RED;
	case OTexture::PixelFormat::CompressedRG:	aIsCompressed = true;	return GL_COMPRESSED_RG;
	case OTexture::PixelFormat::CompressedRGB:	aIsCompressed = true;	return GL_COMPRESSED_RGB;
	case OTexture::PixelFormat::CompressedRGBA:	aIsCompressed = true;	return GL_COMPRESSED_RGBA;
	default:
		throw OException("Unsupported pixel format.");
	}
	return 0;
}

GLenum pixelFormatConvert(OTexture::PixelFormat aPixelFormat)
{
	bool compressed;
	return pixelFormatConvert(aPixelFormat, compressed);
}

GLenum pixelDataTypeConvert(OTexture::PixelDataType aDataType)
{
	switch (aDataType) {
	case OTexture::PixelDataType::Byte:				return GL_BYTE;
	case OTexture::PixelDataType::UnsignedByte:			return GL_UNSIGNED_BYTE;
	case OTexture::PixelDataType::Short:				return GL_SHORT;
	case OTexture::PixelDataType::UnsignedShort:			return GL_UNSIGNED_SHORT;
	case OTexture::PixelDataType::Integer:				return GL_INT;
	case OTexture::PixelDataType::UnsignedInteger:			return GL_UNSIGNED_INT;
	case OTexture::PixelDataType::HalfFloat:			return GL_HALF_FLOAT;
	case OTexture::PixelDataType::Float:				return GL_FLOAT;
	case OTexture::PixelDataType::UnsignedByte332:			return GL_UNSIGNED_BYTE_3_3_2;
	case OTexture::PixelDataType::UnsignedByte233Reversed:		return GL_UNSIGNED_BYTE_2_3_3_REV;
	case OTexture::PixelDataType::UnsignedShort565:			return GL_UNSIGNED_SHORT_5_6_5;
	case OTexture::PixelDataType::UnsignedShort565Reversed:		return GL_UNSIGNED_SHORT_5_6_5_REV;
	case OTexture::PixelDataType::UnsignedShort4444:		return GL_UNSIGNED_SHORT_4_4_4_4;
	case OTexture::PixelDataType::UnsignedShort4444Reversed:	return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case OTexture::PixelDataType::UnsignedShort5551:		return GL_UNSIGNED_SHORT_5_5_5_1;
	case OTexture::PixelDataType::UnsignedShort1555Reversed:	return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case OTexture::PixelDataType::UnsignedInteger8888:		return GL_UNSIGNED_INT_8_8_8_8;
	case OTexture::PixelDataType::UnsignedInteger8888Reversed:	return GL_UNSIGNED_INT_8_8_8_8_REV;
	case OTexture::PixelDataType::UnsignedInteger1010102:		return GL_UNSIGNED_INT_10_10_10_2;
	case OTexture::PixelDataType::UnsignedInteger2101010Reversed:	return GL_UNSIGNED_INT_2_10_10_10_REV;
	default:
		throw OException("Unsupported pixel data type.");
	}

	return 0;
}

// ****************************************************************************
// class implementation
// ****************************************************************************
OOpenGLResourceCommandEncoder::OOpenGLResourceCommandEncoder(OOpenGLCommandBuffer * aCommandBuffer) :
	OOpenGLCommandEncoder(aCommandBuffer)
{
}

void OOpenGLResourceCommandEncoder::load(OVertexBufferDescriptor * aVertexBufferDescriptor)
{
	createHandle(aVertexBufferDescriptor);
	encode([aVertexBufferDescriptor]() {
		glGenVertexArrays(1, reinterpret_cast<GLuint*>(aVertexBufferDescriptor->gpuHandle()));
		glBindVertexArray(*reinterpret_cast<GLuint*>(aVertexBufferDescriptor->gpuHandle())); 
	});
	
	for (uint32_t i = 0; i < aVertexBufferDescriptor->attributeCount; i++) {
		auto& attr = aVertexBufferDescriptor->attributeAtIndex(i);
		GLint size;
		GLenum type;
		switch (attr.type) {
		case OVarType::Float:
			size = 1;
			type = GL_FLOAT;
			break;
		case OVarType::Float2:
			size = 2;
			type = GL_FLOAT;
			break;
		case OVarType::Float3:
			size = 3;
			type = GL_FLOAT;
			break;
		case OVarType::Float4:
			size = 4;
			type = GL_FLOAT;
			break;
		case OVarType::Int:
			size = 1;
			type = GL_INT;
			break;
		case OVarType::Int2:
			size = 2;
			type = GL_INT;
			break;
		case OVarType::Int3:
			size = 3;
			type = GL_INT;
			break;
		case OVarType::Int4:
			size = 4;
			type = GL_INT;
			break;
		case OVarType::UnsignedInt:
			size = 1;
			type = GL_UNSIGNED_INT;
			break;
		case OVarType::UnsignedInt2:
			size = 2;
			type = GL_UNSIGNED_INT;
			break;
		case OVarType::UnsignedInt3:
			size = 3;
			type = GL_UNSIGNED_INT;
			break;
		case OVarType::UnsignedInt4:
			size = 4;
			type = GL_UNSIGNED_INT;
			break;
		default:
			throw OException("Invalid variable type.");
		}
		encode(Bind(glEnableVertexAttribArray, i));
		encode(Bind(glVertexAttribPointer, i, size, type, GL_FALSE, aVertexBufferDescriptor->stride() - attr.size(), nullptr));
	}

	encode(Bind(glBindVertexArray, 0));
}

void OOpenGLResourceCommandEncoder::unload(OVertexBufferDescriptor * aVertexBufferDescriptor)
{
	encode(Bind(glDeleteVertexArrays, 1, reinterpret_cast<GLuint*>(aVertexBufferDescriptor->gpuHandle())));
	destroyHandle(aVertexBufferDescriptor);
}

void OOpenGLResourceCommandEncoder::load(OVertexBuffer * aVertexBuffer)
{
	GLenum usage = getUsageType(aVertexBuffer);
	createHandle(aVertexBuffer);
	encode([aVertexBuffer, usage]() {
		if (aVertexBuffer->gpuHandle() == nullptr) {
			glGenBuffers(1, reinterpret_cast<GLuint*>(aVertexBuffer->gpuHandle()));
		}
		glBindBuffer(GL_ARRAY_BUFFER, *reinterpret_cast<GLuint*>(aVertexBuffer->gpuHandle())); 
		glBufferData(GL_ARRAY_BUFFER, aVertexBuffer->size(), aVertexBuffer->buffer(), usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});

}

void OOpenGLResourceCommandEncoder::unload(OVertexBuffer * aVertexBuffer)
{
	encode(Bind(glDeleteBuffers, 1, reinterpret_cast<GLuint*>(aVertexBuffer->gpuHandle())));
	destroyHandle(aVertexBuffer);
}

void OOpenGLResourceCommandEncoder::load(OIndexBuffer * aIndexBuffer)
{
	GLenum usage = getUsageType(aIndexBuffer);
	createHandle(aIndexBuffer);
	encode([aIndexBuffer, usage]() {
		glGenBuffers(1, reinterpret_cast<GLuint*>(aIndexBuffer->gpuHandle()));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *reinterpret_cast<GLuint*>(aIndexBuffer->gpuHandle()));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndexBuffer->size(), aIndexBuffer->buffer(), usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	});
}

void OOpenGLResourceCommandEncoder::unload(OIndexBuffer * aIndexBuffer)
{
	encode(Bind(glDeleteBuffers, 1, reinterpret_cast<GLuint*>(aIndexBuffer->gpuHandle())));
	destroyHandle(aIndexBuffer);
}

void OOpenGLResourceCommandEncoder::load(OTexture * aTexture)
{
	encode([aTexture]() {
		glGenTextures(1, reinterpret_cast<GLuint*>(aTexture->gpuHandle()));
		glBindTexture(GL_TEXTURE_2D, *reinterpret_cast<GLuint*>(aTexture->gpuHandle()));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeConvert(aTexture->wrapType(OTexture::Coordinate::S)));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeConvert(aTexture->wrapType(OTexture::Coordinate::T)));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterConvert(aTexture->minFilter()));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterConvert(aTexture->magFilter()));

		glPixelStorei(GL_PACK_ALIGNMENT, static_cast<GLint>(aTexture->packAlignment()));
		glPixelStorei(GL_UNPACK_ALIGNMENT, static_cast<GLint>(aTexture->unpackAlignment()));

		bool isCompressed;
		GLenum srcFmt = pixelFormatConvert(aTexture->sourcePixelFormat(), isCompressed);
		GLenum dstFmt = pixelFormatConvert(aTexture->destinationPixelFormat());
		GLenum pxDataType = pixelDataTypeConvert(aTexture->pixelDataType());

		for (uint32_t i = 0; i < aTexture->mipmapLevelCount(); i++) {
			uint32_t rows = 0;
			uint32_t cols = 0;
			uint8_t* data = nullptr;
			uint32_t size = 0;

			data = aTexture->content(i, rows, cols, size);

			if (isCompressed) {
				glCompressedTexImage2D(GL_TEXTURE_2D, i, srcFmt, cols, rows, 0, 
						       static_cast<GLsizei>(size), data);
			} else {
				glTexImage2D(GL_TEXTURE_2D, i, srcFmt, cols, rows, 0, dstFmt, pxDataType, data);
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	});
}

void OOpenGLResourceCommandEncoder::unload(OTexture * aTexture)
{
	encode(Bind(glDeleteTextures, 1, reinterpret_cast<GLuint*>(aTexture->gpuHandle())));
	destroyHandle(aTexture);
}

void OOpenGLResourceCommandEncoder::load(OShaderArgumentInstance * aAttributeInstance, OShaderProgram * aShader, const char * aName)
{
	createHandle(aAttributeInstance);
	auto uniformName = strdup(aName);
	OExceptionPointerCheck(aName);
	try {
		encode([aAttributeInstance, aShader, uniformName]() {
			auto uniform = glGetUniformLocation(*reinterpret_cast<GLuint*>(aShader->gpuHandle()), uniformName);
			free(uniformName);
			*reinterpret_cast<GLint*>(aAttributeInstance->gpuHandle()) = uniform;
		});
	} catch (OException& e) {
		free(uniformName);
		throw e;
	}
}

void OOpenGLResourceCommandEncoder::unload(OShaderArgumentInstance * aAttributeInstance)
{
	destroyHandle(aAttributeInstance);
}

void OOpenGLResourceCommandEncoder::end()
{
}
