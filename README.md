---
Author: Hamid Khan
---

# Task 1: eBPF Program for process Monitoring

## Steps to Execute Task 1

1. **Install Required Dependencies**:
   Make sure that you have BCC installed on your system. You can install it by running the following command:
   ```bash
   sudo apt-get install bpfcc-tools linux-headers-$(uname -r)
   sudo apt-get install python3-bpfcc

2. **Clone the Repo**:
    Clone this repository using the following command:
    ```bash
    git clone <https://github.com/Hamid-Khichi/Wanclouds_assessment_task>
    cd </home/hamid/Wanclouds_Assignment>

3. **Run the C Program that is to be monitored**:
    Replace the <your_name> with any name in the command-line
    ```bash
    ./my_app <your_name>

4. **Run the eBPF Program**:
    In another terminal, start the eBPF Program via the following (make sure to replace <your_name> with the name you wrote in the command-line for the C Program):
    ```bash
    sudo python3 monitor_syscalls.py <my_name>

5. **Monitor the Output**:
    eBPF Program will log system calls made by that C Program, but make sure that you run the eBPF program after the C program, Press CTRL+C to stop monitoring.

## Libraries/Resources Used
1. **BCC** (BPF Compiler Collection): For writing and running the eBPF program
2. **psutil**: To iterate over system processes and getting the command-line arguments
3. **ctypes**: For handling BPF structures in Python
4. **time** and **signal**: For managing delays and signal handling in Python

## Challenges Encountered
* **Iovisor BCC Installation Issue**:
Installing the *iovisor* BCC package was a challenge as I encountered multiple iovisor list issues, so I had to use alternative methods and install BCC through github
* **Linux Distribution Issue**:
My Linux distribution *Virginia* didn't support the latest iovisor list so I had to ensure compatibility with my linux distribution
* **Filtering Specific Process**:
I needed to filter processes containing the name in the command-line argument and to exclude the python script itself, as the python script would contain the name in the command-line as well, I had to ensure that the script would not recall syscalls of itself which was resolved using psutil
* **Managing System Call Output**:
There was continuous monitoring of system calls produced that overwhelmed the output and it became harder to read so I had to add delays to make the log more readable

## Additional Features/Improvements
* **Graceful Termination**:
Added graceful termination by CTRL+C
* **Real-time Process Monitoring**:
The Program detects PIDs in real time

# Task 2: Doubly Linked List in C

## Steps to Execute Task 2

1. **Clone the Repo**:
    Clone this repository using the following command:
    ```bash
    git clone <https://github.com/Hamid-Khichi/Wanclouds_assessment_task>
    cd </home/hamid/Wanclouds_Assignment>

2. **Compile the C program**:
    Use the following command to compile the C program:
    ```bash
    gcc doubly_linked_list.c -o linked_list

3. **Run the Program**:
    Execute the Program via:
    ```bash
    ./doubly_linked_list

4. **Program Interaction**:
    Use Menu to insert nodes, delete nodes by ID, printing the list, sorting the list by ID

5. **Exiting the Program**:
    You can exit the program by selecting the exit option from the menu

## Libraries/Resources Used
* **Standard C Libraries**:
    * **stdio.h**: Used for I/O operations like *printf* and *scanf*
    * **stdlib.h**: For functions like *malloc*, *free* and *exit* that are used for memory allocation
    * **string.h**: For string operations such as *strdup* and *strcpy*

## Challenges Encountered

1. **Memory Management**:
Managing memory is essential to avoid leaks when we have to allocate memory for new nodes and freeing up when deleting

2. **Implementing Merge Sort**:
Implementing merge sort for a doubly linked list was quite difficult when also managing previous and next links during the sorting process

3. **Pointer Management**:
I had to properly manage the *prev* and *next* pointers when splitting and merging lists and it was quite challenging

## Additional Features/Improvements

* **Merge Sort for Doubly Linked List**:
Implemented merge sort to allow efficient sorting of the list by ID, it made insertion and deletion easier on a sorted list

* **Dynamic Node Insertion**:
Nodes can be added dynamically, with every node holding a name which is input by the user and a unique ID