/* **********************************************************
** Importador NoEscape 1.0
** Curso de Experto en Desarrollo de Videojuegos
** Escuela Superior de Informatica - Univ. Castilla-La Mancha
** Carlos Gonzalez Morcillo - David Vallejo Fernandez
************************************************************/

#include <GraphVertex.h>

GraphVertex::GraphVertex
(Node& data)
:_data(data)
{
}

GraphVertex::~GraphVertex()
{
	std::vector<GraphEdge*>::iterator itEdge;
	for (itEdge = _edges.begin();
		itEdge != _edges.end();
		++itEdge)
		delete *itEdge;

	_edges.clear();
}

std::vector<GraphVertex*>
GraphVertex::adjacents() const
{
	std::vector<GraphVertex*> result;
	std::vector<GraphEdge*>::const_iterator it;

	for (it = _edges.begin();
		it != _edges.end();
		++it)
		result.push_back((*it)->getDestination());

	return result;
}

GraphVertex* GraphVertex::nextVertx(Direcction direction, GraphVertex* actualVertx)
{
	GraphVertex *nextVertex = NULL;
	if (actualVertx){
		for (GraphEdge* e : actualVertx->getEdges())
		{
			if (!checkVertex(e->getDestination(), "CageDoor"))
			{
				switch (direction)
				{

				case Direcction::UP:

					if (actualVertx->getData().getPosition().z < e->getDestination()->getData().getPosition().z)
					{
						nextVertex = e->getDestination();
					}

					break;
				case Direcction::DOWN:
					if (actualVertx->getData().getPosition().z > e->getDestination()->getData().getPosition().z)
					{
						nextVertex = e->getDestination();
					}

					break;
				case Direcction::RIGHT:

					if (actualVertx->getData().getPosition().x < e->getDestination()->getData().getPosition().x)
					{
						nextVertex = e->getDestination();
					}

					break;
				case Direcction::LEFT:
					if (actualVertx->getData().getPosition().x > e->getDestination()->getData().getPosition().x)
					{
						nextVertex = e->getDestination();
					}
					break;
				}
			}
		}
	}
	return nextVertex;
}

bool  GraphVertex::checkVertex(GraphVertex* actualVertx, std::string type)
{
	bool result = false;
	if (actualVertx)
	{
		result = actualVertx->getData().getType() == type;
	}
	return result;
}