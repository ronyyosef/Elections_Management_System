#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "DynamicArray.h"
#include "citizen.h"
#include <vector>
#include <string>

namespace election
{
	using namespace std;
	class Citizen;
	class CitizenList;

	class Party
	{
	private:
		string _name;
		int _delegatorCount;
		int _votesCounter;
		Citizen* _leader;
		string _leaderID;
		static int partyCount;
		int _serial;
		int _elector;
		vector <Citizen*> _citizenVector;
	public:
		//ctor
		Party(const string& name, Citizen* citizen);
		Party(const string& name, const string& leaderID);
		// copy-ctor
		Party(const Party& source);
		//dtor
		~Party() { partyCount = 0; }
		//operators
		Party& operator = (const Party& party);
		//setters
		void setVotes() { _votesCounter++; }
		void setLeader(Citizen* leader) { _leader = leader; }
		void setElectors(int elector) { _elector = elector; }
		//getters
		const string& getName()const { return _name; }
		void addDelegate(Citizen* citizen);
		int getserial()const { return _serial; }
		int getDelegatorCount()const { return _delegatorCount; }
		Citizen* getLeader()const { return _leader; }
		const string& getLeaderID() const { return _leaderID; }
		int getElectors()const { return _elector; }

		int getVotes() const { return _votesCounter; }
		void printParty();
		void printParty_NoCounties() ;
		friend ostream& operator << (ostream& os, const Party& party);
	};
}