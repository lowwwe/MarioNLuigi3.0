/// <summary>
/// author Pete Lowe May 2025
/// you need to change the above line or lose marks
/// Also don't have any member properties called Delete...
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
#pragma warning( push )
#pragma warning( disable : 4275 )
// ignore this warning
// C:\SFML - 3.0.0\include\SFML\System\Exception.hpp(41, 47) : 
// warning C4275 : non dll - interface class 'std::runtime_error' used as base for dll - interface class 'sf::Exception'

/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the #endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(const std::optional<sf::Event> t_event);
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupTexts();
	void setupSprites();
	void setupAudio();

	
	sf::RenderWindow m_window; // main SFML window
	
	
	sf::Font m_marioFont;// font used for character names
	sf::Text m_characterName{ m_marioFont };// text for character names

	sf::Texture m_marioTexture; // texture used for Mario and Luigi
	sf::Sprite m_MarioSprite{ m_marioTexture };// sprite used for both Mario and Luigi
	
	sf::SoundBuffer m_marioSoundBuffer;// sound buffer for mario wav
	sf::Sound m_marioSound{ m_marioSoundBuffer };// sound effect for mario

	
	bool m_DELETEexitGame; // control exiting game

};

#pragma warning( pop ) 
#endif // !GAME_HPP

