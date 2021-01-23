#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "DynamicArray.h"
#include "citizen.h"
#include <vector>

namespace election
{
	using namespace std;
	class Citizen;
	class Party;

	class County
	{
	protected:
		static int countS;
		int _serial;
		int _ciCount;
		int _voted;
		string _name;
		int _numOfElectors;
		int _howManyElectors;
		vector<Citizen*> _citizenVector;
		vector<Citizen*> _delegatorVector;
		Party* _winner;
	public:
		//ctor
		County(const string&, int numOfElectors);
		//copy - ctor
		County(const County& source);
		//dtor
		virtual ~County();
		//operators
		County& operator=(County& other) = delete;
		//setters
		void setnumOfElectors(int numOfElectors) { _numOfElectors = numOfElectors; }
		void setWinner(int partyNum);
		//getters
		int getserial() const { return _serial; }
		int getCountS()const { return countS; }
		int getNumOfElectors() const { return _numOfElectors; }
		int getCitizenCount() const { return _ciCount; }
		int getVoters() const { return _voted; }
		const string& getName() const { return _name; }
		int getElectorCounter() const { return _howManyElectors; }
		float getVotingPrecentage()const;
		int getMaxVotesPartyNum(int* votes, const int numParties);
		Party* getWinner() const { return _winner; }
		Party* getPartyByNum(int serialNum)const;
		
		virtual void calc_electors(vector<Party*>& partyVector, int* votes) = 0;
		int* calculate_VotingSegmentation(const int numOfParties);
		void addVoter() { _voted++; }
		Citizen* addCitizen(const string& name, const string& ID,int birthYear, County* living, const int Elections_year);
		void addDelegator(Citizen* delegator);
		//print-related
		void printCitizens() const;
		virtual void printCountyType() const = 0;
		//void printCitizenWithoutCounty() const;
		void printElectorsFromPartyByQuantity(int electorsNum, int partyNum) const;
		friend ostream& operator << (ostream& os, const County& county);
	};
}