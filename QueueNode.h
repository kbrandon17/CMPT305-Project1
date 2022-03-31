// Definition of a Queue Node including arrival and service time

struct QueueNode {
    double eval_arrival_time;     // customer evaluation arrival time
    double eval_service_time;     // customer evaluation service time (exponential)
    double eval_waiting_time;     // customer time spent waiting in eval queue
    double priority_arrival_time; // customer priority arrival time
    double priority_service_time; // customer priority service time
    double priority_waiting_time; // customer priority waiting time
    double priority_departure_time; //time customer departs/room ready for cleaning
    double time_to_clean_room;  // time it takes the janitor to clean their room
    int priority;                 // 1 - low, 2 - med, 3 - high
    struct QueueNode *next;       // next element in line; NULL if this is the last element
};

struct QueueNode* CreateNode(double Narrival_time, double Nservice_time, double Neval_time);