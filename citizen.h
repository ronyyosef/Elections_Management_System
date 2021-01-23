#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "county.h"
#include "party.h"
#include <string>
namespace election
{
	#define IDLEN 9

	using namespace std;
	class County;
	class Party;
	class Citizen
	{
	private:
		string _ID;
		string _name;
		int _birth_year;
		bool _inParty;
		bool _hasVoted;
		Party* _votedTo;
		Party* _memberIn;
		County* _county;
		County* _countyDelegator;
	public:
		//ctor
		Citizen(const string& name, const string& ID, int birth_year, County* county,const int Elections_year);
		//copy-ctor
		Citizen(Citizen& source);
		//dtor
		~Citizen() = default;
		//operators
		Citizen& operator=(Citizen& source) = delete;
		//Setters
		void setID(const string& ID);
		void setBY(const int& birth_year, const int& Elections_year) ;
		void setPartyStatus(County* const delegating);
		void setVote(Party* const votedTo);
		void setPartyMember(Party* const party) { _memberIn = party; }
		//Getters
		bool getHasVoted() const { return _hasVoted; }
		bool getInParty() const { return _inParty; }
		const string& getID() const { return _ID; }
		const int& getBY() const { return _birth_year; }
		Party* getParty()const { return _memberIn; }
		const string& getName() const { return _name; }
		County* getCounty() const { return _county; }
		County* getDelegating() const { return _countyDelegator; }
		Party* getVotedTo()const { return _votedTo; }
		//Print-related
		void printCitizen() const;
		friend ostream& operator << (ostream& os, const Citizen& citizen);
	};
}