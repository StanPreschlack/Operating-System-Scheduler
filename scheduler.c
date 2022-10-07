// must compile with: gcc  -std=c99 -Wall -o scheduling scheduling.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// struct to represent each process
typedef struct process
{
    int id;
    int cpuTime;
    // to know when to io
    int totalCpuTime;
    int IOTime;
    int arrivalTime;
    // state key
    //  0 -> new
    //  1 -> ready
    //  2 -> running
    //  3 -> blocked
    //  4 -> done
    int state;
    int sortingValue;
    int cycleStarted;
    int cycleFinished;
    // keep track for RR
    int runningCycles;
} process;

// helper functions
int areProcessesRemaining();
void printStats(FILE *filePtr, int cycles);
void printState(FILE *filePtr, int cycle);
int areProcessesRunning();
int areProcessesWaiting();
void runningToReady();
int allButOneAreDone();
int isLowestRemaining(process proc);

void CPU(FILE *writeFilePtr, void(f_case_zero)(int, int), void(f_case_one(int)), void(f_case_two(int, int)), void(f_case_three(int, int)), void(final_f()));

// first come
void firstComeCaseZero(int a, int clock);
void firstComeCaseOne(int a);
void firstComeCaseTwo(int a, int clock);
void firstComeCaseThree(int a, int clock);
void firstComeFinalF();
void firstCome(FILE *writeFilePtr);

// round robin
void roundRobinCaseZero(int a, int clock);
void roundRobinCaseOne(int a);
void roundRobinCaseTwo(int a, int clock);
void firstComeCaseThree(int a, int clock);
void firstComeFinalF();
void roundRobin(FILE *writeFilePtr);

// shortest remaining
void shortestRemainingCaseZero(int a, int clock);
void shortestRemainingCaseOne(int a);
void shortestRemainingCaseTwo(int a, int clock);
void shortestRemainingCaseThree(int a, int garbage);
void shortestRemainingFinalF(int a);
void shortestRemainingFinalF(int a);
void shortestRemaining(FILE *writeFilePtr);

// main() helper functions
void addProcessFromString(char str[], int i);
void chooseAlgorithm(int scheduling, char fileName[]);
void invalidInputError();
void cannotOpenFileError(char fileName[]);

// static master array (can handle 10 processes)
process masterProcessArray[10];

// mutable array (same at first but changed)
process processArray[10];

int numberOfProcesses = 0;

int areProcessesRemaining()
{
    for (int a = 0; a < numberOfProcesses; a++)
    {
        if (processArray[a].state != 4)
        {
            return 1;
        }
    }
    return 0;
}

void printStats(FILE *filePtr, int cycles)
{
    int workingCycles = 0;
    for (int a = 0; a < numberOfProcesses; a++)
    {
        workingCycles += masterProcessArray[a].cpuTime;
    }
    fprintf(filePtr, "CPU Utilization: %f\n", (float)workingCycles / (float)cycles);
    for (int b = 0; b < numberOfProcesses; b++)
    {
        fprintf(filePtr, "Turnaround process %d: %d\n", b, (processArray[b].cycleFinished - processArray[b].cycleStarted));
    }
}

void printState(FILE *filePtr, int cycle)
{
    if (areProcessesRemaining())
    {
        fprintf(filePtr, "%d ", cycle);
        for (int a = 0; a < numberOfProcesses; a++)
        {
            // state key
            //  0 -> new
            //  1 -> ready
            //  2 -> running
            //  3 -> blocked
            //  4 -> done
            switch (processArray[a].state)
            {
            case 0:
                break;
            case 1:
                fprintf(filePtr, "%d: ", a);
                fprintf(filePtr, "ready ");
                break;
            case 2:
                fprintf(filePtr, "%d: ", a);
                fprintf(filePtr, "running ");
                break;
            case 3:
                fprintf(filePtr, "%d: ", a);
                fprintf(filePtr, "blocked ");
                break;
            case 4:
                break;
            }
        }
        fprintf(filePtr, "\n");
    }
    else
    {
        fprintf(filePtr, "\n\n");
        fprintf(filePtr, "Finishing time: %d\n", cycle - 1);
        printStats(filePtr, cycle);
    }
}

int areProcessesRunning()
{
    for (int a = 0; a < numberOfProcesses; a++)
    {
        if (processArray[a].state == 2)
        {
            return 1;
        }
    }
    return 0;
}

int areProcessesWaiting()
{
    for (int a = 0; a < numberOfProcesses; a++)
    {
        if (processArray[a].state == 1)
        {
            return 1;
        }
    }
    return 0;
}

void runningToReady()
{
    for (int a = 0; a < numberOfProcesses; a++)
    {
        if (processArray[a].state == 2)
        {
            processArray[a].state = 1;
        }
    }
}

int allButOneAreDone()
{
    int notDoneCount = 0;
    for (int a = 0; a < numberOfProcesses; a++)
    {
        if (processArray[a].state != 4)
        {
            notDoneCount++;
        }
    }
    return (notDoneCount == 1) ? 1 : 0;
}

int isLowestRemaining(process proc)
{
    if (allButOneAreDone() && proc.state < 3 && proc.state != 0)
    {
        return 1;
    }
    else
    {
        process lowest;
        lowest.cpuTime = 100;
        lowest.id = 100;
        for (int a = 0; a < numberOfProcesses; a++)
        {
            if (processArray[a].state < 3 && processArray[a].state != 0)
            {
                if (processArray[a].cpuTime <= lowest.cpuTime)
                {
                    if (processArray[a].cpuTime == lowest.cpuTime)
                    {
                        lowest = (processArray[a].id < lowest.id) ? processArray[a] : lowest;
                    }
                    else
                    {
                        lowest = processArray[a];
                    }
                }
            }
        }
        if (lowest.id == proc.id)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

// functions for each scheduling alg

// CPU function takes pointers to functions defined by the scheduling algorythm
void CPU(FILE *writeFilePtr, void(f_case_zero)(int, int), void(f_case_one(int)), void(f_case_two(int, int)), void(f_case_three(int, int)), void(final_f()))
{
    int clock = 0;
    // each loop is a clock cycle
    while (areProcessesRemaining())
    {
        for (int a = 0; a < numberOfProcesses; a++)
        {
            // state key
            //  0 -> new
            //  1 -> ready
            //  2 -> running
            //  3 -> blocked
            //  4 -> done
            switch (processArray[a].state)
            {
            // new
            case 0:
                f_case_zero(a, clock);
                break;
            // ready
            case 1:
                f_case_one(a);
                break;
            // running
            case 2:
                f_case_two(a, clock);
                break;
            // blocked
            case 3:
                f_case_three(a, clock);
                // else it is done
            }
        }
        // if no processes are running (to avoid having to do prediction)
        final_f();
        printState(writeFilePtr, clock);
        clock++;
    }
    fclose(writeFilePtr);
}

// first come functions

void firstComeCaseZero(int a, int clock)
{
    if (processArray[a].arrivalTime <= 0)
    {
        if (areProcessesRunning())
        {
            processArray[a].state = 1;
        }
        else
        {
            processArray[a].state = 2;
        }
        processArray[a].cycleStarted = clock;
    }
    else
    {
        processArray[a].arrivalTime -= 1;
    }
}

void firstComeCaseOne(int a)
{
    if (!areProcessesRunning())
    {
        processArray[a].state = 2;
    }
}

void firstComeCaseTwo(int a, int clock)
{
    processArray[a].cpuTime -= 1;
    if (((processArray[a].totalCpuTime / processArray[a].cpuTime) >= 2) && processArray[a].IOTime > 0)
    {
        processArray[a].state = 3;
    }
    if (processArray[a].cpuTime == 0)
    {
        processArray[a].cycleFinished = clock;
        processArray[a].state = 4;
    }
}

void firstComeCaseThree(int a, int clock)
{
    processArray[a].IOTime -= 1;
    if (processArray[a].IOTime <= 0)
    {
        if (!areProcessesRunning())
        {
            processArray[a].state = 2;
        }
        else
        {
            processArray[a].state = 1;
        }
    }
    if (processArray[a].IOTime <= 0 && processArray[a].cpuTime <= 0)
    {
        processArray[a].cycleFinished = clock;
        processArray[a].state = 4;
    }
}

void firstComeFinalF()
{
    if (!areProcessesRunning())
    {
        for (int b = 0; b < numberOfProcesses; b++)
        {
            if (processArray[b].state == 1)
            {
                processArray[b].state = 2;
                break;
            }
        }
    }
}

void firstCome(FILE *writeFilePtr)
{
    CPU(writeFilePtr, &firstComeCaseZero, &firstComeCaseOne, &firstComeCaseTwo, &firstComeCaseThree, &firstComeFinalF);
}

// round robin functions

void roundRobinCaseZero(int a, int clock)
{
    if (processArray[a].arrivalTime <= 0)
    {
        if (areProcessesRunning())
        {
            processArray[a].state = 1;
        }
        else
        {
            processArray[a].state = 2;
            processArray[a].runningCycles++;
        }
        processArray[a].cycleStarted = clock;
    }
    else
    {
        processArray[a].arrivalTime -= 1;
    }
}

void roundRobinCaseOne(int a)
{
    if (!areProcessesRunning())
    {
        processArray[a].state = 2;
        processArray[a].runningCycles++;
    }
}

void roundRobinCaseTwo(int a, int clock)
{
    processArray[a].cpuTime -= 1;
    processArray[a].runningCycles++;
    if (((processArray[a].totalCpuTime / processArray[a].cpuTime) >= 2) && processArray[a].IOTime > 0)
    {
        processArray[a].state = 3;
        processArray[a].runningCycles = 0;
    }
    if (processArray[a].cpuTime == 0)
    {
        processArray[a].cycleFinished = clock;
        processArray[a].runningCycles = 0;
        processArray[a].state = 4;
    }
    if (processArray[a].runningCycles > 2 && areProcessesWaiting())
    {
        processArray[a].state = 1;
        processArray[a].runningCycles = 0;
    }
}

void roundRobinCaseThree(int a, int clock)
{
    processArray[a].IOTime -= 1;
    if (processArray[a].IOTime <= 0)
    {
        if (!areProcessesRunning())
        {
            processArray[a].state = 2;
        }
        else
        {
            processArray[a].state = 1;
        }
    }
    if (processArray[a].IOTime <= 0 && processArray[a].cpuTime <= 0)
    {
        processArray[a].cycleFinished = clock;
        processArray[a].state = 4;
    }
}

void roundRobinFinalF()
{
    if (!areProcessesRunning())
    {
        for (int b = 0; b < numberOfProcesses; b++)
        {
            if (processArray[b].state == 1)
            {
                processArray[b].state = 2;
                processArray[b].runningCycles++;
                break;
            }
        }
    }
}

void roundRobin(FILE *writeFilePtr)
{
    CPU(writeFilePtr, &roundRobinCaseZero, &roundRobinCaseOne, &roundRobinCaseTwo, &roundRobinCaseThree, &roundRobinFinalF);
}

// shortest remaining functions

void shortestRemainingCaseZero(int a, int clock)
{
    if (processArray[a].arrivalTime <= 0)
    {
        processArray[a].cycleStarted = clock;
        if (isLowestRemaining(processArray[a]) || !areProcessesRunning())
        {
            runningToReady();
            processArray[a].state = 2;
        }
        else
        {
            processArray[a].state = 1;
        }
    }
    processArray[a].arrivalTime--;
}

void shortestRemainingCaseOne(int a)
{
    if (isLowestRemaining(processArray[a]) || !areProcessesRunning())
    {
        runningToReady();
        processArray[a].state = 2;
    }
}

void shortestRemainingCaseTwo(int a, int clock)
{
    processArray[a].cpuTime--;
    if (!isLowestRemaining(processArray[a]))
    {
        processArray[a].state = 1;
    }
    else if ((processArray[a].totalCpuTime / processArray[a].cpuTime) >= 2 && processArray[a].IOTime > 0)
    {
        processArray[a].state = 3;
    }
    else if (processArray[a].cpuTime <= 0)
    {
        processArray[a].cycleFinished = clock;
        processArray[a].state = 4;
    }
}

void shortestRemainingCaseThree(int a, int garbage)
{
    processArray[a].IOTime -= 1;
    if (processArray[a].IOTime <= 0)
    {
        if (isLowestRemaining(processArray[a]))
        {
            runningToReady();
            processArray[a].state = 2;
        }
        else
        {
            processArray[a].state = 1;
        }
    }
}

void shortestRemainingFinalF(int a)
{
    for (int b = 0; b < numberOfProcesses; b++)
    {
        if (isLowestRemaining(processArray[b]))
        {
            runningToReady();
            processArray[b].state = 2;
        }
    }
}

void shortestRemaining(FILE *writeFilePtr)
{
    CPU(writeFilePtr, &shortestRemainingCaseZero, &shortestRemainingCaseOne, &shortestRemainingCaseTwo, &shortestRemainingCaseThree, &shortestRemainingFinalF);
}

// adds process from a string within the main() for loop

void addProcessFromString(char str[], int i)
{
    // create new process struct
    process *newProcess = malloc(sizeof(process));
    newProcess->id = (int)str[0] - 48;
    newProcess->cpuTime = (int)str[2] - 48;
    newProcess->totalCpuTime = newProcess->cpuTime;
    newProcess->IOTime = (int)str[4] - 48;
    newProcess->arrivalTime = (int)str[6] - 48;
    processArray[i - 1] = *newProcess;
}

// choose alg based on argv after array of processes is done
void chooseAlgorithm(int scheduling, char fileName[])
{
    // create new file for functions to write to
    strcat(fileName, "-scheduling.txt");
    FILE *fpw = fopen(fileName, "a+");
    switch (scheduling)
    {
    case 0:
        firstCome(fpw);
        break;
    case 1:
        roundRobin(fpw);
        break;
    case 2:
        shortestRemaining(fpw);
    }
    // close the write file in the functions
}

// error functions to simplify main()

void invalidInputError()
{
    printf("usage:  ./scheduling alg input\n");
    printf("alg: the scheduling algorithm: 0, 1, or 2\n");
    printf("input: the processes inut file\n");
    exit(1);
}

void cannotOpenFileError(char fileName[])
{
    printf("Cannot open file %s\n", fileName);
    exit(1);
}

int main(int argc, char *argv[])
{

    int scheduling;
    FILE *fp;
    // Check that the command line is correct
    if (argc != 3)
    {
        invalidInputError();
    }

    scheduling = (int)atoi(argv[1]); // the scheduling algorithm

    // Check that the file specified by the user exists and open it
    if (!(fp = fopen(argv[2], "r")))
    {
        cannotOpenFileError(argv[2]);
    }
    else
    {
        fp = fopen(argv[2], "r");
        char n[2];

        fgets(n, 2, fp);
        // let other sysems know the number of processes
        numberOfProcesses = atoi(n);

        for (int i = 0; i < atoi(n) + 1; i++)
        {
            char str[10];
            fgets(str, 10, fp);
            // first line is blank
            if (i > 0)
            {
                addProcessFromString(str, i);
            }
        }
        // duplicate the process array into the master array for keeping track of progress
        memcpy(&masterProcessArray, &processArray, sizeof(processArray));
        //  close the processes file
        fclose(fp);
        chooseAlgorithm(scheduling, argv[1]);
    }
    return 0;
}
