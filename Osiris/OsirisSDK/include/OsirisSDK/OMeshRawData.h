#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OException.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"
#include "OsirisSDK/OMaterial.h"
#include "OsirisSDK/OString.hpp"
#include "OsirisSDK/OStringDefs.h"

/**
 @brief Source mesh data produced by a mesh-file parser.
 */
class OAPI OMeshRawData : public OMemoryManagedObject<OGraphicsAllocators::Default>
{
public:
    using Allocator = OGraphicsAllocators::Default;

    struct TexCoord {
        float u, v, w;
    };

    struct Normal {
        float x, y, z;
    };

    struct Position {
        float x, y, z, w;
    };

    struct Index {
        bool operator==(const Index& aOther) const
        {
            return vert == aOther.vert && norm == aOther.norm && tex == aOther.tex;
        }

        bool operator<(const Index& aOther) const
        {
            if (vert == aOther.vert) {
                if (norm == aOther.norm) return tex < aOther.tex;
                return norm < aOther.norm;
            }
            return vert < aOther.vert;
        }

        uint32_t vert = 0;
        uint32_t norm = 0;
        uint32_t tex = 0;
    };

    struct Face {
        std::vector<Index> corners;
        uint32_t materialIndex = 0;
    };

    OMeshRawData() = default;
    ~OMeshRawData() = default;

    void addTexCoordinate(const TexCoord& aTexCoord);
    void addNormal(const Normal& aNormal);
    void addPosition(const Position& aPosition);
    void addFace(Face aFace);

    uint32_t addMaterial(OMaterial&& aMaterial);
    uint32_t getMaterialIndex(const OString& aMaterialName);
    void useMaterial(const OString& aMaterialName);
    const OString& currentMaterial() const;
    uint32_t currentMaterialIndex() const;
    uint32_t materialCount() const;
    const OString& materialName(uint32_t aIndex) const;
    const OMaterial& material(uint32_t aIndex) const;

    const TexCoord& texCoord(uint32_t aIndex) const;
    const Normal& normal(uint32_t aIndex) const;
    const Position& position(uint32_t aIndex) const;

    uint32_t faceCount() const;
    const Face& face(uint32_t aIndex) const;

    bool hasTexCoords() const;
    bool hasNormals() const;
    bool hasIndices() const;
    uint32_t positionComponents() const;
    uint32_t textureComponents() const;
    void setPositionComponents(uint32_t aCount);
    void setTextureComponents(uint32_t aCount);

private:
    std::vector<TexCoord> _texCoords;
    std::vector<Normal> _normals;
    std::vector<Position> _positions;
    std::vector<Face> _faces;
    std::vector<OMaterial> _materials;
    uint32_t _currentMaterialIndex = 0;
    uint32_t _positionComponents = 0;
    uint32_t _textureComponents = 0;
};

inline void OMeshRawData::addTexCoordinate(const TexCoord& aTexCoord)
{
    _texCoords.push_back(aTexCoord);
}

inline void OMeshRawData::addNormal(const Normal& aNormal)
{
    _normals.push_back(aNormal);
}

inline void OMeshRawData::addPosition(const Position& aPosition)
{
    _positions.push_back(aPosition);
}

inline void OMeshRawData::addFace(Face aFace)
{
    aFace.materialIndex = _currentMaterialIndex;
    _faces.push_back(std::move(aFace));
}

inline uint32_t OMeshRawData::addMaterial(OMaterial&& aMaterial)
{
    _materials.emplace_back(std::move(aMaterial));
    return static_cast<uint32_t>(_materials.size()-1);
}

inline uint32_t OMeshRawData::getMaterialIndex(const OString& aMaterialName)
{
    for (uint32_t index = 0; index < _materials.size(); ++index) {
        if (_materials[index].name() == aMaterialName) return index;
    }
    throw OEx(OString::Fmt("Unable to find index for material name: %s", aMaterialName.cString())); 
}

inline void OMeshRawData::useMaterial(const OString& aMaterialName)
{
    _currentMaterialIndex = getMaterialIndex(aMaterialName);
}

inline const OString& OMeshRawData::currentMaterial() const
{
    return _materials[_currentMaterialIndex].name();
}

inline uint32_t OMeshRawData::currentMaterialIndex() const
{
    return _currentMaterialIndex;
}

inline uint32_t OMeshRawData::materialCount() const
{
    return static_cast<uint32_t>(_materials.size());
}

inline const OString& OMeshRawData::materialName(uint32_t aIndex) const
{
    return _materials.at(aIndex).name();
}

inline const OMaterial& OMeshRawData::material(uint32_t aIndex) const
{
    return _materials.at(aIndex);
}

inline const OMeshRawData::TexCoord& OMeshRawData::texCoord(uint32_t aIndex) const
{
    return _texCoords.at(aIndex);
}

inline const OMeshRawData::Normal& OMeshRawData::normal(uint32_t aIndex) const
{
    return _normals.at(aIndex);
}

inline const OMeshRawData::Position& OMeshRawData::position(uint32_t aIndex) const
{
    return _positions.at(aIndex);
}

inline uint32_t OMeshRawData::faceCount() const
{
    return static_cast<uint32_t>(_faces.size());
}

inline const OMeshRawData::Face& OMeshRawData::face(uint32_t aIndex) const
{
    return _faces.at(aIndex);
}

inline bool OMeshRawData::hasTexCoords() const
{
    return !_texCoords.empty();
}

inline bool OMeshRawData::hasNormals() const
{
    return !_normals.empty();
}

inline bool OMeshRawData::hasIndices() const
{
    return !_faces.empty();
}

inline uint32_t OMeshRawData::positionComponents() const
{
    return _positionComponents;
}

inline uint32_t OMeshRawData::textureComponents() const
{
    return _textureComponents;
}

inline void OMeshRawData::setPositionComponents(uint32_t aCount)
{
    _positionComponents = aCount;
}

inline void OMeshRawData::setTextureComponents(uint32_t aCount)
{
    _textureComponents = aCount;
}
