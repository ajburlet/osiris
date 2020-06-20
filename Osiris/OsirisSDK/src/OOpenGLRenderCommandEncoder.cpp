#include <functional>

#include "OsirisSDK/GLdefs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OShaderProgram.h"
#include "OsirisSDK/OVertexBuffer.h"
#include "OsirisSDK/OIndexBuffer.h"
#include "OsirisSDK/OTexture.h"
#include "OsirisSDK/OShaderArgument.h"
#include "OsirisSDK/OShaderArgumentInstanceList.h"
#include "OsirisSDK/OOpenGLRenderCommandEncoder.h"


OOpenGLRenderCommandEncoder::OOpenGLRenderCommandEncoder(OOpenGLCommandBuffer* aCommandBuffer) :
	OOpenGLCommandEncoder(aCommandBuffer)
{
	for (auto& tex : _texture) tex = nullptr;
}

void OOpenGLRenderCommandEncoder::setFaceCullingOptions(OCullFace aFace, OCullFront aFront)
{
	if (aFace != OCullFace::Undefined) {
		encode(Bind(glEnable, GL_CULL_FACE));
		encode(Bind(glCullFace, (aFace == OCullFace::Back) ? GL_BACK : GL_FRONT));
		encode(Bind(glFrontFace, (aFront == OCullFront::CCW) ? GL_CCW : GL_CW));
	} else {
		encode(Bind(glDisable, GL_CULL_FACE));
	}
}

void OOpenGLRenderCommandEncoder::setShaderProgram(OShaderProgram * aShaderProgram)
{
	encode(Bind(glUseProgram, (aShaderProgram != nullptr) ? handle(aShaderProgram) : 0));
	_shaderProgram = aShaderProgram;
}

void OOpenGLRenderCommandEncoder::setVertexBuffer(OVertexBuffer * aVertexBuffer)
{
	encode(Bind(glBindBuffer, GL_ARRAY_BUFFER, (aVertexBuffer != nullptr) ? handle(aVertexBuffer) : 0));
	_vertexBuffer = aVertexBuffer;
}

void OOpenGLRenderCommandEncoder::setIndexBuffer(OIndexBuffer * aIndexBuffer)
{
	encode(Bind(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, (aIndexBuffer != nullptr) ? handle(aIndexBuffer) : 0));
	_indexBuffer = aIndexBuffer;
}

void OOpenGLRenderCommandEncoder::setTexture(OTexture * aTexture, uint32_t aIndex)
{
	encode(Bind(glActiveTexture, gOpenGLTextureID[aIndex]));
	encode(Bind(glBindTexture, GL_TEXTURE_2D, (aTexture != nullptr) ? handle(aTexture) : 0));
	_texture[aIndex] = aTexture;
}

void OOpenGLRenderCommandEncoder::setUniformArgumentList(OShaderArgumentInstanceList * aUniformArguments)
{
	for (auto arg : *aUniformArguments) {
		switch (arg->type()) {
		case OVarType::Float:
			encode(Bind(glUniform1fv, handle(arg), arg->arrayLength(), arg->castTo<GLfloat*>()));
			break;
		
		case OVarType::Float2:
			encode(Bind(glUniform2fv, handle(arg), arg->arrayLength(), arg->castTo<GLfloat*>()));
			break;
		
		case OVarType::Float3:
			encode(Bind(glUniform3fv, handle(arg), arg->arrayLength(), arg->castTo<GLfloat*>()));
			break;
		
		case OVarType::Float4:
			encode(Bind(glUniform4fv, handle(arg), arg->arrayLength(), arg->castTo<GLfloat*>()));
			break;
			
		case OVarType::UnsignedInt:
			encode(Bind(glUniform1uiv, handle(arg), arg->arrayLength(), arg->castTo<GLuint*>()));
			break;

		case OVarType::Float2x2:
			encode(Bind(glUniformMatrix2fv, handle(arg), arg->arrayLength(), false, arg->castTo<GLfloat*>()));
			break;
		
		case OVarType::Float3x3:
			encode(Bind(glUniformMatrix3fv, handle(arg), arg->arrayLength(), false, arg->castTo<GLfloat*>()));
			break;

		case OVarType::Float4x4:
			encode(Bind(glUniformMatrix4fv, handle(arg), arg->arrayLength(), false, arg->castTo<GLfloat*>()));
			break;

		default:
			throw OException("Unsupported shader uniform argument type.");
		}
	}
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
		throw OException("Invalid draw mode.");
	}
}

void OOpenGLRenderCommandEncoder::end()
{
	setShaderProgram(nullptr);
	setVertexBuffer(nullptr);
	setIndexBuffer(nullptr);
	clearTextures();
}

