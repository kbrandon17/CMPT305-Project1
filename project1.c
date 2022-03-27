#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>
#include<float.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>


// Definition of a Queue Node including arrival and service time
struct QueueNode {
    double arrival_time;     // customer arrival time, measured from time t=0, inter-arrival times exponential
    double service_time;     // customer service time (exponential)
    double waiting_time;     // customer time spent waiting in queue
    int priority;            // 1 - low, 2 - med, 3 - high
    struct QueueNode *next;  // next element in line; NULL if this is the last element
};

// Evaluation queue to track next arrival

struct EvalQueue {

    struct QueueNode* nextHighPri;    // next high priority to arrive
    struct QueueNode* nextMedPri;    // next medium priority to arrive
    struct QueueNode* nextLowPri;  // next low priority node to arrive
    int totalInSystem;

};

// TODO Priority Queue to track patients after evaluation

struct Queue {

    struct QueueNode* head;    // Point to the first node of the element queue
    struct QueueNode* tail;    // Point to the tail node of the element queue
    struct QueueNode* first;  // Point to the first arrived customer that is waiting for service
    struct QueueNode* firstWaiting; //first node waiting in queue
    struct QueueNode* last;   // Point to the last arrrived customer that is waiting for service
    int waiting_count;     // Current number of customers waiting for service
    double cumulative_response;  // Accumulated response time for all effective departures
    double cumulative_waiting;  // Accumulated waiting time for all effective departures
    double cumulative_idle_times;  // Accumulated times when the system is idle, i.e., no customers in the system
    double cumulative_number;   // Accumulated number of customers in the system
    int totalInSystem;
};


// ------------Global variables------------------------------------------------------

double current_time = 0.0;          // current time during simulation (minutes past 12AM)
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
int numberOfTurnedAwayPatients;  // total number of turned away patients due to full capacity
double prevCurrentTime = 0.0;       // to store previous current time to help calculate stats


//-----------------Queue Functions------------------------------------

struct QueueNode* CreateNode(double Narrival_time, double Nservice_time) {

  struct QueueNode* newNode = malloc(sizeof *newNode);
  newNode->arrival_time = Narrival_time;
  newNode->service_time = Nservice_time;
  newNode->waiting_time = 0.0;
  newNode->next = NULL;
  return newNode;
}

void Insert (struct Queue *q, double Narrival_time, double Nservice_time) {

struct QueueNode* n = CreateNode(Narrival_time, Nservice_time);

if(q->head == NULL) {
  q->head = n;
  q->tail = n;
}
else{
  n->arrival_time = n->arrival_time + (q->tail)->arrival_time;
  (q->tail)->next = n;
  q->tail = n;
}
}


// Initializes the evaluation queue, setting the first arrival of each of the three priorities

struct EvalQueue* InitializeEvalQueue(int seed, double highprilambda, double highprimu, double medprilambda, double medprimu, double lowprilambda, double lowprimu){
  struct EvalQueue* newQueue = malloc(sizeof *newQueue);

  srand(seed);

    double highPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double highPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    double medPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double medPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    double lowPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double lowPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    newQueue->nextHighPri = CreateNode(highPriArr, highPriSer);
    (newQueue->nextHighPri)->priority = 3;
    newQueue->nextMedPri = CreateNode(medPriArr, medPriSer);
    (newQueue->nextMedPri)->priority = 2;
    newQueue->nextLowPri = CreateNode(lowPriArr, lowPriSer);
    (newQueue->nextLowPri)->priority = 1;

  return newQueue;
}


// Printing out the report of statistics at every hour

void PrintStatistics(struct Queue* elementQ, int total_departures, int print_period, double lambda){

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

// Function to process the arrival of a patient to the hospital.

void ProcessEvalArrival(struct EvalQueue* evalQ, struct QueueNode* arrival){

prevCurrentTime = current_time;
current_time = arrival->arrival_time;

//@kbrandon17 might not need below depending on sim function
// //next to arrive is high priority
// if(arrival = evalQ->nextHighPri)
// {
//   current_time = (evalQ->nextHighPri)->arrival_time;
// }
// //next to arrive is medium priority
// else if(arrival = evalQ->nextMedPri)
// {
//   current_time = (evalQ->nextMedPri)->arrival_time;
// }
// //next to arrive is low priority
// else if(arrival = evalQ->nextLowPri)
// {
//   current_time = (evalQ->nextLowPri)->arrival_time;
// }

evalQ->totalInSystem++;


}

// Function to start nurse evaluation

void StartEvaluationService(struct Queue* elementQ)
{
  // (elementQ->firstWaiting)->waiting_time = current_time-((elementQ->firstWaiting)->arrival_time);
  // elementQ->cumulative_waiting += (elementQ->firstWaiting)->waiting_time;
  // elementQ->first = elementQ->firstWaiting;

  // if(((elementQ->firstWaiting)->next != NULL) && 
  //     ((elementQ->firstWaiting)->next)->arrival_time < current_time) {
  //     elementQ->firstWaiting = (elementQ->firstWaiting)->next;
  // }
  // else {
  //   elementQ->firstWaiting = NULL;
  // }
  // elementQ->waiting_count--;

  // printf( "FIRST: %f\n", ((elementQ->first)->arrival_time));
  // printf( "NEXT ARRIVING: %f\n", (((elementQ->last)->next)->arrival_time));

}

// Called after patient has been helped by nurse and begins waiting in priority queue

void ProcessPriorityArrival(struct Queue* elementQ, struct QueueNode* arrival){

// prevCurrentTime = current_time;
// current_time = arrival->arrival_time;

// //printf("node about to arrive: %d\n", elementQ->totalInSystem);
// elementQ->cumulative_number += (current_time - prevCurrentTime)*elementQ->totalInSystem;
// elementQ->totalInSystem++;



// if(elementQ->first == NULL)
// {
//   elementQ->cumulative_idle_times += current_time-prevCurrentTime;
//   elementQ->firstWaiting = arrival;
//   elementQ->last = arrival;
//   StartService(elementQ);
// }

// else if(elementQ->firstWaiting == NULL)
// {
//   elementQ->firstWaiting = arrival;
//   elementQ->last = arrival;
//   elementQ->waiting_count++;
// }
// else {
//   elementQ->last = arrival;
//   elementQ->waiting_count++;
// }

}

// Function to put patient from priority queue into a room

void StartEvaluationService(struct Queue* elementQ)
{
  // (elementQ->firstWaiting)->waiting_time = current_time-((elementQ->firstWaiting)->arrival_time);
  // elementQ->cumulative_waiting += (elementQ->firstWaiting)->waiting_time;
  // elementQ->first = elementQ->firstWaiting;

  // if(((elementQ->firstWaiting)->next != NULL) && 
  //     ((elementQ->firstWaiting)->next)->arrival_time < current_time) {
  //     elementQ->firstWaiting = (elementQ->firstWaiting)->next;
  // }
  // else {
  //   elementQ->firstWaiting = NULL;
  // }
  // elementQ->waiting_count--;

  // printf( "FIRST: %f\n", ((elementQ->first)->arrival_time));
  // printf( "NEXT ARRIVING: %f\n", (((elementQ->last)->next)->arrival_time));

}

// Function for when a patient is finished in a room and leaves (adds an event to janitor queue)

void ProcessPatientDeparture(struct Queue* elementQ){

// prevCurrentTime = current_time;
// current_time = (elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time;

// //printf("node about to depart: %d\n", elementQ->totalInSystem);
// elementQ->cumulative_number += (current_time - prevCurrentTime)*elementQ->totalInSystem;

// elementQ->totalInSystem--;

// elementQ->cumulative_response += (elementQ->first)->service_time + (elementQ->first)->waiting_time;

// if(elementQ->firstWaiting != NULL) {
//     StartService(elementQ);
//   }
// else
//   {
//     elementQ->first = NULL;
//   }

// departure_count++;
}

// Called when a janitor has finished cleaning a room

void JanitorCleanedRoom(struct Queue* elementQ) {

}

// This is the main simulator function
// Runs until 24 hours (1440 minutes)
// Determines what the next event is based on current_time
// Print statistics if current time has passed a full hour

void Simulation(struct Queue* elementQ, double lambda, double mu, int print_period, int total_departures)
{
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

// Free memory allocated for priority queue at the end of simulation

void FreeQueue(struct Queue* elementQ) {
  struct QueueNode* curr;
  while (elementQ->head != NULL) {
    curr = elementQ->head;
    elementQ->head = (elementQ->head)->next;
    free(curr);
  }
  free(elementQ);
}

// Free evaluation queue 

void FreeEvalQueue(struct EvalQueue* elementQ) {
  free(elementQ->nextHighPri);
  free(elementQ->nextLowPri);
  free(elementQ->nextMedPri);
  free(elementQ);
}

// Function to check if input is a number

bool isNumber(char number[]) {
  if(number[0] == 0) {
    return false;
  }
  for(int i = 0; i < strlen(number); i++) {
    if(!isdigit(number[i]) && number[i] != '.') {
      return false;
    }
  }
  return true;
}

// Function to check if input is integer

bool isInteger(char number[]) {
  for(int i = 0; i < strlen(number); i++) {
    if(number[i] == '.') {
      return false;
    }
  }
  return true;
}

// Program's main function

int main(int argc, char* argv[]){


	// input arguments lambda(high pri), lambda(med pri), lambda(low pri), mu(eval), mu(high pri), mu(med pri), mu(low pri), mu(clean), B(max capacity), R(num rooms), 
  // m1(num nurses), m2(num janitors), S(seed)
	if(argc == 14){

		double highPriLambda = atof(argv[1]);
		double medPriLambda = atof(argv[2]);
		double lowPriLambda = atof(argv[3]);
    double evalMu = atof(argv[4]);
    double highPriMu = atof(argv[5]);
    double medPriMu = atof(argv[6]);
    double lowPriMu = atof(argv[7]);
    double cleanMu = atof(argv[8]);
		int maxCapacity = atoi(argv[9]);
    int numRooms = atoi(argv[10]);
    int numNurses = atoi(argv[11]);
    int numJanitors = atoi(argv[12]);
		int random_seed = atoi(argv[13]);

    for(int i = 1; i < 14; i++) {
      if(!isNumber(argv[i])) {
        printf("Provide proper input.\n");
        exit(1);
      }
    }
    for(int i = 9; i < 14; i++) {
      if(!isInteger(argv[i])) {
      printf("Please make sure last three arguments are integers!\n");
      exit(1);
      }
    }

   // Start Simulation
		printf("Simulating Major Hospital Emergency Department with high priority lambda = %f, medium priority lambda = %f, low priority lambda = %f, evaluation mu = %f, high priority mu = %f, medium priority mu - %f, low priority mu = %f, clean mu = %f, max capacity = %d, number of rooms = %d, number of nurses = %d, number of janitors = %d, S = %d\n", highPriLambda, medPriLambda, lowPriLambda, evalMu, highPriMu, medPriMu, lowPriMu, cleanMu, maxCapacity, numRooms, numNurses, numJanitors, random_seed);
		struct EvalQueue* elementQ = InitializeEvalQueue(random_seed, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu);

  //  Simulation(elementQ, lambda, mu, print_period, total_departures);
    FreeEvalQueue(elementQ);
	}
	else printf("Insufficient number of arguments provided!\n");

	return 0;
}
