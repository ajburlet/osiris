#pragma once

#include <OsirisSDK/OBehavior.hpp>
#include <OsirisSDK/OEntity.hpp>

#include <vector>

class EnvironmentAttributes {
public:
	EnvironmentAttributes(OVector3 min, OVector3 max);
	~EnvironmentAttributes();

	OVector3 min() const;
	OVector3 max() const;

	void addPiece(OEntity<void>* ent);
	void removePieces(OEntity<void>* ent);
	int pieceCount() const;
	OEntity<void>* piece(int idx);

private:
	OVector3 _min;
	OVector3 _max;
	std::vector< OEntity<void>* > _pieces;
};

class EnvironmentBehavior : public OBehavior<EnvironmentAttributes> {
public:
	virtual void update(EnvironmentAttributes** attribute,
		ODoubleBuffer<OState>* state,
		OMesh** meshPtr,
		const OTimeIndex& timeIndex,
		int step_us);
};

