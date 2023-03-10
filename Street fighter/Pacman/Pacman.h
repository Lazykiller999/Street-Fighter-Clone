#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;

	// Burbaj AKA main charecter
	Vector2* _burbajPos;
	Rect* _burbajRect;
	Texture2D* _burbajTexture;
	int vel_y = 10, vel_x = 10, _burbajCurrentFrameTime, _burbajFrame;
	const int _cBurbajFrameTime;
	int distance;
	bool jump = false;
	bool attack = false;
	bool direction = 0;
	bool damage = false;
	int allowProximity, attackChoice;
	

#	// enemyplayer
	Vector2* _enemyPos;
	Texture2D* _enemyTexture;
	Rect* _enemyRect;
	int _enemyDirection = 1;

	//health bar
	Texture2D* _health_bar;
	Rect* _healthRect;
	Vector2* _healthPos;
	Texture2D* _greenHealth;
	Vector2* _p1bar;
	Vector2* _p2bar;
	Rect* _p1Rect;
	Rect* _p2Rect;

	//Background
	Vector2* _bgPos;
	Rect* _bgRect;
	Texture2D* _bgTexture;

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	void EnemyCollision();

	void EnemyMovment(int elapsedTime, int distance);

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};