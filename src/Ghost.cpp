#include "Ghost.h"

Ghost::Ghost(SceneManager* sceneManager, std::string nodeName, std::string entName, std::string mesh, PacMan* pacman, GraphVertex* hHome)
{
	sceneNode = sceneManager->createSceneNode(nodeName);
	entity = sceneManager->createEntity(entName, mesh);
	sceneNode->attachObject(entity);
	sceneNode->pitch(Ogre::Degree(180));
	pacMan = pacman;
	finalVertex = pacman->getCurrentVertex();
	currentDirecction = GraphVertex::Direcction::UP;

	targetVertex = NULL;
	home = hHome;

}

Ghost::~Ghost()
{
	entity = NULL;
	sceneNode = NULL;
}


void Ghost::update(const Ogre::FrameEvent& evt)
{
	time += evt.timeSinceLastFrame;
	switch (state)
	{
	case GhostState::WAIT:
		wait();
		break;
	case GhostState::CHASE:
		speed = 0.75;
		if (time < 20)
		{
			chase(pacMan->getCurrentVertex());
		}
		else
		{
			state = GhostState::SCATTER;
			currentVertex = previousVertex;
			time = 0;
		}
		break;
	case GhostState::SCATTER:
		speed = 0.75;
		if (time < 5)
		{
			chase(home);
		}
		else
		{
			state = GhostState::CHASE;
			currentVertex = previousVertex;
			time = 0;
		}

		break;
	case GhostState::SCARED:
		speed = 0.4;
		if (time < 10)
		{
			chase(pacMan->getCurrentVertex());
			entity->setMaterialName("ScaredGhost");
		}
		else
		{
			state = GhostState::CHASE;
			currentVertex = previousVertex;
			time = 0;
			entity->setMaterialName(ghostMaterial);
		}

		break;
	case GhostState::DEAD:
		speed = 1;
		chase(Scene::getSingletonPtr()->getGhostRespawn(0));
		entity->setMaterialName(ghostMaterial + "Dead");
		if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getGhostRespawn(0)->getData().getIndex())
		{
			state = GhostState::EXIT;
			currentVertex = previousVertex;
			entity->setMaterialName(ghostMaterial);
			time = 0;
		}

		break;
	case GhostState::EXIT:
		speed = 0.7;
		chase(Scene::getSingletonPtr()->getExit());
		if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getExit()->getData().getIndex())
		{
			state = GhostState::SCATTER;
			currentVertex = previousVertex;
			time = 0;
		}
		break;
	default:
		break;
	}

	if (pacMan->scaredGhost() && state != GhostState::EXIT && state != GhostState::WAIT && state != GhostState::DEAD)
	{
		state = GhostState::SCARED;
		currentVertex = previousVertex;
		time = 0;
	}

	if (pacMan->getCurrentVertex()->getData().getIndex() == currentVertex->getData().getIndex() && state == GhostState::SCARED)
	{
		state = GhostState::DEAD;
		currentVertex = previousVertex;
		time = 0;
		pacMan->addScore(100);
	}
	else if (!pacMan->killed() && pacMan->getCurrentVertex()->getData().getIndex() == currentVertex->getData().getIndex() && state != GhostState::SCARED && state != GhostState::DEAD)
	{
		pacMan->die();
		time = 0;
		state = GhostState::EXIT;
	}
}

void Ghost::wait()
{
	speed = 0.3;
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

		targetVertex = GraphVertex::nextVertx(getOppositeDirecction(currentDirecction), currentVertex);
		getDirecction(targetVertex, currentVertex);
	}
}



void Ghost::chase(GraphVertex* objective)
{
	if (targetVertex && sceneNode->getPosition().distance(targetVertex->getData().getPosition()) > EPSILON)
	{
		sceneNode->translate(direcction * speed);
	}
	else
	{
		if (targetVertex)
		{
			previousVertex = currentVertex;
			currentVertex = targetVertex;
		}
		if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getRightTeleport()->getData().getIndex())
		{

			currentVertex = Scene::getSingletonPtr()->getLeftTeleport();
			speed = 0.4;
		}
		else if (currentVertex->getData().getIndex() == Scene::getSingletonPtr()->getLeftTeleport()->getData().getIndex())
		{
			currentVertex = Scene::getSingletonPtr()->getRightTeleport();
			speed = 0.4;
		}

		if (targetVertex && GraphVertex::checkVertex(targetVertex, "Teleport"))
		{
			speed = 0.4;
		}
		sceneNode->setPosition(currentVertex->getData().getPosition());

		if (state != GhostState::SCARED)
		{
			targetVertex = closerNextVertx(objective, currentVertex, previousVertex);
		}
		else
		{
			targetVertex = futherNextVertx(objective, currentVertex, previousVertex);
		}
		getDirecction(targetVertex, currentVertex);
	}
}

const bool Ghost::isDead()
{
	return false;
}

float Ghost::getSpeed()
{
	return speed;
}

void Ghost::setSpeed(float fSpeed)
{
	speed = fSpeed;
}

void  Ghost::setDirecction(GraphVertex::Direcction dDirecction)
{
	currentDirecction = dDirecction;
}

void Ghost::getDirecction(GraphVertex* targetVertx, GraphVertex* actualVertx)
{

	if (targetVertx && actualVertx)
	{
		if (actualVertx->getData().getPosition().z < targetVertx->getData().getPosition().z)
		{
			direcction = MOVE_UP;
			currentDirecction = GraphVertex::Direcction::UP;
		}


		if (actualVertx->getData().getPosition().z > targetVertx->getData().getPosition().z)
		{
			direcction = MOVE_DOWN;
			currentDirecction = GraphVertex::Direcction::DOWN;
		}

		if (actualVertx->getData().getPosition().x < targetVertx->getData().getPosition().x)
		{
			direcction = MOVE_RIGHT;
			currentDirecction = GraphVertex::Direcction::RIGHT;
		}

		if (actualVertx->getData().getPosition().x > targetVertx->getData().getPosition().x)
		{
			currentDirecction = GraphVertex::Direcction::LEFT;
			direcction = MOVE_LEFT;
		}
	}
}



GraphVertex* Ghost::closerNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex)
{
	GraphVertex *nextVertex = NULL;
	float closer = 100000000000000;
	if (actualVertx){
		for (GraphEdge* e : actualVertx->getEdges())
		{
			if (e->getDestination()->getData().getIndex() != previousVertex->getData().getIndex())
			{
				if (state == GhostState::DEAD || state == GhostState::EXIT || !GraphVertex::checkVertex(e->getDestination(), "CageDoor")){
					if (e->getDestination()->getData().getPosition().distance(targetVertx->getData().getPosition()) < closer){
						closer = e->getDestination()->getData().getPosition().distance(targetVertx->getData().getPosition());
						nextVertex = e->getDestination();
					}
				}
			}
		}
	}
	return nextVertex;
}



GraphVertex* Ghost::futherNextVertx(GraphVertex* targetVertx, GraphVertex* actualVertx, GraphVertex* previousVertex)
{
	GraphVertex *nextVertex = NULL;
	float futher = 0;
	if (actualVertx){
		for (GraphEdge* e : actualVertx->getEdges())
		{
			if (e->getDestination()->getData().getIndex() != previousVertex->getData().getIndex() && !GraphVertex::checkVertex(e->getDestination(), "CageDoor"))
			{
				if (e->getDestination()->getData().getPosition().distance(targetVertx->getData().getPosition()) > futher)
				{
					futher = e->getDestination()->getData().getPosition().distance(targetVertx->getData().getPosition());
					nextVertex = e->getDestination();
				}
			}
		}
	}
	return nextVertex;
}

GraphVertex::Direcction Ghost::getOppositeDirecction(GraphVertex::Direcction dDirecction)
{
	GraphVertex::Direcction result = GraphVertex::Direcction::NONE;

	switch (dDirecction)
	{
	case GraphVertex::Direcction::UP:
		result = GraphVertex::Direcction::DOWN;
		break;
	case GraphVertex::Direcction::DOWN:
		result = GraphVertex::Direcction::UP;
		break;
	case GraphVertex::Direcction::RIGHT:
		result = GraphVertex::Direcction::LEFT;
		break;
	case GraphVertex::Direcction::LEFT:
		result = GraphVertex::Direcction::RIGHT;
		break;
	}

	return result;
}

void Ghost::init(GraphVertex* vertex, GhostState gState)
{
	currentVertex = vertex;
	sceneNode->setPosition(vertex->getData().getPosition());
	currentVertex = vertex;
	previousVertex = currentVertex;
	targetVertex = NULL;
	state = gState;
	entity->setMaterialName(ghostMaterial);

}