#include "EventQueue.h"
#include<stdio.h>
#include "Simulation.h"
#include "QueueNode.h"

// Function to insert events into event queue in order

struct EventQueue* InitializeEventQueue() {
  struct EventQueue* newQueue = malloc(sizeof *newQueue);
  newQueue->head = NULL;
  newQueue->tail = NULL;

  return newQueue;
}

void InsertIntoEventQueueInOrder(struct EventQueue* q, struct EventQueueNode* n) {
if((q->head)->next != NULL) {
  struct EventQueueNode* curr = (q->head)->next;
  struct EventQueueNode* prev = q->head;
  while (curr != NULL) {
    if(n->event_time >= prev->event_time && n->event_time <= curr->event_time) {
        prev->next = n;
        n->next = curr;
        return;
      }
    curr = curr->next;
    prev = prev->next;
    }
  prev->next = n;
  n->next = NULL;
  q->tail = n;
  return;
}
else {
    struct EventQueueNode* curr = q->head;
    if(n->event_time >= curr->event_time) {
      curr->next = n;
      n->next = NULL;
      q->tail = n;
      return;
    }
    else {
      n->next = curr;
      curr->next = n;
      return;
    }
  }
}

void DeleteServiceNode (struct EventQueue *q) {
  if((q->head)->event_type == 2) {
    struct EventQueueNode* newHead = (q->head)->next;
    free(q->head);
    q->head = newHead;
  }
  return;
}

void DeleteEventNode (struct EventQueue *q) {

if(q->head == NULL) {
  return;
}
if(q->head == q->tail) {
  free(q->head);
  q->head = NULL;
  q->tail = NULL;
  return;
}
if((q->head)->event_type == 2) {
struct EventQueueNode* curr = q->head;
struct EventQueueNode* after = (q->head)->next;
while(after->event_type == 2) {
  curr=curr->next;
  after=after->next;
}
curr->next = after->next;
free(after);
return;
}
struct EventQueueNode* newHead = (q->head)->next;
free(q->head);
q->head = newHead;
}

struct EventQueueNode* CreateEvalArrivalEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = q->eval_arrival_time;
  newNode->event_type = 1;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}


struct EventQueueNode* CreateEvalServiceEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = current_time;
  newNode->event_type = 2;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}


struct EventQueueNode* CreatePriorityArrivalEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = q->eval_arrival_time + q->eval_service_time + q->eval_waiting_time;
  newNode->event_type = 3;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}


struct EventQueueNode* CreatePriorityStartServiceEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = current_time;
  newNode->event_type = 4;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}


struct EventQueueNode* CreateExitHospitalEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = current_time + q->priority_service_time;
  newNode->event_type = 5;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}


struct EventQueueNode* CreateJanitorCleanedRoomEventNode(struct QueueNode* q) {

  struct EventQueueNode* newNode = malloc(sizeof(struct EventQueueNode));
  newNode->event_time = current_time + q->time_to_clean_room;
  newNode->event_type = 6;
  newNode->qnode = q;
  newNode->next = NULL;
  return newNode;
}