#ifndef GameObject_H
#define GameObject_H

#include <Ogre.h>

using namespace Ogre;

class GameObject
{
public:
	GameObject(){};
	GameObject(SceneManager* sceneManager,std::string nodeName, std::string entName, std::string mesh, Vector3 position);
	

	void start();
	virtual void update() = 0;
	void pause();
	void exit();

	SceneNode* getSceneNode(){ return sceneNode;}

protected:	
	Entity* entity;
	SceneNode* sceneNode;
};

#endif