/*********************************************************************
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

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include "GameState.h"

class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:
  IntroState() {}

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
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  bool _exitGame;

  //PARA CEGUI
  CEGUI::OgreRenderer* renderer;
  CEGUI::Window* introStateUI;
  CEGUI::Window* exitButton;
  CEGUI::Window* playButton;
  CEGUI::Window* highscoreButton;
  CEGUI::Window* creditsButton;
  CEGUI::Window* pacmanTittle;
  CEGUI::Window* fondoCredits;
  CEGUI::Window* fondoScore;
  CEGUI::Window* select;
  CEGUI::Window* intro;
  CEGUI::Window* _scoreText;

  bool _play;
  bool _highscore;
  bool _credits;

  float _timeSinceLastFrame;

  bool play(const CEGUI::EventArgs &e);
  bool quit(const CEGUI::EventArgs &e);
  bool highscore(const CEGUI::EventArgs &e);
  bool credits(const CEGUI::EventArgs &e);

  void createGUI();
  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);

};

#endif
