/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef IMPORTER_H
#define IMPORTER_H

#include <OGRE/Ogre.h>
#include <xercesc/dom/DOM.hpp>
#include <Scene.h>

class Importer: public Ogre::Singleton<Importer> {
 public:
  void parseScene (const char* path, Scene *scene);
  
  static Importer& getSingleton ();
  static Importer* getSingletonPtr ();
  
 private:    
  void parseGraph (xercesc::DOMNode* graphNode, Scene* scene);
  void addVertexToScene (xercesc::DOMNode* vertexNode, Scene* scene);
  void addEdgeToScene (xercesc::DOMNode* edgeNode, Scene* scene);

  float getValueFromTag (xercesc::DOMNode* node, const XMLCh *tag);
};

#endif
