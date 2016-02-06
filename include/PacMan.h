#ifndef PacMan_H
#define PacMan_H

#include <Ogre.h>
#include <GraphVertex.h>
#include <Scene.h>
#include <Enums.h>

#include "GameObject.h"

class PacMan : public GameObject
{
public:
	PacMan(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, GraphVertex* vertex);
	PacMan(){};
	~PacMan();


	virtual void update(const Ogre::FrameEvent& evt);

	void move();

	const bool  isDead();
	int   getScore();
	float getSpeed();	
	GraphVertex* getCurrentVertex(){ return currentVertex; };
	void  setScore(int points);
	void  addScore(int points);
	void  setSpeed(float fSpeed);
	void  setDirecction(Direcction dDirecction);
	bool scaredGhost();
	
private:
	int   life = 3;
	int   score = 0;
	float speed = 1;
	bool  dead = 0;

	bool scared = false;

	Ogre::Vector3 direcction = Ogre::Vector3::ZERO;
	// MOVEMENT
	Direcction newDirecction = Direcction::NONE;	
	Direcction currentDirecction = Direcction::NONE;

	GraphVertex* newVertex;
	GraphVertex* targetVertex;
	GraphVertex* currentVertex;


	void directionEnumToVector3(Direcction dDirection);
	void checkVertex(GraphVertex* vertex);
	
};

#endif