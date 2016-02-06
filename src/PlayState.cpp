#include "PlayState.h"
#include "PauseState.h"
#include "IntroState.h"

#include <Importer.h>
#include <Scene.h>

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;

void
PlayState::enter()
{
	_root = Ogre::Root::getSingletonPtr();

	if (_root->hasSceneManager("PlayState") && _sceneMgr->hasCamera(
		"IntroCamera")) {
		_sceneMgr = _root->getSceneManager("PlayState");
		_camera = _sceneMgr->getCamera("IntroCamera");
	}
	else {
		_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "PlayState");
		// set camera
		_camera = _sceneMgr->createCamera("IntroCamera");
	}
	// Se recupera el gestor de escena y la cámara.

	_camera->setPosition(Ogre::Vector3(0, -40, -60));
	//_camera->pitch(Ogre::Degree(90));
	_camera->lookAt(Ogre::Vector3(0, 0, 10));
	_camera->setNearClipDistance(5);
	_camera->setFarClipDistance(10000);
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));
	double width = _viewport->getActualWidth();
	double height = _viewport->getActualHeight();
	//_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Ogre::Light* light = _sceneMgr->createLight("Luz1");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(0, 150, 250);
	_sceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
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

	//Set level
	Ogre::Entity* levelEntity = _sceneMgr->createEntity("PacManLevel.mesh");
	levelNode = _sceneMgr->createSceneNode("PacManNode");
	levelNode->attachObject(levelEntity);
	levelNode->setPosition(0, 0, 0);
	levelNode->pitch(Ogre::Degree(270));
	levelNode->yaw(Ogre::Degree(270));

	_sceneMgr->getRootSceneNode()->addChild(levelNode);


	//Add Pacman
	pacMan = new PacMan(_sceneMgr, "pacManNode", "pacManEntity", "Sinbad.mesh", scene->getPacManRespawn());
	inky = new Ghost(_sceneMgr, "InkyNode", "InkyEntity", "Sinbad.mesh", scene->getGhostRespawn(0), pacMan, GhostState::EXIT, scene->getInkyHome());
	blinky = new Ghost(_sceneMgr, "BlynkyNode", "BlynkyEntity", "Sinbad.mesh", scene->getGhostRespawn(1), pacMan, GhostState::EXIT, scene->getBlinkyHome());
	clyde = new Ghost(_sceneMgr, "ClydeNode", "ClydeEntity", "Sinbad.mesh", scene->getGhostRespawn(2), pacMan, GhostState::EXIT, scene->getClydeHome());
	pinky = new Ghost(_sceneMgr, "PinkyNode", "PinkyEntity", "Sinbad.mesh", scene->getExit(), pacMan, GhostState::SCATTER, scene->getPinkyHome());

	pacMan->getSceneNode()->setScale(0.06, 0.06, 0.06);
	inky->getSceneNode()->setScale(0.06, 0.06, 0.06);
	blinky->getSceneNode()->setScale(0.06, 0.06, 0.06);
	clyde->getSceneNode()->setScale(0.06, 0.06, 0.06);
	pinky->getSceneNode()->setScale(0.06, 0.06, 0.06);
	graphNode->addChild(pacMan->getSceneNode());
	graphNode->addChild(inky->getSceneNode());
	graphNode->addChild(blinky->getSceneNode());
	graphNode->addChild(clyde->getSceneNode());
	graphNode->addChild(pinky->getSceneNode());

	// add two lights
	Ogre::SceneNode* lights = _sceneMgr->createSceneNode("Lights");
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
	_score = 0;
	winBool = false;
	loseBool = false;
	_exitGame = false;
	createGUI();
}

void
PlayState::exit()
{
	_sceneMgr->clearScene();
	delete scene;
	delete importer;
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause()
{
}

void PlayState::resume()
{
	// Se restaura el background colour.
	//_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
	createGUI();
	graphNode->setVisible(true);
	levelNode->setVisible(true);
	_scoreTextGUI->setVisible(true);
	_scoreNumberTextGUI->setVisible(true);
	_lifeText->setVisible(true);
	winBool = false;
	loseBool = false;
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{

	pacMan->update(evt);
	inky->update(evt);
	blinky->update(evt);
	clyde->update(evt);
	pinky->update(evt);

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
	if (!winBool && !loseBool && e.key == OIS::KC_ESCAPE)
	{
		graphNode->setVisible(false);
		levelNode->setVisible(false);
		_scoreTextGUI->setVisible(false);
		_scoreNumberTextGUI->setVisible(false);
		_lifeText->setVisible(false);
		pushState(PauseState::getSingletonPtr());
	}
	//TEMPORAL PARA ACCEDER A LOSE Y WIN GUI
	if (e.key == OIS::KC_W){

		_winUI->setVisible(true);
		winBool = true;
		graphNode->setVisible(false);
		levelNode->setVisible(false);
		_scoreTextGUI->setVisible(false);
		_scoreNumberTextGUI->setVisible(false);
		_lifeText->setVisible(false);
		editbox->activate();
	}
	if (e.key == OIS::KC_L){
		loseBool = true;
		_gameOverUI->setVisible(true);
		graphNode->setVisible(false);
		levelNode->setVisible(false);
		_scoreTextGUI->setVisible(false);
		_scoreNumberTextGUI->setVisible(false);
		_lifeText->setVisible(false);
	}

	if (winBool && e.key == OIS::KC_ESCAPE)
	{
		changeState(IntroState::getSingletonPtr());
	}
	if (loseBool && e.key == OIS::KC_ESCAPE)
	{
		changeState(IntroState::getSingletonPtr());
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

bool PlayState::save(const CEGUI::EventArgs &e) {
	//ReadScores
	std::ofstream _scoresTXT;
	_scoresTXT.open("scores.txt", std::ofstream::app);

	std::stringstream txt;
	txt << _nameText->getText() << " / " << _score << "\n";

	_scoresTXT << txt.str();

	_exitGame = true;
	return true;
}
void PlayState::createGUI() {

	//CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	//CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	//CEGUI::Font::setDefaultResourceGroup("Fonts");
	//CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	//CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	//CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	//CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
	//	"TaharezLook/MouseArrow");

	//// load all the fonts 
	//CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
		"DefaultWindow", "PlayState");

	//Config Window	
	playStateUI = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
		"playLayout.layout");




	_gameOverUI = playStateUI->getChild("FondoGameOver");
	_winUI = playStateUI->getChild("FondoWin");
	_scoreText = _winUI->getChild("LabelScore");
	_scoreTextGUI = playStateUI->getChild("ScoreText");
	_scoreNumberTextGUI = playStateUI->getChild("ScorePlayer");
	_lifeText = playStateUI->getChild("Life");
	_winUI->setVisible(false);
	_gameOverUI->setVisible(false);

	/*
	_resume->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&PlayState::resume, this));
	_exitPause->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&PlayState::quit, this));
	_retry->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&PlayState::retry, this));
	_exitGameOver->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&PlayState::quit, this));

	_save->subscribeEvent(CEGUI::PushButton::EventClicked,
	CEGUI::Event::Subscriber(&PlayState::save, this));

	_winUI->setVisible(false);
	_pauseUI->setVisible(false);
	_gameOverUI->setVisible(false);
	*/
	sheet->addChild(playStateUI);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);


}