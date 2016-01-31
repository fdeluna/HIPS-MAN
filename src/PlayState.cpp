#include "PlayState.h"
#include "PauseState.h"

#include <Importer.h>
#include <Scene.h>
#include <Camera.h>
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter()
{
		_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("IntroCamera");
	_camera->setPosition(Ogre::Vector3(0, -40, -60));
	//_camera->pitch(Ogre::Degree(90));
	_camera->lookAt(Ogre::Vector3(0, 0, 10));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
	double width = _viewport->getActualWidth();
	double height = _viewport->getActualHeight();
	_camera->setAspectRatio(width / height);


	importer = new Importer();
	scene = new Scene(_sceneMgr);
	importer->parseScene("./data/output.xml", scene);

	std::stringstream nameNode;
	std::stringstream nameEntity;
	graphNode = _sceneMgr->createSceneNode("Graph");
	graphNode->pitch(Ogre::Degree(90));
	graphNode->yaw(Ogre::Degree(180));
	graphNode->setPosition(0, 0, 0);


	scene->initMap(graphNode);
	_sceneMgr->getRootSceneNode()->addChild(graphNode);


	/*graphNode->pitch(Ogre::Degree(90));
	graphNode->yaw(Ogre::Degree(180));
	graphNode->setPosition(0, 0, 0);
	int i = 0;
	GraphVertex* actualVertex = NULL;
	for (GraphVertex* v : scene->getGraph()->getVertexes())
	{


		nameNode.str("");
		nameEntity.str("");
		nameNode << "node_" << i << '_';
		nameEntity << "entity_" << i << '_';

		Ogre::Entity* vertexEntity = _sceneMgr->createEntity(nameEntity.str(), "Cube1.mesh");
		Ogre::SceneNode* vertexNode = _sceneMgr->createSceneNode(nameNode.str());
		vertexNode->attachObject(vertexEntity);
		vertexNode->setScale(1, 1, 1);
		vertexNode->setPosition(v->getData().getPosition());
		graphNode->addChild(vertexNode);
		if (v->getData().getType() == "PacmanRespawn")
		{
			actualVertex = v;
		}
		i++;
	}
	
	_sceneMgr->getRootSceneNode()->addChild(graphNode);*/

	//Set level
	Ogre::Entity* levelEntity = _sceneMgr->createEntity("PacManLevel.mesh");
	Ogre::SceneNode* levelNode = _sceneMgr->createSceneNode("PacManNode");
	levelNode->attachObject(levelEntity);
	levelNode->setPosition(0, 0, 0);
	levelNode->pitch(Ogre::Degree(270));
	levelNode->yaw(Ogre::Degree(270));

	_sceneMgr->getRootSceneNode()->addChild(levelNode);


	//Add Pacman
	pacMan = new PacMan(_sceneMgr, "pacManNode", "pacManEntity", "Sinbad.mesh", scene->getPacManRespawn());
	pacMan->getSceneNode()->setScale(0.06, 0.06, 0.06);			
	graphNode->addChild(pacMan->getSceneNode());

	// add two lights
	Ogre::SceneNode* lights= _sceneMgr->createSceneNode("Lights");
	Ogre::Light* light1 = _sceneMgr->createLight();
	light1->setType(Ogre::Light::LT_POINT);
	light1->setPosition(-10, 100, 100);
	light1->setSpecularColour(Ogre::ColourValue::White);
	lights->attachObject(light1);	

	Ogre::Light* light2 = _sceneMgr->createLight();
	light2->setType(Ogre::Light::LT_POINT);
	light2->setPosition(10, -100, -100);
	light2->setSpecularColour(Ogre::ColourValue::White);
	lights->attachObject(light2);

	_sceneMgr->getRootSceneNode()->addChild(lights);

	_exitGame = false;
}

void
PlayState::exit()
{
	_sceneMgr->clearScene();
	delete scene;
	delete importer;
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
	// Se restaura el background colour.
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{
	pacMan->update();

	return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
	// Tecla p --> PauseState.
	if (e.key == OIS::KC_P) 
	{
		pushState(PauseState::getSingletonPtr());
	}
	if (e.key == OIS::KC_RIGHT) 
	{
		pacMan->setDirecction(Direcction::RIGHT);		
	}

	if (e.key == OIS::KC_LEFT) 
	{
		pacMan->setDirecction(Direcction::LEFT);		
	}

	if (e.key == OIS::KC_UP) 
	{
		pacMan->setDirecction(Direcction::UP);
	}
	if (e.key == OIS::KC_DOWN) 
	{
		pacMan->setDirecction(Direcction::DOWN);
	}
}


void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_ESCAPE) {
		_exitGame = true;

	}
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PlayState*
PlayState::getSingletonPtr()
{
	return msSingleton;
}

PlayState&
PlayState::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}