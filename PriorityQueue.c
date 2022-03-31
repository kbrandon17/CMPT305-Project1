#ifndef PRIORITYQUEUE
#include "PriorityQueue.h"
#define PRIORITYQUEUE
#endif
#ifndef QUEUENODE
#include "QueueNode.h"
#define QUEUENODE
#endif
#ifndef EVALQUEUE
#include "EvalQueue.h"
#define EVALQUEUE
#endif
#ifndef EVENTQUEUE
#include "EventQueue.h"
#define EVENTQUEUE
#endif
#ifndef MATH
#include <math.h>
#define MATH
#endif
#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif
#ifndef STDIO
#include <stdio.h>
#define STDIO
#endif
#ifndef STDDEF
#include <stddef.h>
#define STDDEF
#endif
#ifndef SIM
#include "Simulation.h"
#define SIM
#endif

//--------------Janitor Subqueue Stuff--------------------
void InsertJanitorQueue(struct Queue* queue,struct QueueNode* room){
  if (queue->janitorQueueTail != NULL) {
    queue->janitorQueueTail->next = room;
    queue->janitorQueueTail = room;
  } else {
    room->next = NULL;
    queue->janitorQueueHead = room;
    queue->janitorQueueTail = room;
  }
}

struct QueueNode* PopJanitorQueue(struct Queue* queue){
  if (queue->janitorQueueHead == NULL){
    return NULL;
  } else {
    struct QueueNode* room = queue->janitorQueueHead;
    queue->janitorQueueHead = room->next;
    if (queue->janitorQueueHead == NULL) {queue->janitorQueueTail = NULL;}
    room->next = NULL;
    return room;
  }
}


//---------------Priority Queue--------------------------
void InsertPriorityQueue(struct Queue* queue, struct QueueNode* queuenode){
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
  if(pop == queue->tail) { //if queue empty after pop/pop is end of low priority queue
    queue->tail = NULL;
    queue->highTail = NULL;
    queue->mediumTail = NULL;
    queue->head = NULL;
    return pop;
  } else if (pop == queue->mediumTail) { //if pop is last node of medium section
    queue->mediumTail = NULL;
  } else if(pop == queue->highTail) { //if pop is last node of high section
    queue->highTail = NULL;
  }
  //normal tasks for every pops
  queue->head = queue->head->next;
  return pop;
}


struct Queue* CreatePriorityQueue(int available, int janitors){
  struct Queue* queue = malloc(sizeof(struct Queue));
  queue->head=NULL;
  queue->tail=NULL;
  queue->mediumTail=NULL;
  queue->highTail=NULL;
  queue->waiting_count=0;
  queue->cumulative_response=0;
  queue->cumulative_waiting=0;
  queue->cumulative_idle_times=0;
  queue->totalInSystem=0;
  queue->available_rooms = available;
  queue->janitors = janitors;
  queue->janitorQueueHead = NULL;
  queue->janitorQueueTail = NULL;
  return queue;
}

// Called after patient has been helped by nurse and begins waiting in priority queue

void ProcessPriorityArrival(struct EventQueue* eventQ, struct EvalQueue* evalQ, struct Queue* elementQ, struct QueueNode* arrival){
  if((eventQ->head)->event_type == 2) {
    StartEvaluationService(eventQ, evalQ, (eventQ->head)->qnode);
  }
  arrival->priority_arrival_time = current_time;
  evalQ->availableNurses++;
  InsertPriorityQueue(elementQ, arrival);
}

// Function to set the priority queue wait times
void SetPriorityWait(struct QueueNode* patient){
  double waittime = current_time - patient->priority_arrival_time;
  avgPriorityWaitingTimeAll += waittime;
  numSeen++;
  switch(patient->priority){
    case 1:
      avgPriorityWaitingTimeLow += waittime;
      numSeenLow++;
      break;
    case 2:
      avgPriorityWaitingTimeMed += waittime;
      numSeenMed++;
      break;
    case 3:
      avgPriorityWaitingTimeHigh += waittime;
      numSeenHigh++;
      break;
  }

}


// Function to put patient from priority queue into a room

void StartRoomService(struct EventQueue* eventQ, struct Queue* elementQ, double highPriMu, double medPriMu, double lowPriMu)
{
  if(elementQ->available_rooms > 0){
    struct QueueNode* patient = PopPriorityQueue(elementQ);
    if (patient == NULL) {return;}
    SetPriorityWait(patient);
    /*double service_time;
    switch(patient->priority){
      case 1:
         service_time = ((-1/lowPriMu) * log(1-((double) (rand()+1) / RAND_MAX)));
         break;
      case 2:
        service_time = ((-1/medPriMu) * log(1-((double) (rand()+1) / RAND_MAX)));
        break;
      case 3:
        service_time = ((-1/highPriMu) * log(1-((double) (rand()+1) / RAND_MAX)));
        break;
    }
    patient->priority_service_time = service_time;*/
    struct EventQueueNode* event = CreateExitHospitalEventNode(patient);
    InsertIntoEventQueueInOrder(eventQ, event);
   elementQ->available_rooms--;
  }

}

// Function for when a patient is finished in a room and leaves (adds an event to janitor queue)

void ProcessPatientDeparture(struct EventQueue* eventQ, struct Queue* elementQ, struct QueueNode* room, double cleanMu){
   current_time = room->priority_arrival_time + room->priority_service_time;
   room->priority_departure_time = current_time;
   room->time_to_clean_room = ((-1/cleanMu) * log(1-((double) (rand()+1) / RAND_MAX)));
  if(elementQ->janitors > 0){
    elementQ->janitors--;
    struct EventQueueNode* clean_event = CreateJanitorCleanedRoomEventNode(room);
    InsertIntoEventQueueInOrder(eventQ, clean_event);
  } else {
    room->next = NULL;
    InsertJanitorQueue(elementQ, room);
  }
  AddAvgInSystem(prevCurrentTime);
  prevCurrentTime = current_time;
  totalNumberInSystemNow--;
  avgResponseTimeAll += current_time - room->eval_arrival_time;
  switch(room->priority){
    case 1: 
      avgResponseTimeLow += current_time - room->eval_arrival_time;
      departure_count_low++;
      break;
    case 2:
      avgResponseTimeMed += current_time - room->eval_arrival_time;
      departure_count_med++;
      break;
    case 3:
      avgResponseTimeHigh += current_time - room->eval_arrival_time;
      departure_count_high++;
      break;
}
  departure_count++;
}

// Called when a janitor has finished cleaning a room

void JanitorCleanedRoom(struct EventQueue* eventQ, struct Queue* elementQ, struct EventQueueNode* event) {
  current_time = event->event_time;
  elementQ->available_rooms++;
  elementQ->janitors++;
  avgCleanUpTime += current_time - event->qnode->priority_departure_time;
  numCleanedRooms++;
//  free(event->qnode);
  if (elementQ->janitorQueueHead != NULL) {
    elementQ->janitors--;
    struct EventQueueNode* clean_event = CreateJanitorCleanedRoomEventNode(PopJanitorQueue(elementQ));
    InsertIntoEventQueueInOrder(eventQ, clean_event);
  }
  

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