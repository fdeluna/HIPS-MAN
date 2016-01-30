#include "PacMan.h"

PacMan::PacMan(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, GraphVertex* vertex)
{
	sceneNode = sceneManager->createSceneNode(nodeName);
	entity = sceneManager->createEntity(entName, mesh);
	sceneNode->setPosition(vertex->getData().getPosition());
	sceneNode->attachObject(entity);
	currentVertex = vertex;
}

PacMan::~PacMan()
{
	entity = NULL;
	sceneNode = NULL;
}


void PacMan::update()
{
	move();	
}



void PacMan::move()
{
	if (targetVertex && sceneNode->getPosition().distance(targetVertex->getData().getPosition()) > EPSILON)
	{
		sceneNode->translate(direcction);		
	}
	else
	{	
		if (targetVertex)
		{
			currentVertex = targetVertex;
		}

		if (newDirecction != currentDirecction && GraphVertex::nextVertx(newDirecction, targetVertex))
		{
			newVertex = GraphVertex::nextVertx(newDirecction, targetVertex);
		}
		else{
			targetVertex = GraphVertex::nextVertx(currentDirecction, currentVertex);
			if (!targetVertex){
				currentDirecction = Direcction::NONE;
			}
		}

		if (newVertex){
			targetVertex = newVertex;
			currentDirecction = newDirecction;
			newVertex = NULL;
		}

		if (GraphVertex::checkVertex(currentVertex, "Empty")){
			std::cout << currentVertex->getData().getType() << std::endl;
			addScore(10);			
		}				
	}

	directionEnumToVector3(currentDirecction);

}

const bool PacMan::isDead()
{
	return life <= 0 ? true : false;
}

int PacMan::getScore()
{
	return score;
}

float PacMan::getSpeed()
{
	return speed;
}

void PacMan::setScore(int points)
{
	score = points;
}

void PacMan::addScore(int points)
{
	score += points;
}

void PacMan::setSpeed(float fSpeed)
{
	speed = fSpeed;
}

void PacMan::setDirecction(Direcction dDirection)
{	
	if (currentDirecction == Direcction::NONE){
		currentDirecction = dDirection;
		newDirecction = currentDirecction;
	}
	else{
		newDirecction = dDirection;
	}
}

void PacMan::directionEnumToVector3(Direcction dDirection)
{
	switch (dDirection)
	{
	case Direcction::UP:
		direcction = MOVE_UP;
		break;
	case Direcction::DOWN:
		direcction = MOVE_DOWN;
		break;
	case Direcction::RIGHT:
		direcction = MOVE_RIGHT;
		break;
	case Direcction::LEFT:
		direcction = MOVE_LEFT;
		break;
	}
}