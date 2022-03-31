 #ifndef SIMULATION
 #include "Simulation.h"
 #define SIMULATION
 #endif
 #ifndef EVALQUEUE
 #include "EvalQueue.h"
 #define EVALQUEUE
 #endif
 #ifndef EVENTQUEUE
 #include "EventQueue.h"
 #define EVENTQUEUE
 #endif
 #ifndef STDLIB
 #include <stdlib.h>
 #define STDLIB
 #endif
 #ifndef STDDEF
 #include <stddef.h>
 #define STDDEF
 #endif
 #ifndef MATH
 #include <math.h>
 #define MATH
 #endif
 #ifndef STDIO
 #include <stdio.h>
 #define STDIO
 #endif
 
 // ------------Simulation variables------------------------------------------------------

    double current_time = 0.0;          // current time during simulation (minutes past 12AM)
    double prevCurrentTime = 0.0;       // to store previous current time to calculate averages
    int hoursPassed = 1;                    // stores number of hours passed to know whether to print stats
    int departure_count;                // total departures of patients leaving hospital
    int departure_count_low = 0;                // total departures of low priority patients leaving hospital
    int departure_count_med = 0;                // total departures of medium priority patients leaving hospital
    int departure_count_high = 0;                // total departures of high priority patients leaving hospital
    double avgInSystem = 0;                 // average number of patients in system
    double avgResponseTimeAll;          // average response time of all patients
    double avgResponseTimeHigh;         // average response time of high priority patients
    double avgResponseTimeMed;          // average response time of medium priority patients
    double avgResponseTimeLow;          // average response time of low priority patients
    double avgEvalWaitingTime;          // average waiting time in evaluation queue
    int numEval = 0;                        //number of evaluated patients
    int numSeen = 0;                        //number of patients past priority queue (seen/being seen)
    int numSeenLow = 0;                     //number of low priority patients past priority queue (seen/being seen)
    int numSeenMed = 0;                     //number of med priority patients past priority queue (seen/being seen)
    int numSeenHigh = 0;                    //number of high priority patients past priority queue (seen/being seen)
    double avgPriorityWaitingTimeAll;   // average waiting time in priority queue of all patients
    double avgPriorityWaitingTimeHigh;  // average waiting time in priority queue of high priority patients
    double avgPriorityWaitingTimeMed;   // average waiting time in priority queue of medium priority patients
    double avgPriorityWaitingTimeLow;   // average waiting time in priority queue of low priority patients
    double avgCleanUpTime;              // average time to clean up the patient room
    int numAvailableRooms;
    int numCleanedRooms;                  // number of rooms that have been cleaned
    int numberOfTurnedAwayPatients;     // total number of turned away patients due to full capacity
    int totalNumberInSystemNow;         // tracks total number of patients in hospital at given moment
    int numRooms;

//


// Printing out the report of statistics at every hour

void PrintStatistics(struct Queue* elementQ, struct EvalQueue* evalQ, int hoursPassed){

   if(current_time >= 1440) {
     printf("\nEnd of Simulation - at 12AM the next day:\n");
   }
   else printf("\nAt %d O'Clock:\n", hoursPassed);

  printf("Total departures: %d\n", departure_count);
  printf("Average in system: %.2f\n", ((avgInSystem) + (totalNumberInSystemNow * ((hoursPassed*60)-prevCurrentTime)))/(hoursPassed*60));
  if(departure_count > 0){ printf("Average response time for all patients: %.2f\n", avgResponseTimeAll/departure_count); }
  else { printf("Average response time for all patients: N/A\n");}
  if(departure_count_high > 0){ printf("Average response time for high priority patients: %.2f\n", avgResponseTimeHigh/departure_count_high); }
  else { printf("Average response time for high priority patients: N/A\n");}
  if(departure_count_med > 0){ printf("Average response time for medium priority patients: %.2f\n", avgResponseTimeMed/departure_count_med); }
  else { printf("Average response time for medium priority patients: N/A\n");}
  if(departure_count_low > 0){ printf("Average response time for low priority patients: %.2f\n", avgResponseTimeLow/departure_count_low); }
  else { printf("Average response time for low priority patients: N/A\n");}
  if(numEval > 0){ printf("Average Evaluation Waiting Time: %.2f\n", avgEvalWaitingTime/numEval); }
  else { printf("Average Evaluation Waiting Time: N/A\n");}
  if(numSeen > 0){ printf("Average waiting time for all priority patients: %.2f\n", avgPriorityWaitingTimeAll/numSeen); }
  else { printf("Average waiting time for all priority patients: N/A\n");}
  if(numSeenHigh > 0){  printf("Average waiting time for high priority patients: %.2f\n", avgPriorityWaitingTimeHigh/numSeenHigh); }
  else {   printf("Average waiting time for high priority patients: N/A\n");}
  if(numSeenMed > 0){ printf("Average waiting time for medium priority patients: %.2f\n", avgPriorityWaitingTimeMed/numSeenMed); }
  else { printf("Average waiting time for medium priority patients: N/A\n");}
  if(numSeenLow > 0){ printf("Average waiting time for low priority patients: %.2f\n", avgPriorityWaitingTimeLow/numSeenLow); }
  else { printf("Average waiting time for low priority patients: N/A\n");}
  if(numSeenLow > 0){ printf("Average cleaning time for patient rooms: %.2f\n", avgCleanUpTime/numCleanedRooms); }
  else { printf("Average cleaning time for patient rooms: N/A\n");}
  printf("Number of turned away patients due to max capacity: %d\n", numberOfTurnedAwayPatients);

}

//Adds the number of people in system * time they were there

void AddAvgInSystem(double lastTime){
  avgInSystem += totalNumberInSystemNow * (current_time - lastTime);
}

//Printing helper 
void HourPassedPrint(struct EventQueueNode* next, struct Queue* priorityQ, struct EvalQueue* evalQ) {
      while(next->event_time > hoursPassed * 60 && hoursPassed*60 <=1440) {
      PrintStatistics(priorityQ, evalQ, hoursPassed);
      hoursPassed++;
    }
}

// This is the main simulator function
// Runs until 24 hours (1440 minutes)
// Determines what the next event is based on current_time
// Print statistics if current time has passed a full hour

void Simulation(int random_seed, struct EventQueue* eventQ, struct EvalQueue* evalQ, struct Queue* priorityQ, int numNurses, double highPriLambda, double highPriMu, double medPriLambda, double medPriMu, double lowPriLambda, double lowPriMu, double evalMu, double cleanMu, int numJanitors, int numRooms2, int maxCapacity)
{

  numRooms = numRooms2;
  while(current_time < 1440) {
 
    if((eventQ->head)->event_type == 1) {
      HourPassedPrint(eventQ->head, priorityQ, evalQ);
      current_time = eventQ->head->event_time;
      ProcessEvalArrival(eventQ, evalQ, (eventQ->head)->qnode, random_seed, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu, evalMu, maxCapacity, priorityQ->available_rooms);
    }
    else if(((eventQ->head)->event_type == 2) || (eventQ->head)->event_type == 4) {
      struct EventQueueNode* curr = eventQ->head;
      while(curr != NULL) {
        if(curr->event_type == 1) {
          HourPassedPrint(curr, priorityQ, evalQ);
          current_time = curr->event_time;
          ProcessEvalArrival(eventQ, evalQ, curr->qnode, random_seed, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu, evalMu, maxCapacity, priorityQ->available_rooms);
          break;
        }
        else if(curr->event_type == 3) {
          HourPassedPrint(curr, priorityQ, evalQ);
          current_time = curr->event_time;
          ProcessPriorityArrival(eventQ, evalQ, priorityQ, curr->qnode);
          StartRoomService(eventQ, priorityQ, highPriMu, medPriMu, lowPriMu);
          DeleteEventNode(eventQ);
          break;
        }
        else if(curr->event_type == 5) {
          HourPassedPrint(curr, priorityQ, evalQ);
          current_time = curr->event_time;
          ProcessPatientDeparture(eventQ, priorityQ, curr->qnode, cleanMu);
          DeleteEventNode(eventQ);
          break;
        }
        else if(curr->event_type == 6) {
          HourPassedPrint(curr, priorityQ, evalQ);
          current_time = curr->event_time;
          JanitorCleanedRoom(eventQ, priorityQ, curr);
          StartRoomService(eventQ, priorityQ, highPriMu, medPriMu, lowPriMu);
          DeleteEventNode(eventQ);
          break;
        }
        curr = curr->next;
      }
    }
    else if((eventQ->head)->event_type == 3) {
      HourPassedPrint(eventQ->head, priorityQ, evalQ);
      current_time = eventQ->head->event_time;
      ProcessPriorityArrival(eventQ, evalQ, priorityQ, (eventQ->head)->qnode);
      DeleteEventNode(eventQ);
      StartRoomService(eventQ, priorityQ, highPriMu, medPriMu, lowPriMu);
    }
    else if((eventQ->head)->event_type == 5) {
      HourPassedPrint(eventQ->head, priorityQ, evalQ);
      current_time = eventQ->head->event_time;
      ProcessPatientDeparture(eventQ, priorityQ, (eventQ->head)->qnode, cleanMu);
      DeleteEventNode(eventQ);
    }
    else if((eventQ->head)->event_type == 6) {
      HourPassedPrint(eventQ->head, priorityQ, evalQ);
      current_time = eventQ->head->event_time;
      JanitorCleanedRoom(eventQ, priorityQ, (eventQ->head));
      DeleteEventNode(eventQ);
      StartRoomService(eventQ, priorityQ, highPriMu, medPriMu, lowPriMu);
    }
  }
  //PrintStatistics(priorityQ, evalQ);
}