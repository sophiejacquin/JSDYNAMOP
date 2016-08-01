//
//  Data.h
//  Project
//
//  Created by Sophie Jacquin on 13/04/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#ifndef Project_Data_h
#define Project_Data_h
#include "Jobs.h"
#include<stdio.h>

class Data{
private:
    int N;
    vector<Jobs> jobs;
    public :
    Data(vector<Jobs> Jobs_)
    {
        jobs=Jobs_;
        N=jobs.size();
    }
    Data()
    {
        N=0;
    }
    int getN()const
    {
        return N;
    }
    Jobs getJob(int i)
    {
        return jobs[i];
    }
    
};


#endif
