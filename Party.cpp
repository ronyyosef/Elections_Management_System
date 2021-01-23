#include "party.h"

namespace election {

	int Party::partyCount = 0;

	Party::Party(const string& name, Citizen* leader) : _leader(leader)
	{
		_name = name;
		partyCount = partyCount + 1;
		_leaderID = _leader->getID();
		_elector = 0;
		_serial = partyCount;
		_delegatorCount = 0;
		_votesCounter = 0;
	}

	Party::Party(const string& name, const string& leaderID)
	{
		_name = name;
		partyCount = partyCount + 1;
		_leaderID = leaderID;
		_serial = partyCount;
		_delegatorCount = 0;
		_elector = 0;
		_votesCounter = 0;
	}

	Party::Party(const Party& source)
	{
		this->_name = source._name;
		this->_delegatorCount = source._delegatorCount;
		this->_votesCounter = source._votesCounter;
		this->_leader = source._leader;
		this->_leaderID = source._leaderID;
		this->_serial = source._serial;
		this->_elector = source._elector;
		this->_citizenVector = _citizenVector;
	}

	void Party::addDelegate(Citizen* citizen)
	{
		_citizenVector.push_back(citizen);
		_delegatorCount++;
	}

	void Party::printParty()
	{
		sort(_citizenVector.begin(), _citizenVector.end(), [](Citizen* a, Citizen* b)
			{return a->getDelegating()->getserial() < b->getDelegating()->getserial(); });
		string currName;
		cout << *this;
		if (_citizenVector.empty())
		{
			cout << "There are no Electors." << endl;
		}
		else
		{
			auto currCitizen = _citizenVector.begin();
			while (currCitizen != _citizenVector.end())
			{
				currName = (*currCitizen)->getDelegating()->getName();
				cout << "Electors of " << currName << " county:" << endl;
				while ((currCitizen != _citizenVector.end()) && ((*currCitizen)->getDelegating()->getName() == currName))
				{
					cout << (*currCitizen)->getName() << endl;
					currCitizen++;
				}
			}
			cout << endl;
		}
	}

	void Party::printParty_NoCounties() 
	{
		sort(_citizenVector.begin(), _citizenVector.end(), [](Citizen* a, Citizen* b)
			{return a->getDelegating()->getserial() < b->getDelegating()->getserial(); });
		string currName;
		cout << "For " << _name << " Party:" << endl;
		cout << "Leader: " << _leader->getName() << endl;
		cout << _name << "'s elector list:" << endl;
		auto currCitizen = _citizenVector.begin();
		if (_citizenVector.empty())
		{
			cout << "There are no Electors." << endl;
		}
		else
		{
			cout << "Party member list:" << endl;
			std::for_each(_citizenVector.begin(), _citizenVector.end(), [](Citizen* p) {p->printCitizen(); });
		}
		cout << endl;
	}

	ostream& operator << (ostream& os, const Party& party)
	{
		os << "Party's serial number: " << party._serial << endl;
		os << "Party's name: " << party._name << endl;
		os << "Leader's name: " << party._leader->getName() << endl;
		return os;
	}

	Party& Party::operator =(const Party& source)
	{
		if (this != &source)
		{
			this->_name = source._name;
			this->_delegatorCount = source._delegatorCount;
			this->_votesCounter = source._votesCounter;
			this->_leader = source._leader;
			this->_leaderID = source._leaderID;
			this->_serial = source._serial;
			this->_elector = source._elector;
			this->_citizenVector = _citizenVector;
		}
		return *this;
	}
}