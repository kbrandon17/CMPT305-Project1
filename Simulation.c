

void Simulation(){
    // ------------Simulation variables------------------------------------------------------

    double current_time = 0.0;          // current time during simulation (minutes past 12AM)
    double prevCurrentTime = 0.0;       // to store previous current time to calculate averages
    int departure_count;                // total departures of patients leaving hospital
    double avgInSystem;                 // average number of patients in system
    double avgResponseTimeAll;          // average response time of all patients
    double avgResponseTimeHigh;         // average response time of high priority patients
    double avgResponseTimeMed;          // average response time of medium priority patients
    double avgResponseTimeLow;          // average response time of low priority patients
    double avgEvalWaitingTime;          // average waiting time in evaluation queue
    double avgPriorityWaitingTimeAll;   // average waiting time in priority queue of all patients
    double avgPriorityWaitingTimeHigh;  // average waiting time in priority queue of high priority patients
    double avgPriorityWaitingTimeMed;   // average waiting time in priority queue of medium priority patients
    double avgPriorityWaitingTimeLow;   // average waiting time in priority queue of low priority patients
    double avgCleanUpTime;              // average time to clean up the patient room
    int numberOfTurnedAwayPatients;     // total number of turned away patients due to full capacity
}

// Printing out the report of statistics at every hour

void PrintStatistics(struct Queue* elementQ, struct EvalQueue* evalQ){

   if(current_time >= 1440) {
     printf("End of Simulation - at 12AM the next day:\n", departure_count);
   }
   else printf("At %f O'Clock:\n", current_time/60);



  printf("Total departures: %d\n", departure_count);
  printf("Average in system: %f\n", avgInSystem);
  printf("Average response time for all patients: %f\n", avgResponseTimeAll);
  printf("Average response time for high priority patients: %f\n", avgResponseTimeHigh);
  printf("Average response time for medium priority patients: %f\n", avgResponseTimeMed);
  printf("Average response time for low priority patients: %f\n", avgResponseTimeLow);
  printf("Average Evaluation Waiting Time: %f\n", avgEvalWaitingTime);
  printf("Average waiting time for all priority patients: %f\n", avgPriorityWaitingTimeAll);
  printf("Average waiting time for high priority patients: %f\n", avgPriorityWaitingTimeHigh);
  printf("Average waiting time for medium priority patients: %f\n", avgPriorityWaitingTimeMed);
  printf("Average waiting time for low priority patients: %f\n", avgPriorityWaitingTimeLow);
  printf("Average cleaning time for patient rooms: %f\n", avgCleanUpTime);
  printf("Number of turned away patients due to max capacity: %d\n", numberOfTurnedAwayPatients);

}

// This is the main simulator function
// Runs until 24 hours (1440 minutes)
// Determines what the next event is based on current_time
// Print statistics if current time has passed a full hour

void Simulation(struct Queue* elementQ, double lambda, double mu, int print_period, int total_departures)
{
    struct EvalQueue* evalQ = InitializeEvalQueue(numNurses, random_seed, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu, evalMu);
  // while(departure_count != total_departures)
  // {
  //     if((elementQ->first != NULL) && (elementQ->last)->next != NULL
  //     && (((elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time)
  //           == (((elementQ->last)->next)->arrival_time))) {
  //             ProcessDeparture(elementQ);
  //             if((elementQ->last)->next != NULL) {
  //               ProcessArrival(elementQ, (elementQ->last)->next);
  //             }
  //               continue;

  //     }



  //     if(elementQ->first != NULL
  //       && ((elementQ->last)->next == NULL || (((elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time)
  //         <= (((elementQ->last)->next)->arrival_time))))
  //     {

  //       ProcessDeparture(elementQ);
  //       if(departure_count%print_period == 0)
  //       {
  //           PrintStatistics(elementQ, total_departures, print_period, lambda);
  //       }
  //     }
  //     else
  //     {
  //       if(departure_count == 0 && (elementQ->first == NULL))
  //       {
  //         //printf("TEST");
  //         ProcessArrival(elementQ, elementQ->head);
  //       }
  //       else
  //       {

  //         ProcessArrival(elementQ, (elementQ->last)->next);
  //       }
  //     }
  // }
  //           PrintStatistics(elementQ, total_departures, print_period, lambda);
}