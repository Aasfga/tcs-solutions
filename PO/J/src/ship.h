#ifndef PO10_SHIP_HEADER
#define PO10_SHIP_HEADER

#include "galaxy.h"

class Ship
{
protected:
	int size;
	int startSystem;
	int *paths;
	Galaxy *galaxy;
	int index = 0;
	Galaxy::Starsystem *crrSystem;

public:
	virtual void setPath(std::istream &in);

	virtual Galaxy::Starsystem *start(Galaxy *g);

	virtual Galaxy::Starsystem *advance();

	virtual ~Ship();
};

class Hero : public Ship
{
public:
	bool isEnd();
};

class Reaper : public Ship
{
};

#endif
