#include "CGA.h"

CGA::CGA(int iPopSize, double dMutationProb, double dMatingProb, CLFLnetEvaluator& Cevaluator, int iMutationDeviation, int iCrossoverPercent,int iTournamentSize) {
	this->iPopSize = iPopSize;
	this->dMutationProb = dMutationProb;
	this->dMatingProb = dMatingProb;
	this->CEvaluator = Cevaluator;
	this->iMutationDeviation = iMutationDeviation;
	this->iCrossoverPercent = iCrossoverPercent;
	this->iTournamentSize = iTournamentSize;
	v_initialize_Population();
}
void CGA::v_initialize_Population() {
	size_t iGenotypeSize = (size_t)CEvaluator.iGetNumberOfBits();
	pvec_pC_Population = new vector<CIndividual*>();
	pvec_pC_Population->reserve(iPopSize);
	for (int ii = 0; ii < iPopSize; ii++)
	{
		vector<int>* genotype = new vector<int>(iGenotypeSize);
		for (int ij = 0; ij < iGenotypeSize; ij++)
		{
			genotype->at(ij) = lRand(CEvaluator.iGetNumberOfValues(ij));


		}
		//cout << genotype->size() << endl;
		CIndividual* pC_Individual = new CIndividual(genotype);
		pvec_pC_Population->push_back(pC_Individual);
	}

}
void CGA::v_eval_Population() {
	for (int i = 0; i < iPopSize; i++)
	{
		//(*pvec_pC_Population)[i]->v_print_genotype();
		double dFitness = (*pvec_pC_Population)[i]->evaluate(CEvaluator);
		if (dFitness > dBestFitness) {
			dBestFitness = dFitness;
			std::cout << "Closer to Holy Grail (new fitness): " << dBestFitness << endl;
		}
	}
}


void CGA::run(int iIterations) {
	for (int i = 0; i < iIterations; i++) {
		cout <<"Iteration: " << i << "best was: "<<dBestFitness<<endl;
		v_run_one_iteration();
	}
}
void CGA::v_run_one_iteration() {
	//cout << pvec_pC_Population->size();
	v_eval_Population();
	vector<CIndividual*>* pvec_pC_NewPopulation = new vector<CIndividual*>();
	
	pvec_pC_NewPopulation->reserve(iPopSize);


	for (int i = 0; i < iPopSize; i = i + 2) {
		CIndividual* pC_Parent1 = pC_make_tournament(iTournamentSize);
		CIndividual* pC_Parent2 = pC_make_tournament(iTournamentSize);

		CIndividual* pC_Child1, *pC_Child2;

		if (dRand() < dMatingProb) {
			v_make_children_by_crossover(pC_Parent1, pC_Parent2, &pC_Child1, &pC_Child2);
			pC_Child1->v_mutate_genotype(CEvaluator, dMutationProb, iMutationDeviation);
			pC_Child2->v_mutate_genotype(CEvaluator, dMutationProb, iMutationDeviation);
			pvec_pC_NewPopulation->push_back(pC_Child1);
			pvec_pC_NewPopulation->push_back(pC_Child2);


		}
		else {
			CIndividual* pC_Parent1_Copy = new CIndividual(*pC_Parent1);
			CIndividual* pC_Parent2_Copy = new CIndividual(*pC_Parent2);
			pvec_pC_NewPopulation->push_back(pC_Parent1_Copy);
			pvec_pC_NewPopulation->push_back(pC_Parent2_Copy);



		}
	}

		for (int i = 0; i < iPopSize; i++)
		{
			
			delete (*pvec_pC_Population)[i];
		}
		pvec_pC_Population->clear();
		//cout << "dupa" << endl;
		pvec_pC_Population = pvec_pC_NewPopulation;
	}



CIndividual* CGA::pC_make_tournament(int iShots) {
	
	double dMaxFitness;
	double dFitness;
	int iShotIndex = lRand(iPopSize);
	CIndividual* pc_Winner =  pvec_pC_Population->at(iShotIndex);

	dMaxFitness= ((pc_Winner->getFitness()));
	
	for (int i = 0; i < iShots-1; i++)
	{
		CIndividual* pc_Contestant;
		pc_Contestant = pvec_pC_Population->at(iShotIndex);
		iShotIndex = lRand(iPopSize);
		dFitness = ((pc_Contestant->getFitness()));
		if (dFitness > dMaxFitness) {
			dMaxFitness = dFitness;
			pc_Winner = pc_Contestant;
		}

	}
	return pc_Winner;

}

void CGA::v_make_children_by_crossover(CIndividual* CParent1, CIndividual* CParent2, CIndividual** Child1, CIndividual** Child2) {
	vector<int>* pvec_of_child1,*pvec_of_child2;
	
	int iVecSize = CParent1->getGenotype()->size();
	int iNumberOfCrossoverPoints = iVecSize * iCrossoverPercent / 100;
	

	pvec_of_child1 = (pvec_i_rewrite_vector(CParent1->getGenotype()));
	pvec_of_child2 = (pvec_i_rewrite_vector(CParent2->getGenotype()));

	for (int i = 0; i < iNumberOfCrossoverPoints; i++)
	{
		int iCrossoverIndex = lRand(iVecSize);
		int iBit1 = (*pvec_of_child1)[iCrossoverIndex];
		int iBit2 = (*pvec_of_child2)[iCrossoverIndex];

		(*pvec_of_child1)[iCrossoverIndex] = iBit2;
		(*pvec_of_child2)[iCrossoverIndex] = iBit1;

	}
	*Child1 = new CIndividual(pvec_of_child1);
	*Child2 = new CIndividual(pvec_of_child2);
}


