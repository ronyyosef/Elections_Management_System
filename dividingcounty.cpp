#include "dividingcounty.h"

#define MAX_SIZE 256
namespace election
{
	void DividingCounty::calc_electors(vector<Party*>& partyVector, int* votes)
	{
		int j;
		float tempfloat;
		for (j = 0; j < partyVector.size(); j++)
		{
			if (votes[j] != 0)
			{
				tempfloat = (float)(votes[j]) / (float)(this->getVoters());
				cout << "From " << partyVector[j]->getName() << " party:" << endl;
				this->printElectorsFromPartyByQuantity((int)(tempfloat * this->getNumOfElectors()), j + 1);
				partyVector[j]->setElectors(partyVector[j]->getElectors() + (int)(tempfloat * this->getNumOfElectors()));
				//ASSUMPTION: ELECTOR'S NUMBER OF A SPECIFIC PARTY IS ROUNDED DOWN
			}
		}
	}
}