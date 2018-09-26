// Timber.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);
	
	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	//Make a tree sprite
	Texture treeTexture;
	treeTexture.loadFromFile("graphics/tree.png");
	Sprite treeSprite; 
	treeSprite.setTexture(treeTexture);
	treeSprite.setPosition(810, 0);

	//Make a bee
	Texture beeTexture;
	beeTexture.loadFromFile("graphics/bee.png");
	Sprite beeSprite;
	beeSprite.setTexture(beeTexture);
	beeSprite.setPosition(0, 800);
	bool isBeeFlying = false;
	float beeSpeed = 0.0;

	//Make clouds
	Texture cloudTexture;
	cloudTexture.loadFromFile("graphics/cloud.png");

	Sprite cloud1;
	Sprite cloud2;
	Sprite cloud3;

	cloud1.setTexture(cloudTexture);
	cloud2.setTexture(cloudTexture);
	cloud3.setTexture(cloudTexture);

	cloud1.setPosition(0, 0);
	cloud2.setPosition(0, 250);
	cloud3.setPosition(0, 500);

	// are clouds on screen 
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	float cloud1Speed = 0.0;
	float cloud2Speed = 0.0;
	float cloud3Speed = 0.0;

	//controlling time
	Clock clock;

	//Time Bar
	RectangleShape TimeBar;
	float TimeBarWidth = 400;
	float timeBarHeight = 80;
	TimeBar.setSize(Vector2f(TimeBarWidth, timeBarHeight));
	TimeBar.setFillColor(Color::Red);
	TimeBar.setPosition((1920 / 2) - TimeBarWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = TimeBarWidth / timeRemaining;


	bool paused = true;


	// Draw some text
	int score = 0;

	sf::Text messageText;
	sf::Text scoreText;

	// We need to choose a font
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	//preparing 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	//preparing the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 620);

	//player starts from the left 
	side playerSide = side::LEFT;

	//preparing the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//preparing the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;
	// Prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	while (window.isOpen())
	{

		// score ++;
		Event event;
		while (window.pollEvent(event))
		{


			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}

		}

		//******************************************
		//Handling player's input
		//******************************************

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
			paused = false;
			score = 0;
			timeRemaining = 6;

			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// Move the player into position
			spritePlayer.setPosition(580, 720);

			acceptInput = true;

			// Prepare the sound
			SoundBuffer chopBuffer;
			chopBuffer.loadFromFile("sound/chop.wav");
			Sound chop;
			chop.setBuffer(chopBuffer);

			SoundBuffer deathBuffer;
			deathBuffer.loadFromFile("sound/death.wav");
			Sound death;
			death.setBuffer(deathBuffer);

			// Out of time
			SoundBuffer ootBuffer;
			ootBuffer.loadFromFile("sound/out_of_time.wav");
			Sound outOfTime;
			outOfTime.setBuffer(ootBuffer);


		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			// More code here next...
			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);



				spritePlayer.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;


				acceptInput = false;

				// Play a chop sound
				chop.play();

			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);


				spritePlayer.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;


				acceptInput = false;

				// Play a chop sound
				chop.play();

			}

		}

		//****************************************
		//Update the scene
		//****************************************
		if (!paused)
		{
			//measure time
			Time dt = clock.restart();

			//calculate the time remaining 
			timeRemaining -= dt.asSeconds();
			//size up the time bar
			TimeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				paused = true;

				messageText.setString("out of time !!");

				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}


			if (!isBeeFlying)
			{

				// How fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				beeSprite.setPosition(2000, height);
				isBeeFlying = true;

			}
			else
				// Move the bee
			{

				beeSprite.setPosition(
					beeSprite.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					beeSprite.getPosition().y);

				// Has the bee reached the right hand edge of the screen?
				if (beeSprite.getPosition().x < -100)
				{
					// Set it up ready to be a whole new cloud next frame
					isBeeFlying = false;
				}
			}

			// Manage the clouds
			// Cloud 1
			if (!cloud1Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				cloud1.setPosition(-200, height);
				cloud1Active = true;


			}
			else
			{

				cloud1.setPosition(
					cloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					cloud1.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (cloud1.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud1Active = false;
				}
			}
			// Cloud 2
			if (!cloud2Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				cloud2.setPosition(-200, height);
				cloud2Active = true;


			}
			else
			{

				cloud2.setPosition(
					cloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					cloud2.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (cloud2.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{

				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				cloud3.setPosition(-200, height);
				cloud3Active = true;


			}
			else
			{

				cloud3.setPosition(
					cloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					cloud3.getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (cloud3.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					cloud3Active = false;
				}
			}

			//uodate the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{

				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(600, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);

				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log				
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the insect reached the right hand edge of the screen?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound
				death.play();


			}
		}

		//****************************************
		// Draw the scene
		//****************************************
		


		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		window.draw(cloud1);
		window.draw(cloud2);
		window.draw(cloud3);

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}


		window.draw(treeSprite);

		window.draw(spritePlayer);

		window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(beeSprite);

		window.draw(scoreText);
		
		if (paused)
		{
			window.draw(messageText);
		}



		// Show everything we just drew
		window.display();


	}

    return 0;
}

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}


}