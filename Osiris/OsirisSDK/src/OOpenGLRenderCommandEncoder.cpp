#include <functional>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/ORenderComponents.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OOpenGLRenderCommandEncoder.h"
#include "OsirisSDK/OVector.hpp"

GLenum gOpenGLTextureID[] = {
	GL_TEXTURE0,
	GL_TEXTURE1,
	GL_TEXTURE2,
	GL_TEXTURE3,
	GL_TEXTURE4,
	GL_TEXTURE5,
	GL_TEXTURE6,
	GL_TEXTURE7,
	GL_TEXTURE8,
	GL_TEXTURE9,
	GL_TEXTURE10,
	GL_TEXTURE11,
	GL_TEXTURE12,
	GL_TEXTURE13,
	GL_TEXTURE14,
	GL_TEXTURE15,
	GL_TEXTURE16,
	GL_TEXTURE17,
	GL_TEXTURE18,
	GL_TEXTURE19,
	GL_TEXTURE20,
	GL_TEXTURE21,
	GL_TEXTURE22,
	GL_TEXTURE23,
	GL_TEXTURE24,
	GL_TEXTURE25,
	GL_TEXTURE26,
	GL_TEXTURE27,
	GL_TEXTURE28,
	GL_TEXTURE29,
	GL_TEXTURE30,
	GL_TEXTURE31
};

OOpenGLRenderCommandEncoder::OOpenGLRenderCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer) :
	OOpenGLCommandEncoder(aCommandBuffer)
{
	for (auto& tex : _texture) tex = nullptr;
}

void OOpenGLRenderCommandEncoder::setShaderProgram(OShaderProgram * aShaderProgram)
{
	encode([this, aShaderProgram]() {
		glUseProgram((aShaderProgram != nullptr) ? handle(aShaderProgram) : 0);
	});
	_shaderProgram = aShaderProgram;
}

void OOpenGLRenderCommandEncoder::setVertexBuffer(OVertexBuffer * aVertexBuffer)
{
	_vertexBuffer = aVertexBuffer;
}

void OOpenGLRenderCommandEncoder::setIndexBuffer(OIndexBuffer * aIndexBuffer)
{
	_indexBuffer = aIndexBuffer;
}

void OOpenGLRenderCommandEncoder::setTexture(OTexture * aTexture, uint32_t aIndex)
{
	encode(Bind(glActiveTexture, gOpenGLTextureID[aIndex]));
	encode([this, aTexture]() {
		glBindTexture(GL_TEXTURE_2D, (aTexture != nullptr) ? handle(aTexture) : 0); 
	});
	_texture[aIndex] = aTexture;
}

void OOpenGLRenderCommandEncoder::setRenderComponents(ORenderComponents * aRenderComponents)
{
	auto GetBlendGLFactor = [](OBlendFactor aFactor) {
		switch (aFactor) {
		case OBlendFactor::Zero:			return GL_ZERO;
		case OBlendFactor::One:				return GL_ONE;
		case OBlendFactor::SourceColor:			return GL_SRC_COLOR;
		case OBlendFactor::OneMinusSourceColor:		return GL_ONE_MINUS_SRC_COLOR;
		case OBlendFactor::DestinationColor:		return GL_DST_COLOR;
		case OBlendFactor::OneMinusDestinationColor:	return GL_ONE_MINUS_DST_COLOR;
		case OBlendFactor::SourceAlpha:			return GL_SRC_ALPHA;
		case OBlendFactor::OneMinusSourceAlpha:		return GL_ONE_MINUS_SRC_ALPHA;
		case OBlendFactor::DestinationAlpha:		return GL_DST_ALPHA;
		case OBlendFactor::OneMinusDestinationAlpha:	return GL_ONE_MINUS_DST_ALPHA;
		case OBlendFactor::ConstantColor:		return GL_CONSTANT_COLOR;
		case OBlendFactor::OneMinusConstantColor:	return GL_ONE_MINUS_CONSTANT_COLOR;
		case OBlendFactor::ConstantAlpha:		return GL_CONSTANT_ALPHA;
		case OBlendFactor::OneMinusConstantAlpha:	return GL_ONE_MINUS_CONSTANT_ALPHA;
		case OBlendFactor::SourceAlphaSaturated:	return GL_SRC_ALPHA_SATURATE;
		case OBlendFactor::Source1Color:		return GL_SRC1_COLOR;
		case OBlendFactor::OneMinusSource1Color:	return GL_ONE_MINUS_SRC1_COLOR;
		case OBlendFactor::Source1Alpha:		return GL_SRC1_ALPHA;
		case OBlendFactor::OneMinusSource1Alpha:	return GL_ONE_MINUS_SRC1_ALPHA;
		default:
			throw OEx("Invalid color blending setting.");
		}
		return GL_ONE;
	};

	if (aRenderComponents->colorBlendingEnabled()) {
		encode(Bind(glEnable, GL_BLEND));
		encode(Bind(glBlendFunc, GetBlendGLFactor(aRenderComponents->colorBlendingSourceFactor()),
					 GetBlendGLFactor(aRenderComponents->colorBlendingDestinationFactor())));
	}

	if (aRenderComponents->faceCullingEnabled()) {
		encode(Bind(glEnable, GL_CULL_FACE));
		encode(Bind(glCullFace, (aRenderComponents->cullingFace() == OCullFace::Back) ? GL_BACK : GL_FRONT));
		encode(Bind(glFrontFace, (aRenderComponents->cullingFrontFace() == OCullFront::CCW) ? GL_CCW : GL_CW));
	} else {
		encode(Bind(glDisable, GL_CULL_FACE));
	}

	encode(Bind(aRenderComponents->depthTestingEnabled() ? glEnable : glDisable, GL_DEPTH_TEST));
	encode(Bind(glDepthMask, aRenderComponents->depthBufferWriteEnabled() ? GL_TRUE : GL_FALSE));

	encode([this, aRenderComponents]() {
		glBindVertexArray(aRenderComponents->gpuHandleCastTo<GLuint>());
	});
}

void OOpenGLRenderCommandEncoder::setUniformArgument(OShaderUniformArgument * aUniformArgument)
{
	encode([this, aUniformArgument]() {
		switch (aUniformArgument->type()) {

		case OVarType::Float:
			glUniform1fv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLfloat*>());
			break;
		
		case OVarType::Float2:
			glUniform2fv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLfloat*>());
			break;
		
		case OVarType::Float3:
			glUniform3fv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLfloat*>());
			break;
		
		case OVarType::Float4:
			glUniform4fv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLfloat*>());
			break;
			
		case OVarType::Float2x2:
			glUniformMatrix2fv(handle(aUniformArgument), aUniformArgument->arrayLength(), false, aUniformArgument->castTo<GLfloat*>());
			break;
		
		case OVarType::Float3x3:
			glUniformMatrix3fv(handle(aUniformArgument), aUniformArgument->arrayLength(), false, aUniformArgument->castTo<GLfloat*>());
			break;

		case OVarType::Float4x4:
			glUniformMatrix4fv(handle(aUniformArgument), aUniformArgument->arrayLength(), false, aUniformArgument->castTo<GLfloat*>());
			break;
		
		case OVarType::Int:
			glUniform1iv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLint*>());
			break;

		case OVarType::Int2:
			glUniform2iv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLint*>());
			break;

		case OVarType::Int3:
			glUniform3iv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLint*>());
			break;

		case OVarType::Int4:
			glUniform4iv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLint*>());
			break;

		case OVarType::UnsignedInt:
			glUniform1uiv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLuint*>());
			break;

		case OVarType::UnsignedInt2:
			glUniform2uiv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLuint*>());
			break;

		case OVarType::UnsignedInt3:
			glUniform3uiv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLuint*>());
			break;

		case OVarType::UnsignedInt4:
			glUniform4uiv(handle(aUniformArgument), aUniformArgument->arrayLength(), aUniformArgument->castTo<GLuint*>());
			break;

		default:
			throw OEx("Unsupported shader uniform argument type.");
		}
	});
}

OShaderProgram * OOpenGLRenderCommandEncoder::shaderProgram()
{
	return _shaderProgram;
}

OVertexBuffer * OOpenGLRenderCommandEncoder::vertexBuffer()
{
	return _vertexBuffer;
}

OIndexBuffer * OOpenGLRenderCommandEncoder::indexBuffer()
{
	return _indexBuffer;
}

OTexture * OOpenGLRenderCommandEncoder::texture(uint32_t aIndex)
{
	return _texture[aIndex];
}

void OOpenGLRenderCommandEncoder::clearTextures()
{
	for (uint32_t i=0; i < 32; ++i) {
		if (_texture[i] != nullptr) {
			setTexture(nullptr, i);
		}
	}
}

void OOpenGLRenderCommandEncoder::draw(ORenderMode aRenderType)
{
	switch (aRenderType) {
	case ORenderMode::Triangle:
		encode(Bind(glDrawArrays, GL_TRIANGLES, 0, _vertexBuffer->vertexCount()));
		break;
	case ORenderMode::IndexedTriangle:
		encode(Bind(glDrawElements, GL_TRIANGLES, _indexBuffer->faceCount(), GL_UNSIGNED_INT, nullptr));
		break;
	case ORenderMode::TriangleStrip:
		encode(Bind(glDrawArrays, GL_TRIANGLE_STRIP, 0, _vertexBuffer->vertexCount()));
		break;
	default:
		throw OEx("Invalid draw mode.");
	}
}

void OOpenGLRenderCommandEncoder::clearRenderTarget(const OVector4F & aColor)
{
	encode(Bind(glClearColor, aColor[OVectorAxis::R], aColor[OVectorAxis::G], aColor[OVectorAxis::B], aColor[OVectorAxis::A]));
	encode(Bind(glClear, GL_COLOR_BUFFER_BIT));
}

void OOpenGLRenderCommandEncoder::clearDepthBuffer(float aValue)
{
	encode(Bind(glClearDepth, aValue));
	encode(Bind(glClear, GL_DEPTH_BUFFER_BIT));
}

void OOpenGLRenderCommandEncoder::end()
{
	setShaderProgram(nullptr);
	setVertexBuffer(nullptr);
	setIndexBuffer(nullptr);
	clearTextures();
}

