#ifndef QUEUENODE
#include "QueueNode.h"
#define QUEUENODE
#endif

// Event queue to execute simulation in order

struct EventQueue {
  struct EventQueueNode* head;       // next event to occur that is queied 
  struct EventQueueNode* tail;       // last event to occur that is queued
};

// Nodes to be used in event queue tracking when the event will occur and what will happen

struct EventQueueNode {
  double event_time;                // time that the event will occur
  int event_type;                   // 1: eval queue arrival 2: eval service start 3: priority queue arrival 4: priority queue service start 5: patient leaves hospital 6: room cleaned
  struct QueueNode* qnode;
  struct EventQueueNode *next;
};

struct EventQueue* InitializeEventQueue();
void InsertIntoEventQueueInOrder(struct EventQueue* q, struct EventQueueNode* n);
struct EventQueueNode* CreateEvalArrivalEventNode(struct QueueNode* q);
struct EventQueueNode* CreateEvalServiceEventNode(struct QueueNode* q);
struct EventQueueNode* CreatePriorityArrivalEventNode(struct QueueNode* q);
struct EventQueueNode* CreatePriorityStartServiceEventNode(struct QueueNode* q);
struct EventQueueNode* CreateExitHospitalEventNode(struct QueueNode* q);
struct EventQueueNode* CreateJanitorCleanedRoomEventNode(struct QueueNode* q);
void DeleteEventNode (struct EventQueue *);
void DeleteServiceNode (struct EventQueue *);
