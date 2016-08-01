//
//  Jobs.h
//  Project
//
//  Created by Sophie Jacquin on 13/04/15.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#ifndef Project_Jobs_h
#define Project_Jobs_h

class Jobs {
private: int p,r,d, alpha,beta;
    
public:
    Jobs(int p_,int r_, int d_,int alpha_,int beta_)
    {
        p=p_;
        r=r_;
        d=d_;
        alpha=alpha_;
        beta=beta_;
    }
    int getP()
    {
        return p;
    }
    int getR()
    {
        return r;
    }
    int getD()
    {
        return d;
    }
    int getAlpha()
    {
        return alpha;
    }
    int getBeta()
    {
        return beta;
    }
    
};

#endif
