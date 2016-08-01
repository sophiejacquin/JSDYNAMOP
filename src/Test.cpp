#include <iostream>
using namespace std;

// eo general include
#include "eo"
// the real bounds (not yet in general eo include)
#include "utils/eoRealVectorBounds.h"

// include here whatever specific files for your representation
// Basically, this should include at least the following

/** definition of representation:
 * class eoOneMachine MUST derive from EO<FitT> for some fitness
 */
#include "eoOneMachine.h"

/** definition of initilizqtion:
 * class eoOneMachineInit MUST derive from eoInit<eoOneMachine>
 */
#include "eoOneMachineInit.h"

/** definition of evaluation:
 * class eoOneMachineEvalFunc MUST derive from eoEvalFunc<eoOneMachine>
 * and should test for validity before doing any computation
 * see tutorial/Templates/evalFunc.tmpl
 */
#include "eoOneMachineEvalFunc.h"

/** definitions of operators: write as many classes as types of operators
 * and include them here. In this simple example,
 * one crossover (2->2) and one mutation (1->1) operators are used
 */
#include "eoOneMachineQuadCrossover.h"
#include "eoOneMachineMutation.h"

/* and (possibly) your personal statistics */
#include "eoOneMachineStat.h"
#include "Parser.h"
#include "Data.h"
#include "hybridDP.h"
#include "Timing.h"
typedef eoMinimizingFitness MyFitT ;	// type of fitness
// END fitness type
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// Then define your EO objects using that fitness type
typedef eoOneMachine<MyFitT> Indi;
main()
{
	Parser p ("../bky20/bky20_1.txt");
      	Data data=p();
	 eoOneMachineInit<Indi> init(data);
	 eoOneMachineEvalFunc<Indi> plainEval(data);
	Timing<Indi> mutTiming(data);
	hybridDP<Indi> mutH(data);
	eoOneMachineMutation<Indi> mut1(5,data);
	Indi indi;
	init(indi);
	plainEval(indi);
	cout<<indi<<endl;
	mutH(indi);
	indi.invalidate();
	plainEval(indi);
	cout<<indi<<endl;
}
