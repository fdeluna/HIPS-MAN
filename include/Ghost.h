#ifndef Ghost_H
#define Ghost_H

#include <Ogre.h>
#include <GraphVertex.h>
#include <Enums.h>
#include "PacMan.h"
#include "GameObject.h"

class Ghost : public GameObject
{
public:
	Ghost(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, GraphVertex* vertex, PacMan* pacman, GhostState gState, GraphVertex* hHome);
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
	void  setSpeed(float fSpeed);	
	void  setDirecction(Direcction dDirecction);

	GraphVertex* closerNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex);
	GraphVertex* futherNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex);
	Direcction getOppositeDirecction(Direcction dDirecction);
	
private:	
	float speed = 0.75;
	//bool  dead = false;

	GhostState state;

	// MOVEMENT
	Ogre::Vector3 direcction = Ogre::Vector3::ZERO;		
	Direcction currentDirecction = Direcction::NONE;

	PacMan* pacMan;
	GraphVertex* home;

	GraphVertex* previousVertex;
	GraphVertex* targetVertex;
	GraphVertex* currentVertex;
	GraphVertex* finalVertex;

	float time = 0;

	void getDirecction(GraphVertex* targetVertx, GraphVertex* actualVertx);
	void directionEnumToVector3(Direcction dDirection);
	void checkVertex(GraphVertex* vertex);	
};

#endif