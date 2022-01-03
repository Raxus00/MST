#ifndef RAILROAD_HPP
#define RAILROAD_HPP

#include<vector>
#include<string>
#include <fstream>
#include<iostream>


struct Node
{
	std::vector<int> Move_cost;	//vektor för hur mycket det kostar att lägga just den vägen
	std::vector<Node*> adjacent_nodes;	//Håller reda på vilken nod som koppla till vilken
	std::string Node_name; // representerar namnet på noderna
};
class Railroad
{
public:

	void Read(std::string& path);	//Läser in data från en fil
	void write(std::vector<int> parimary, std::vector<std::vector<int>> Graph);	//skriver ut mST trädet
	void PrimA();	//skapar MST trädet
	int min(std::vector<int> key, std::vector<bool> in_mst);	//tar fram indexet till den nod med minsta förflytningskostnaden
	void error(int number);	//egen funtion för error koder

private:
	std::vector<Node> the_nodes; //vektor av structen node som kan berätta all information om alla noder
	std::vector<std::vector<int>> Graph; //En nestlad vektor som ska representera all kant kostnader för alla noder
};





#endif // !RAILROAD_HPP
