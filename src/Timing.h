#ifndef eoOneMachineTiming_H
#define eoOneMachineTiming_H
#include "Timing.h"

#include <eoOp.h>
template<class GenotypeT>
class Timing: public eoMonOp<GenotypeT>
{
public:
	Timing(Data _data)
	{
		data=_data;
	}
	bool operator()(GenotypeT & _genotype)
	{
		vector<int> ordre;
		vector<bool> dejaVus;
		vector<int>  temps;
		for(int i=0;i<data.getN();i++)
		{
			ordre.push_back(_genotype.getDernierVu(i));
			dejaVus.push_back(0);
		}
		timing(ordre,temps);
		miseAJour(0,data.getN()-1,temps,ordre,dejaVus,_genotype);
		return true;
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
	void timing(const vector<int> & ordre, vector<int> & temps)
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
