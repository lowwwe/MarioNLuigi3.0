/// <summary>
/// author Pete Lowe May 2025
/// you need to change the above line or lose marks
/// Mario N Luigi game
/// 
/// 
/// estimate 60 min
/// 14/5 45 min
/// 15/5 15 min
/// actual 60 min
/// </summary>


#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the texts
/// load and setup the images
/// load and setup the sounds
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{800U, 600U}, 32U }, "Mario N Luigi" },
	m_exitGame{false} //when true game will exit
{
	setupTexts(); // load font 
	setupSprites(); // load texture
	setupAudio(); // load sounds
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but method needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) // close window message 
		{
			m_exitGame = true;
		}
		if (newEvent->is<sf::Event::KeyPressed>()) //user pressed a key
		{
			processKeyPresses(newEvent);
		}
		if (newEvent->is<sf::Event::KeyReleased>()) // user released a key
		{
			processKeyReleases(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeyPresses(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyPressed *newKeypress = t_event->getIf<sf::Event::KeyPressed>();
	if (sf::Keyboard::Key::Escape == newKeypress->code)
	{
		m_exitGame = true; 
	}
	
}

/// <summary>
/// deal with key release event
/// </summary>
/// <param name="t_event">key release event</param>
void Game::processKeyReleases(const std::optional<sf::Event> t_event)
{
	const sf::Event::KeyReleased* newKeyRelease = t_event->getIf<sf::Event::KeyReleased>();
	if (sf::Keyboard::Key::Space == newKeyRelease->code)
	{
		changeCharacter();
	}
}

/// <summary>
/// Check if any keys are currently pressed
/// </summary>
void Game::checkKeyboardState()
{
	m_direction = Direction::None;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		m_exitGame = true; 
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)  
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_direction = Direction::Up; // go up
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_direction = Direction::Down;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_direction = Direction::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_direction = Direction::Right;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();
	if (m_exitGame)
	{
		m_window.close();
	}
	move();
	checkBoundaries();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(ULTRAMARINE);

	m_window.draw(m_characterName);
	m_window.draw(m_MarioSprite);
	
	
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupTexts()
{
	if (!m_marioFont.openFromFile("ASSETS/FONTS/SuperMario256.ttf"))
	{
		std::cout << "problem with font" << std::endl;
	}
	m_characterName.setFont(m_marioFont);
	m_characterName.setString("Mario");
	m_characterName.setCharacterSize(80U);
	m_characterName.setPosition(sf::Vector2f{ 100.0f,100.0f });
	m_characterName.setFillColor(sf::Color::Red);
	m_characterName.setOutlineColor(sf::Color::Black);
	m_characterName.setOutlineThickness(2.0f);
	


}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprites()
{
	if (!m_marioTexture.loadFromFile("ASSETS\\IMAGES\\mario-luigi-64.png"))
	{
		std::cout << "Problem finding Mario" << std::endl;
	}
	m_MarioSprite.setTexture(m_marioTexture);
	m_MarioSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{64,148} });
	m_MarioSprite.setPosition(m_location);

	
	
}

/// <summary>
/// load sound file and assign buffers
/// </summary>
void Game::setupAudio()
{
	if (!m_marioSoundBuffer.loadFromFile("ASSETS/AUDIO/mario.wav"))
	{
		std::cout << "problem with mario sound" << std::endl;
	}
	if (!m_luigiSoundBuffer.loadFromFile("ASSETS/AUDIO/luigi.wav"))
	{
		std::cout << "problem with mario sound" << std::endl;
	}
	m_marioSound.setBuffer(m_marioSoundBuffer);
	m_luigiSound.setBuffer(m_luigiSoundBuffer);
	
}

/// <summary>
/// check m_direction and move sprite
/// </summary>
void Game::move()
{
	sf::Vector2f movement{ 0.0f,0.0f }; // movement
	float speed = 2.5f;// speed of movement

	switch (m_direction)
	{
	case Direction::None:
		break;
	case Direction::Up:
		movement.y = -speed;
		break;
	case Direction::Down:
		movement.y = speed;
		break;
	case Direction::Left:
		movement.x = -speed;
		break;
	case Direction::Right:
		movement.x = speed;
		break;
	default:
		break;
	}

	m_location += movement;
	m_MarioSprite.setPosition(m_location);
}

/// <summary>
/// check all 4 side and keep sprite inside screen
/// </summary>
void Game::checkBoundaries()
{
	if (m_location.x < 0.0f)
	{
		m_location.x = 0.0f;
	}
	if (m_location.y < 0.0f)
	{
		m_location.y = 0.0f;
	}
	if (m_location.y > (600.0f - 148.0f)) // 148 is height
	{
		m_location.y = 600.0f - 148.0f;
	}
	if (m_location.x > (800.0f - 64.0f)) // 64 is width
	{
		m_location.x = 800.0f - 64.0f;
	}

	m_MarioSprite.setPosition(m_location);
}


/// <summary>
/// switch between Mario and Luigi texture rect, Text, and colour
/// play sound
/// </summary>
void Game::changeCharacter()
{
	if (m_isMario)
	{
		m_MarioSprite.setTextureRect(sf::IntRect{sf::Vector2i{64,0}, sf::Vector2i{64,148}});
		m_luigiSound.play();
		m_characterName.setString("Luigi");
		m_characterName.setFillColor(sf::Color::Green);
	}
	else
	{
		m_MarioSprite.setTextureRect(sf::IntRect{ sf::Vector2i{0,0}, sf::Vector2i{64,148} });
		m_marioSound.play();
		m_characterName.setString("Mario");
		m_characterName.setFillColor(sf::Color::Red);
	}
	centreText();
	m_isMario = !m_isMario;
}

/// <summary>
/// centre text on screen
/// </summary>
void Game::centreText()
{
	sf::Vector2f position{ 0.0f,200.0f };
	//use global bound width to calculate left co-ordinate
	position.x = (800.0f - m_characterName.getGlobalBounds().size.x) / 2.0f;
	m_characterName.setPosition(position);
}
