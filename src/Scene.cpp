#include <Scene.h>
template<> Scene* Ogre::Singleton<Scene>::msSingleton = 0;

Scene::Scene()
{
	_graph = new Graph;
}

Scene::Scene(Ogre::SceneManager* sceneManager)
{
	_graph = new Graph;
	_sceneManager = sceneManager;
}



Scene::~Scene()
{
}

void Scene::addCamera
(Camera* camera)
{
	_cameras.push_back(camera);
}


Scene* Scene::getSingletonPtr()
{
	return msSingleton;
}

Scene& Scene::getSingleton()
{
	assert(msSingleton);
	return *msSingleton;
}

void Scene::initMap(Ogre::SceneNode* node)
{
	mapItems.clear();
	std::stringstream nameNode;
	std::stringstream nameEntity;
	for (GraphVertex* v : _graph->getVertexes())
	{
		nameNode.str("");
		nameEntity.str("");
		nameNode << v->getData().getType() << "_node_" << v->getData().getIndex();
		nameEntity << v->getData().getType() << "_entity_" << v->getData().getIndex();

		if (v->getData().getType() == "PacmanRespawn")
		{
			pacManRespawn = v;
		}
		else if (v->getData().getType() == "Bonus")
		{
			bonus = v;
		}
		else if (v->getData().getType() == "rTeleport")
		{
			rTeleport = v;
		}
		else if (v->getData().getType() == "lTeleport")
		{
			lTeleport = v;
		}
		else if (v->getData().getType() == "Pill")
		{
			mapItems.push_back(v->getData().getIndex());
			Ogre::Entity* vertexEntity = _sceneManager->createEntity(nameEntity.str(), "Cube1.mesh");
			Ogre::SceneNode* vertexNode = _sceneManager->createSceneNode(nameNode.str());
			vertexNode->attachObject(vertexEntity);
			vertexNode->setScale(1, 1, 1);
			vertexNode->setPosition(v->getData().getPosition());
			node->addChild(vertexNode);
		}
		else if (v->getData().getType() == "Empty")
		{
			mapItems.push_back(v->getData().getIndex());
			Ogre::Entity* vertexEntity = _sceneManager->createEntity(nameEntity.str(), "Cube1.mesh");
			Ogre::SceneNode* vertexNode = _sceneManager->createSceneNode(nameNode.str());
			vertexNode->attachObject(vertexEntity);
			vertexNode->setScale(1, 1, 1);
			vertexNode->setPosition(v->getData().getPosition());
			node->addChild(vertexNode);
		}
		else if (v->getData().getType() == "GhostRespawn")
		{
			ghostRespawn.push_back(v->getData().getPosition());
		}
	}
}

GraphVertex* Scene::getPacManRespawn()
{
	return pacManRespawn;
}


GraphVertex* Scene::getBonus()
{
	return bonus;
}

GraphVertex* Scene::getRightTeleport()
{
	return rTeleport;
}

GraphVertex* Scene::getLeftTeleport()
{
	return lTeleport;
}


void Scene::removeMapItem(GraphVertex* v)
{	
	mapItems.erase(std::remove(mapItems.begin(), mapItems.end(), v->getData().getIndex()), mapItems.end());
	std::stringstream nameNode;
	nameNode.str("");
	nameNode << v->getData().getType() << "_node_" << v->getData().getIndex();
	Ogre::SceneNode* node = _sceneManager->getSceneNode(nameNode.str());
	_sceneManager->destroySceneNode(nameNode.str());
}

bool Scene::contaisMapItem(GraphVertex* v)
{
	return std::find(mapItems.begin(), mapItems.end(), v->getData().getIndex()) != mapItems.end();
}
