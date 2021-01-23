#pragma once
#include "county.h"
#include <string>
namespace election
{
	class UnitedCounty : public County
	{
	public:
		//ctor
		UnitedCounty(const string& name, int numOfElectors) : County(name, numOfElectors) {};
		//copy-ctor
		UnitedCounty(UnitedCounty& source) = delete;
		//dtor
		~UnitedCounty() = default;
		//operators
		UnitedCounty& operator=(UnitedCounty& other) = delete;
		virtual void calc_electors(vector<Party*>& partyVector, int* votes);
		virtual void printCountyType() const { std::cout << "County's type: United county" << std::endl; }
	};
}