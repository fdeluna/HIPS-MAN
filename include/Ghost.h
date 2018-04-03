#ifndef Ghost_H
#define Ghost_H

#include <Ogre.h>
#include <GraphVertex.h>
#include "PacMan.h"
#include "GameObject.h"

class Ghost : public GameObject
{
public:

	enum  GhostState
	{
		WAIT,
		CHASE,
		SCATTER,
		SCARED,
		DEAD,
		EXIT
	};

	Ghost(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, PacMan* pacman, GraphVertex* hHome);
	Ghost(){};
	~Ghost();

	virtual void update(const Ogre::FrameEvent& evt);

	virtual void chase(GraphVertex* objective);
	void wait();
	void dead();
	void scatter();
	void scared();

	const bool  isDead();
	void Ghost::isScared();
	float getSpeed();
	GraphVertex* getCurrentVertex(){ return currentVertex; };
	void init(GraphVertex* vertex, GhostState gState);
	void  setSpeed(float fSpeed);
	void  setGhostMaterial(std::string material){ ghostMaterial = material; };
	void  setDirecction(GraphVertex::Direcction dDirecction);

	GraphVertex* closerNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex);
	GraphVertex* futherNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex);
	GraphVertex::Direcction getOppositeDirecction(GraphVertex::Direcction dDirecction);

private:
	float speed = 0.75;	

	GhostState state;

	// MOVEMENT
	Ogre::Vector3 direcction = Ogre::Vector3::ZERO;
	GraphVertex::Direcction currentDirecction = GraphVertex::Direcction::NONE;

	PacMan* pacMan;
	GraphVertex* home;

	GraphVertex* previousVertex;
	GraphVertex* targetVertex;
	GraphVertex* currentVertex;
	GraphVertex* finalVertex;
	std::string  ghostMaterial;

	float time = 0;

	void getDirecction(GraphVertex* targetVertx, GraphVertex* actualVertx);
	void directionEnumToVector3(GraphVertex::Direcction dDirection);
	void checkVertex(GraphVertex* vertex);
};

#endif