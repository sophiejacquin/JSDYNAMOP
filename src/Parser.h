//
//  Parser.h
//  Project
//
//  Created by Sophie Jacquin on 13/04/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#ifndef Project_Parser_h
#define Project_Parser_h
#include"Data.h"
#include<fstream>
#include<stdio.h>
class Parser{
public:
    Parser(const std::string & _filename):filename(_filename){
        
    }
    Data operator()() 
    {
        ifstream fichier(filename.c_str(),ios::in);
        vector<Jobs> jobs;
        if(fichier)
        {
            int N;
            fichier>>N;
            
            for(int i=0;i<N;i++)
            {
                int p,r,d,alpha,betta;
                fichier >>p>>r>>d>>alpha>>betta;
                jobs.push_back(Jobs(p,r,d,alpha,betta));
            }
           
            fichier.close();
            
            
        }
        else
            cerr <<" impossible d'ouvrir le fichier, vérifiez qu'il existe bien "<<filename<<endl;
        return Data(jobs);    
    }
            private:
            std::string filename;
};


#endif
