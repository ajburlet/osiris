#include "Environment.h"

// ***********************************************************************
// ATTRIBUTES
// ***********************************************************************
EnvironmentAttributes::EnvironmentAttributes(OVector3 min, OVector3 max) :
	_min(min),
	_max(max)
{
}

EnvironmentAttributes::~EnvironmentAttributes()
{
}

OVector3 EnvironmentAttributes::min() const
{
	return _min;
}

OVector3 EnvironmentAttributes::max() const
{
	return _max;
}

void EnvironmentAttributes::addPiece(OEntity<void>* ent)
{
	_pieces.push_back(ent);
}

void EnvironmentAttributes::removePieces(OEntity<void>* ent)
{
	//_pieces.erase();
}

int EnvironmentAttributes::pieceCount() const
{
	return _pieces.size();
}

OEntity<void>* EnvironmentAttributes::piece(int idx)
{
	return _pieces[idx];
}

// ***********************************************************************
// BEHAVIOR 
// ***********************************************************************
void EnvironmentBehavior::update(EnvironmentAttributes ** attribute, ODoubleBuffer<OState>* state, 
				 OMesh ** meshPtr, const OTimeIndex & timeIndex)
{
	for (int i = 0; i < (*attribute)->pieceCount(); i++) {
		OEntity<void>* piece = (*attribute)->piece(i);
		/*
		if (piece->state()->curr()->position().x() <= (*attribute)->min().x() || 
		    piece->state()->curr()->position().x() >= (*attribute)->max().x()) {
			piece->state()->next()->motionComponent(1).setX(piece->state()->curr()->motionComponent(1).x());
		}
		if (piece->state()->curr()->position().z() <= (*attribute)->min().z() || 
		    piece->state()->curr()->position().z() >= (*attribute)->max().z()) {
			piece->state()->next()->motionComponent(1).setZ(piece->state()->curr()->motionComponent(1).z());
		}
		*/
		if (piece->state()->curr()->position().x() <= (*attribute)->min().x())
			piece->state()->next()->motionComponent(1).setX(fabs(piece->state()->curr()->motionComponent(1).x()));
		if (piece->state()->curr()->position().x() >= (*attribute)->max().x()) 
			piece->state()->next()->motionComponent(1).setX(-fabs(piece->state()->curr()->motionComponent(1).x()));
		if (piece->state()->curr()->position().z() <= (*attribute)->min().z())
			piece->state()->next()->motionComponent(1).setZ(fabs(piece->state()->curr()->motionComponent(1).z()));
		if (piece->state()->curr()->position().z() >= (*attribute)->max().z()) 
			piece->state()->next()->motionComponent(1).setZ(-fabs(piece->state()->curr()->motionComponent(1).z()));
	}
}
