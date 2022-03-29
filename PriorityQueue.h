
// Priority Queue to track patients after evaluation

struct Queue {

    struct QueueNode* head;         // Point to the first node of the element queue
    struct QueueNode* tail;         // Point to the tail node of the element queue
    struct QueueNode* mediumTail;   // Head node of medium priority section of list
    struct QueueNode* highTail;      // Head node of high priority section of list
    int waiting_count;              // Current number of customers waiting for service
    double cumulative_response;     // Accumulated response time for all effective departures
    double cumulative_waiting;      // Accumulated waiting time for all effective departures
    double cumulative_idle_times;   // Accumulated times when the system is idle, i.e., no customers in the system
    double cumulative_serving;       // Accumulated number of customers in the system
    int totalInSystem;
};

void InsertPriorityQueue(struct Queue* queue, struct QueueNode* queuenode);
struct QueueNode* PopPriorityQueue(struct Queue* queue);
struct Queue* CreatePriorityQueue();
void ProcessPriorityArrival(struct EvalQueue* evalQ, struct Queue* elementQ, struct QueueNode* arrival);
void StartRoomService(struct Queue* elementQ);
void ProcessPatientDeparture(struct Queue* elementQ);
void JanitorCleanedRoom(struct Queue* elementQ);
void FreeQueue(struct Queue* elementQ);

