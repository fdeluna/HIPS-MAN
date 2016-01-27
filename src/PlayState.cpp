#include "PlayState.h"
#include "PauseState.h"
#include <Importer.h>
#include <Scene.h>
#include <Camera.h>
template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter()
{
	//Importer *importer = new Importer();
	//Scene *scene = new Scene();
	importer = new Importer();
	scene = new Scene();
	importer->parseScene("./data/output1.xml", scene);
	vector<Camera*> cameras = scene->getCameras();
	movimientroOgre = (0, 0, 0);
	actualDirection = Ogre::Vector3(0, 0, 0);
	//v = scene->getGraph()->getVertex(3)->adjacents();
	//cout << "-------------------------------" << nextVertx(v, direccionOgre, scene->getGraph()->getVertex(1))->getData().getIndex() << endl;		


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




	std::stringstream nameNode;
	std::stringstream nameEntity;
	graphNode = _sceneMgr->createSceneNode("Graph");
	graphNode->pitch(Ogre::Degree(90));
	graphNode->yaw(Ogre::Degree(180));
	graphNode->setPosition(0, 0, 0);
	int i = 0;
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
		if (v->getData().getType() == "PacmanRespaw")
		{
			actualVertex = v;
		}
		i++;
	}

	adjacentsVertex = actualVertex->adjacents();
	std::cout << "**** SIZE ****" << std::endl;
	std::cout << adjacentsVertex.size() << std::endl;
	lastDirection = actualDirection;
	//nextVertex = nextVertx(adjacentsVertex, actualDirection, actualVertex);
	_sceneMgr->getRootSceneNode()->addChild(graphNode);

	//Set level
	Ogre::Entity* levelEntity = _sceneMgr->createEntity("PacManLevel.mesh");
	Ogre::SceneNode* levelNode = _sceneMgr->createSceneNode("PacManNode");
	levelNode->attachObject(levelEntity);
	levelNode->setPosition(0, 0, 0);
	levelNode->pitch(Ogre::Degree(270));
	levelNode->yaw(Ogre::Degree(270));

	_sceneMgr->getRootSceneNode()->addChild(levelNode);




	//Añadir un Objeto
	Ogre::Entity* ent1 = _sceneMgr->createEntity("Sinbad.mesh");
	node1 = _sceneMgr->createSceneNode("SinbadNode");
	node1->attachObject(ent1);
	node1->setScale(0.06, 0.06, 0.06);
	Node nod5 = actualVertex->getData();
	Ogre::Vector3 position3 = nod5.getPosition();

	for (GraphEdge* e : actualVertex->getEdges())
	{
		std::cout << "Edge origen Index :" << e->getOrigin()->getData().getIndex() << std::endl;
		std::cout << "Edge origen position:" << e->getOrigin()->getData().getPosition() << std::endl;
		std::cout << "Edge destino Index :" << e->getDestination()->getData().getIndex() << std::endl;
		std::cout << "Edge destino position:" << e->getDestination()->getData().getPosition() << std::endl;

	}

	nextVertex = actualVertex;
	node1->setPosition(position3);
	graphNode->addChild(node1);

	// add two lights
	Ogre::Light* light1 = _sceneMgr->createLight();
	light1->setType(Ogre::Light::LT_POINT);
	light1->setPosition(-10, 100, 100);
	light1->setSpecularColour(Ogre::ColourValue::White);
	node1->attachObject(light1);
	cout << "-------------------------------" << node1->getPosition() << endl;

	Ogre::Light* light2 = _sceneMgr->createLight();
	light2->setType(Ogre::Light::LT_POINT);
	light2->setPosition(10, -100, -100);
	light2->setSpecularColour(Ogre::ColourValue::White);
	node1->attachObject(light2);
	// Nuevo background colour.
	// _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));

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
	if (nextVertex && node1->getPosition().distance(nextVertex->getData().getPosition()) > epsilon)
	{		
		node1->translate(actualDirection);
		entreNodos = true;		
	}
	else
	{		
		if (nextVertex){
			actualVertex = nextVertex;
		}
		if (lastDirection != actualDirection && nextVertx(lastDirection, nextVertex))
		{
			targetVertex = nextVertx(lastDirection, nextVertex);
		}
		else{
			nextVertex = nextVertx(actualDirection, actualVertex);						
			lastDirection = actualDirection;
			if (!nextVertex){
				actualDirection = Ogre::Vector3(0, 0, 0);
			}
		}

		if (targetVertex){
			nextVertex = targetVertex;
			actualDirection = lastDirection;
			targetVertex = NULL;
		}		
	}

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
	if (e.key == OIS::KC_P) {
		pushState(PauseState::getSingletonPtr());
	}
	if (e.key == OIS::KC_RIGHT) {
		std::cout << "PRESSED" << std::endl;

		if (actualDirection == Ogre::Vector3(0, 0, 0)){
			actualDirection = Ogre::Vector3(0.01, 0, 0);
			lastDirection = actualDirection;
		}
		else
		{
			lastDirection = Ogre::Vector3(0.01, 0, 0);
		}
	}
	if (e.key == OIS::KC_LEFT) {
		if (actualDirection == Ogre::Vector3(0, 0, 0))
		{
			actualDirection = Ogre::Vector3(-0.01, 0, 0);
			lastDirection = actualDirection;
		}
		else
		{
			lastDirection = Ogre::Vector3(-0.01, 0, 0);
		}

		/*nextVertex = nextVertx(lastDirection, actualVertex);
		if (nextVertex){
		actualDirection = lastDirection;
		}*/
	}
	if (e.key == OIS::KC_UP) {

		if (actualDirection == Ogre::Vector3(0, 0, 0)){
			actualDirection = Ogre::Vector3(0, 0, 0.01);
			lastDirection = actualDirection;
		}
		else{
			lastDirection = Ogre::Vector3(0, 0, 0.01);
		}

	}
	if (e.key == OIS::KC_DOWN) {

		if (actualDirection == Ogre::Vector3(0, 0, 0)){
			actualDirection = Ogre::Vector3(0, 0, -0.01);
			lastDirection = actualDirection;
		}
		else{
			lastDirection = Ogre::Vector3(0, 0, -0.01);
		}
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

GraphVertex*
PlayState::nextVertx
(Ogre::Vector3 direction, GraphVertex* actualVertx)
{
	Node nextNode;
	//GraphVertex *nextVertex = new GraphVertex(nextNode);
	GraphVertex *nextVertex = NULL;
	//std::cout << actualVertex->getEdges().size();
	if (actualVertx){

		std::cout << "Actual Index :" << actualVertx->getData().getIndex() << std::endl;
		std::cout << "Actual Position :" << actualVertx->getData().getPosition() << std::endl;
		for (GraphEdge* e : actualVertex->getEdges())
		{
			// LEFT
			if (direction.x < 0)
			{
				if (actualVertx->getData().getPosition().x > e->getDestination()->getData().getPosition().x){
					//nextNode = adjacents[i]->getData();
					nextVertex = e->getDestination();					
					//return nextVertex;
				}
			}

			// RIGHT
			if (direction.x > 0)
			{
				if (actualVertx->getData().getPosition().x < e->getDestination()->getData().getPosition().x){
					//nextNode = adjacents[i]->getData();
					nextVertex = e->getDestination();					
					//return nextVertex;
				}
			}

			// UP
			if (direction.z > 0)
			{
				if (actualVertx->getData().getPosition().z < e->getDestination()->getData().getPosition().z){
					//nextNode = adjacents[i]->getData();
					nextVertex = e->getDestination();
					
					//return nextVertex;
				}
			}

			//down
			if (direction.z < 0)
			{
				if (actualVertx->getData().getPosition().z > e->getDestination()->getData().getPosition().z){
					//nextNode = adjacents[i]->getData();
					nextVertex = e->getDestination();
					
					//return nextVertex;
				}
			}
			

		}		
	}

	return nextVertex;

}
void PlayState::movePlayer(){


}