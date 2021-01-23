#include "BaseElections.h"
using namespace std;
#define MAX_SIZE 256
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

namespace election
{	
	BaseElections::BaseElections(const int& day, const int& month, const int& year)
	{
		date_check(day, month, year);
		_day = day;
		_month = month;
		_year = year;
	}

	void BaseElections::date_check(const int& day, const int& month, const int& year) const
	{
		string error;
		if (year < MINIMUM) {
			error = "Year value has to be a positive number";
			throw error;
		}
		if ((month < JANUARY) || (month) > DECEMBER) {
			error = "Invalid month value (not between 1 to 12)";
			throw error;
		}
		if ((month == APRIL || month == JUNE || month == SEPTEMBER || month == NOVERMBER) && ((month < MINIMUM)
			|| (day > MAX_SHORT))) {
			error = "Invalid day value";
			throw error;
		}
		else if ((month != FEBRUARY) && ((month < MINIMUM) || (day > MAX_LONG))) {
			error = "Invalid day value";
			throw error;
		}
		else if ((month == FEBRUARY) && ((month < MINIMUM) || (day > MAX_FEBRUARY))) {
			error = "Febuary doesn't have more than 28 days";
			throw error;
		}
	}

	void BaseElections::addCounty(const string& name, int numOfElectors, bool isUnited)
	{//In case any of the allocations fails (may it be inside Vector library, 'new' or any of counties)-> throws to menu 
		County* newCounty;
		if (isUnited)
			newCounty = new UnitedCounty(name, numOfElectors);
		else
			newCounty = new DividingCounty(name, numOfElectors);
		_countyArr.push_back(newCounty);
	}
	BaseElections::~BaseElections()
	{
		std::for_each(_countyArr.begin(), _countyArr.end(), [](County* p) {delete p; });
		std::for_each(_partyVector.begin(), _partyVector.end(), [](Party* p) {delete p; });
	}


	void BaseElections::addCitizen(const string& name, const string& ID, int birthYear, int countynum)
	{
		if (std::find_if(Find_Citizen_ID(ID)) != _citizenVector.end())
		{
			string error("The program has found a citizen with an ID of " + ID + '\n' +
				"A new citizen data won't be saved in order to prevent duplication of ID.");
			throw error;
		}
		DynamicArray<County*>::iterator living = std::find_if(Find_county_Serial_Num(countynum));
		if (living == _countyArr.end())
		{
			string error("There is no county with " + to_string(countynum) + " as it's serial number");
			throw error;
		}
		Citizen* newCitizen = (*living)->addCitizen(name, ID, birthYear, *living, _year);
		_citizenVector.push_back(newCitizen);
	}

	void BaseElections::addParty(const string& name, const string& ID)
	{
		vector<Citizen*>::iterator leader = std::find_if(Find_Citizen_ID(ID));
		if (leader == _citizenVector.end())
		{
			string error("No citizen has been found");
			throw error;
		}
		Party* newParty = new Party(name, *leader);
		_partyVector.push_back(newParty);
	}

	void BaseElections::addParty_BY_ID(const string& name, const string& ID)
	{
		Party* newParty = new Party(name, ID);
		_partyVector.push_back(newParty);
	}

	void BaseElections::addOneVote(const string& ID, int numOfParty)
	{
		vector<Citizen*>::iterator citizen = std::find_if(Find_Citizen_ID(ID));
		if (citizen == _citizenVector.end())
		{
			string error("There no citizen with ID " + ID + "exist");
			throw error;
		}
		if ((*citizen)->getHasVoted()) {
			string error("The ID " + ID + " has already voted." + '\n' + "This vote won't be counted.");
			throw error;
		}
		else {
			vector<Party*>::iterator votedTo = std::find_if(Find_Party_Serial_Num(numOfParty));
			if (votedTo == _partyVector.end()) {
				string error("No party with serial number: " + to_string(numOfParty) + " exist.");
				throw error;
			}
			(*votedTo)->setVotes();
			(*citizen)->setVote(*votedTo);
			(*citizen)->getCounty()->addVoter();
		}
	}

	void BaseElections::printDate() const
	{
		cout << endl << "Election's date: " << _day << "/" << _month << "/" << _year << endl << endl;
	}
	void BaseElections::restartElectors()
	{
		std::for_each(_partyVector.begin(), _partyVector.end(), [](Party* p) {p->setElectors(0); });
	}

	void BaseElections::save(const string& fileName, int election_type)
	{
		/* --------------------------------------------------------------------------------------------------
		County will be written in this format :
		isUnited(bool) -> name_len(int) -> name(char*) -> numOfElectors(int)

		Party will be written in this format :
		name_len(int) -> name(char*) -> party_leader_id(int)

		Citizen will be written in this format:
		id(int) -> name_len(int) -> name(char*) -> BY(int) -> inpartybool(bool)
		-> hasvoted(bool) -> voted_to(int) -> party_member(int) -> in_county(int) ->county_delegating(int)
		-----------------------------------------------------------------------------------------------------*/
		int num_of_county = _countyArr.size(), num_of_party = _partyVector.size(), num_of_citizen = _citizenVector.size(); //for loops
		int name_len, i; // <all>
		string id;
		bool isUnited; int numOfElectors;	 // <county>
		string party_leader_id, error;// <party> 
		int BY; bool inpartybool, hasvoted; int voted_to, party_member, in_county, county_delegating;// <citizen> 	
		int total_sets;	
		ofstream file;
		file.open(fileName, ios::binary | ios::out);
		if (file.is_open() == false)
		{
			error = "File didn't open.";
			throw error;
		}
		file.write(rcastcc(&election_type), sizeof(int));
		file.write(rcastcc(&_day), sizeof(int));
		file.write(rcastcc(&_month), sizeof(int));
		file.write(rcastcc(&_year), sizeof(int));
		if (election_type == 2)
			total_sets = _countyArr[0]->getNumOfElectors();
		else
			total_sets = -1;
		file.write(rcastcc(&total_sets), sizeof(int));
		file.write(rcastcc(&num_of_county), sizeof(int));
		for (i = 0; i < _countyArr.size(); i++)
		{
			if (strcmp(typeid(*(_countyArr[i])).name(), "class election::UnitedCounty") == 0)
				isUnited = 1;
			else
				isUnited = 0;
			name_len = _countyArr[i]->getName().length();
			numOfElectors = _countyArr[i]->getNumOfElectors();

			file.write(rcastcc(&isUnited), sizeof(bool));
			file.write(rcastcc(&name_len), sizeof(int));
			file.write(_countyArr[i]->getName().c_str(), name_len * sizeof(char));
			file.write(rcastcc(&numOfElectors), sizeof(int));
		}
		file.write(rcastcc(&num_of_party), sizeof(int));
		for (i = 0; i < _partyVector.size(); i++)
		{
			name_len = _partyVector[i]->getName().length();
			party_leader_id = _partyVector[i]->getLeader()->getID();

			file.write(rcastcc(&name_len), sizeof(int));
			file.write(_partyVector[i]->getName().c_str(), name_len * sizeof(char));
			file.write(party_leader_id.c_str(), party_leader_id.length() * sizeof(char));
		}
		file.write(rcastcc(&num_of_citizen), sizeof(int));
		for (i = 0; i < _citizenVector.size(); i++)
		{
			id = _citizenVector[i]->getID();
			name_len = _citizenVector[i]->getName().length();
			BY = _citizenVector[i]->getBY();
			inpartybool = _citizenVector[i]->getInParty();
			hasvoted = _citizenVector[i]->getHasVoted();
			if (_citizenVector[i]->getVotedTo() == nullptr)
				voted_to = 0;
			else
				voted_to = _citizenVector[i]->getVotedTo()->getserial();
			if (_citizenVector[i]->getParty() != nullptr)
				party_member = _citizenVector[i]->getParty()->getserial();
			else
				party_member = 0;
			in_county = _citizenVector[i]->getCounty()->getserial();
			if (_citizenVector[i]->getInParty())
				county_delegating = _citizenVector[i]->getDelegating()->getserial();
			else
				county_delegating = 0;
			file.write(id.c_str(), id.length() * sizeof(char));
			file.write(rcastcc(&name_len), sizeof(int));
			file.write(_citizenVector[i]->getName().c_str(), name_len * sizeof(char));
			file.write(rcastcc(&BY), sizeof(int));
			file.write(rcastcc(&inpartybool), sizeof(bool));
			file.write(rcastcc(&hasvoted), sizeof(bool));
			file.write(rcastcc(&voted_to), sizeof(int));
			file.write(rcastcc(&party_member), sizeof(int));
			file.write(rcastcc(&in_county), sizeof(int));
			file.write(rcastcc(&county_delegating), sizeof(int));
		}
		file.close();
	}

	void BaseElections::read(const string& fileName)
	{
		//ASSUMPTION: THE FILE HAS BEEN WRITTEN CORRECTLY, AS WE WROTE "BaseElections::save"
		int num_of_county, num_of_party, num_of_citizen; // Loops parameters
		char name[MAX_SIZE];
		int name_len, i, grabage; // <all>
		string id, error;
		char tempstr[MAX_SIZE];
		bool isUnited; int numOfElectors;	 // <county>
		string party_leader_id;// <party> 
		int BY; bool inpartybool, hasvoted; int voted_to, party_member, in_county, county_delegating;// <citizen> 
		ifstream file(fileName, ios::in | ios::binary);
		if (file.is_open() == false)
		{
			error = "File didn't open.";
			throw error;
		}
		for (i = 0; i < 5; i++)
			file.read(rcastc(&grabage), sizeof(int)); // read the first five int to grabage		
		file.read(rcastc(&num_of_county), sizeof(int));
		for (i = 0; i < num_of_county; i++)
		{
			file.read(rcastc(&isUnited), sizeof(bool));
			file.read(rcastc(&name_len), sizeof(int));
			file.read(rcastc(name), sizeof(char) * name_len); name[name_len] = '\0';
			file.read(rcastc(&numOfElectors), sizeof(int));
			BaseElections::addCounty(name, numOfElectors, isUnited);
		}
		file.read(rcastc(&num_of_party), sizeof(int));
		for (i = 0; i < num_of_party; i++)
		{
			file.read(rcastc(&name_len), sizeof(int));
			file.read(rcastc(name), sizeof(char) * name_len); name[name_len] = '\0';
			file.read(tempstr, IDLEN * sizeof(char));
			tempstr[IDLEN] = '\0'; party_leader_id = tempstr;
			addParty_BY_ID(name, party_leader_id);
		}
		file.read(rcastc(&num_of_citizen), sizeof(int));
		for (i = 0; i < num_of_citizen; i++)
		{
			file.read(tempstr, IDLEN * sizeof(char));
			tempstr[IDLEN] = '\0'; id = tempstr;
			file.read(rcastc(&name_len), sizeof(int));
			file.read(rcastc(name), sizeof(char) * name_len); name[name_len] = '\0';
			file.read(rcastc(&BY), sizeof(int));
			file.read(rcastc(&inpartybool), sizeof(bool));
			file.read(rcastc(&hasvoted), sizeof(bool));
			file.read(rcastc(&voted_to), sizeof(int));
			file.read(rcastc(&party_member), sizeof(int));
			file.read(rcastc(&in_county), sizeof(int));
			file.read(rcastc(&county_delegating), sizeof(int)); // the county the member delegating
			addCitizen(name, id, BY, in_county);
			if (hasvoted == true)
				addOneVote(id, voted_to);
			if (inpartybool == true)
				addCitizenAsDelegateCounty(id, party_member, county_delegating);
		}
		setLeaders();
		file.close();
	}

	void BaseElections::setLeaders()
	{
		string currID;
		for (int i = 0; i < _partyVector.size(); i++)
		{
			currID = _partyVector[i]->getLeaderID();
			vector<Citizen*>::const_iterator leader = std::find_if(Find_Citizen_ID(currID));
			_partyVector[i]->setLeader(*leader);
		}
	}
}