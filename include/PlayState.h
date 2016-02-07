/*********************************************************************
* Módulo 1. Curso de Experto en Desarrollo de Videojuegos
* Autor: David Vallejo Fernández    David.Vallejo@uclm.es
*
* Código modificado a partir de Managing Game States with OGRE
* http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
* Inspirado en Managing Game States in C++
* http://gamedevgeek.com/tutorials/managing-game-states-in-c/
*
* You can redistribute and/or modify this file under the terms of the
* GNU General Public License ad published by the Free Software
* Foundation, either version 3 of the License, or (at your option)
* and later version. See <http://www.gnu.org/licenses/>.
*
* This file is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*********************************************************************/

#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <Importer.h>
#include <Scene.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "PacMan.h"
#include "Ghost.h"
#include "GameState.h"
#include "Enums.h"

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
public:
	PlayState() {}

	void enter();
	void exit();
	void pause();
	void resume();

	void keyPressed(const OIS::KeyEvent &e);
	void keyReleased(const OIS::KeyEvent &e);

	void mouseMoved(const OIS::MouseEvent &e);
	void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

	// Heredados de Ogre::Singleton.
	static PlayState& getSingleton();
	static PlayState* getSingletonPtr();
	CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

	GraphVertex* nextVertx(Ogre::Vector3 direction, GraphVertex* actualVertx);
	void movePlayer();
	void createGUI();

private:
	Ogre::Root* _root;
	Ogre::SceneManager* _sceneMgr;
	Ogre::Viewport* _viewport;
	Ogre::Camera* _camera;
	Importer* importer;
	Scene* scene;
	bool _exitGame;		
	
	Ogre::SceneNode* graphNode;		
	Ogre::SceneNode*levelNode;

	PacMan* pacMan;
	Ghost* inky;
	Ghost* blinky;
	Ghost* clyde;
	Ghost* pinky;
	int _score = 0;
	float _timeGetReady = 0;

	//ESTADOS
	bool winBool = false;
	bool loseBool = false;
	GameFlow gameState = GameFlow::INIT;

	// CEGUI
	

	bool quit(const CEGUI::EventArgs &e);
	
	bool save(const CEGUI::EventArgs &e);

	CEGUI::Window* playStateUI;
	CEGUI::Window* _gameOverUI;
	CEGUI::Window* _winUI;
	CEGUI::Window* _exitGameOver;
	CEGUI::Window* _scoreText;
	CEGUI::Window* _scoreTextLose;
	CEGUI::Window* _getReadyText;
	CEGUI::Window* _scoreTextGUI;
	CEGUI::Window* _scoreNumberTextGUI;
	CEGUI::Window* _lifeText;
	CEGUI::Window*	_nameText;
	CEGUI::Window*	_nameTextLose;

	CEGUI::Window* _saveGameOver;
	CEGUI::Window* _saveWin;
	CEGUI::Window* _heart1;
	CEGUI::Window* _heart2;
	CEGUI::Window* _heart3;


	CEGUI::Window* _turnText;
	




	// Time match for cegui GUI
	float _time = 0;
};

#endif