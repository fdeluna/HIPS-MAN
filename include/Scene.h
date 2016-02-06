/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <Graph.h>
#include <GraphVertex.h>
#include <Ogre.h>

class Scene : public Ogre::Singleton<Scene>
{
 public:
  Scene ();
  Scene(Ogre::SceneManager* sceneManager);
  ~Scene ();
    
  Graph* getGraph () const { return _graph;}    

  void initMap(Ogre::SceneNode* node);

  GraphVertex* getRightTeleport();
  GraphVertex* getLeftTeleport();
  GraphVertex* getBonus();
  GraphVertex* getExit();
  GraphVertex* getClydeHome();
  GraphVertex* getInkyHome();
  GraphVertex* getBlinkyHome();
  GraphVertex* getPinkyHome();
  GraphVertex* getPacManRespawn();  
  GraphVertex* getGhostRespawn(int index);

  void removeMapItem(GraphVertex* index);
  bool contaisMapItem(GraphVertex* index);



  // Heredados de Ogre::Singleton.
  static Scene& getSingleton();
  static Scene* getSingletonPtr();

    
 private:
  Graph *_graph;  
  std::vector<int> mapItems;
  std::vector<GraphVertex*> ghostRespawn;

  GraphVertex* pacManRespawn;
  GraphVertex* lTeleport;
  GraphVertex* rTeleport;
  GraphVertex* bonus;
  GraphVertex* exit;

  GraphVertex* clydeHome;
  GraphVertex* inkyHome;
  GraphVertex* blinkyHome;
  GraphVertex* pinkyHome;

  Ogre::SceneManager* _sceneManager;

};

#endif

