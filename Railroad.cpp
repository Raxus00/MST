#include "Railroad.hpp"

void Railroad::error(int number)
{
	switch (number)
	{
	case 1: 
		std::cerr << "Wrong number om arguments" << std::endl;
		exit(1);
	case 2:
		std::cerr << "Could not open file" << std::endl;
		exit(1);
	case 3: 
		std::cerr << "Could not find the smallest key number" << std::endl;
		exit(1);
	default:
		break;
	}
}
void Railroad::Read(std::string& path)
{
	std::ifstream read;	//läser data
	std::string data;	//variabel som tar upp datan från filen
	bool all_nodes_found = false;	//bool variabel som håller koll på om alla noder lästs in från filen
	std::vector<int> temp_vector; //temporär vektor för att plasera kostnaderna i grafen för senare uträkning
	read.open(path);	//öppnar filen path för läsning

	if (!read)
	{
		error(2);	//Kallar på en error utskrift och terminerar programet
	}
	else
	{
		while (std::getline(read, data))
		{
			if (data.size() == 0)	//Om data.size() är noll är det tomt och noderna och kanterna separeras med en blankrad vilket betyder att alla noder är omhändertagna 
			{
				all_nodes_found = true; //alla noder hittade
			}
			else if(all_nodes_found ==false)
			{
				Node node; //node objekt som hjälper oss placera noderna i en vektor där vi kan hålla reda på mer information om dem
				node.Node_name = data; //node_name är namnet på den noden som lästs in och sparas i structen 
				the_nodes.push_back(node);	//Lägerr till namnet på de enskilda noderna i structen i en vektor för alla noder
			}
			else	//om inget av de andra fallen infträffar har vi nått delan av filen där kanterna ochkostnaden kommer vi behöver separera den informationen till mindre delar för att ta nytta av det, vi använder oss därför av substrängar
			{
				std::string edge = data;	//strömmar informationen från data in i edge
				std::string src; //ursprungs noden
				std::string dest; //destinations noden
				int cost; //förflyttnigs kostnaden

				src = edge.substr(0, edge.find('\t'));	//gör en sträng src, som består av allt från början av edge tills dess att den hittar en tab
				edge.erase(0, src.length() + 1); // tar bort informatione om den substräng vi skapat från edge plus taben som vi hittade
				dest = edge.substr(0, edge.find('\t'));	//Skapar en substräng dest, med info om vilket node som kopplar med src noden, slutar när en tab hittas precis som innnan
				edge.erase(0, dest.length() + 1); //tar bort informationen från edge som nu finns i substräng plus taben vi hittat
				cost = std::stoi(edge);	//Gör om kostnaden av förflyttingen till en int frpn string för att kunna jämföra med andra senare
				for (unsigned int i = 0; i < the_nodes.size(); i++)
				{
					if (the_nodes[i].Node_name == src)	// om noden på plats i i vektorn är samma som src
					{
						for (unsigned int j = 0; j < the_nodes.size(); j++)
						{
							if(the_nodes[j].Node_name == dest)	//om noden på plats j är samma som dest
							{
								the_nodes[i].Move_cost[j] = cost;	//Då är kostnaden mellan dem vad cost är 
								the_nodes[j].Move_cost[i] = cost;	//detsamma gäller om de är det andra hållet runt 
							}
						}
					}
				}

			}
		}
		read.close(); //stänger filen från läsning

		for (unsigned int i = 0; i < the_nodes.size(); i++)	
		{
			for (unsigned int j = 0; j < the_nodes.size(); j++)
			{
				temp_vector.push_back(the_nodes[i].Move_cost[j]);	//Lägger till alla möjliga kostnader för att flytta till den noden i temp_vector
			}
			Graph.push_back(temp_vector); //lägger sedan till alla de talen i vår graph 2D vektor 
		}
	}
}

int Railroad::min(std::vector<int> key, std::vector<bool> in_mst)
{
	int min = INT_MAX;	//En stor int för jämförelse ifall någon annan är mindre
	for (unsigned int i = 0; i < Graph.size(); i++)
	{
		if (in_mst[i] == false && key[i] < min)
		{
			min = key[i];	//sätter en ny min som värdet på index plats i inom vektorn key 
			int min_index = i;	//indexplatsen för den minsta kostnaden är i
			return i;	//retunerar indexet för den minsta kostnaden
		}
	}
	error(3);	//Kallar på en errorutskrift
	return -1;
}

void Railroad::write(std::vector<int> primary, std::vector<std::vector<int>> Graph)
{
	std::ofstream write;	//skriver till en fil

	write.open("Answer.txt");	//Öppnar filen om den finns annars skapas den
	if (!write)
	{
		error(2);
	}
	else
	{
		for (unsigned int i = 0; i < Graph.size(); i++)
		{
			write << the_nodes[i].Node_name << std::endl;	//skriver ut all noderna 
		}
		write << '\n';	//skriver ut ett blankslag
		for (unsigned int j = 0; j < Graph.size(); j++)
		{
			write << the_nodes[primary[j]].Node_name << '\t' << the_nodes[j].Node_name << '\t' << Graph[j][primary[j]] << std::endl;	//skriver ut alla vägar vi tagit i ordningen vi tagit dem, i formatet "start	mål		kostnaden"
		}
	}
}

void Railroad::PrimA()
{
	std::vector<int> primary;	
	std::vector<int> key; 
	std::vector<bool> in_mst; 


	for (unsigned int i = 0; i < Graph.size(); i++)
	{
		primary.push_back(INT_MAX); //Sätter värdet på alla platser till oändligheten(eller ja bara ett väldigt stort tal)
		key.push_back(INT_MAX);	//Sätter alla nycklar till samma sak för att det ska bli lättare att se vilken som är minst när vi ska leeta efter den minst
		in_mst.push_back(false); //Eftersom vi inte valt något 'n behöver alla positioner i in_mst vara false 
	}
	key[0] = 0; //initialiserar värdet på plats noll till noll så att första noden ska vara vår root nod
	primary[0] = -1;	//initialliserar värdet på plats noll i mst till -1 för att det är vår rot nod

	for (unsigned int j = 0; j < Graph.size() - 1; j++)
	{
		int k = min(key, in_mst);	//tar reda på den minsta kostnaden

		in_mst[k] = true;	//sätter in det framtagna indexet som inkluderat i mst

		for (unsigned int l = 0; l < Graph.size(); l++)
		{
			if (Graph[k][l] && in_mst[l] == false && Graph[k][l] < key[l])	// tar fram nya kostnader mellan noder för jämförelse om noden inte redan finns i mst och graph[k][l] måste var mindre än INT_MAX
			{
				primary[l] = k;	//lägger till kostnaden för flytten på plats l i mst
				key[l] = Graph[k][l]; // sätter key[l] till ett nytt nummer som är mindre än INT_MAX
			}
		}
	}
	write(primary, Graph); //kallar på en funktion som ska skriva ut grafen till en fil

}