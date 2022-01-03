#include "Railroad.hpp"
int main(int argc, char* argv[])
{
	Railroad R;	//Rialroad objekt f�r att n� funktioner och variabler
	if (argc != 2)
	{
		R.error(1);	//Kallar p� en error utskrift och terminerar programet
	}
	std::string path = argv[1];	//l�gger det andra argumentet som �r fil s�kv�gen i en str�ng 
	R.Read(path);	//Kallar p� funktion f�r att l�sa filens inneh�ll
	R.PrimA();
}