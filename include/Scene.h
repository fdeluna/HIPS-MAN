/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <Camera.h>
#include <Graph.h>
#include <GraphVertex.h>
#include <Ogre.h>

class Scene : public Ogre::Singleton<Scene>
{
 public:
  Scene ();
  Scene(Ogre::SceneManager* sceneManager);
  ~Scene ();
  
  void addCamera (Camera* camera);    
  Graph* getGraph () const { return _graph;}  
  std::vector<Camera*> getCameras () const { return _cameras; }

  void initMap(Ogre::SceneNode* node);

  GraphVertex* getRightTeleport();
  GraphVertex* getLeftTeleport();
  GraphVertex* getBonus();
  GraphVertex* getPacManRespawn();

  void removeMapItem(GraphVertex* index);
  bool contaisMapItem(GraphVertex* index);



  // Heredados de Ogre::Singleton.
  static Scene& getSingleton();
  static Scene* getSingletonPtr();

    
 private:
  Graph *_graph;
  std::vector<Camera*> _cameras;
  std::vector<int> mapItems;
  std::vector<Ogre::Vector3> ghostRespawn;

  GraphVertex* pacManRespawn;
  GraphVertex* lTeleport;
  GraphVertex* rTeleport;
  GraphVertex* bonus;

  Ogre::SceneManager* _sceneManager;

};

#endif

