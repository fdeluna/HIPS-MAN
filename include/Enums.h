#ifndef Enum_H
#define Enum_H

#include <Ogre.h>

#define MOVE_SPEED  0.01
#define MOVE_UP  Ogre::Vector3(0, 0, MOVE_SPEED)
#define MOVE_DOWN  Ogre::Vector3(0, 0, -MOVE_SPEED)
#define MOVE_RIGHT  Ogre::Vector3(MOVE_SPEED, 0, 0)
#define MOVE_LEFT  Ogre::Vector3(-MOVE_SPEED, 0, 0)
#define EPSILON 0.1

enum class Direcction
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	NONE
};


enum class GameFlow
{
	INIT,
	PLAY,
	GAMEOVER,
	PAUSE,
	WIN
};

enum class GhostState
{
	WAIT,
	CHASE,
	SCATTER,
	SCARED,
	DEAD,
	EXIT
};
#endif