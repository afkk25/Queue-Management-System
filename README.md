# Queue-Management-System
This C program implements a seat reservation system with two queues: a main queue (CSC4352_queue) and a waiting list using a linked list (front_LL, rear_LL). The logic handles queue management, reservations, cancellations, and checking both the main queue and waiting list.

# Key Features:
## 1. Main Queue and Waiting List:

The main queue has a fixed size (CSC4352_QUEUE_SIZE), and the waiting list uses dynamic memory allocation via a linked list. If the main queue is full, students are added to the waiting list.
## 2. Enqueue and Dequeue Operations:

enqueue: Adds a student to the main queue.
enqueue_LL: Adds a student to the linked list (waiting queue).
dequeue_LL: Removes a student from the waiting list.
cancel_registration: Cancels a student's registration from either the main queue or the waiting list.
## 3. Menu Operations:

Reserve a seat: Adds a student to the main queue if there's space; otherwise, adds them to the waiting list.
Cancel reservation: Removes a student by ID, shifting any waiting list students to the main queue.
Check queue content: Displays the current contents of either the main queue or the waiting list.
Pre-registration is over: Generates a text file of registered students and empties the queues.
## 4. File Generation:

generate_file writes the students in the main queue to a file and clears the queue.
The waiting list is also destroyed after file generation with destroy_waitlist.
