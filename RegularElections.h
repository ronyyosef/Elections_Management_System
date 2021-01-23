#pragma once
#include "BaseElections.h"

namespace election
{
	class RegularElections : public BaseElections
	{
	private:

	public:
		//ctor
		RegularElections(int day, int month, int year) : BaseElections(day, month, year) { }
		//copy-ctor
		RegularElections(RegularElections& other) = delete;
		//dtor
		~RegularElections() = default;
		//operators
		RegularElections& operator=(RegularElections& other) = delete;

		virtual void addCitizenAsDelegateCounty(const string& ID, int partyNum, int CountyNum);
		//print
		virtual void printAllCounties() const;
		virtual void printAllCitizens() const;
		virtual void printAllParties() const;
		virtual void printVoteStatus();
	};
}

