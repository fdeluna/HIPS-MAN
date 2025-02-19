#include <Graph.h>

Graph::Graph()
{
	// Reserva de memoria inicial.
	_vertexes.reserve(25);
	_edges.reserve(25);
}

Graph::~Graph()
{
	// Liberar vértices.
	std::vector<GraphVertex*>::iterator itVert;
	for (itVert = _vertexes.begin();
		itVert != _vertexes.end();
		++itVert)
		delete *itVert;

	// Liberar arcos.
	std::vector<GraphEdge*>::iterator itEdge;
	for (itEdge = _edges.begin();
		itEdge != _edges.end();
		++itEdge)
		delete *itEdge;

	_vertexes.clear();
	_edges.clear();
}

void Graph::addVertex(GraphVertex* pVertex)
{
	_vertexes.push_back(pVertex);
}

void Graph::addEdge(GraphVertex* pOrigin, GraphVertex* pDestination, bool undirected)
{
	GraphEdge* pEdge = new GraphEdge(pOrigin, pDestination);
	_edges.push_back(pEdge);
	pOrigin->addEdge(pEdge);

	if (undirected) {
		GraphEdge* pEdge2 = new GraphEdge(pDestination, pOrigin);
		_edges.push_back(pEdge2);
		pDestination->addEdge(pEdge2);
	}
}

std::vector<GraphVertex*>
Graph::adjacents
(int index) const
{
	std::vector<GraphVertex*> result;
	std::vector<GraphVertex*>::const_iterator it;

	for (it = _vertexes.begin();
		it != _vertexes.end();
		++it)
		if ((*it)->getData().getIndex() == index)
			return (*it)->adjacents();

	return result;
}

GraphVertex* Graph::getVertex(int index) const
{
	std::vector<GraphVertex*>::const_iterator it;

	for (it = _vertexes.begin();
		it != _vertexes.end();
		++it)
		if ((*it)->getData().getIndex() == index)
			return (*it);

	return NULL;
}
