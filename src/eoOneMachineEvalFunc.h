/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-
 
The above line is usefulin Emacs-like editors
 */
 
/*
Template for evaluator in EO, a functor that computes the fitness of an EO
==========================================================================
*/
 
#ifndef _eoOneMachineEvalFunc_h
#define _eoOneMachineEvalFunc_h

// include whatever general include you need
#include <stdexcept>
#include <fstream>

// include the base definition of eoEvalFunc
#include "eoEvalFunc.h"
#include "Data.h"
/** 
  Always write a comment in this format before class definition
  if you want the class to be documented by Doxygen
*/
template <class EOT>
class eoOneMachineEvalFunc : public eoEvalFunc<EOT>
{
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  eoOneMachineEvalFunc(Data & data_)
  {
      data=data_;
  }

  /** Actually compute the fitness
   *
   * @param EOT & _eo the EO object to evaluate
   *                  it should stay templatized to be usable 
   *                  with any fitness type
   */
  void operator()(EOT & _eo)
  {
    // test for invalid to avoid recomputing fitness of unmodified individuals
    if (_eo.invalid())
      {
          double fit=_eo.evaluation();		   // to hold fitness value
          for(int i=0;i<data.getN(); i++)
          {
              if(_eo.getModif(i))
              {
                  int job=_eo.getDernierVu(i);
                  int e=0; int t=0;
                  int c=_eo.getCompletionTime(i);
                  int d=data.getJob(job).getD();
                  int alpha=data.getJob(job).getAlpha();
                  int beta =data.getJob(job).getBeta();
                  //cout<<alpha<<" "<<beta<<" c "<<c<<" "<<_eo.getEval(i)<<" d "<<d<<" fit "<<fit<<endl;
                  if(c<d)
                  {
                      e=alpha*(d-c);
                  }
                  else if(c>d)
                  {
                      t=beta*(c-d);
                  }
                  fit=fit +e+t - _eo.getEval(i);
                  _eo.setEval(i, e+t);
                  _eo.setModif(i,false);
              }
          }
	_eo.fitness(fit);
          _eo.evaluation(fit);
      }
  }

private:
    Data data;
};


#endif
