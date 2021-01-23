#include "SimpleElections.h"
#include <vector>
#include <algorithm>

namespace election
{
	SimpleElections::SimpleElections(int day, int month, int year, int elector_num) : BaseElections(day, month, year)
	{
		string name = "One";
		BaseElections::addCounty(name, elector_num, 1);// One is United county
	}

	void SimpleElections::addCounty(const string& name, int numOfElectors, bool isUnited)
	{
		string error("YOU CANNOT ADD A COUNTY IN THIS MODE.");
		throw error;
	}

	void SimpleElections::addCitizenAsDelegateCounty(const string& ID, int partyNum, int CountyNum)
	{
		CountyNum = 1;
		vector<Citizen*>::iterator citizen = std::find_if(Find_Citizen_ID(ID));
		if (citizen == _citizenVector.end())
		{
			string error("No citizen with ID: " + ID + " exist");
			throw error;
		}
		vector<Party*>::iterator party = std::find_if(Find_Party_Serial_Num(partyNum));
		if (party == _partyVector.end()) {
			string error("No party with serial number: " + to_string(partyNum) + " exist");
			throw error;
		}
		DynamicArray<County*>::iterator citizDelegating = std::find_if(Find_county_Serial_Num(CountyNum));
		if (citizDelegating == _countyArr.end()) {
			string error ("No county with serial number: " + to_string(CountyNum) + " exist");
			throw error;
		}
		(*citizen)->setPartyStatus(*citizDelegating);
		(*party)->addDelegate(*citizen);
		(*citizen)->setPartyMember(*party);
		(*citizDelegating)->addDelegator(*citizen);
	}

	void SimpleElections::printAllCitizens() const 
	{
		std::for_each(_citizenVector.begin(), _citizenVector.end(),[](Citizen* p){p->printCitizen();});
	}

	void SimpleElections::printAllParties() const
	{
		std::for_each(_partyVector.begin(), _partyVector.end(),[](Party* p){p->printParty_NoCounties();});
	}

	void SimpleElections::printVoteStatus()
	{
		int sumOfVotes = 0;
		int elec_num = _countyArr[0]->getNumOfElectors();
		for (int i = 0; i < _partyVector.size(); i++)
			sumOfVotes += _partyVector[i]->getVotes();
		printDate();
		cout << "Total votes precentage: " << ((float)((sumOfVotes * 100) / _citizenVector.size())) << "%" << endl << endl;
		sort(_partyVector.begin(), _partyVector.end(), [](Party* a, Party* b) 
			{return a->getVotes() > b->getVotes();});
		std::for_each(_partyVector.begin(), _partyVector.end(), [sumOfVotes, elec_num](Party* p)
			{
				int  numOfElectors;
				cout << "For party: " << p->getName() << endl;
				numOfElectors = std::min(p->getDelegatorCount(), elec_num * p->getVotes() / sumOfVotes);
				cout << "Number of electors the party won: " << numOfElectors << endl;
				cout << "Number of votes the party won: " << p->getVotes() << endl;
				cout << "Vote precentage the party won: " << (float)((100 * (p->getVotes())) / sumOfVotes) << "%" << endl;
				cout << "Party's candidate: " << p->getLeader()->getName() << endl;
				//ASSUMPTION: ELECTOR'S NUMBER OF A SPECIFIC PARTY IS ROUNDED DOWN
				cout << endl;});
		sort(_partyVector.begin(), _partyVector.end(), [](Party* a, Party* b) {
			return a->getserial() < b->getserial(); });
	}
}