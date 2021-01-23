#include "county.h"

namespace election {

	int County::countS = 0;

	County::County(const string& name , int numOfElectors)
	{
		if (numOfElectors < 0) {
			string error("Please enter a POSITIVE number of electors (or zero) next time.");
			throw error;
		}
		_name = name;
		_numOfElectors = numOfElectors;
		_ciCount = 0;
		_voted = 0;
		_howManyElectors = 0;
		countS = countS + 1;
		_serial = countS;
		_winner = nullptr;
	}

	County::County(const County& source)
	{
		this->_name = source._name;
		this->_numOfElectors = source._numOfElectors;
		this->_ciCount = source._ciCount;
		this->_voted = source._voted;
		this->_howManyElectors = source._howManyElectors;
		this->_citizenVector = source._citizenVector;
		this->_delegatorVector = source._delegatorVector;
		this->_serial = source._serial;
		this->_winner = source._winner;
	}

	County::~County()
	{
		countS = 0;
		std::for_each(_citizenVector.begin(), _citizenVector.end(), [](Citizen* p) {delete p; });
	}

	void County::setWinner(int partyNum)
	{
		_winner = getPartyByNum(partyNum);
	}

	Citizen* County::addCitizen(const string& name, const string& ID, int birthYear, County* living, const int Elections_year)
	{
		Citizen* newCitizen = new Citizen(name, ID, birthYear, living, Elections_year);
		_citizenVector.push_back(newCitizen);
		_ciCount++;
		return newCitizen;
	}


	void County::addDelegator(Citizen* delegator)
	{
		_delegatorVector.push_back(delegator);
		sort(_delegatorVector.begin(), _delegatorVector.end(), [](Citizen* p1, Citizen* p2) 
			{return p1->getParty()->getserial() < p2->getParty()->getserial();});
		_howManyElectors++;
	}

	void County::printCitizens()const
	{
		std::for_each(_citizenVector.begin(), _citizenVector.end(), [](Citizen* p) {std::cout << *p; });
	}

	float County::getVotingPrecentage()const 
	{ 
		if (_ciCount == 0)
			return 100;
		return (((float)_voted / (float)_ciCount) * 100); 
	}
	int* County::calculate_VotingSegmentation(const int numOfParties)
	{
		int * segArray = new int[numOfParties]();
		for_each(_citizenVector.begin(), _citizenVector.end(), [segArray](Citizen* p) 
		{
			if (p->getVotedTo() != nullptr)
				segArray[p->getVotedTo()->getserial() - 1]++;
		});
		return segArray;
		// index [0] of array is party with serial number 1, 
		//index [1] of array is party with serial number 2, etc...
	}

	int County::getMaxVotesPartyNum(int* votes, const int numParties)
	{
		int max = votes[numParties - 1], maxIndex = numParties - 1;
		for (int i = maxIndex - 1; i >= 0; i--)
		{
			if (votes[i] >= max)
			{
				maxIndex = i;
				max = votes[i];
			}
		}
		return maxIndex + 1; // "+1" Because of index-serialNum match system.
	}

	Party* County::getPartyByNum(int serialNum)const
	{
		Party* votedTo;
		auto itr = _citizenVector.begin();
		while (itr != _citizenVector.end())
		{
			votedTo = (*itr)->getVotedTo();
			if ((votedTo != nullptr) && (votedTo->getserial()) == serialNum)
				return votedTo;
			itr++;
		}
		return nullptr;
	}


	void County::printElectorsFromPartyByQuantity(int electorsNum, int partyNum)const
	{
		bool flag = false;
		if (electorsNum == 0 || _delegatorVector.empty())
		{
			std::cout << "There are no electors from this party" << std::endl;
			return;
		}
		auto itr = _delegatorVector.begin();
		while(itr != _delegatorVector.end())
		{
			if ((*itr)->getParty()->getserial() > partyNum || electorsNum == 0)
			{
				if (flag == false)
					std::cout << "There are no electors from this party" << std::endl;
				return;
			}
			else if ((*itr)->getParty()->getserial() < partyNum)
				itr++;
			else
			{
				std::cout << (*itr)->getName() << std::endl;
				electorsNum--;
				flag = true;
				itr++;
			}
		}	
	}
	
	ostream& operator << (ostream& os, const County& county)
	{
		os << "County's serial number: " << county._serial << endl;
		os << "County's name: " << county._name << endl;
		os << "County's delegator number: " << county._numOfElectors << endl;
		return os;
	}
	
}