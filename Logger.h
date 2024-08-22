#pragma once
#include <iostream>
#include <fstream>
class AException;
class Player;

class Logger
{
private:
	static Logger *instance;
	std::ofstream logFile;
	std::ofstream playerFile;


	Logger();
	Logger(Logger *l) = delete;
	void operator=(const Logger &) = delete;

public:
	static void destroyInstance();
	void logPlayerStats(const Player &player);
	void printPlayerStats();
	static Logger *getInstance();
	void addError(const AException &exception);
	~Logger();
};

inline Logger* Logger::instance = nullptr;