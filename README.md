This is a simple scheduler in c with three possible scheuling algorithms implemented. <br />

compiles with: gcc  -std=c99 -Wall -o scheduling scheduling.c <br />
to run ./scheduler <algorithm(0, 1 or 2)> <path to input file> <br />
  
sample input file text:<br />
3 <- first number is the number of processes, each line is a process<br />
0 4 2 0 <- first number: PID, second: cpu time, third: IO time, fourth: arrival time. <br />
1 6 1 2 <br />
2 8 4 3<br />

this input will produce this output: with algorithm #2 (shortest time remaining)<br />

0 0: running<br />
1 0: running<br />
2 0: blocked 1: running<br />
3 0: blocked 1: running 2: ready<br />
4 0: running 1: ready 2: ready<br />
5 0: running 1: ready 2: ready<br />
6 1: running 2: ready<br />
7 1: blocked 2: running<br />
8 1: running 2: ready<br />
9 1: running 2: ready<br />
10 1: running 2: ready<br />
11 2: running<br />
12 2: running<br />
13 2: running<br />
14 2: blocked<br />
15 2: blocked<br />
16 2: blocked<br />
17 2: blocked<br />
18 2: running<br />
19 2: running<br />
20 2: running<br />
21 2: running<br />

Finishing time: 21<br />
CPU Utilization: 0.818182<br />
Turnaround process 0: 6<br />
Turnaround process 1: 9<br />
Turnaround process 2: 19<br />
  
The output is written to a new file in the local directory
  
  
