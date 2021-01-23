#pragma once
#include "county.h"

namespace election
{

	class DividingCounty : public County
	{
	public:
		//ctor
		DividingCounty(const string& name, int numOfElectors) : County(name, numOfElectors) {};
		//copy-ctor
		DividingCounty(const DividingCounty& other) = delete;
		//dtor
		~DividingCounty() = default;
		//operators
		DividingCounty& operator=(DividingCounty& source) = delete;

		virtual void calc_electors(vector<Party*>& partyVector, int* votes);
		virtual void printCountyType() const {std::cout << "County's type: Divided county" << std::endl;}
	};
}