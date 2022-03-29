// Initializes the evaluation queue, setting the first arrival of each of the three priorities

struct EvalQueue* InitializeEvalQueue(int numNurses, int seed, double highprilambda, double highprimu, double medprilambda, double medprimu, double lowprilambda, double lowprimu, double evalmu){
  struct EvalQueue* newQueue = malloc(sizeof(struct EvalQueue));

  srand(seed);

    double highPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double highPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    double medPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double medPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    double lowPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double lowPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    double evalSer = ((-1/evalmu) * log(1-((double) (rand()+1) / RAND_MAX)));

    newQueue->nextHighPri = CreateNode(highPriArr, highPriSer, evalSer);
    (newQueue->nextHighPri)->priority = 3;

    newQueue->nextMedPri = CreateNode(medPriArr, medPriSer, evalSer);
    (newQueue->nextMedPri)->priority = 2;

    newQueue->nextLowPri = CreateNode(lowPriArr, lowPriSer, evalSer);
    (newQueue->nextLowPri)->priority = 1;

    newQueue->availableNurses = numNurses;
    newQueue->cumulative_waiting = 0.0;
    newQueue->totalInSystem = 0;

  return newQueue;
}

// Function to process the arrival of a patient to the hospital.

void ProcessEvalArrival(struct EvalQueue* evalQ, struct QueueNode* arrival, int seed, double highprilambda, double highprimu, double medprilambda, double medprimu, double lowprilambda, double lowprimu, double evalmu){

prevCurrentTime = current_time;
current_time = arrival->eval_arrival_time;

    srand(seed);
    double evalSer = ((-1/evalmu) * log(1-((double) (rand()+1) / RAND_MAX)));
if(arrival->priority == 3) {

    double highPriArr = ((-1/highprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double highPriSer = ((-1/highprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    evalQ->nextHighPri = CreateNode(highPriArr, highPriSer, evalSer);
}

else if(arrival->priority == 2) {

    double medPriArr = ((-1/medprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double medPriSer = ((-1/medprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    evalQ->nextMedPri = CreateNode(medPriArr, medPriSer, evalSer);
}

else if(arrival->priority == 1) {

    double lowPriArr = ((-1/lowprilambda) * log(1-((double) (rand()+1) / RAND_MAX)));
    double lowPriSer = ((-1/lowprimu) * log(1-((double) (rand()+1) / RAND_MAX)));
    evalQ->nextLowPri = CreateNode(lowPriArr, lowPriSer, evalSer);
}

evalQ->totalInSystem++;

if(evalQ->availableNurses > 0) {
  StartEvaluationService(evalQ, arrival);
}
else {
  evalQ->waiting_count++;
}


}

// Function to start nurse evaluation

void StartEvaluationService(struct EvalQueue* evalQ, struct QueueNode* servNode)
{
  evalQ->availableNurses--;
  (servNode)->eval_waiting_time = current_time-(servNode->eval_arrival_time);
   evalQ->cumulative_waiting += (servNode)->eval_waiting_time;



  // elementQ->waiting_count--;

  evalQ->totalInSystem--;
}

// Free evaluation queue 

void FreeEvalQueue(struct EvalQueue* elementQ) {
  free(elementQ->nextHighPri);
  free(elementQ->nextLowPri);
  free(elementQ->nextMedPri);
  free(elementQ);
}