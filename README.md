# Emergency Department Simulation
This project involves implementing an event-driven simulation of a system with two dependent queues, modeled after the operations of a major metropolitan hospital's Emergency Department. The simulation will encompass various aspects including patient arrival, evaluation, treatment, room assignment, and cleanup, while tracking important performance metrics.

## System Description
The emergency department operates 24/7 with a total capacity of B patients, comprising R single-occupancy patient rooms and B-R patients in the waiting area. Patients arriving at the department are evaluated based on severity into high, medium, or low priority groups. Inter-arrival times and service times follow exponential distributions.

## Queues and Servers
The system consists of two queues:

1. Evaluation Queue ("E") - M/M/m queue with FCFS discipline, serviced by "m1" nurses.
2. Priority Queue ("P") - Priority queue where patients wait for rooms to be freed up and cleaned, serviced by R rooms.
Janitors are responsible for cleaning rooms, with the number of janitors denoted by "m2".

## Event Routines
Event routines include arrival, evaluation, room assignment, treatment, room cleanup, and departure from the system.

## Simulation Details
* Simulation runs from 12 AM to 12 AM the following day.
* Performance metrics such as current patient count, response time, waiting time in queues, cleanup time, and patients turned away are tracked.
* A report is generated periodically and at the end of the simulation, summarizing the performance metrics.

## Random Variate Generation
Random variates for arrival times and service times are generated based on exponential distributions using C's uniformly-distributed random number generator.

## Project Groups and Submission Instructions
* Project groups consist of 2-3 students.
* Submission involves a tarball "proj1.tar.gz" containing source code, header files, a Makefile, and appropriate comments.
* Executable runs with parameters ./project1 ![image](https://github.com/kbrandon17/CMPT305-Project/assets/87689021/e649ce9b-5173-46f8-a17e-474ee69663c1)

## Acknowledgments
This project is part of CMPT 305 at Simon Fraser University.
