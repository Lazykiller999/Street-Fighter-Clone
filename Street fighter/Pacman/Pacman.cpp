#include "Pacman.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cBurbajFrameTime(250)
{
	_frameCount = 0;
	srand(time(NULL));
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1600, 900, false, 25, 25, "Stritfitter", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

	// Initialise vars
	_burbajCurrentFrameTime = 0;
	_burbajFrame = 0;
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	//Burbaj
	_burbajTexture = new Texture2D();
	_burbajTexture->Load("Textures/sprite_sheet.png", false);
	_burbajPos = new Vector2(304.0f, 600.0f);
	_burbajRect = new Rect(0.0f, 0.0f, 70, 106);
	//Enemy Player
	_enemyTexture = new Texture2D();
	_enemyTexture->Load("Textures/enemy_sprite_sheet.png", false);
	_enemyPos = new Vector2(1200.0f, 600.0f);
	_enemyRect = new Rect(0.0f, 0.0f, 70, 106);
	_enemyDirection = 1;
	//Background
	_bgTexture = new Texture2D();
	_bgTexture->Load("Textures/Background.jpg", true);
	_bgPos = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);;
	_bgRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());;
	//Health bar
	_health_bar = new Texture2D();
	_health_bar->Load("Textures/health_bar.png", true);
	_healthRect = new Rect(0.0f, 0.0f, 950, 250);
	_healthPos = new Vector2(328, 13);
	_greenHealth = new Texture2D;
	_greenHealth->Load("Textures/green_health.png", false);
	_p1Rect = new Rect(0.0f, 0.0f, 318, 16);
	_p2Rect = new Rect(0.0f, 0.0f, 318, 16);
	_p1bar = new Vector2(418.0f, 114.0f);
	_p2bar = new Vector2(864.0f, 114.0f);
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::EnemyCollision() {
	int _hbPlayerBottem = _burbajPos->Y + _burbajRect->Height;
	int _hbEnemyBottem = _enemyPos->Y + _enemyRect->Height;
	int _hbPlayerLeft = _burbajPos->X;
	int _hbEnemyLeft = _enemyPos->X;
	int _hbPlayerRight = _burbajPos->X + _burbajRect->Width;
	int _hbEnemyRight = _enemyPos->X + _enemyRect->Width;
	int _hbPlayerTop = _burbajPos->Y;
	int _hbEnemyTop = _enemyPos->Y;

	if ((attack == true) && (_hbPlayerBottem > _hbEnemyTop) && (_hbEnemyTop < _hbPlayerBottem) && (_hbPlayerRight > _hbEnemyLeft) && (_hbPlayerLeft < _hbEnemyRight)) {
		if (_p2Rect->Width > 0) {
			_p2Rect->Width -= 3;
		}
		else if (_p2Rect->Width == 0) {
			cout << "Game ended!!!!" << endl;
		}
	}
}

void Pacman::EnemyMovment(int elapsedTime, int dist) {
	allowProximity = rand() % 2;
	attackChoice = rand() % 3;
	if (dist > 150) {
		if (_enemyDirection == 1) {
			_enemyRect->Width = 70;
			_enemyPos->X -= 0.15f * elapsedTime;
			_enemyRect->Y = _enemyRect->Height * _enemyDirection;
			_enemyRect->X = _enemyRect->Width * _burbajFrame;

		}
		else if (_enemyDirection == 0) {
			_enemyRect->Width = 70;
			_enemyPos->X += 0.15f * elapsedTime;
			_enemyRect->Y = _enemyRect->Height * _enemyDirection;
			_enemyRect->X = _enemyRect->Width * _burbajFrame;
		}
	}
	else if (dist < 75 && (allowProximity == 1)) {
		if (_enemyDirection != 1) {
			_enemyRect->Width = 70;
			_enemyPos->X -= 0.15f * elapsedTime;
			_enemyRect->Y = _enemyRect->Height * 1;
			_enemyRect->X = _enemyRect->Width * _burbajFrame;

		}
		else if (_enemyDirection != 0) {
			_enemyRect->Width = 70;
			_enemyPos->X += 0.15f * elapsedTime;
			_enemyRect->Y = _enemyRect->Height * 0;
			_enemyRect->X = _enemyRect->Width * _burbajFrame;
		}
	}

	switch (attackChoice)
	{
		case 0:
			if (_enemyDirection == 1) {
				_enemyRect->Width = 90;
				_enemyRect->Y = _enemyRect->Height * 2;
				_enemyRect->X = _enemyRect->Width * 0;
			}
			if (_enemyDirection == 0) {
				_enemyRect->Width = 90;
				_enemyRect->Y = _enemyRect->Height * 3;
				_enemyRect->X = _enemyRect->Width * 0;
			}
		case 1:
			if (_enemyDirection == 0) {
				_enemyRect->Width = 90;
				_enemyRect->Y = _enemyRect->Height * 3;
				_enemyRect->X = _enemyRect->Width * 0;
			}
			if (_enemyDirection == 1) {
				_enemyRect->Width = 90;
				_enemyRect->Y = _enemyRect->Height * 3;
				_enemyRect->X = _enemyRect->Width * 1;
			}
		case 2:
			_enemyRect->Width = 70;
	}
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	distance = (_enemyPos->X + _enemyRect->Width) - (_burbajPos->X + _burbajRect->Width);
	cout << distance << endl;
	EnemyMovment(elapsedTime, distance);
	EnemyCollision();

	// direction = 0 -> right
	if (keyboardState->IsKeyDown(Input::Keys::D)) {
		direction = 0;
		_burbajRect->Width = 70;
		_burbajPos->X += 0.175f * elapsedTime; //Moves Pacman across X axis
		_burbajRect->Y = _burbajRect->Height * direction;
		_burbajRect->X = _burbajRect->Width * _burbajFrame;
	}
	if (keyboardState->IsKeyDown(Input::Keys::D) && keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT)) {
		direction = 0;
		_burbajRect->Width = 70;
		_burbajPos->X += 0.25f * elapsedTime;
		_burbajRect->Y = _burbajRect->Height * direction;
		_burbajRect->X = _burbajRect->Width * _burbajFrame;
	}
	// punching
	if (keyboardState->IsKeyDown(Input::Keys::L) && direction == 0) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 2;
		_burbajRect->X = _burbajRect->Width * 0;
	}

	if (keyboardState->IsKeyDown(Input::Keys::L) && direction == 1) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 2;
		_burbajRect->X = _burbajRect->Width * 1;
	}
	// kicking
	if (keyboardState->IsKeyDown(Input::Keys::K) && direction == 0) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 3;
		_burbajRect->X = _burbajRect->Width * 0;
	}

	if (keyboardState->IsKeyDown(Input::Keys::K) && direction == 1) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 3;
		_burbajRect->X = _burbajRect->Width * 1;
	}
	//jump kicking
	if (keyboardState->IsKeyDown(Input::Keys::K) && keyboardState->IsKeyDown(Input::Keys::SPACE) && direction == 0) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 4;
		_burbajRect->X = _burbajRect->Width * 0;
	}

	if (keyboardState->IsKeyDown(Input::Keys::K) && keyboardState->IsKeyDown(Input::Keys::SPACE) && direction == 1) {
		attack = true;
		_burbajRect->Width = 90;
		_burbajRect->Y = _burbajRect->Height * 4;
		_burbajRect->X = _burbajRect->Width * 1;
	}
	// walking left
	if (keyboardState->IsKeyDown(Input::Keys::A)) {
		direction = 1;
		_burbajRect->Width = 70;
		_burbajPos->X -= 0.175f * elapsedTime;
		_burbajRect->Y = _burbajRect->Height * direction;
		_burbajRect->X = _burbajRect->Width * _burbajFrame;
	}
	if (keyboardState->IsKeyDown(Input::Keys::A) && keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT)) {
		direction = 1;
		_burbajRect->Width = 70;
		_burbajPos->X -= 0.25f * elapsedTime;
		_burbajRect->Y = _burbajRect->Height * direction;
		_burbajRect->X = _burbajRect->Width * _burbajFrame;
	}
	// jumping
	if (keyboardState->IsKeyDown(Input::Keys::SPACE) && jump == false) {
		jump = true;
	}

	_burbajCurrentFrameTime += elapsedTime;
	if (jump == true) {
		_burbajPos->Y -= vel_y * 3;
		vel_y -= 1;
		if (vel_y < -10) {
			jump = false;
			vel_y = 10;
		}
	}

	//animation
	if (_burbajCurrentFrameTime > _cBurbajFrameTime) {
		_burbajFrame++;

		if (_burbajFrame >= 4) {
			_burbajFrame = 0;
		}
		_burbajCurrentFrameTime = 0;
	}
	if (_burbajFrame == 1 && attack == true) {
		_burbajRect->Width = 70;
		_burbajRect->Y = _burbajRect->Height * direction;
		_burbajRect->X = _burbajRect->Width * 0;
		attack = false;
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Burbaj X: " << _burbajPos->X << " Y: " << _burbajPos->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_bgTexture, _bgRect, nullptr);
	SpriteBatch::Draw(_health_bar, _healthPos, _healthRect);
	SpriteBatch::Draw(_greenHealth, _p1bar, _p1Rect);
	SpriteBatch::Draw(_greenHealth, _p2bar, _p2Rect);
	SpriteBatch::Draw(_burbajTexture, _burbajPos, _burbajRect);
	SpriteBatch::Draw(_enemyTexture, _enemyPos, _enemyRect);

	if (_frameCount < 30)
	{
		// Draws Red Munchie

		_frameCount++;
	}
	else {
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}