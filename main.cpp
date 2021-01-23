#include <iostream>
#include "citizen.h"
#include "RegularElections.h"
#include "SimpleElections.h"
#include "BaseElections.h"
#include <fstream>
#include <string>
#define rcastc reinterpret_cast<char*>
#define MAX_SIZE 256
using namespace std;

enum Options {
	ADD_COUNTY = 1, ADD_CITIZEN, ADD_PARTY, ADD_CITIZEN_PARTY_MEMBER, SHOW_COUNTY,
	SHOW_CITIZEN, SHOW_PARTY, VOTE, RESULT, EXIT, SAVE, READ
};

enum Election_Type {
	FULL_ELECTIONS = 1, READ_DATA, QUIT
};

void Menu(election::BaseElections* app);
election::BaseElections* ReadElectionBaseData(const string& filename);
election::BaseElections* create_Elections(int day, int month, int year, int electionType, int total_sets);

void main()
{
	int option;
	bool run = true;
	int total_sets = -1;
	char name[MAX_SIZE];	
	ifstream file;
	election::BaseElections* app;
	int day, month, year, electionType;
	while(run)
	{
		cout << "Choose one of the following modes: " << endl;
		cout << "1-> Create new alections." << endl;
		cout << "2-> Load previous alections' data." << endl;
		cout << "3-> Exit." << endl;
		cin >> option;
		try {
			switch (option)
			{
			case (Election_Type::FULL_ELECTIONS): // 1
				cout << "Enter elections' date and a type of election according to the following format:" << endl;
				cout << "<day> <month> <year> <elections' type: 1->Regular || 2-> Simple >" << endl;
				cin >> day >> month >> year >> electionType;
				if (electionType != 1 && electionType != 2)
				{
					string error = "WRONG INPUT : SHOULD BE 1 OR 2.";
					throw error;
				}
				if (electionType == 2)
				{
					cout << "Enter the total number of electors:" << endl;
					cin >> total_sets;
					if(total_sets <=0)
					{
					string error = "WRONG INPUT : SHOULD BE POSITIVE.";
					throw error;
					}
				}
				app = create_Elections(day, month, year, electionType,total_sets);
				Menu(app);
				run = false;
				break;
			case (Election_Type::READ_DATA): //2
				/*open elections according to type
				  REMEMBER TO ADD : enter the date to the file and
				  file open-> scanning the type of elections
				  open the file in the main and send it to the func

				  when we writh to a file need to add this part
				*/
				cout << "Enter file name:" << endl;
				cin >> name;
				app = ReadElectionBaseData(name);
				app->read(name);
				Menu(app);
				run = false;
				break;
			case (Election_Type::QUIT): //3
				run = false;
				break;
			}
		}
		catch (const string& error)
		{
			cout << "Error: " << error << endl;
		}
		catch (bad_alloc& error)
		{
			cout << error.what() << endl;
			exit(1);
		}
	}
}

election::BaseElections* create_Elections(int day, int month, int year, int electionType, int total_sets)
{
	election::BaseElections* app = nullptr;
	if (electionType == 1)
		app = new election::RegularElections(day, month, year);
	else
		app = new election::SimpleElections(day, month, year, total_sets);
	return app;
}

void Menu(election::BaseElections* app)
{
	bool  type;
	string name;
	int num1, num2, num3; //Generic names because purpose may change in accordance to chosen option.
	string id_str;
	int option = 0;
	while (option != 10)
	{	
		try
		{
			cout << "Choose an option between 1 - 10 :" << endl;
			cout << "1 - Add county" << endl;
			cout << "2 - Add citizen" << endl;
			cout << "3 - Add party" << endl;
			cout << "4 - Add a citizen as a party member" << endl;
			cout << "5 - Show all county data" << endl;
			cout << "6 - Show all citizen data" << endl;
			cout << "7 - Show all party data" << endl;
			cout << "8 - Vote" << endl;
			cout << "9 - Election's result" << endl;
			cout << "10 - Exit" << endl;
			cout << "11 - Save data" << endl;
			cout << "12 - Read data" << endl;

			cin >> option;
			switch (option)
			{
			case Options::ADD_COUNTY:
				cout << "Enter county in this format: (name, numOfElectors, type of county)" << endl;
				cout << "(Note: for type of county, '0' means divided county, else for united county." << endl;
				cin >> name;
				//ASSUMTION : THE NAME IS A SINGLE WORD
				/*ASSUMTION : TYPE OF COUNTY WILL BE 0 FOR DIVIDED COUNTY, ELSE -> UNITED COUNTY
							  MEANS THAT THE INPUT WILL BE NUMERIC */
				cin >> num1 >> type;
				/*num1 = number of electors*/
				app->addCounty(name, num1, type);
				cout << endl;
				break;
			case Options::ADD_CITIZEN:
				cout << "Enter citizen in this format: name, ID, birthyear, county serial-number" << endl;
				cin >> name;
				cin >> id_str >> num2 >> num3;
				/*id_str = Person ID , num2 = Birth year , num3 = County serial Number*/
				if (strcmp(typeid(*app).name(), "class election::RegularElections") == 0)
					app->addCitizen(name, id_str, num2, num3);
				else
					app->addCitizen(name, id_str, num2, 1);
				cout << endl;
				break;
			case Options::ADD_PARTY:
				cout << "Enter party in this format: name, id" << endl;
				cin >> name;
				cin >> id_str;
				/*id_str = Leader's ID*/
				app->addParty(name, id_str);
				cout << endl;
				break;
			case Options::ADD_CITIZEN_PARTY_MEMBER:
				cout << "Enter citizen as a party member in this format:" << endl;
				cout << "Person ID, Party Serial Number, County Number" << endl;
				cin >> id_str >> num2 >> num3;
				/*id_str = Person ID , num2 = Party Serial Number , num3 = County Number*/
				app->addCitizenAsDelegateCounty(id_str, num2, num3);
				cout << endl;
				break;
			case Options::SHOW_COUNTY:
				cout << "A list of all the counties:" << endl;
				app->printAllCounties();
				cout << endl;
				break;
			case Options::SHOW_CITIZEN:
				cout << "A list of all the Citizens:" << endl;
				app->printAllCitizens();
				cout << endl;
				break;
			case Options::SHOW_PARTY:
				cout << "A list of all optional parties:" << endl;
				app->printAllParties();
				cout << endl;
				break;
			case Options::VOTE:
				cout << "Enter information of voting citizen in this format:" << endl;
				cout << "Person ID, Party Serial Number" << endl;
				cin >> id_str >> num2;
				/*id_str = Person ID, num2 = Party Serial Number*/
				app->addOneVote(id_str, num2);
				cout << endl;
				break;
			case Options::RESULT:
				app->printVoteStatus();
				cout << endl;
				break;
			case Options::EXIT:
				cout << "Proggram running has stopped." << endl;
				break;
			case Options::SAVE:
				cout << "Enter file name :" << endl;
				cin >> name;
				if (strcmp(typeid(*app).name(), "class election::RegularElections") == 0)
					app->save(name, 1);
				else
					app->save(name, 2);
				cout << endl;
				break;
			case Options::READ:
				cout << "Enter file name:" << endl;
				cin >> name;
				cout << "Deleting existing data . . ." << endl;
				delete app;
				app = ReadElectionBaseData(name);
				app->read(name);
				cout << endl;
				break;
			default:
				break;
			}
		}
		catch (const string& error)
		{
			cout << "Error: " << error << endl;
		}
		catch (bad_alloc& error)
		{
			cout << error.what() << endl;
			exit(1);
		}
	}
}

election::BaseElections* ReadElectionBaseData(const string& filename)
{
	int electionType, day, month, year, total_sets;
	ifstream file;
	election::BaseElections* app = nullptr;
	file.open(filename, ios::in | ios::binary);
	if (file.is_open() == false)
	{
		string error("CANNOT OPEN FILE.");
		throw error;
	}
	file.read(rcastc(&electionType), sizeof(int));
	file.read(rcastc(&day), sizeof(int));
	file.read(rcastc(&month), sizeof(int));
	file.read(rcastc(&year), sizeof(int));
	file.read(rcastc(&total_sets), sizeof(int));
	app = create_Elections(day, month, year, electionType, total_sets);
	file.close();
	return app;
}