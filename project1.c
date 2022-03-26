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

// Suggested queue definition
// Feel free to use some of the functions you implemented in HW2 to manipulate the queue
// You can change the queue definition and add/remove member variables to suit your implementation
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
// Feel free to add or remove
static double computed_stats[4];  // Store computed statistics: [E(n), E(r), E(w), p0]
static double simulated_stats[4]; // Store simulated statistics [n, r, w, sim_p0]
int departure_count = 0;         // current number of departures from queue
double current_time = 0.0;          // current time during simulation
double prevCurrentTime = 0.0;

//-----------------Queue Functions------------------------------------
// Feel free to add more functions or redefine the following functions

// The following function initializes all "D" (i.e., total_departure) elements in the queue
// 1. It uses the seed value to initialize random number generator
// 2. Generates "D" exponentially distributed inter-arrival times based on lambda
//    And inserts "D" elements in queue with the correct arrival times
//    Arrival time for element i is computed based on the arrival time of element i+1 added to element i's generated inter-arrival time
//    Arrival time for first element is just that element's generated inter-arrival time
// 3. Generates "D" exponentially distributed service times based on mu
//    And updates each queue element's service time in order based on generated service times
// 4. Returns a pointer to the generated queue

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

struct Queue* InitializeQueue(int seed, double lambda, double mu, int total_departures){
  struct Queue* newQueue = malloc(sizeof *newQueue);

  srand(seed);
  for (int i = 0; i<total_departures; i++) {
    double randomArr = ((-1/lambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double randomSer = ((-1/mu) * log(1-((double) (rand()+1) / RAND_MAX)));
    Insert(newQueue, randomArr, randomSer);
  }

   newQueue->cumulative_number = 0.0;
   newQueue->cumulative_waiting = 0.0;
   newQueue->cumulative_response = 0.0;
   newQueue->cumulative_idle_times = 0.0;
   newQueue->waiting_count = 0;
   newQueue->first = NULL;
   newQueue->last = NULL;
   newQueue->firstWaiting = NULL;
   newQueue->totalInSystem = 0;
  return newQueue;
}


// Use the M/M/1 formulas from class to compute E(n), E(r), E(w), p0
void GenerateComputedStatistics(double lambda, double mu){

double trafficInt = lambda/mu;

computed_stats[0] = trafficInt/(1 - trafficInt);
computed_stats[1] = 1/(mu*(1-trafficInt));
computed_stats[2] = trafficInt/(mu*(1-trafficInt));
computed_stats[3] = 1 - trafficInt;

}

// This function should be called to print periodic and/or end-of-simulation statistics
// Do not modify output format
void PrintStatistics(struct Queue* elementQ, int total_departures, int print_period, double lambda){

  simulated_stats[0] = elementQ->cumulative_number/current_time;
  simulated_stats[1] = elementQ->cumulative_response/departure_count;
  simulated_stats[2] = elementQ->cumulative_waiting/departure_count;
  simulated_stats[3] = (elementQ->cumulative_idle_times)/current_time;

  printf("\n");
  if(departure_count == total_departures) {
    printf("End of Simulation - after %d departures\n", departure_count);
  }
  else printf("After %d departures\n", departure_count);

  printf("Mean n = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[0], computed_stats[0]);
  printf("Mean r = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[1], computed_stats[1]);
  printf("Mean w = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[2], computed_stats[2]);
  printf("p0 = %.4f (Simulated) and %.4f (Computed)\n", simulated_stats[3], computed_stats[3]);

}



// This function is called from simulator if next event is "start_service"
//  Should update queue statistics
void StartService(struct Queue* elementQ)
{
  (elementQ->firstWaiting)->waiting_time = current_time-((elementQ->firstWaiting)->arrival_time);
  elementQ->cumulative_waiting += (elementQ->firstWaiting)->waiting_time;
  elementQ->first = elementQ->firstWaiting;

  if(((elementQ->firstWaiting)->next != NULL) && 
      ((elementQ->firstWaiting)->next)->arrival_time < current_time) {
      elementQ->firstWaiting = (elementQ->firstWaiting)->next;
  }
  else {
    elementQ->firstWaiting = NULL;
  }
  elementQ->waiting_count--;

  // printf( "FIRST: %f\n", ((elementQ->first)->arrival_time));
  // printf( "NEXT ARRIVING: %f\n", (((elementQ->last)->next)->arrival_time));

}

// This function is called from simulator if the next event is an arrival
// Should update simulated statistics based on new arrival
// Should update current queue nodes and various queue member variables
// *arrival points to queue node that arrived
// Returns pointer to node that will arrive next
void ProcessArrival(struct Queue* elementQ, struct QueueNode* arrival){

prevCurrentTime = current_time;
current_time = arrival->arrival_time;

//printf("node about to arrive: %d\n", elementQ->totalInSystem);
elementQ->cumulative_number += (current_time - prevCurrentTime)*elementQ->totalInSystem;
elementQ->totalInSystem++;



if(elementQ->first == NULL)
{
  elementQ->cumulative_idle_times += current_time-prevCurrentTime;
  elementQ->firstWaiting = arrival;
  elementQ->last = arrival;
  StartService(elementQ);
}

else if(elementQ->firstWaiting == NULL)
{
  elementQ->firstWaiting = arrival;
  elementQ->last = arrival;
  elementQ->waiting_count++;
}
else {
  elementQ->last = arrival;
  elementQ->waiting_count++;
}



}

// This function is called from simulator if the next event is a departure
// Should update simulated queue statistics
// Should update current queue nodes and various queue member variables
void ProcessDeparture(struct Queue* elementQ){

prevCurrentTime = current_time;
current_time = (elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time;

//printf("node about to depart: %d\n", elementQ->totalInSystem);
elementQ->cumulative_number += (current_time - prevCurrentTime)*elementQ->totalInSystem;

elementQ->totalInSystem--;

elementQ->cumulative_response += (elementQ->first)->service_time + (elementQ->first)->waiting_time;

if(elementQ->firstWaiting != NULL) {
    StartService(elementQ);
  }
else
  {
    elementQ->first = NULL;
  }

departure_count++;
}

// This is the main simulator function
// Should run until departure_count == total_departures
// Determines what the next event is based on current_time
// Calls appropriate function based on next event: ProcessArrival(), StartService(), ProcessDeparture()
// Advances current_time to next event
// Updates queue statistics if needed
// Print statistics if departure_count is a multiple of print_period
// Print statistics at end of simulation (departure_count == total_departure)
void Simulation(struct Queue* elementQ, double lambda, double mu, int print_period, int total_departures)
{
  while(departure_count != total_departures)
  {
      if((elementQ->first != NULL) && (elementQ->last)->next != NULL
      && (((elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time)
            == (((elementQ->last)->next)->arrival_time))) {
              ProcessDeparture(elementQ);
              if((elementQ->last)->next != NULL) {
                ProcessArrival(elementQ, (elementQ->last)->next);
              }
                continue;

      }



      if(elementQ->first != NULL
        && ((elementQ->last)->next == NULL || (((elementQ->first)->arrival_time + (elementQ->first)->service_time + (elementQ->first)->waiting_time)
          <= (((elementQ->last)->next)->arrival_time))))
      {

        ProcessDeparture(elementQ);
        if(departure_count%print_period == 0)
        {
            PrintStatistics(elementQ, total_departures, print_period, lambda);
        }
      }
      else
      {
        if(departure_count == 0 && (elementQ->first == NULL))
        {
          //printf("TEST");
          ProcessArrival(elementQ, elementQ->head);
        }
        else
        {

          ProcessArrival(elementQ, (elementQ->last)->next);
        }
      }
  }
            PrintStatistics(elementQ, total_departures, print_period, lambda);
}
// Free memory allocated for queue at the end of simulation
void FreeQueue(struct Queue* elementQ) {
  struct QueueNode* curr;
  while (elementQ->head != NULL) {
    curr = elementQ->head;
    elementQ->head = (elementQ->head)->next;
    free(curr);
  }
  free(elementQ);
}

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
	if(argc == 13){

		double highPriLambda = atof(argv[1]);
		double medPriLambda = atof(argv[2]);
		double lowPriLambda = atoi(argv[3]);
    double evalMu = atoi(argv[4]);
    double highPriMu = atoi(argv[5]);
    double medPriMu = atoi(argv[6]);
    double lowPriMu = atoi(argv[7]);
    double cleanMu = atoi(argv[8]);
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
	//	struct Queue* elementQ = InitializeQueue(random_seed, lambda, mu, total_departures);

  //  Simulation(elementQ, lambda, mu, print_period, total_departures);
  //  FreeQueue(elementQ);
	}
	else printf("Insufficient number of arguments provided!\n");

	return 0;
}
