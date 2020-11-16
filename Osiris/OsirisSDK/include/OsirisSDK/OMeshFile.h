#pragma once

#include "OsirisSDK/defs.h"
#include "OsirisSDK/OFile.h"
#include "OsirisSDK/OMemoryManagedObject.h"
#include "OsirisSDK/OGraphicsAllocators.h"

#ifndef OMESHFILE_BLOCKSIZE
#define OMESHFILE_BLOCKSIZE	128
#endif

#ifndef OMESHFILE_OBJNAMEARRAY_BLOCKSIZE
#define OMESHFILE_OBJNAMEARRAY_BLOCKSIZE	4
#endif

template <typename T, class Allocator> class OArray;
template <typename T, class Allocator, size_t BlockSize> class ODynArray;

/**
 @brief Base mesh file interface.
 */
class OAPI OMeshFile : public OFile
{
protected:
	using Allocator = OGraphicsAllocators::Default; // specific members are allocated in graphucs scope

public:
	/**
	 @brief Geometry raw data container.
	 */
	struct OAPI RawData : public OMemoryManagedObject<Allocator> {
		template <typename T>
		using Array = ODynArray<T, Allocator, OMESHFILE_BLOCKSIZE>;

		struct TexCoord : public OMemoryManagedObject<Allocator> {
			float u = 0, v = 0, w = 0;
			uint32_t dim = 0;
		};
		using TexCoordArray = Array<TexCoord>;
		
		struct Normal : public OMemoryManagedObject<Allocator> {
			float x = 0, y = 0, z = 0;
		};
		using NormalArray = Array<Normal>;
		
		struct Vertex : public OMemoryManagedObject<Allocator> { 
			float x = 0, y = 0, z = 0, w = 0; 
			TexCoord* texCoord = nullptr;
			Normal* normal = nullptr;
		};
		using VertexArray = Array<Vertex>;
		
		struct Index : public OMemoryManagedObject<Allocator> {
			uint32_t i = 0, j = 0, k = 0;
		};
		using IndexArray = Array<Index>;
		
		/**
		 @brief Struct default constructor.
		 */
		RawData();

		/**
		 @brief Struct descructor.
		 */
		~RawData();

		/**
		 @brief Returns a reference to the texture coordinates array.
		 */
		TexCoordArray& texCoordArray();

		/**
		 @brief Returns a reference to the normal array.
		 */
		NormalArray& normalArray();

		/**
		 @brief Returns a reference to the vertex array.
		 */
		VertexArray& vertexArray();

		/**
		 @brief Returns a reference to the index array.
		 */
		IndexArray& indexArray();

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
		 @brief Vertex component count.
		 */
		uint32_t vertexComponents() const;

		/**
		 @brief Texture component count.
		 */
		uint32_t textureComponents() const;

		/**
		 @brief Sets the vertex component count.
		 */
		void setVertexComponents(uint32_t aCount);

		/**
		 @brief Sets the texture component count.
		 */
		void setTextureComponents(uint32_t aCount);

	private:
		TexCoordArray*	_texCoordArray	= nullptr;
		NormalArray*	_normalArray	= nullptr;
		VertexArray*	_vertexArray	= nullptr;
		IndexArray*	_indexArray	= nullptr;
		uint32_t	_vertexComps	= 0;
		uint32_t	_texComps	= 0;
	};

	/**
	 @brief Class constructor.
	 @param aFilename Mesh geometry file name.
	 */
	OMeshFile(const char* aFilename) : OFile(aFilename, Mode::Read) {}

	/**
	 @brief Class destructor.
	 */
	virtual ~OMeshFile() = default;

	/**
	 @brief Loads a given object into an OMesh class object, previously created.
	 @param aObjName Object name.
	 @param aGeometry Geometry rawdata output.
	 */
	virtual void loadMesh(const char* aObjName, RawData& aRawData) = 0;

	/**
	 @brief Array that contains mesh object names.
	 */
	using ObjectNameArray = ODynArray<char*, Allocator, 4>;

	/**
	 @brief Provides a list with all object names in the mesh file.
	 */
	virtual const ObjectNameArray& objectArray() const = 0;
};

inline uint32_t OMeshFile::RawData::vertexComponents() const
{
	return _vertexComps;
}

inline uint32_t OMeshFile::RawData::textureComponents() const
{
	return _texComps;
}

inline void OMeshFile::RawData::setVertexComponents(uint32_t aCount)
{
	_vertexComps = aCount;
}

inline void OMeshFile::RawData::setTextureComponents(uint32_t aCount)
{
	_texComps = aCount;
}
