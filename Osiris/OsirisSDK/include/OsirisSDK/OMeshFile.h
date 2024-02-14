#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OFile.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OIndexedDrawInfo.h"
#include "OsirisSDK/OGraphicsAllocators.h"

#ifndef OMESHFILE_OBJNAMEARRAY_BLOCKSIZE
#define OMESHFILE_OBJNAMEARRAY_BLOCKSIZE	4
#endif

template <typename T, class Allocator> class OArray;
template <typename T, class Allocator> class OList;
template <typename T, class Allocator, size_t BlockSize> class ODynArray;
class OMaterial;
class OIndexedDrawInfo;

/**
 @brief Base mesh file interface.
 */
class OAPI OMeshFile : public OFile
{
public:
	using Allocator = OGraphicsAllocators::Default; // specific members are allocated in graphics scope

	/**
	 @brief Geometry raw data container.
	 */
	class OAPI RawData : public OMemoryManagedObject<Allocator> {
	    public:
		template <typename T>
		using List = OList<T, Allocator>;

		/**
		 @brief Structure that holds texture coordinates.
		 */
		struct TexCoord {
			float u, v, w;
		};
		
		/**
		 @brief Structure that holds normal vector.
		 */
		struct Normal {
			float x, y, z;
		};
		
		/**
		 @brief Structure that holds vertex position;
		 */
		struct Position {
			float x, y, z, w;
		};
	
		/**
		 @brief Structure that holds pointers to position, normal and texture coordinate values.
		 */
		struct VertexData {
			Position*	pos;		/**< Vertex position. */
			Normal*		normal;		/**< Vertex normals. */
			TexCoord*	texCoord;	/**< Texture coordinates */
		};
		
		/**
		 @brief Structure that holds indexes for vertex, normals and texture coordinates for a single
			vertex.
		 */
		struct Index { 
			bool operator==(const Index& aOther) const 
			{
				return ((vert == aOther.vert) && (norm == aOther.norm) && (tex == aOther.tex));
			}
			bool operator<(const Index& aOther) const 
			{
				if (vert == aOther.vert) {
					if (norm == aOther.norm) {
						return (tex < aOther.tex);
					}
					return (norm < aOther.norm);
				}
				return (vert < aOther.vert);
			}
			uint32_t vert, norm, tex; 
		};

		/**
		 @brief Structure that holds the vertex indexes for a triangular face.
		 */
		struct Face { 
			bool operator<(const Face& aOther) const
			{ 
				if (v1 == aOther.v1) {
					if (v2 == aOther.v2) {
						return (v3 < aOther.v3);
					}
					return (v2 < aOther.v2);
				}
				return (v1 < aOther.v1);
			}
			Index v1, v2, v3; 
		};

		/**
		 @brief Struct default constructor.
		 */
		RawData();

		/**
		 @brief Struct descructor.
		 */
		~RawData();

		/**
		 @brief Adds a texture coordinate.
		 */
		void addTexCoordinate(const TexCoord& aTexCoord);

		/**
		 @brief Adds a normal vector.
		 */
		void addNormal(const Normal& aNormal);

		/**
		 @brief Adds a position coordinate.
		 */
		void addPosition(const Position& aPosition);

		/**
		 @brief Adds a triangular face.
		 */
		void addFace(const Face& aFace);

		/**
		 @brief Sets the material that must be used for future faces.
		 @param aMaterial Material to be used, <code>nullptr</code> if none.
		 */
		void useMaterial(OMaterial* aMaterial);

		/**
		 @brief Returns the name of the current material.
		 @return The string containing the name, <code>nullptr</code> if none is being used.
		 */
		const OString& currentMaterial() const;

		/**
		 @brief Returns a reference to the texture coordinates array.
		 */
		TexCoord& texCoord(uint32_t aIndex);

		/**
		 @brief Returns a reference to the normal array.
		 */
		Normal& normal(uint32_t aIndex);

		/**
		 @brief Returns a reference to the vertex array.
		 */
		Position& position(uint32_t aIndex);

		/**
		 @brief Provides access to position, normals and texture coordinates for a given index.
		 @return A structure with pointers to all vertex data.
		 */
		VertexData vertexData(uint32_t aIndex) const;

		/**
		 @brief Returns the total vertex count.
		 */
		uint32_t vertexCount () const;

		/**
		 @brief Array of indexed draw info (index buffer + material).
		 */
		OIndexedDrawInfo::Array&& indexedDrawInfoArray();

		/**
		 @brief Returns true if the geometry includes texture coordinates.
		 */
		bool hasTexCoords() const;
		
		/**
		 @brief Returns true if the geometry includes normal vectors.
		 */
		bool hasNormals() const;

		/**
		 @brief Returns true if geometry has vertex indexes for faces.
		 */
		bool hasIndices() const;

		/**
		 @brief Position vector component count.
		 */
		uint32_t positionComponents() const;

		/**
		 @brief Texture component count.
		 */
		uint32_t textureComponents() const;

		/**
		 @brief Sets the vertex component count.
		 */
		void setPositionComponents(uint32_t aCount);

		/**
		 @brief Sets the texture component count.
		 */
		void setTextureComponents(uint32_t aCount);

	private:
		bool		_hasTexCoords	= 0;
		bool		_hasNormals	= 0;
		uint32_t	_posComps	= 0;
		uint32_t	_texComps	= 0;
		/**
		 @cond HIDDEN
		 */
		struct Impl;
		Impl* _impl = nullptr;
		/**
		 @endcond
		 */
	};

	/**
	 @brief Class constructor.
	 @param aFilename Mesh geometry file name.
	 */
	OMeshFile(const OString& aFilename) : OFile(aFilename, Mode::Read) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshFile() = default;

	/**
	 @brief Loads a given object into an OMesh class object, previously created.
	 @param aObjName Object name.
	 @param aGeometry Geometry rawdata output.
	 */
	virtual void loadMesh(const OString& aObjName, RawData& aRawData) = 0;
};

inline uint32_t OMeshFile::RawData::positionComponents() const
{
	return _posComps;
}

inline uint32_t OMeshFile::RawData::textureComponents() const
{
	return _texComps;
}

inline void OMeshFile::RawData::setPositionComponents(uint32_t aCount)
{
	_posComps = aCount;
}

inline void OMeshFile::RawData::setTextureComponents(uint32_t aCount)
{
	_texComps = aCount;
}

