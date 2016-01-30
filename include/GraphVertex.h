/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos 
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#ifndef GRAPHVERTEX_H
#define GRAPHVERTEX_H

#include <iostream>
#include <vector>
#include <GraphEdge.h>
#include <Node.h>
#include <Enums.h>

using namespace std;

class GraphEdge;

class GraphVertex
{
 public:
  GraphVertex (const Node& data);
  ~GraphVertex ();

  Node getData () const { return _data; }
  void addEdge (GraphEdge* pEdge) { _edges.push_back(pEdge); }

  std::vector<GraphEdge*> getEdges () const { return _edges; }
  std::vector<GraphVertex*> adjacents () const;

  static GraphVertex* nextVertx(Direcction direction, GraphVertex* actualVertx);
  static bool checkVertex(GraphVertex* actualVertx, std::string type);
  
 private:
  Node _data;
  std::vector<GraphEdge*> _edges;
};

#endif
