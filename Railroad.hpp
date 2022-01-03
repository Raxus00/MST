#ifndef RAILROAD_HPP
#define RAILROAD_HPP

#include<vector>
#include<string>
#include <fstream>
#include<iostream>


struct Node
{
	std::vector<int> Move_cost;	//vektor f�r hur mycket det kostar att l�gga just den v�gen
	std::vector<Node*> adjacent_nodes;	//H�ller reda p� vilken nod som koppla till vilken
	std::string Node_name; // representerar namnet p� noderna
};
class Railroad
{
public:

	void Read(std::string& path);	//L�ser in data fr�n en fil
	void write(std::vector<int> parimary, std::vector<std::vector<int>> Graph);	//skriver ut mST tr�det
	void PrimA();	//skapar MST tr�det
	int min(std::vector<int> key, std::vector<bool> in_mst);	//tar fram indexet till den nod med minsta f�rflytningskostnaden
	void error(int number);	//egen funtion f�r error koder

private:
	std::vector<Node> the_nodes; //vektor av structen node som kan ber�tta all information om alla noder
	std::vector<std::vector<int>> Graph; //En nestlad vektor som ska representera all kant kostnader f�r alla noder
};





#endif // !RAILROAD_HPP
