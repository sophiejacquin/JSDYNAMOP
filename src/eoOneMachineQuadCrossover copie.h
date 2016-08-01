/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for simple quadratic crossover operators
=================================================

Quadratic crossover operators modify the both genotypes
*/

#ifndef eoOneMachineQuadCrossover_H
#define eoOneMachineQuadCrossover_H

#include <eoOp.h>

/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class eoOneMachineQuadCrossover: public eoQuadOp<GenotypeT>
{
public:
  
  eoOneMachineQuadCrossover(Data & d)
   {
       data=d;
       
    
  }

  /// The class name. Used to display statistics
  string className() const { return "eoOneMachineQuadCrossover"; }

   bool operator()(GenotypeT& _genotype1, GenotypeT & _genotype2)
  {
      bool oneAtLeastIsModified(true);
      int Sd[2];
      int Sf[2];
      int N=_genotype1.getN();
      eoUniformGenerator<int> choixSd(1,N-2);
      Sd[0]=choixSd();
      Sd[1]=Sd[0];
      Sf[0]=chercheSommetFinal( Sd[0],_genotype1, _genotype2);
      Sf[1]=chercheSommetFinal( Sd[1],_genotype2, _genotype1);
      vector<int> ordre1;
      vector<int> ordre2;
      vector<int> completion1;
      vector<int> completion2;
      ordoner( ordre1,completion1, Sd[0], Sf[0], _genotype1, _genotype2);
      ordoner( ordre2,completion2, Sd[1], Sf[1], _genotype2, _genotype1);
      TimingConservateur(_genotype1.getCompletionTime(Sd[0]),completion1, ordre1);
      TimingConservateur(_genotype2.getCompletionTime(Sd[1]),completion2, ordre2);
      /*if(Sf[1]<N-1 && completion2[Sf[1]-Sd[1]-1]>_genotype1.getCompletionTime(Sf[1]))
      { cout <<"attention G2 : " <<completion2[Sf[1]-Sd[1]-1]<<" "<<_genotype1.getCompletionTime(Sf[1])<<endl;
          cout<<_genotype2.getCompletionTime(Sd[1])<<endl;
          for(int i=0;i<Sf[1]-Sd[1];i++)
          {
              cout<<"("<<ordre2[i]<<" , "<<completion2[i]<< ") ";
          }
          cout<<endl;
      }*/
      int minSf=Sf[0];
      if(Sf[1]<minSf)
          minSf=Sf[1];
      for(int i=minSf+1; i<N;i++)
      {
          int dernierVu1=_genotype1.getDernierVu(i);
          _genotype1.setDernierVu(i,_genotype2.getDernierVu(i));
          _genotype2.setDernierVu(i,dernierVu1);
          vector<bool> dejaVus1=_genotype1.getDejaVus(i);
          _genotype1.setDejaVus(i,_genotype2.getDejaVus(i));
          _genotype2.setDejaVus(i,dejaVus1);
          int completionTime1=_genotype1.getCompletionTime(i);
          _genotype1.setCompletionTime(i,_genotype2.getCompletionTime(i));
          _genotype2.setCompletionTime(i,completionTime1);
          _genotype1.setModif(i,true);
          _genotype2.setModif(i,true);
          
      }
      vector<bool> dejaVus1=_genotype1.getDejaVus(Sd[0]);
      vector<bool> dejaVus2=_genotype2.getDejaVus(Sd[1]);
      miseAJour(Sd[0]+1,Sf[0], completion1, ordre1, dejaVus1,_genotype1);
      miseAJour(Sd[1]+1,Sf[1], completion2, ordre2, dejaVus2,_genotype2);
      if(!verifier(_genotype1))
      {
          cout<<"erreur G1, SD :"<<Sd[0]<<" sF :  "<<Sf[0]<<endl; 
      }
      if(!verifier(_genotype2))
      {
          cout<<"erreur G2, SD :"<<Sd[1]<<" sF :  "<<Sf[1]<<" G1 "<<Sd[0]<<" sF :  "<<Sf[0]<<endl; 
      }
        return oneAtLeastIsModified;
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
        //cout<<_genotype.getDernierVu(S2)<<" "<<_genotype.getDejaVu(S2,_genotype.getDernierVu(S2)) <<" "<<numjobs.size()<<" "<<S2+1-S1<<endl;
    }
    void ordoner(vector<int> & ordre,vector<int> & completion, int Sd, int Sf, GenotypeT & _genotype1,GenotypeT & _genotype2)
    {
       // cout<<"deb ord "<<Sd<<" "<<Sf<<endl;
        int i=Sf;
        int nbAd=0;
        int job;
        while(nbAd<Sf-Sd)
        {
            job=_genotype2.getDernierVu(i);
            //if(!_genotype2.getDejaVu(Sf,job)) cout<<"PROBLEME i="<<i<<" Sf="<<Sf<<endl;
            if(!_genotype1.getDejaVu(Sd,job))
            {
                ordre.insert(ordre.begin(),job);
                completion.insert(completion.begin(),_genotype2.getCompletionTime(i));
                nbAd++;
            }
            i--;
           // cout<<i<<" "<<nbAd<<endl;
        }
        //cout<<"fin ord Sf "<<Sf<<" Sd "<<Sd<<" i "<<i<<" nbAd "<<nbAd<<" "<<ordre[Sf-Sd-1]<<" "<<job<<endl;
        
    }
    void TimingConservateur(int Tini,vector<int> & completion, const vector<int> & ordre)
    {
        int p=data.getJob(ordre[0]).getP();
        int r=data.getJob(ordre[0]).getR();
        int d=data.getJob(ordre[0]).getD();
        int t=Tini+p;
        if(t<r+p) t=r+p;
        if(t<completion[0] && t<d)
        {
            t=completion[0];
            if(t>d)
                t=d;
        }
        completion[0]=t;
        for(int i=1;i<completion.size();i++)
        {
             p=data.getJob(ordre[i]).getP();
            r=data.getJob(ordre[i]).getR();
            d=data.getJob(ordre[i]).getD();
            int t=completion[i-1]+p;
            if(t<r+p) t=r+p;
            if(t<completion[i] && t<d)
            {
                t=completion[i];
                if(t>d)
                    t=d;
            }
            completion[i]=t;
        }
        
    }
    int chercheSommetFinal(int & Sd,GenotypeT & _genotype1,GenotypeT & _genotype2)
    {
       // cout<<"deb chercher"<<endl;
        int Sf=Sd;
        bool trouve=false;
        vector<bool> dejaVus =_genotype1.getDejaVus(Sd);
        int nbTrue=0;
        int j=0;
        int Tmin=0;
        while(!trouve && Sf<_genotype1.getN()-1)
        {
            Sf++;
            bool inclu=true;

            while(nbTrue<Sd+2 && inclu)
            {
                if(dejaVus[j])
                {
                    
                    inclu=_genotype2.getDejaVu(Sf,j);
                    if(inclu){ j++;nbTrue++;}
                }
                else {j++;}
            }
            trouve=inclu;
            
        }
        
        nbTrue=0;
        j=0;
        while(nbTrue<Sf-Sd)
        {
            if(!dejaVus[j] && _genotype2.getDejaVu(Sf,j) )
            {
                Tmin+= data.getJob(j).getP();
                nbTrue++;
                
            }
            j++;
        }
        if(_genotype1.getCompletionTime(Sd)+Tmin<=_genotype2.getCompletionTime(Sf))
        {
             //cout<<"fin chercher1 "<<Tmin<<" "<<_genotype1.getCompletionTime(Sd)<<" "<<Sf<<" "<<j-1<<" "<<dejaVus[j-1] << _genotype2.getDejaVu(Sf,j-1)<<" completion time "<<_genotype2.getCompletionTime(Sf)<<endl;
            return Sf;
        }
        else
        {
            //cout<<"dans else "<<endl;
            
            while ( Sf<_genotype2.getN()-1)
            {
                Sf++;
                Tmin+=data.getJob(_genotype2.getDernierVu(Sf)).getP();
                if(_genotype1.getCompletionTime(Sd)+Tmin<=_genotype2.getCompletionTime(Sf))
                {// cout<<"fin chercher2"<<endl;
                    return Sf;}
            }
             //cout<<"fin chercher3"<<endl;
            return _genotype2.getN()-1;
            
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
            if(_genotype.getDejaVu(i,job)==false)
            {
                cout<<"incoherence dernier vu deja vu "<<i<<endl;
                return  false;
            }
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
    Data data;
    
};

#endif
