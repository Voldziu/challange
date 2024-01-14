#pragma once
#include "CIndividual.h"
#include "functionalities.h"




	class CGA
	{
	public:
		CGA(int iPopSize, double dMutationProb, double dCrossoverProb, CLFLnetEvaluator& Cevaluator, int iMutationDeviation, int iCrossoverPercent,int iTournamentSize);
		void run(int iIterations);

	private:
		int iPopSize;
		int iTournamentSize;
		std::vector<CIndividual*>* pvec_pC_Population;
		double dMutationProb;
		double dMatingProb;
		double dBestFitness;
		CLFLnetEvaluator CEvaluator;
		int iMutationDeviation;
		int iCrossoverPercent;
		CIndividual* pC_make_tournament(int iShots);
		void v_make_children_by_crossover(CIndividual* CParent1, CIndividual* CParent2, CIndividual** Child1, CIndividual** Child2);

		void v_initialize_Population();
		void v_eval_Population();
		void v_run_one_iteration();



	};


