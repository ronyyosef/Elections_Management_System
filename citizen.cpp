#include "citizen.h"


namespace election {
	Citizen::Citizen(const string& name,const string& ID, int birth_year, County* county ,const int Elections_year)
	{
		setID(ID);
		setBY(birth_year, Elections_year);
		this->_county = county;
		this->_inParty = false;
		this->_hasVoted = false;
		this->_countyDelegator = nullptr;
		this->_votedTo = nullptr;
		this->_memberIn = nullptr;
		this->_name = name;
	}

	Citizen::Citizen(Citizen& source)
	{//dont need to check valid birth year and ID because the will be check in the ctor; 
		this->_ID = source._ID;
		this->_birth_year = source._birth_year;
		this->_inParty = source._inParty;
		this->_hasVoted = source._hasVoted;
		this->_votedTo = source._votedTo;
		this->_memberIn = source._memberIn;
		this->_county = source._county;
		this->_countyDelegator = source._countyDelegator;
		this->_name = source._name;
	}

	void Citizen::setPartyStatus(County* const delegating)
	{
		if (delegating != nullptr)
		{
			_inParty = true;
			_countyDelegator = delegating;
		}
	}

	void Citizen::printCitizen() const
	{
		std::cout << "Name: " << _name << std::endl;
		std::cout << "ID number: " << _ID << std::endl;
	}

	void Citizen::setVote(Party* const votedTo)
	{
		_votedTo = votedTo;
		_hasVoted = true;
	}

	ostream& operator << (ostream& os, const Citizen& citizen)
	{
		os << "Name: " << citizen._name << endl;
		os << "ID number: " << citizen._ID << endl;
		os << "Lives in: " << citizen._county->getName() << endl;
		if (citizen._votedTo)
			os << "voted " << citizen._votedTo->getName() << endl;
		os << endl;
		return os;
	}

	void Citizen::setID(const string& ID)
	{
		if (ID.length() != IDLEN)
		{
			string error("ID must be 9 digits");
			throw error;
		}

		for (int i = 0; i < ID.length(); i++)
		{
			if(isdigit(ID[i]) == 0)
			{
				string error("ID must include numbers only");
				throw error;
			}
		}
		_ID = ID;
	}

	void Citizen::setBY(const int& birth_year, const int& Elections_year)
	{
		if ((Elections_year - birth_year) < 18)
		{
			string error("Cannot join election, under 18 years old");
			throw error;
		}
		_birth_year = birth_year;
	}
}