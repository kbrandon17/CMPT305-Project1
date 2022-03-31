#ifndef EVALQUEUE
#include "EvalQueue.h"
#define EVALQUEUE
#endif
#ifndef EVENTQUEUE
#include "EventQueue.h"
#define EVENTQUEUE
#endif
#ifndef PRIORITYQUEUE
#include "PriorityQueue.h"
#define PRIORITYQUEUE
#endif

void AddAvgInSystem(double lastTime);
void Simulation(int random_seed, struct EventQueue* eventQ, struct EvalQueue* evalQ, struct Queue* priorityQ, int numNurses, double highPriLambda, double highPriMu, double medPriLambda, double medPriMu, double lowPriLambda, double lowPriMu, double evalMu, double cleanMu, int numJanitors, int numRooms, int maxCapacity);


    extern double current_time;         
    extern double prevCurrentTime;       
    extern int hoursPassed;                   
    extern int departure_count;      
    extern int departure_count_low;   
    extern int departure_count_med;   
    extern int departure_count_high;            
    extern double avgInSystem;                
    extern double avgResponseTimeAll;         
    extern double avgResponseTimeHigh;        
    extern double avgResponseTimeMed;         
    extern double avgResponseTimeLow;          
    extern double avgEvalWaitingTime; 
    extern int numEval;      
    extern int numSeen;   
    extern int numSeenLow;
    extern int numSeenMed;
    extern int numSeenHigh;
    extern double avgPriorityWaitingTimeAll;   
    extern double avgPriorityWaitingTimeHigh;  
    extern double avgPriorityWaitingTimeMed;   
    extern double avgPriorityWaitingTimeLow;   
    extern double avgCleanUpTime;    
    extern int numCleanedRooms;          
    extern int numberOfTurnedAwayPatients;     
    extern int totalNumberInSystemNow;         
    extern int numRooms;