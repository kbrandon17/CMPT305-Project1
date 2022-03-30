//#ifndef EVENTQUEUE
#include "EventQueue.h"
#define EVENTQUEUE
//#endif
//#ifndef PRIORITYQUEUE
#include "PriorityQueue.h"
#define PRIORITYQUEUE
//#endif
//#ifndef SIMULATION
#include "Simulation.h"
#define SIMULATION
//#endif
#include<stdio.h>
#define STDIO
#include<time.h>
#define TIME
#include<math.h>
#define MATH
#include<stdlib.h>
#define STDLIB
#include<unistd.h>
#define UNISTD
#include<assert.h>
#define ASSERT
#include<float.h>
#define FLOAT
#include<stdbool.h>
#define STDBOOL
#include<string.h>
#define STRING
#include<ctype.h>
#define CTYPE
#include <stddef.h>
#define STDDEF
#include <stdbool.h>
#define STDBOOL



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

    srand(random_seed);

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

    struct EventQueue* eventQ = InitializeEventQueue();
    struct EvalQueue* evalQ = InitializeEvalQueue(eventQ, numNurses, random_seed, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu, evalMu);
    struct Queue* priorityQ = CreatePriorityQueue(numRooms, numJanitors);
    Simulation(random_seed, eventQ, evalQ, priorityQ, numNurses, highPriLambda, highPriMu, medPriLambda, medPriMu, lowPriLambda, lowPriMu, evalMu, cleanMu, numJanitors, numRooms, maxCapacity);
    FreeEvalQueue(evalQ);
	}
	else printf("Insufficient number of arguments provided!\n");

	return 0;
}