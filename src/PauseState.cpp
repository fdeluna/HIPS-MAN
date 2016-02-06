#include "PauseState.h"

template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;

void
PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  if (_root->hasSceneManager("PauseState") && _sceneMgr->hasCamera(
	  "IntroCamera")) {
	  _sceneMgr = _root->getSceneManager("PauseState");
	  _camera = _sceneMgr->getCamera("IntroCamera");
  }
  else {
	  _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "PauseState");
	  // set camera
	  _camera = _sceneMgr->createCamera("IntroCamera");
  }

  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.

  _exitGame = false;
  createGUI();

}


void
PauseState::exit ()
{
}

void
PauseState::pause ()
{
}

void
PauseState::resume ()
{
}

bool
PauseState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PauseState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PauseState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  
  if (e.key == OIS::KC_ESCAPE) {
	  _exitGame = true;

  }
  if (e.key == OIS::KC_R) {
	  _pauseUI->setVisible(false);
	  popState();
	 
  }
}

void
PauseState::keyReleased
(const OIS::KeyEvent &e)
{
	
}

void
PauseState::mouseMoved
(const OIS::MouseEvent &e)
{
}

void
PauseState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

void
PauseState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
}

PauseState*
PauseState::getSingletonPtr ()
{
return msSingleton;
}

PauseState&
PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}
void PauseState::createGUI() {

	/*CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
		"TaharezLook/MouseArrow");*/

	// load all the fonts 
	//CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
		"DefaultWindow", "Pause");

	//Config Window	
	playStateUI = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
		"playLayout.layout");


	_pauseUI = playStateUI->getChild("FondoPause");
	_pauseUI->setVisible(true);
	_gameOverUI = playStateUI->getChild("FondoGameOver");
	_winUI = playStateUI->getChild("FondoWin");
	_scoreText = _winUI->getChild("LabelScore");
	_scoreTextGUI = playStateUI->getChild("ScoreText");
	_scoreNumberTextGUI = playStateUI->getChild("ScorePlayer");
	_lifeText = playStateUI->getChild("Life");

	_scoreTextGUI->setVisible(false);
	_scoreNumberTextGUI->setVisible(false);
	_lifeText->setVisible(false); 
	//_winUI->setVisible(false);
	//_gameOverUI->setVisible(false);
	/*
	_resume = _pauseUI->getChild("Resume");
	_exitPause = _pauseUI->getChild("Exit");
	_retry = _gameOverUI->getChild("Retry");
	_exitGameOver = _gameOverUI->getChild("Exit");

	_nameText = _winUI->getChild("NameText");
	_save = _winUI->getChild("Save");

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
	_nameText->setVisible(false);
	*/
	sheet->addChild(playStateUI);

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);


}