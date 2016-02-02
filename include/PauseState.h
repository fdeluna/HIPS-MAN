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

#ifndef PauseState_H
#define PauseState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "GameState.h"

class PauseState : public Ogre::Singleton<PauseState>, public GameState
{
 public:
  PauseState() {}

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  // Heredados de Ogre::Singleton.
  static PauseState& getSingleton ();
  static PauseState* getSingletonPtr ();
  void createGUI();
  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;
  bool _resumeGame;
  // CEGUI
  CEGUI::Window* playStateUI;

  CEGUI::Window* _winUI;
  CEGUI::Window* _pauseUI;
  CEGUI::Window* _resume;
  CEGUI::Window* _exitPause;
  CEGUI::Window* _exitGameOver;
  CEGUI::Window* _retry;
  CEGUI::Window* _scoreText;
  CEGUI::Window*	_nameText;
  CEGUI::Window* _save;
  CEGUI::Window* _turnText;
  CEGUI::Window* _scoreTextGUI;
  CEGUI::Window* _scoreNumberTextGUI;
  CEGUI::Window* _lifeText;
  CEGUI::Window* _gameOverUI;
  CEGUI::Window* _heart1;
  CEGUI::Window* _heart2;
  CEGUI::Window* _heart3;

  bool quit(const CEGUI::EventArgs &e);
  bool resume(const CEGUI::EventArgs &e);
};

#endif
