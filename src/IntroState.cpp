#include "IntroState.h"
#include "PlayState.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

void IntroState::enter()
{
	_root = Ogre::Root::getSingletonPtr();

	if (_root->hasSceneManager("IntroState") && _sceneMgr->hasCamera(
		"IntroCamera")) {
		_sceneMgr = _root->getSceneManager("IntroState");
		_camera = _sceneMgr->getCamera("IntroCamera");
	}
	else {
		_sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "IntroState");
		//Inicializacion de CEGUI
		renderer = &CEGUI::OgreRenderer::bootstrapSystem();
		// set camera
		_camera = _sceneMgr->createCamera("IntroCamera");
	}
	_viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
	_viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 0.0));

	_exitGame = false;

	//CEGUI
	createGUI();
}

void IntroState::exit()
{
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause()
{
}

void IntroState::resume()
{
}

bool IntroState::frameStarted(const Ogre::FrameEvent& evt)
{
	_timeSinceLastFrame = evt.timeSinceLastFrame;
	CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(
		_timeSinceLastFrame);
	return true;
}

bool IntroState::frameEnded(const Ogre::FrameEvent& evt)
{
	if (_exitGame)
		return false;

	return true;
}

void IntroState::keyPressed(const OIS::KeyEvent &e)
{

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(
		static_cast<CEGUI::Key::Scan> (e.key));
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);

	if (!_credits && !_highscore && e.key == OIS::KC_ESCAPE) {
		_exitGame = true;
	}
	if (_credits && e.key == OIS::KC_ESCAPE) {

		exitButton->setVisible(true);
		playButton->setVisible(true);
		highscoreButton->setVisible(true);
		creditsButton->setVisible(true);
		pacmanTittle->setVisible(true);
		fondoCredits->setVisible(false);
		fondoScore->setVisible(false);
		_credits = false;

	}
	if (e.key == OIS::KC_ESCAPE && _highscore) {

		introStateUI->setVisible(true);
		exitButton->setVisible(true);
		playButton->setVisible(true);
		highscoreButton->setVisible(true);
		creditsButton->setVisible(true);
		pacmanTittle->setVisible(true);
		fondoCredits->setVisible(false);
		fondoScore->setVisible(false);
		_highscore = false;

	}

}

void IntroState::keyReleased(const OIS::KeyEvent &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(
		static_cast<CEGUI::Key::Scan> (e.key));
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(
		e.state.X.rel, e.state.Y.rel);
}

void IntroState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(
		convertMouseButton(id));
}

void IntroState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(
		convertMouseButton(id));
}

IntroState* IntroState::getSingletonPtr()
{
	return msSingleton;
}

IntroState& IntroState::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}

bool IntroState::play(const CEGUI::EventArgs &e) {

	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	changeState(PlayState::getSingletonPtr());

	return true;
}
bool IntroState::quit(const CEGUI::EventArgs &e) {
	_exitGame = true;
	_sceneMgr->clearScene();
	_root->getAutoCreatedWindow()->removeAllViewports();
	return true;
}
bool IntroState::highscore(const CEGUI::EventArgs &e) {
	_highscore = true;

	exitButton->setVisible(false);
	playButton->setVisible(false);
	highscoreButton->setVisible(false);
	creditsButton->setVisible(false);
	pacmanTittle->setVisible(false);
	fondoCredits->setVisible(false);
	fondoScore->setVisible(true);


	std::ifstream scoreFile;
	scoreFile.open("scores.txt", std::ifstream::in);
	std::stringstream scores;
	std::string scoreResult;
	std::pair<int, std::string> pair;
	std::vector<std::pair<int, std::string>> vectorPairs;

	while (std::getline(scoreFile, scoreResult)){
		std::string name = "";
		int score = 0;

		name = scoreResult.substr(0, scoreResult.find("/", 0));
		score = atoi(scoreResult.substr(scoreResult.find("/", 0) + 1, scoreResult.length()).c_str());
		pair = std::pair<int, std::string>(score, name);
		vectorPairs.push_back(pair);
	}

	scoreFile.close();

	for (std::pair<int, std::string> p : vectorPairs)
	{
		scores << p.second << "................................................ " << p.first << "\n";
	}

	_scoreText->setText(scores.str());
	_scoreText->setVisible(true);
	return true;
}
bool IntroState::credits(const CEGUI::EventArgs &e) {
	_credits = true;
	exitButton->setVisible(false);
	playButton->setVisible(false);
	highscoreButton->setVisible(false);
	creditsButton->setVisible(false);
	pacmanTittle->setVisible(false);
	fondoCredits->setVisible(true);
	fondoScore->setVisible(false);
	/*
	_credits = true;
	_highscore = false;
	return true;
	*/
	return true;
}


void IntroState::createGUI() {

	//CEGUI	
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(
		"DejaVuSans-12");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage(
		"TaharezLook/MouseArrow");

	//Sheet
	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow(
		"DefaultWindow", "Ex1/Sheet");

	introStateUI = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(
		"introLayout.layout");

	exitButton = introStateUI->getChild("Exit");
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&IntroState::quit, this));
	playButton = introStateUI->getChild("Play");
	playButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&IntroState::play, this));
	highscoreButton = introStateUI->getChild("Score");
	highscoreButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&IntroState::highscore, this));
	creditsButton = introStateUI->getChild("Credits");
	creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&IntroState::credits, this));
	pacmanTittle = introStateUI->getChild("PacManTittle");
	fondoCredits = introStateUI->getChild("fondoCredits");
	fondoScore = introStateUI->getChild("fondoScore");
	_scoreText = fondoScore->getChild("scoreText");

	sheet->addChild(introStateUI);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

}

//Conversion mouse
CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id) {
	CEGUI::MouseButton ceguiId;
	switch (id) {
	case OIS::MB_Left:
		ceguiId = CEGUI::LeftButton;
		break;
	case OIS::MB_Right:
		ceguiId = CEGUI::RightButton;
		break;
	case OIS::MB_Middle:
		ceguiId = CEGUI::MiddleButton;
		break;
	default:
		ceguiId = CEGUI::LeftButton;
	}
	return ceguiId;
}