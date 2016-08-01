/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is useful in Emacs-like editors
 */

/*
Template for simple mutation operators
======================================
*/

#ifndef eoOneMachineMutation_H
#define eoOneMachineMutation_H


#include <eoOp.h>

#include "Timing.h"

template<class GenotypeT>
class eoOneMachineMutation: public eoMonOp<GenotypeT>
{
public:

  eoOneMachineMutation(int L_, Data d)
  {
      L=L_;
      data=d;
  }

  /// The class name. Used to display statistics
  string className() const { return "eoOneMachineMutation"; }

  /**
   * modifies the parent
   * @param _genotype The parent genotype (will be modified)
   */
  bool operator()(GenotypeT & _genotype)
  {
	
      int i;
      bool isModified(true);
      int N=_genotype.getN();
      eoUniformGenerator<int> choixS1 (0,N-1);
      int S1=choixS1();
      int Bsup=S1+L;
      int S2;
      if(Bsup>N-1)
          S2=N-1;
      else
      {
          eoUniformGenerator<int> choixS2(S1+1,Bsup);
          S2=choixS2();
      }
      vector<int> numJob;
      for(i=S1;i<S2+1;i++)
      {
          numJob.push_back(_genotype.getDernierVu(i));
      }
      
      int tini, tfin;
      if(S1==0)
      {
          tini=0;
      }
      else
      {
          tini=_genotype.getCompletionTime(S1-1);
      }
      if(S2==N-1)
      {
          tfin=999999999;
      }
      else
      {
          tfin=_genotype.getCompletionTime(S2+1)-data.getJob(_genotype.getDernierVu(S2+1)).getP();
      }
      melange(numJob,tfin);
      vector<int> timing;
      TimingAleatoire( timing,  numJob,  tini,  tfin);
      vector<bool> dejaVus;
      if(S1==0)
      {
          for(i=0;i<N;i++){
              
              dejaVus.push_back(0);
          }
      }
      else dejaVus=_genotype.getDejaVus(S1-1);
      miseAJour(S1, S2,  timing,  numJob, dejaVus, _genotype);
     /* if(!verifier(_genotype))
          cout<<"PB MUT "<<S1<<" "<<S2<<endl;*/
	//Timing<GenotypeT> timingExact(data);
	//timingExact(_genotype);
	
    return isModified;
    // END code for mutation of the _genotype object
  }
    void miseAJour(int S1, int S2, const vector<int> & timing, const vector<int> & numjobs, vector<bool> & dejaVus,GenotypeT & _genotype)
    {
        int i;
        for(i=S1;i<S2+1;i++)
        {
            _genotype.setDernierVu(i,numjobs[i-S1]);
            dejaVus[numjobs[i-S1]]=true;
            _genotype.setDejaVus(i,dejaVus);
            _genotype.setCompletionTime(i,timing[i-S1]);
            _genotype.setModif(i,true);
        }
    }
    void melange(vector<int> & numJobs,int tmax)
    {
        int i;
        int N=numJobs.size();
        vector<int> numJobsMelanges;
        int tpsMin=0;
        for(i=0;i<N;i++)
        {
            int job=numJobs[i];
            int p=data.getJob(job).getP();
            tpsMin+=p;
        }
        i=0;
        while(i<N)
        {
            eoUniformGenerator<int> G(0,numJobs.size());
            int j=G();
            int job=numJobs[j];
            int r=data.getJob(job).getR();
            int p=data.getJob(job).getP();
            if(r+tpsMin<=tmax)
            {
                numJobsMelanges.push_back(numJobs[j]);
                numJobs.erase(numJobs.begin()+j);
                tpsMin-=p;
                i++;
            }
            
            
        }
        numJobs=numJobsMelanges;
        
        
    }
    void TimingAleatoire(vector<int> & timing, const vector<int> & numJobs, int tini, int tfin)
    {
        int i;
        int idleTime=tfin;
        int sumT=0;
        vector<int> avance;
        for(i=0;i<numJobs.size();i++)
        {
            int job=numJobs[i];
            int p=data.getJob(job).getP();
            int r= data.getJob(job).getR();
            int d=data.getJob(job).getD();
            int t;
            if(i==0)
            {
                t=tini+p;
                if(t<r+p)
                    t=r+p;
            }
            else
            {
                t=timing[i-1]+p;
                if(t<r+p)
                    t=r+p;
            }
            timing.push_back(t);
            if(t<d)
                avance.push_back(i);
            sumT=t;
        }
        idleTime-=sumT;
        if(idleTime<0) cout<<" idleTime : " <<idleTime<<" tfin : "<<tfin<<" sumT "<<sumT;
        while(avance.size()>0 && idleTime>0)
        {
            eoUniformGenerator<int> choixJob(0,avance.size());
            int j=choixJob();
            int delta;
            int max=idleTime;
            int job=numJobs[avance[j]];
            int d=data.getJob(job).getD();
            if(d-timing[avance[j]]<max)
                max=d-timing[avance[j]];
            if(max>0)
            {
            eoUniformGenerator<int> choixDelta(0,max);
            delta=choixDelta();
                for(int k=avance[j];k<numJobs.size();k++)
                {
                    timing[k]=timing[k]+delta;
                }
                idleTime-=delta;
            }
            avance.erase(avance.begin()+j);
            
        }
    }
    bool verifier(GenotypeT & _genotype)
    {
        int i;
        for(i=1;i<_genotype.getN();i++)
        {
            int job=_genotype.getDernierVu(i);
            int p=data.getJob(job).getP();
            int r=data.getJob(job).getR();
            int c=_genotype.getCompletionTime(i);
            int cPred=_genotype.getCompletionTime(i-1);
            if(c<cPred+p)
            {
                cout<<"erreur "<<i<<" ieme job c: "<<c<<" cPred "<<cPred<< " p : "<<p<<endl;
                return false;
            }
            else if (c<r+p)
            {
                cout<<"erreur "<<i<<" ieme job c: "<<c<<" r "<<r<< " p : "<<p<<endl;
                return false;
            }
                
        }
        return true;
    }

private:
    int L;
    Data data;
};

#endif
