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
	importer->parseScene("./data/sample.xml", scene);
	vector<Camera*> cameras = scene->getCameras();
	movimientroOgre = (0, 0, 0);
	actualDirection = Ogre::Vector3(0.005, 0, 0);
	Node nod3 = scene->getGraph()->getVertex(3)->getData();
	Node nod1 = scene->getGraph()->getVertex(1)->getData();
	//v = scene->getGraph()->getVertex(3)->adjacents();
	//cout << "-------------------------------" << nextVertx(v, direccionOgre, scene->getGraph()->getVertex(1))->getData().getIndex() << endl;
	actualVertex = scene->getGraph()->getVertex(5);
	adjacentsVertex = actualVertex->adjacents();
	lastDirection = Ogre::Vector3(0.005, 0, 0);
	nextVertex = nextVertx(adjacentsVertex, actualDirection, actualVertex);
	cout << "-------------------------------" << actualDirection << endl;
	cout << "-------------------------------" << actualVertex->getData().getIndex() << endl;
	cout << "-------------------------------" << nextVertex->getData().getIndex() << endl;
	cout << "-------------------------------" << nextVertex->getData().getPosition() << endl;


	Ogre::Vector3 position2 = nod1.getPosition();


	_root = Ogre::Root::getSingletonPtr();

	// Se recupera el gestor de escena y la cámara.
	_sceneMgr = _root->getSceneManager("SceneManager");
	_camera = _sceneMgr->getCamera("IntroCamera");
	_camera->setPosition(Ogre::Vector3(5, 100, 100));
	_camera->lookAt(Ogre::Vector3(0, 0, 0));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
	double width = _viewport->getActualWidth();
	double height = _viewport->getActualHeight();
	_camera->setAspectRatio(width / height);

	//Añadir un Objeto
	Ogre::Entity* ent1 = _sceneMgr->createEntity("Sinbad.mesh");
	node1 = _sceneMgr->createSceneNode("SinbadNode");
	node1->attachObject(ent1);
	_sceneMgr->getRootSceneNode()->addChild(node1);
	node1->setScale(0.2, 0.2, 0.2);
	Node nod5 = scene->getGraph()->getVertex(5)->getData();
	Ogre::Vector3 position3 = nod3.getPosition();

	node1->setPosition(-0.5,1,0);
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
	

	cout << "-------------------------------" << node1->getPosition() << endl;


	if (node1->getPosition().distance(nextVertex->getData().getPosition()) < epsilon){
		cout << "-------------------------------" << "misma posicion" << endl;

		actualDirection = lastDirection;
		cout << "-------------------------------" << lastDirection << endl;
		actualVertex = nextVertex;
		cout << "-------------------------------" << actualVertex->getData().getIndex() << endl;

		adjacentsVertex = actualVertex->adjacents();
		nextVertex = nextVertx(adjacentsVertex, actualDirection, actualVertex);
	}
	
	if (canNext == false){
		actualDirection = (0, 0, 0);
	}
	node1->translate(actualDirection);
	
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
		//direccionOgre = Ogre::Vector3(0.005,0,0);
		lastDirection = Ogre::Vector3(0.005, 0, 0);
		//nextVertex = nextVertx(v, direccionOgre, scene->getGraph()->getVertex(3));
		adjacentsVertex = actualVertex->adjacents();
		nextVertex = nextVertx(adjacentsVertex, lastDirection, actualVertex);
		if (canNext == true){
			actualDirection = lastDirection;
		}
	}
	if (e.key == OIS::KC_LEFT) {
		//direccionOgre = Ogre::Vector3(-0.005, 0, 0);
		lastDirection = Ogre::Vector3(-0.005, 0, 0);
		//nextVertex = nextVertx(v, direccionOgre, scene->getGraph()->getVertex(3));
		adjacentsVertex = actualVertex->adjacents();
		nextVertex = nextVertx(adjacentsVertex, lastDirection, actualVertex);
		if (canNext == true){
			actualDirection = lastDirection;
		}
	}
	if (e.key == OIS::KC_UP) {
		//direccionOgre = Ogre::Vector3(0 ,0, -0.005);
		lastDirection = Ogre::Vector3(0, 0, -0.005);
		//nextVertex = nextVertx(v, direccionOgre, scene->getGraph()->getVertex(3));
		adjacentsVertex = actualVertex->adjacents();
		nextVertex = nextVertx(adjacentsVertex, lastDirection, actualVertex);
		if (canNext == true){
			actualDirection = lastDirection;
		}

	}
	if (e.key == OIS::KC_DOWN) {
		//direccionOgre = Ogre::Vector3(0, 0, +0.005);
		lastDirection = Ogre::Vector3(0, 0, 0.005);
		//nextVertex = nextVertx(v, direccionOgre, scene->getGraph()->getVertex(3));
		adjacentsVertex = actualVertex->adjacents();
		nextVertex = nextVertx(adjacentsVertex, lastDirection, actualVertex);
		cout << "-------------------------------" << nextVertex->getData().getPosition() << endl;
		cout << "-------------------------------" << nextVertex->getData().getIndex() << endl;

		if (canNext == true){
			actualDirection = lastDirection;
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
(std::vector<GraphVertex*> adjacents, Ogre::Vector3 direction, GraphVertex* actualVertx)
{
	Node nextNode;
	GraphVertex *nextVertex = new GraphVertex(nextNode);
	for (int i = 0; i < adjacents.size(); i++)
	{

		if (direction.x > 0){

			if (actualVertx->getData().getPosition().x < adjacents[i]->getData().getPosition().x){
				nextNode = adjacents[i]->getData();
				nextVertex = adjacents[i];
				canNext = true;
				cout << "-------------------------------" << "Derecha" << endl;
				return nextVertex;
			}
			
		}else if (direction.x < 0){

			if (actualVertx->getData().getPosition().x > adjacents[i]->getData().getPosition().x){
				nextNode = adjacents[i]->getData();
				nextVertex = adjacents[i];
				canNext = true;
				cout << "-------------------------------" << "Izq" << endl;

				return nextVertex;
			}
			
		}else if (direction.z > 0){

			if (actualVertx->getData().getPosition().y > adjacents[i]->getData().getPosition().y){
				nextNode = adjacents[i]->getData();
				nextVertex = adjacents[i];
				canNext = true;
				cout << "-------------------------------" << "abajo" << endl;

				return nextVertex;
			}
			
		}else if (direction.z < 0){

			if (actualVertx->getData().getPosition().y < adjacents[i]->getData().getPosition().y){
				nextNode = adjacents[i]->getData();
				nextVertex = adjacents[i];
				canNext = true;
				cout << "-------------------------------" << "arriba" << endl;

				return nextVertex;
			}
			
		}
		
			

		

	


	}
	canNext = false;
	cout << "-------------------------------" << "nopuede" << endl;
	return nextVertex;
	

}
void PlayState::movePlayer(){


}