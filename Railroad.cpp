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
	std::ifstream read;	//l�ser data
	std::string data;	//variabel som tar upp datan fr�n filen
	bool all_nodes_found = false;	//bool variabel som h�ller koll p� om alla noder l�sts in fr�n filen
	std::vector<int> temp_vector; //tempor�r vektor f�r att plasera kostnaderna i grafen f�r senare utr�kning
	read.open(path);	//�ppnar filen path f�r l�sning

	if (!read)
	{
		error(2);	//Kallar p� en error utskrift och terminerar programet
	}
	else
	{
		while (std::getline(read, data))
		{
			if (data.size() == 0)	//Om data.size() �r noll �r det tomt och noderna och kanterna separeras med en blankrad vilket betyder att alla noder �r omh�ndertagna 
			{
				all_nodes_found = true; //alla noder hittade
			}
			else if(all_nodes_found ==false)
			{
				Node node; //node objekt som hj�lper oss placera noderna i en vektor d�r vi kan h�lla reda p� mer information om dem
				node.Node_name = data; //node_name �r namnet p� den noden som l�sts in och sparas i structen 
				the_nodes.push_back(node);	//L�gerr till namnet p� de enskilda noderna i structen i en vektor f�r alla noder
			}
			else	//om inget av de andra fallen inftr�ffar har vi n�tt delan av filen d�r kanterna ochkostnaden kommer vi beh�ver separera den informationen till mindre delar f�r att ta nytta av det, vi anv�nder oss d�rf�r av substr�ngar
			{
				std::string edge = data;	//str�mmar informationen fr�n data in i edge
				std::string src; //ursprungs noden
				std::string dest; //destinations noden
				int cost; //f�rflyttnigs kostnaden

				src = edge.substr(0, edge.find('\t'));	//g�r en str�ng src, som best�r av allt fr�n b�rjan av edge tills dess att den hittar en tab
				edge.erase(0, src.length() + 1); // tar bort informatione om den substr�ng vi skapat fr�n edge plus taben som vi hittade
				dest = edge.substr(0, edge.find('\t'));	//Skapar en substr�ng dest, med info om vilket node som kopplar med src noden, slutar n�r en tab hittas precis som innnan
				edge.erase(0, dest.length() + 1); //tar bort informationen fr�n edge som nu finns i substr�ng plus taben vi hittat
				cost = std::stoi(edge);	//G�r om kostnaden av f�rflyttingen till en int frpn string f�r att kunna j�mf�ra med andra senare
				for (unsigned int i = 0; i < the_nodes.size(); i++)
				{
					if (the_nodes[i].Node_name == src)	// om noden p� plats i i vektorn �r samma som src
					{
						for (unsigned int j = 0; j < the_nodes.size(); j++)
						{
							if(the_nodes[j].Node_name == dest)	//om noden p� plats j �r samma som dest
							{
								the_nodes[i].Move_cost[j] = cost;	//D� �r kostnaden mellan dem vad cost �r 
								the_nodes[j].Move_cost[i] = cost;	//detsamma g�ller om de �r det andra h�llet runt 
							}
						}
					}
				}

			}
		}
		read.close(); //st�nger filen fr�n l�sning

		for (unsigned int i = 0; i < the_nodes.size(); i++)	
		{
			for (unsigned int j = 0; j < the_nodes.size(); j++)
			{
				temp_vector.push_back(the_nodes[i].Move_cost[j]);	//L�gger till alla m�jliga kostnader f�r att flytta till den noden i temp_vector
			}
			Graph.push_back(temp_vector); //l�gger sedan till alla de talen i v�r graph 2D vektor 
		}
	}
}

int Railroad::min(std::vector<int> key, std::vector<bool> in_mst)
{
	int min = INT_MAX;	//En stor int f�r j�mf�relse ifall n�gon annan �r mindre
	for (unsigned int i = 0; i < Graph.size(); i++)
	{
		if (in_mst[i] == false && key[i] < min)
		{
			min = key[i];	//s�tter en ny min som v�rdet p� index plats i inom vektorn key 
			int min_index = i;	//indexplatsen f�r den minsta kostnaden �r i
			return i;	//retunerar indexet f�r den minsta kostnaden
		}
	}
	error(3);	//Kallar p� en errorutskrift
	return -1;
}

void Railroad::write(std::vector<int> primary, std::vector<std::vector<int>> Graph)
{
	std::ofstream write;	//skriver till en fil

	write.open("Answer.txt");	//�ppnar filen om den finns annars skapas den
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
			write << the_nodes[primary[j]].Node_name << '\t' << the_nodes[j].Node_name << '\t' << Graph[j][primary[j]] << std::endl;	//skriver ut alla v�gar vi tagit i ordningen vi tagit dem, i formatet "start	m�l		kostnaden"
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
		primary.push_back(INT_MAX); //S�tter v�rdet p� alla platser till o�ndligheten(eller ja bara ett v�ldigt stort tal)
		key.push_back(INT_MAX);	//S�tter alla nycklar till samma sak f�r att det ska bli l�ttare att se vilken som �r minst n�r vi ska leeta efter den minst
		in_mst.push_back(false); //Eftersom vi inte valt n�got 'n beh�ver alla positioner i in_mst vara false 
	}
	key[0] = 0; //initialiserar v�rdet p� plats noll till noll s� att f�rsta noden ska vara v�r root nod
	primary[0] = -1;	//initialliserar v�rdet p� plats noll i mst till -1 f�r att det �r v�r rot nod

	for (unsigned int j = 0; j < Graph.size() - 1; j++)
	{
		int k = min(key, in_mst);	//tar reda p� den minsta kostnaden

		in_mst[k] = true;	//s�tter in det framtagna indexet som inkluderat i mst

		for (unsigned int l = 0; l < Graph.size(); l++)
		{
			if (Graph[k][l] && in_mst[l] == false && Graph[k][l] < key[l])	// tar fram nya kostnader mellan noder f�r j�mf�relse om noden inte redan finns i mst och graph[k][l] m�ste var mindre �n INT_MAX
			{
				primary[l] = k;	//l�gger till kostnaden f�r flytten p� plats l i mst
				key[l] = Graph[k][l]; // s�tter key[l] till ett nytt nummer som �r mindre �n INT_MAX
			}
		}
	}
	write(primary, Graph); //kallar p� en funktion som ska skriva ut grafen till en fil

}