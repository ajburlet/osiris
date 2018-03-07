#include "OsirisSDK/OEntity.h"
#include "OsirisSDK/OCollection.hpp"
#include "OsirisSDK/OMath.h"

#include "OsirisSDK/OSpatialGrid.h"

using namespace std;

// ****************************************************************************
// OSpatialGrid
// ****************************************************************************
OSpatialGrid::OSpatialGrid(int xCells, int yCells, int zCells) :
	_xCells(xCells),
	_yCells(yCells),
	_zCells(zCells)
{
	_grid.resize(xCells*yCells*zCells);
}


OSpatialGrid::~OSpatialGrid()
{
	for (map<OEntity*, Node*>::iterator it = _nodeMap.begin(); it != _nodeMap.end(); it++) 
		delete it->second;
}

OCollection<OEntity>* OSpatialGrid::entities()
{
	return &_entities;
}

void OSpatialGrid::process()
{
	OVector3 min, max;

	/*
	for (map<OEntity*, Node*>::iterator it = _nodeMap.begin(); it != _nodeMap.end(); it++) {
		if (it == _nodeMap.begin()) {
		}
	}
	*/
}


// ****************************************************************************
// OSpatialGrid::Node
// ****************************************************************************
OSpatialGrid::Node::Node(OEntity * entity, Node * next) : 
	_ent(entity), 
	_next(next)
{
}

OSpatialGrid::Node::~Node()
{
}

OSpatialGrid::Node * OSpatialGrid::Node::next()
{
	return _next;
}

void OSpatialGrid::Node::setNext(Node * next)
{
	_next = next;
}

OEntity * OSpatialGrid::Node::entity()
{
	return _ent;
}

// ****************************************************************************
// OSpatialGrid::Cell
// ****************************************************************************
OSpatialGrid::Cell::Cell() :
	_first(NULL),
	_last(NULL)
{
}

OSpatialGrid::Cell::~Cell()
{
}

OSpatialGrid::Node * OSpatialGrid::Cell::first()
{
	return _first;
}

OSpatialGrid::Node * OSpatialGrid::Cell::last()
{
	return _last;
}

void OSpatialGrid::Cell::add(Node * node)
{
	if (_first == NULL) _first = node;
	_last->setNext(node);
	node->setNext(NULL);
}

void OSpatialGrid::Cell::reset()
{
	_first = NULL;
	_last = NULL;
}

