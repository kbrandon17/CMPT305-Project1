#include "PriorityQueue.h"

//---------------Priority Queue--------------------------
void InsertPriorityQueue(struct Queue* queue, struct QueueNode* queuenode){
  queue->cumulative_number++;
  queue->cumulative_waiting++;
  queuenode->next = NULL;
  switch(queuenode->priority){
    case 1: //if low priority
      if(queue->head == NULL){ //if queue empty
        queue->head = queuenode;
        queue->tail = queuenode;
      }
      else { //if queue has nodes
        queue->tail->next = queuenode;
        queue->tail = queuenode;
      }
    case 2: //if medium priority
      if(queue->head == NULL){ //if queue empty
        queue->head = queuenode;
        queue->tail = queuenode;
        queue->mediumTail = queuenode;
      } else if (queue->mediumTail == NULL){//if medium section of queue is empty
        queue->mediumTail = queuenode;
        if (queue->highTail != NULL){ //if high priority section has members
          queuenode->next = queue->highTail->next;
          queue->highTail->next = queuenode;
        } else { //else if only low priority section has members
          queuenode->next = queue->head;
          queue->head = queuenode;
        }
      } else { //else if nodes exist in medium section of queue
        queuenode->next = queue->mediumTail->next;
        queue->mediumTail->next = queuenode;
        queue->mediumTail = queuenode;
      }


    case 3: //if high priority
      if(queue->head == NULL){ //if queue empty
        queue->head = queuenode;
        queue->tail = queuenode;
        queue->highTail = queuenode;
      } else if (queue->highTail == NULL) { //if high priority section empty
        queue->highTail = queuenode;
        queuenode->next = queue->head;
        queue->head = queuenode;
      } else { // if high priority section has nodes
        queuenode->next = queue->highTail->next;
        queue->highTail->next = queuenode;
        queue->highTail = queuenode;
      }
  }

}

struct QueueNode* PopPriorityQueue(struct Queue* queue){
  struct QueueNode* pop = queue->head;
  queue->cumulative_number--;
  if(pop == queue->tail) { //if queue empty after pop/pop is end of low priority queue
    queue->tail = NULL;
    queue->highTail = NULL;
    queue->mediumTail = NULL;
  } else if (pop == queue->mediumTail) { //if pop is last node of medium section
    queue->mediumTail = NULL;
  } else if(pop == queue->highTail) { //if pop is last node of high section
    queue->highTail = NULL;
  }
  //normal tasks for every pop
  queue->head = queue->head->next;
  return pop;
}


struct Queue* CreatePriorityQueue(){
  struct Queue* queue = malloc(sizeof(struct Queue));
  queue->head=NULL;
  queue->tail=NULL;
  queue->mediumHead=NULL;
  queue->lowhead=NULL;
  queue->waiting_count=0;
  queue->cumulative_response=0;
  queue->cumulative_waiting=0;
  queue->cumulative_idle_times=0;
  queue->cumulative_number=0;
  queue->totalInSystem=0;
  return queue;
}

// Called after patient has been helped by nurse and begins waiting in priority queue

void ProcessPriorityArrival(struct EvalQueue* evalQ, struct Queue* elementQ, struct QueueNode* arrival){

evalQ->availableNurses++;
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
  elementQ->totalInSystem++;
}

// Function to put patient from priority queue into a room

void StartRoomService(struct Queue* elementQ)
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


//TODO: SET JANITOR CLEAN TIME BEFORE CREATING JANITOR EVENT
elementQ->totalInSystem--;
departure_count++;
}

// Called when a janitor has finished cleaning a room

void JanitorCleanedRoom(struct Queue* elementQ) {

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