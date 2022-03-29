// Function to insert events into event queue in order

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
  newNode->event_time = q->priority_arrival_time + q->priority_service_time + q->priority_waiting_time;
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