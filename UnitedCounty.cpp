#include "unitedcounty.h"
namespace election
{
	void UnitedCounty::calc_electors(vector<Party*>& partyVector, int* votes)
	{
		int temp;
		if (this->getWinner() != nullptr)
		{
			cout << "From " << this->getWinner()->getName() << " party:" << endl;
			this->printElectorsFromPartyByQuantity((this->getNumOfElectors()), this->getWinner()->getserial());
			temp = this->getWinner()->getserial() - 1;
			partyVector[temp]->setElectors(partyVector[temp]->getElectors() + this->getNumOfElectors());
		}
		else
			cout << "no electors" << endl;
	}
}