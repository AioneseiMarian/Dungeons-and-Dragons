#include <iostream>
#include <vector>
#include "DungeonMaster.h"
#include <string>
#include "Exception.h"
#include "Logger.h"

int main()
{
	srand(time(NULL));

	try 
	{
		DungeonMaster *dm = DungeonMaster::getInstance();
		dm->playGame();
		
		
		
	}
	catch (Exception &exc)
	{
		exc.printError();
	}


	return 0;
}