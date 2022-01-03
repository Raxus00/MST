#include "Railroad.hpp"
int main(int argc, char* argv[])
{
	Railroad R;	//Rialroad objekt för att nå funktioner och variabler
	if (argc != 2)
	{
		R.error(1);	//Kallar på en error utskrift och terminerar programet
	}
	std::string path = argv[1];	//lägger det andra argumentet som är fil sökvägen i en sträng 
	R.Read(path);	//Kallar på funktion för att läsa filens innehåll
	R.PrimA();
}