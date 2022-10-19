This is a simple scheduler in c with three possible scheuling algorithms implemented. 

compiles with: gcc  -std=c99 -Wall -o scheduling scheduling.c
to run ./scheduler <algorithm(0, 1 or 2)> <path to input file>
  
sample input file text:
3 <- first number is the number of processes, each line is a process
0 4 2 0 <- first number: PID, second: cpu time, third: IO time, fourth: arrival time. 
1 6 1 2 
2 8 4 3

this input will produce this output: with algorithm #2 (shortest time remaining)

0 0: running
1 0: running
2 0: blocked 1: running
3 0: blocked 1: running 2: ready
4 0: running 1: ready 2: ready
5 0: running 1: ready 2: ready
6 1: running 2: ready
7 1: blocked 2: running
8 1: running 2: ready
9 1: running 2: ready
10 1: running 2: ready
11 2: running
12 2: running
13 2: running
14 2: blocked
15 2: blocked
16 2: blocked
17 2: blocked
18 2: running
19 2: running
20 2: running
21 2: running

Finishing time: 21
CPU Utilization: 0.818182
Turnaround process 0: 6
Turnaround process 1: 9
Turnaround process 2: 19
  
  
