/** -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

The above line is usefulin Emacs-like editors
 blabla sophie xxx*/

/*
Template for creating a new representation in EO
================================================

Mandatory:
- a default constructor (constructor without any argument)
- the I/O functions (readFrom and printOn)

However, if you are using dynamic memory, there are 2 places
to allocate it: the default constructor (if possible?), or, more in
the EO spirit, the eoInit object, that you will need to write anyway
(template file init.tmpl). 

But remember that a COPY CONSTRUCTOR will be used in many places in EO,
so make sure that the default copy constructor works, or, even better,
do write your own if in doubt. 
And of course write the corresponding destructor!

*/

#ifndef _eoOneMachine_h
#define _eoOneMachine_h

/** 
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen

 * Note that you MUST derive your structure from EO<fitT>
 * but you MAY use some other already prepared class in the hierarchy
 * like eoVector for instance, if you handle a vector of something....

 * If you create a structure from scratch,
 * the only thing you need to provide are 
 *        a default constructor
 *        IO routines printOn and readFrom
 *
 * Note that operator<< and operator>> are defined at EO level
 * using these routines
 */
template< class FitT>
class eoOneMachine: public EO<FitT> {
public:
  /** Ctor: you MUST provide a default ctor.
   * though such individuals will generally be processed 
   * by some eoInit object
   */
  eoOneMachine() 
  { 
      N=0;
  }

  /** Copy Ctor: you MUST provide a copy ctor if the default
   * one is not what you want
   * If this is the case, uncomment and fill the following
   */
  /*
  eoOneMachine(const eoOneMachine &) 
  {  
    // START Code of copy Ctor of an eoOneMachine object
    // END   Code of copy Ctor of an eoOneMachine object
  } 
  */


  virtual ~eoOneMachine()
  {
    // START Code of Destructor of an eoEASEAGenome object
    // END   Code of Destructor of an eoEASEAGenome object
  }

  virtual string className() const { return "eoOneMachine"; }

  /** printing... */
    void printOn(ostream& os) const
    {
      // First write the fitness
      EO<FitT>::printOn(os);
      os << ' ';
        for(int i=0;i<N;i++)
        {
            os<<"("<<dernierVus[i]<<","<<completionTime[i]<<") ";
        }
        
    }

  /** reading... 
   * of course, your readFrom must be able to read what printOn writes!!!
   */
    void readFrom(istream& is)
      {
	// of course you should read the fitness first!
	EO<FitT>::readFrom(is);

      }
    void setN(int N_)
    {
        N=N_;
    }
    int getN() const
    {
        return N;
    }
    void adDejaVus(vector<bool> b)
    {
        dejaVus.push_back(b);
        modif.push_back(true);
        eval.push_back(0);
        N++;
    }
    bool getDejaVu(int i,int j) const
    {
        return dejaVus[i][j];
    }
    vector<bool> getDejaVus(int i) const
    {
        return dejaVus[i];
    }
    void setDejaVu(int i,int j, bool b)
    {
        dejaVus[i][j]=b;
    }
    void setDejaVus(int i, vector<bool> b)
    {
        dejaVus[i]=b;
    }
    vector<bool> getDejaVu(int i) const
    {
        return dejaVus[i];
    }
    void adDernierVu(int i)
    {
        dernierVus.push_back(i);
    }
    int getDernierVu(int i)const
    {
        return dernierVus[i];
    }
    void setDernierVu(int i, int j)
    {
        dernierVus[i]=j;
    }
    void setModif(int i,bool b)
    {
        modif[i]=b;
    }
    bool getModif(int i) const
    {
        return modif[i];
    }
    void adCompletionTime(int i)
    {
        completionTime.push_back(i);
    }
    int getCompletionTime(int i)
    {
        return completionTime[i];
    }
    void setCompletionTime(int i,int j)
    {
        completionTime[i]=j;
    }
    double getEval(int i) const
    {
        return eval[i];
    }
    void setEval(int i, double e)
    {
        eval[i]=e;
    }
    double evaluation()
    {
        return ev;
    }
    void evaluation(double e)
    {
        ev=e;
    }
private:			   // put all data here
    int N;
    vector< vector<bool> > dejaVus;
    vector<int> dernierVus;
    vector<int> completionTime;
    vector<bool> modif;
    vector<double> eval;
    double ev;
};

#endif

