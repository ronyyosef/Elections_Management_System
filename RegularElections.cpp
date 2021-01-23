#include "RegularElections.h"

namespace election
{
	void RegularElections::addCitizenAsDelegateCounty(const string& ID, int partyNum, int CountyNum)
	{
		vector<Citizen*>::const_iterator citizen = std::find_if(Find_Citizen_ID(ID));
		if (citizen == _citizenVector.end())
		{
			string error("Error,no citizen with ID: " + ID + " exist" );
			throw error;
		}
		vector<Party*>::const_iterator party = std::find_if(Find_Party_Serial_Num(partyNum));
		if (party == _partyVector.end()) {
			string error("Error,no party with serial number: " + to_string(partyNum) + " exist");
			throw error;
		}
		DynamicArray<County*>::const_iterator citizDelegating = std::find_if(Find_county_Serial_Num(CountyNum));
		if (citizDelegating == _countyArr.end()) {
			string error("Error,no county with serial number: " +to_string(CountyNum) + " exist");
			throw error;
		}
		(*citizen)->setPartyStatus(*citizDelegating);
		(*party)->addDelegate(*citizen);
		(*citizen)->setPartyMember(*party);
		(*citizDelegating)->addDelegator(*citizen);
	}

	void RegularElections::printAllCounties() const {
		std::for_each(_countyArr.begin(), _countyArr.end(), [](County* p)
			{
				std::cout << *p ;
				p->printCountyType();
				std::cout << std::endl;
			});
	}

	void RegularElections::printAllCitizens() const {
		std::for_each(_countyArr.begin(), _countyArr.end(), [](County* p){ p->printCitizens();});
	}

	void RegularElections::printAllParties() const {
		std::for_each(_partyVector.begin(), _partyVector.end(), [](Party* p){ p->printParty();});
	}

	void  RegularElections::printVoteStatus()
	{
		int i, j, numParties = _partyVector.size();
		int* numOfElectorsForParty = new int[numParties]();
		int** votes = new int* [_countyArr.size()];
		float tempfloat;
		int temp;
		printDate();
		for (i = 0; i < _countyArr.size(); i++)
		{
			votes[i] = _countyArr[i]->calculate_VotingSegmentation(numParties);
			_countyArr[i]->setWinner(_countyArr[i]->getMaxVotesPartyNum(votes[i], numParties));
			cout << "County's name: " << _countyArr[i]->getName() << endl;
			cout << "County delegators' number: " << _countyArr[i]->getNumOfElectors() << endl;
			if (_countyArr[i]->getWinner() == nullptr){cout << "there is no winner in this county " << endl;}
			else{cout << "The winning candidate in this county is: " << _countyArr[i]->getWinner()->getLeader()->getName() << endl;}
			cout << "Voting precentage in the county: " << _countyArr[i]->getVotingPrecentage() << "%" << endl;
			for (j = 0; j < numParties; j++)
			{
				cout << votes[i][j] << " votes for \"" << _partyVector[j]->getName() << "\"" << endl;
			}
			cout << _countyArr[i]->getName() << "'s Electors:" << endl;
			
			_countyArr[i]->calc_electors(_partyVector, votes[i]);
			cout << endl;
			
		}
		sort(_partyVector.begin(), _partyVector.end(), [](Party* a, Party* b) {
			return a->getElectors() > b->getElectors();});
		std::for_each(_partyVector.begin(), _partyVector.end(), [](Party* p)
			{
				std::cout << "Party name: " << p->getName() << std::endl;
				std::cout << "Party leader: " << p->getLeader()->getName() << std::endl;
				std::cout << "sum of Electors the party won: " << p->getElectors() << std::endl;
				std::cout << "total votes number: " << p->getVotes() << std::endl;
				std::cout << std::endl;
			});
		sort(_partyVector.begin(), _partyVector.end(), [](Party* a, Party* b) {
			return a->getserial() < b->getserial(); });
		restartElectors();
		std::cout << std::endl;
	}
}
