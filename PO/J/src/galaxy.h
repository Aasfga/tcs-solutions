#ifndef PO10_GALAXY_HEADER
#define PO10_GALAXY_HEADER

#include "result.h"
#include <istream>
#include <vector>

class Hero;

class Reaper;

class Galaxy
{


public:

	Galaxy &load(std::istream &in);

	Galaxy &add(Reaper *r);

	Galaxy &add(Hero *h);

	Result simulate();

	class Starsystem
	{
		bool visited = false;
		int size;
		int *connections;
		Galaxy *galaxy;


	public:
		Starsystem &load(Galaxy *g, std::istream &in);

		Starsystem *adjacent(size_t idx);

		bool isTarget();

		void setVisited();

		~Starsystem();

		bool isVisited();
	};

	Starsystem *getSystem(size_t idx);

	~Galaxy();


private:

	Starsystem *starsystems;
	std::vector<Reaper*> reapers;
	Hero *hero;
	int size;
	int target;
};


#endif

