/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 */

/*
Template for EO objects initialization in EO
============================================
*/

#ifndef _eoOneMachineInit_h
#define _eoOneMachineInit_h

// include the base definition of eoInit
#include <eoInit.h>
#include"Data.h"
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>

/** 
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * There is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO (e.g. to initialize 
 *    atoms of an eoVector you will need an eoInit<AtomType>)
 */
template <class GenotypeT>
class eoOneMachineInit: public eoInit<GenotypeT> {
public:
	/// Ctor - no requirement
// START eventually add or modify the anyVariable argument
  eoOneMachineInit(Data data_)
 
  {
      data=data_;
  }


  /** initialize a genotype
   *
   * @param _genotype  generally a genotype that has been default-constructed
   *                   whatever it contains will be lost
   */
  void operator()(GenotypeT & _genotype)
  {
      int N=data.getN();
      int i;
      vector<int> numJobs;
      vector<bool> dejaVus;
      vector<int> timing;
      for(i=0;i<N;i++){
          numJobs.push_back(i);
          dejaVus.push_back(0);
      }
      
      melange(numJobs);
      timingOpt(numJobs,timing);
	//TimingAleatoire(timing,  numJobs);
      for(i=0;i<N;i++)
      {
          _genotype.adDernierVu(numJobs[i]);
          dejaVus[numJobs[i]]=true;
          _genotype.adDejaVus(dejaVus);
          _genotype.adCompletionTime(timing[i]);
          
        
      }
      _genotype.evaluation(0);
      if(!verifier(_genotype))cout<<"erreur ini !!!!!!!!"<<endl;
          _genotype.invalidate();	   // IMPORTANT in case the _genotype is old
  }
    void melange(vector<int> & numJobs)
    {
        int i;
        int N=numJobs.size();
        vector<int> numJobsMelanges;
        for(i=0;i<N;i++)
        {
            eoUniformGenerator<int> G(0,N-i);
            int j=G();
            numJobsMelanges.push_back(numJobs[j]);
            numJobs.erase(numJobs.begin()+j);
            
            
        }
        numJobs=numJobsMelanges;
        

    }
    void TimingAleatoire(vector<int> & timing, const vector<int> & numJobs)
    {
        int job=numJobs[0];
        int p=data.getJob(job).getP();
        int r= data.getJob(job).getR();
        int d=data.getJob(job).getD();
        int max=p;
        if(max<r+p)
            max=r+p;
        int t=max;
        eoUniformGenerator<bool> boolG;
        if(t<d && boolG())
        {
            eoUniformGenerator<int> G(max,d+1);
            t=G();
        }
        timing.push_back(t);
        for(int i=1;i<numJobs.size();i++)
        {
            job=numJobs[i];
             p=data.getJob(job).getP();
             r= data.getJob(job).getR();
             d=data.getJob(job).getD();
             max=p+timing[i-1];
            if(max<r+p)
                max=r+p;
             t=max;
            eoUniformGenerator<bool> boolG;
            if(t<d && boolG())
            {
                eoUniformGenerator<int> G(max,d+1);
                t=G();
            }
             timing.push_back(t);
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
    
void timingOpt(const vector<int> & ordre, vector<int> & temps)
	{
		
		int n=data.getN();
		vector<int> H;
		H.push_back(0);
		int ld=0;int lf=0;
		vector<int> gamma;
		gamma.push_back(0);;
		vector<int> C ;
		C.push_back(0);;
		int P=0;
		for(int k=1; k<n+1;k++)
		{
			
			int rk=data.getJob(ordre[k-1]).getR();
			int rpred=0;
			if(k>1) rpred=data.getJob(ordre[k-2]).getR();
			int pk=data.getJob(ordre[k-1]).getP();
			int dk=data.getJob(ordre[k-1]).getD();
			int alphak=data.getJob(ordre[k-1]).getAlpha();
			int betak=data.getJob(ordre[k-1]).getBeta();
			int x=C[k-1];
			if(x<rk)x=rk;
			x=x+pk-dk;
			if(rk>P) P=rk;
			P+=pk;
			int compressionMax=0;
			if(rk-C[k-1]<0) compressionMax=rk-C[k-1];
			
			if(H[lf]<compressionMax)
			{
				//cout<<"mise à jour H"<<endl;
				int diff=H[lf]-compressionMax;
				int l=lf;
				while(l>ld-1)
				{
					H[l]=H[l]-diff;
					if(H[l]>=0)
					{
						H[l]=0; ld=l;
					}
					l--;
				}

			}
			if(x<=0)
			{
				
				if(x<0)
				{	//cout<<"x<0"<<endl;
					lf=lf+1;
					int Hl=H[lf-1]+x;
					gamma.push_back(0);
					H.push_back(Hl);
				}
				gamma[lf]+=alphak;
				C.push_back(dk);
			}
			else
			{
				
				int Hnew=H[lf]+x;
				if(Hnew<0){
				
					int newP=lf;
					while(newP>ld && H[newP-1]<=Hnew)newP=newP-1;
					
					if(Hnew==H[newP])
					{
						gamma[newP]+=alphak+betak;
					}
					else
					{
						
						gamma.insert(gamma.begin()+newP,alphak+betak);
						
						H.insert(H.begin()+newP,Hnew);
						lf++;
					}
					
	
				} 
				gamma[lf]-=betak;
				int i=lf;
				C.push_back(P-H[i]);
				while(gamma[i]<=0 && i>ld)
				{
					gamma[i-1]=gamma[i-1]+gamma[i];
					C[k]=P-H[i];
					i=i-1;
					lf--;
				}
				
				while(lf>gamma.size())
				{
					gamma.pop_back(); H.pop_back();
				}
				//cout<<H[ld]<<endl;
			}
		}
		int avail=C[n];
		
		for(int k=n;k>0;k--)
		{
			if(C[k]>=avail)
				C[k]=avail;
			else avail=C[k];
			avail-=data.getJob(ordre[k-1]).getP();
		}
		C.erase(C.begin());
		temps=C;
		
	}
    

private:
    Data data;
};

#endif

