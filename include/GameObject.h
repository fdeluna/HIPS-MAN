#ifndef GameObject_H
#define GameObject_H

#include <Ogre.h>

#define MOVE_SPEED  0.01
#define MOVE_UP  Ogre::Vector3(0, 0, MOVE_SPEED)
#define MOVE_DOWN  Ogre::Vector3(0, 0, -MOVE_SPEED)
#define MOVE_RIGHT  Ogre::Vector3(MOVE_SPEED, 0, 0)
#define MOVE_LEFT  Ogre::Vector3(-MOVE_SPEED, 0, 0)
#define EPSILON 0.1

using namespace Ogre;

class GameObject
{
public:
	GameObject(){};
	GameObject(SceneManager* sceneManager,std::string nodeName, std::string entName, std::string mesh, Vector3 position);
	
	void start();
	virtual void update(const Ogre::FrameEvent& evt) = 0;
	void pause();
	void exit();

	SceneNode* getSceneNode(){ return sceneNode;}

protected:	
	Entity* entity;
	SceneNode* sceneNode;
};

#endif