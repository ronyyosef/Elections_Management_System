#pragma once
#include <fstream>
#include "citizen.h"
#include "party.h"
#include "County.h"
#include "UnitedCounty.h"
#include "dividingcounty.h"
#include "DynamicArray.h"
#include <vector>

namespace election
{
	//Macro Functions which repeat several times through the function implementation.
	#define Find_Citizen_ID(ID) _citizenVector.begin(), _citizenVector.end(), [ID](const Citizen* p) { return p->getID() == ID; }
	#define Find_county_Serial_Num(countynum)_countyArr.begin(), _countyArr.end(),[countynum](County* p) { return p->getserial() == countynum; }
	#define Find_Party_Serial_Num(numOfParty)_partyVector.begin(), _partyVector.end(),[numOfParty](const Party* p) { return p->getserial() == numOfParty;}

	class BaseElections
	{
	protected:
		DynamicArray <County*> _countyArr;
		vector <Party*> _partyVector;
		vector <Citizen*> _citizenVector;
		int _day, _month, _year;
		enum Month {
			JANUARY=1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVERMBER, DECEMBER 
		};
		enum Days {
			MINIMUM=1, MAX_FEBRUARY=28 ,MAX_SHORT=30, MAX_LONG
		};
		void date_check(const int& day, const int& month, const int& year) const;
	public:
		//ctor
		BaseElections(const int& day, const int& month, const int& year);
		//copy-ctor
		BaseElections(const BaseElections& other) = delete;
		//dtor
		~BaseElections();
		
		//Operators
		BaseElections& operator=(const BaseElections& other) = delete;

		void setLeaders();
		void restartElectors();
		//Adders
		virtual void addCounty(const string& name, int numOfElectors, bool isUnited);
		void addCitizen(const string& name, const string& ID, int birthYear, int countynum);
		void addParty(const string& name, const string& ID);
		void addParty_BY_ID(const string& name, const string& ID);
		virtual void addCitizenAsDelegateCounty(const string& ID, int partyNum, int CountyNum) = 0;
		void addOneVote(const string& ID, int numOfParty);
		//print
		void printDate() const;
		virtual void printAllCounties() const {}
		virtual void printAllCitizens() const {}
		virtual void printAllParties() const {}
		virtual void printVoteStatus() {}
		//files
		void save(const string& fileName, int election_type);
		void read(const string& fileName);
	};
}