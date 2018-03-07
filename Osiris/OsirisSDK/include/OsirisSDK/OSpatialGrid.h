#pragma once

#include <vector>
#include <map>

class OEntity;
template<class T> class OCollection;

/**
 @brief Spatial grid class, designed to ease collision detection.

 In order to facilitate the solution for the n-body problem, we divide the space
 occupied by the entities in cells, so we limit the number of collion process 
 iterations.
 */
class OSpatialGrid
{
public:
	OSpatialGrid(int xCells, int yCells, int zCells);
	virtual ~OSpatialGrid();

	OCollection<OEntity>* entities();

	void process();


protected:
	/**
	 @brief Linked list node for each spatial grid.
	 */
	class Node {
	public:
		/**
		 @brief Class Constructor.
		 @brief entity Node entity.
		 @brief next Next node.
		 */
		Node(OEntity* entity, Node* next=NULL);

		/**
		 @brief Class destructor.
		 */
		virtual ~Node();

		/**
		 @brief Returns next node.
		 */
		Node* next();

		/**
		 @brief Sets new next node.
		 */
		void setNext(Node* next);

		/**
		 @brief Returns pointer to node entity.
		 */
		OEntity* entity();

	private:
		OEntity* _ent;
		Node* _next;
	};
	
	/**
	 @brief Spatial grid cell.
	 */
	class Cell {
	public:
		/**
		 @brief Class constructor.
		 */
		Cell();

		/**
		 @brief Class destructor.
		 */
		virtual ~Cell();

		/**
		 @brief Returns pointer to the first node.
		 */
		Node* first();

		/**
		 @brief Returns pointer to the last node.
		 */
		Node* last();

		/**
		 @brief Adds new node to the cell.
		 */
		void add(Node* node);

		/**
		 @brief Reset linked list, removing all items.
		 */
		void reset();
	private:
		Node* _first;
		Node* _last;
	};

private:
	std::vector<Cell> _grid;
	std::map<OEntity*, Node*> _nodeMap;
	OCollection<OEntity> _entities;
	int _xCells;
	int _yCells;
	int _zCells;
	
};

