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
    double arrival_time;  // customer arrival time, measured from time t=0, inter-arrival times exponential
    double service_time;  // customer service time (exponential)
    double waiting_time;  // customer time spent waiting in queue
    struct QueueNode *next;  // next element in line; NULL if this is the last element
};

// Evaluation queue to track next arrival

struct EvalQueue {

    struct QueueNode* nextHighPri;    // next high priority to arrive
    struct QueueNode* nextMedPri;    // next medium priority to arrive
    struct QueueNode* nextLowPri;  // next low priority node to arrive

};

// Priority Queue to track patients after evaluation

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

static double simulated_stats[4];   // Store simulated statistics [n, r, w, sim_p0]
double current_time = 0.0;          // current time during simulation (minutes past 12AM)
int departure_count;                // total departures of patients leaving hospital
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
    newQueue->nextMedPri = CreateNode(medPriArr, medPriSer);
    newQueue->nextLowPri = CreateNode(lowPriArr, lowPriSer);

  return newQueue;
}


// Printing out the report of statistics at every hour

void PrintStatistics(struct Queue* elementQ, int total_departures, int print_period, double lambda){

  // simulated_stats[0] = elementQ->cumulative_number/current_time;
  // simulated_stats[1] = elementQ->cumulative_response/departure_count;
  // simulated_stats[2] = elementQ->cumulative_waiting/departure_count;
  // simulated_stats[3] = (elementQ->cumulative_idle_times)/current_time;

  // printf("\n");
  // if(departure_count == total_departures) {
  //   printf("End of Simulation - after %d departures\n", departure_count);
  // }
  // else printf("After %d departures\n", departure_count);

  // printf("Mean n = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[0], computed_stats[0]);
  // printf("Mean r = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[1], computed_stats[1]);
  // printf("Mean w = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[2], computed_stats[2]);
  // printf("p0 = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[3], computed_stats[3]);

}

// Function to process the arrival of a patient to the hospital.

void ProcessEvalArrival(struct EvalQueue* elementQ, struct QueueNode* arrival){

prevCurrentTime = current_time;
current_time = arrival->arrival_time;

//next to arrive is high priority
if(arrival = elementQ->nextHighPri)
{
  current_time = (elementQ->nextHighPri)->arrival_time;
}
//next to arrive is medium priority
else if(arrival = elementQ->nextMedPri)
{
  current_time = (elementQ->nextMedPri)->arrival_time;
}
//next to arrive is low priority
else if(arrival = elementQ->nextLowPri)
{
  current_time = (elementQ->nextLowPri)->arrival_time;
}


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
