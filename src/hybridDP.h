//
//  hybridDP.h
//  Project
//
//  Created by Sophie Jacquin on 28/04/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#ifndef Project_hybridDP_h
#define Project_hybridDP_h
#include <iostream>
#include <string>
#include <unordered_map>
#include <eoOp.h>
typedef struct{
    
    vector <vector<int> > tFin;
    vector<double>  cout;
    vector< vector<int> > chemin;
    vector<int> jobs;
    int tMaxF;
}Etat;
template <class GenotypeT>
class hybridDP: public eoMonOp<GenotypeT> {
    
private:
    int step;
    int taille;
    Data data;
    int MAXI;
public:
    hybridDP(Data d)
    {
        step=1;
        taille=6;
        data=d;
	MAXI=500;
    }
    hybridDP(int MAXI_,int taille_, Data d)
    {
        step=1;
        taille=taille_;
        data=d;
        MAXI=MAXI_;
    }
    /*hybridDP(int step_,int taille_, Data d)
    {
        step=step_;
        taille=taille_;
        data=d;
        MAXI=50;
    }*/
    bool operator()(GenotypeT & _genotype)
    {
	//cout<<"debH"<<endl;
        int NbJobs=_genotype.getN();
        eoUniformGenerator<int> choixTDeb(-1,NbJobs-taille);
        int tDeb=choixTDeb();
	//cout<<"tDeb="<<tDeb<<endl;
        int i;
	double cout_max=0;
        vector<int> jobs;
	for(i=tDeb+1;i<=tDeb+taille;i++)
	{
		if(!_genotype.getModif(i))cout_max+=_genotype.getEval(i);
		else
		{
		  int job=_genotype.getDernierVu(i);
		  int e=0; int t=0;
                  int c=_genotype.getCompletionTime(i);
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
		   cout_max+=t+e;
			//cout<<cout_max<<endl;
		//cout<<job<<endl;
		}
	}
	

        int tmaxf=_genotype.getCompletionTime(tDeb+taille);
	if(tDeb>=0){
        for(i=0;i<NbJobs;i++)
        {
            if(_genotype.getDejaVu(tDeb,i)!=_genotype.getDejaVu(tDeb+taille,i))
            {
               jobs.push_back(i);
                tmaxf-=data.getJob(i).getP();
		//cout<<i<<endl;
            }
        }}
	else{
        for(i=0;i<NbJobs;i++)
        {
            if(_genotype.getDejaVu(tDeb+taille,i))
            {
               jobs.push_back(i);
                tmaxf-=data.getJob(i).getP();
            }
        }}
        unordered_map<std::string,Etat> mapPred;
        int tini;
	if(tDeb>=0)tini=_genotype.getCompletionTime(tDeb);
	else tini=0;
	
        //états initiaux
        for(i=0;i<taille;++i)
        {
		
            int p=data.getJob(jobs[i]).getP();
            int d=data.getJob(jobs[i]).getD();
            int r=data.getJob(jobs[i]).getR();
            int alpha=data.getJob(jobs[i]).getAlpha();
            int beta=data.getJob(jobs[i]).getBeta();
            int tmax1=d;
            if(tmax1>tmaxf+p)
                tmax1=tmaxf+p;
            int tmax2=tini;
            if(tmax2<r)
                tmax2=r;
            tmax2+=p;
            int tmax=tmax1;
            if(tmax<tmax2)
                tmax=tmax2;
            int tmin=tmax2;

            vector<int> chemin;
            chemin.push_back(jobs[i]);
            vector< vector<int> > chemins;
            vector <double>  couts;
            vector< vector <int> > tempsFins;
            int  tMaxF=tmaxf+p;
            vector <int> numJobs;
            numJobs.push_back(i);
            if(tmin<=tmaxf+p)
            {
		if (tmin<_genotype.getCompletionTime(tDeb+1)-MAXI)
			tmin=_genotype.getCompletionTime(tDeb+1)-MAXI;
		if (tmax<_genotype.getCompletionTime(tDeb+1)-MAXI)
			tmax=_genotype.getCompletionTime(tDeb+1)-MAXI;
		if(tmax>_genotype.getCompletionTime(tDeb+1)+MAXI)
			tmax=_genotype.getCompletionTime(tDeb+1)+MAXI;
            for(int t=tmax;t>=tmin;t-=step)
            {
                chemins.push_back(chemin);
                vector<int> tempsFin;
                tempsFin.push_back(t);
                tempsFins.push_back(tempsFin);
                int c=0;
                if(t>d)
                    c=beta*(t-d);
                else if(t<d)
                    c=alpha*(d-t);
                couts.push_back(c);
            }
            }
            Etat etat;
            etat.tMaxF=tMaxF;
            etat.tFin=tempsFins;
            etat.cout=couts;
            etat.chemin=chemins;
            etat.jobs=numJobs;
            string label=std::to_string(i);
            std::pair<std::string,Etat> stock (label,etat);
            mapPred.insert(stock);
            
        }
	//cout<<"autres états"<<endl;
        //autres états :

        for(i=1;i<taille;i++)
        {
		
            unordered_map<std::string,Etat> mapCurent;
            for(auto it=mapPred.begin();it!=mapPred.end();++it)
            {
		//cout<<mapPred.size()<<endl;
                Etat ePred=it->second;
		//cout<<"first : "<<it->first<<endl;
                int last=ePred.jobs[i-1];
                for(int k=last+1;k<taille;k++)
                {
			
                    Etat etatCourant;
                    etatCourant.jobs=ePred.jobs;
                    etatCourant.jobs.push_back(k);
                    int p=data.getJob(jobs[k]).getP();
                    etatCourant.tMaxF=ePred.tMaxF+p;
                    string label=it->first+"-"+to_string(k);
			
                    inserer(jobs[k],ePred,etatCourant,_genotype.getCompletionTime(tDeb+i+1),cout_max);
			
                    for(int l=0;l<i;l++)
                    {
			
                        string chaine;
                        int b=0;
                        if(l==0)b=1;
                        chaine=to_string(etatCourant.jobs[b]);
                        for(int j=b+1;j<l;j++)
                            {
				chaine+="-"+to_string( etatCourant.jobs[j]);
				//cout<<j<<" "<<etatCourant.jobs[j]<<" "<<l<<endl;
				}
			b=l+1;
			if(l==0) b=2;
                        for(int j=b;j<=i;j++)
			{
                            chaine+="-"+to_string( etatCourant.jobs[j]);
			   // cout<<j<<" "<<etatCourant.jobs[j]<<" "<<l<<endl;
			}
			//cout<<chaine<<endl;
			/*for(auto ot=mapPred.begin();ot!=mapPred.end();++ot)
				cout<<ot->first;*/
                        Etat etatPred;
                        etatPred= mapPred.find(chaine)->second;
			//cout<<"pb find ?"<<endl;
                        //cout<<"av inserer"<<endl;
                        inserer(jobs[etatCourant.jobs[l]],etatPred,etatCourant,_genotype.getCompletionTime(tDeb+i+1),cout_max);
                        //cout<<"ap inserer"<<endl;  
                        
                    }
//cout<<"fin petite "<<i<<" "<<etatCourant.chemin.size()<<endl;
			
			//
			/*if(etatCourant.chemin.size()>MAXI)
			{
				Etat nouv;
				nouv.jobs=etatCourant.jobs;
				nouv.tMaxF=etatCourant.tMaxF;
				int pas=etatCourant.chemin.size()/MAXI;
				for(int p=0;p<pas*MAXI && p<etatCourant.chemin.size();p+=pas)
				{
					nouv.chemin.push_back(etatCourant.chemin[p]);
					nouv.cout.push_back(etatCourant.cout[p]);
					nouv.tFin.push_back(etatCourant.tFin[p]);
				}
				etatCourant=nouv;
			}*/
                    std::pair<std::string,Etat> stock (label,etatCourant);
                    mapCurent.insert(stock);
                }
                
            }
            mapPred=mapCurent;
        }
//cout<<"recup "<<tDeb<<endl; 
//cout<<" first "<<mapPred.begin()->first<<endl;
	
        //recuperation du resultat :
        Etat eF = mapPred.begin()->second;
	
        vector<bool> dejaVus;
	if(tDeb>-1) dejaVus=_genotype.getDejaVu(tDeb);
	else
	{
		
		for(int i=0;i<NbJobs;i++)
			dejaVus.push_back(false);
	}
	//cout<<"pb deja vu? "<<eF.tFin.size()<<" " <<eF.chemin.size()<<endl;
        vector<int> timing =eF.tFin[0];
;
        vector<int> numJobs=eF.chemin[0];

        miseAJour(tDeb+1,tDeb+taille,timing,numJobs,dejaVus,_genotype);
      // cout<<"fin H"<<endl;
	if(!verifier(_genotype))
		cout<<"Erreur tDb="<<tDeb<<endl;
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
    void inserer (int j,Etat etatPred,  Etat & etatCourant,double m,double cout_max)
    {
	//cout<<"deb inserer "<< j<<endl;
        int NbChemins=etatPred.chemin.size();
        int tailleChemin;
        if(NbChemins>0) tailleChemin=etatPred.chemin[0].size();
        int p=data.getJob(j).getP();
        int d=data.getJob(j).getD();
        int r=data.getJob(j).getR(); 
        int alpha=data.getJob(j).getAlpha();
        int beta=data.getJob(j).getBeta();
	//cout<<"NBchemins :"<<NbChemins<<endl;
        for(int i=0;i<NbChemins;i++)
        {

            int tmin=r;
            if(tmin<etatPred.tFin[i][tailleChemin-1])
                tmin=etatPred.tFin[i][tailleChemin-1];
            tmin+=p;
            int tmax=tmin;
            int ttruc=d;
            if(ttruc>etatPred.tMaxF+p)
                ttruc=etatPred.tMaxF+p;
            if(tmax<ttruc)
                tmax=ttruc;
            if(tmin<=etatPred.tMaxF+p)
            {
		if (tmin<m-MAXI)
			tmin=m-MAXI;
		if (tmax<m-MAXI)
			tmax=m-MAXI;
		if(tmax>m+MAXI)
			tmax=m+MAXI;
		
               // if(tmin>tmax) cout <<"tmin="<<tmin<<">tmax="<<tmax<<" m+M="<<m+MAXI<<" m-Maxi="<<m-MAXI<<" d "<<d<<endl;
                for(int t=tmin;t<=tmax;t+=step)
                {
			
                    double cost=etatPred.cout[i];
                    if(t<d)
                        cost+=alpha*(d-t);
                    else if(t>d)
                        cost+=beta*(t-d);
		
		
                    int it=0;
                    bool trouve=false;
			//cout<<"deb while"<<endl;
			if(cost>cout_max) {trouve=true;}// cout<<" cout trop fort "<<cost<<" cm "<<cout_max<<" "<<etatPred.cout[i]<<" j="<<j<<" d="<<d<<" alpha="<<alpha<<" bata="<<beta<<" t "<<t<<" m "<<m<<endl; }
			//else{cout<<"ok "<<i<<endl;}
			
                    while(it<etatCourant.chemin.size() && !trouve)
                    {
			//cout<<"tFin "<<etatCourant.tFin[it][tailleChemin-1]<<" it "<<it<<endl;
                        if(t>etatCourant.tFin[it][tailleChemin])
                        {
                            trouve=true;
                            if(cost<etatCourant.cout[it])
                            {
                                vector<int> fin=etatPred.tFin[i];
                                fin.push_back(t);
                                etatCourant.tFin.insert(etatCourant.tFin.begin()+it,fin);
                                etatCourant.cout.insert(etatCourant.cout.begin()+it,cost);
                                vector<int> chemin=etatPred.chemin[i];
                                chemin.push_back(j);
				
                                etatCourant.chemin.insert(etatCourant.chemin.begin()+it,chemin);
                            }
				
                        }
                        else 
                        {
				if(cost<=etatCourant.cout[it]){
                            etatCourant.chemin.erase(etatCourant.chemin.begin()+it);
                            etatCourant.tFin.erase(etatCourant.tFin.begin()+it);
                            etatCourant.cout.erase(etatCourant.cout.begin()+it);
				}
				
			   else if(t==etatCourant.tFin[it][tailleChemin] && cost>=etatCourant.cout[it])
				{trouve=true;
					
				}
			   else it ++;
                        }
                        
                    }
			//cout<<"fin while"<<endl;
                    if(!trouve)
                    {
                        vector<int> fin=etatPred.tFin[i];
                        fin.push_back(t);
                        etatCourant.tFin.push_back(fin);
                        etatCourant.cout.push_back(cost);
                        vector<int> chemin=etatPred.chemin[i];
			chemin.push_back(j);
			
                        etatCourant.chemin.push_back(chemin);                   
                    }
                    
                }
            }
        }
	//cout<<"fin inserer"<<endl;
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

};

#endif
