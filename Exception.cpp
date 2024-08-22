#include "Exception.h"
#include "Logger.h"

void Exception::printError() const
{
	Logger::getInstance()->addError(*this);
}
