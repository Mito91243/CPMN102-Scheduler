#include <iostream>
#include "Schudeler.h"

using namespace std;

int main()
{
	Schudeler* pSch = new Schudeler;

	pSch->Run();

	delete pSch;
	return 0;

}