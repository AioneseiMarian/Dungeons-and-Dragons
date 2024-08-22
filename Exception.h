#pragma once
#include <iostream>
#include <string>
class Logger;

class AException
{
private:
	std::string exceptie;
public:
	AException(const std::string &exc) : exceptie{ exc } {}
	virtual void printError() const = 0;
	friend Logger;
};

class Exception : public AException
{
public:
	Exception(const std::string& exc) : AException{exc}{}
	void printError() const override;
};
