//
// Created by Dominik Grybos on 31.05.2017.
//


#include <iostream>
#include "galaxy.h"
#include "main.h"

Galaxy &Galaxy::load(std::istream &in)
{
	in >> size >> target;
	starsystems = new Starsystem[size];
	for(int i = 0; i < size; i++)
		starsystems[i].load(this, in);


	return *this;
}

Galaxy &Galaxy::add(Reaper *r)
{
	reapers.push_back(r);
	return *this;
}

Galaxy &Galaxy::add(Hero *h)
{
	hero = h;
	return *this;
}

Result Galaxy::simulate()
{
	Starsystem *heroSystem = hero->start(this);
	Starsystem *reaperSystem;
	if(heroSystem == nullptr)
		return Result::Invalid;

	if(heroSystem->isTarget())
		return Result::Success;

	for(Reaper *r: reapers)
	{
		reaperSystem = r->start(this);
		if(reaperSystem == heroSystem)
			return Result::Failure;
		reaperSystem->setVisited();
	}

	while(!hero->isEnd())
	{
		heroSystem = hero->advance();
		if(heroSystem == nullptr)
			return Result::Invalid;

		if(heroSystem->isTarget())
			return Result::Success;
		if(heroSystem->isVisited())
			return Result::Failure;
		for(Reaper *r : reapers)
		{
			reaperSystem = r->advance();
			if(reaperSystem == heroSystem)
				return Result::Failure;
			reaperSystem -> setVisited();
		}
	}


	return Result::Invalid;
}

Galaxy::Starsystem *Galaxy::getSystem(size_t idx)
{
	if(idx >= size)
		return nullptr;
	else
		return &starsystems[idx];
}

Galaxy::~Galaxy()
{
	delete[] starsystems;
	for(Reaper *r : reapers)
		delete r;
	delete hero;
}

Galaxy::Starsystem &Galaxy::Starsystem::load(Galaxy *g, std::istream &in)
{
	galaxy = g;
	in >> size;
	connections = new int[size];

	for(int i = 0; i < size; i++)
		in >> connections[i];

	return *this;
}

Galaxy::Starsystem *Galaxy::Starsystem::adjacent(size_t idx)
{
	if(idx >= size)
		return nullptr;
	else
		return &galaxy->starsystems[connections[idx]];
}

bool Galaxy::Starsystem::isTarget()
{
	return &galaxy->starsystems[galaxy->target] == this;
}

Galaxy::Starsystem::~Starsystem()
{
	delete[] connections;
}

void Galaxy::Starsystem::setVisited()
{
	visited = true;
}

bool Galaxy::Starsystem::isVisited()
{
	return visited;
}
