#pragma once
#include "BaseElections.h"

namespace election
{
	class SimpleElections : public BaseElections
	{
	public:
		//ctor
		SimpleElections(int day, int month, int year, int elector_num);
		//copy-ctor
		SimpleElections(SimpleElections& other) = delete;
		//dtor
		~SimpleElections() = default;
		//operators
		SimpleElections& operator=(SimpleElections& other) = delete;

		virtual void addCounty(const string& name, int numOfElectors, bool isUnited);
		virtual void addCitizenAsDelegateCounty(const string& ID, int partyNum, int CountyNum);

		//print
		virtual void printAllCounties() const { string error("NO COUNTIES IN THIS MODE"); throw error; }
		virtual void printAllCitizens() const;
		virtual void printAllParties() const;
		virtual void printVoteStatus();
	};
}
