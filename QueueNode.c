#ifndef QUEUENODE
#include "QueueNode.h"
#define QUEUENODE
#endif
#ifndef STDIO
#include <stdio.h>
#define STDIO
#endif
#ifndef STDLIB
#include <stdlib.h>
#define STDLIB
#endif
#ifndef STDDEF
#include <stddef.h>
#define STDDEF
#endif

struct QueueNode* CreateNode(double Narrival_time, double Nservice_time, double Neval_time) {

  struct QueueNode* newNode = malloc(sizeof(struct QueueNode));
  newNode->eval_arrival_time = Narrival_time;
  newNode->eval_service_time = Nservice_time;
  newNode->eval_waiting_time = Neval_time;
  newNode->priority_arrival_time = 0.0;
  newNode->priority_service_time = 0.0;
  newNode->priority_waiting_time = 0.0;
  newNode->time_to_clean_room = 0.0;
  newNode->next = NULL;
  return newNode;
}