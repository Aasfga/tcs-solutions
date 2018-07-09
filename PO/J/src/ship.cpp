#include <iostream>
#include "ship.h"

void Ship::setPath(std::istream &in)
{
	in >> size >> startSystem;
	size--;
	paths = new int[size];
	for(int i = 0; i < size; i++)
	{
		in >> paths[i];
	}
}

Galaxy::Starsystem *Ship::start(Galaxy *g)
{
	galaxy = g;
	crrSystem = g->getSystem(startSystem);
	return crrSystem;
}

Galaxy::Starsystem *Ship::advance()
{
	if(index < size)
		crrSystem = crrSystem->adjacent(paths[index++]);
	return crrSystem;
}

Ship::~Ship()
{
	delete[] paths;
}

bool Hero::isEnd()
{
	return index >= size;
}
