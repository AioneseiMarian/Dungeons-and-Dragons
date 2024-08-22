#include "Logger.h"
#include "Exception.h"
#include "Character.h"

Logger::Logger()
{
    logFile.open("logger.txt");
    if (!logFile.is_open())
        exit(1);
}

void Logger::destroyInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void Logger::logPlayerStats(const Player &player)
{
	playerFile.open("playerInfo.txt");
	if (!playerFile.is_open())
		throw Exception("Eroare la deschiderea fisierului playerInfo.txt");

	playerFile << "Player name: " << player.getName() << std::endl;
	playerFile << "Player health: " << player.getMaxHp() << std::endl;
	playerFile << "Player race: " << player.getRace() << std::endl;
	playerFile << "Player class: " << player.getClass() << std::endl;
	playerFile << "Player background: " << player.getBackground() << std::endl;
	playerFile << "Player combat abilities: " << std::endl;
	for (auto i : player.cAbilities)
		playerFile << i << std::endl;
	playerFile << "Player abilities: " << std::endl;
	playerFile << player.abilities << std::endl;
	
	playerFile.close();
}

void Logger::printPlayerStats()
{
	std::ifstream playerFile("playerInfo.txt");
	if (!playerFile.is_open())
		throw Exception("Eroare la deschiderea fisierului playerInfo.txt");
	
	std::string line;	
	while (std::getline(playerFile, line))
		std::cout << line << std::endl;
	playerFile.close();
	
}

Logger *Logger::getInstance()
{
    if (instance == nullptr)
        instance = new Logger;
    return instance;
}

 void Logger::addError(const AException &exception)
 {
     logFile << exception.exceptie << std::endl;
 }

Logger::~Logger()
{
    if (logFile.is_open())
        logFile.close();
	if(playerFile.is_open())
		playerFile.close();
}
