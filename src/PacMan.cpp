#include "PacMan.h"

PacMan::PacMan(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, GraphVertex* vertex)
{
	sceneNode = sceneManager->createSceneNode(nodeName);
	entity = sceneManager->createEntity(entName, mesh);
	sceneNode->setPosition(vertex->getData().getPosition());
	sceneNode->attachObject(entity);
	currentVertex = vertex;
	life = 3;
}

PacMan::~PacMan()
{
	entity = NULL;
	sceneNode = NULL;
}


void PacMan::update(const Ogre::FrameEvent& evt)
{	
	move();

	if (Scene::getSingletonPtr()->contaisMapItem(currentVertex))
	{
		scared = false;
		if (GraphVertex::checkVertex(currentVertex, "Empty"))
		{
			addScore(10);
		}
		else if (GraphVertex::checkVertex(currentVertex, "Pill"))
		{
			scared = true;
			addScore(20);
		}
		Scene::getSingletonPtr()->removeMapItem(currentVertex);
	}
}



void PacMan::move()
{
	if (targetVertex && sceneNode->getPosition().distance(targetVertex->getData().getPosition()) > EPSILON)
	{
		sceneNode->translate(direcction * speed);
	}
	else
	{
		if (targetVertex)
		{
			currentVertex = targetVertex;
		}
		if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getRightTeleport()->getData().getIndex())
		{

			currentVertex = Scene::getSingletonPtr()->getLeftTeleport();
			speed = 0.75;
		}
		else if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getLeftTeleport()->getData().getIndex())
		{
			currentVertex = Scene::getSingletonPtr()->getRightTeleport();
			speed = 0.75;
		}
		sceneNode->setPosition(currentVertex->getData().getPosition());

		if (newDirecction != currentDirecction && GraphVertex::nextVertx(newDirecction, targetVertex))
		{
			newVertex = GraphVertex::nextVertx(newDirecction, targetVertex);
		}
		else
		{
			targetVertex = GraphVertex::nextVertx(currentDirecction, currentVertex);
			if (!targetVertex)
			{
				currentDirecction = Direcction::NONE;
			}
		}

		if (newVertex)
		{
			targetVertex = newVertex;
			currentDirecction = newDirecction;
			newVertex = NULL;
		}
	}
	directionEnumToVector3(currentDirecction);
}

const bool PacMan::isDead()
{
	return life == 0 ? true : false;
}

int PacMan::getScore()
{
	return score;
}

float PacMan::getSpeed()
{
	return speed;
}

int PacMan::getLife()
{
	return life;
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
void PacMan::die()
{
	life--;
	dead = true;
}

void PacMan::init(GraphVertex* vertex)
{
	currentVertex = vertex;
	sceneNode->setPosition(vertex->getData().getPosition());
	currentVertex = vertex;
	targetVertex = NULL;
	currentDirecction = Direcction::NONE;
	newDirecction = currentDirecction;
	dead = false;
}

void PacMan::setDirecction(Direcction dDirection)
{
	if (currentDirecction == Direcction::NONE)
	{
		currentDirecction = dDirection;
		newDirecction = currentDirecction;
	}
	else
	{
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

bool PacMan::scaredGhost()
{
	return scared;
}

bool PacMan::killed()
{
	return dead;
}
