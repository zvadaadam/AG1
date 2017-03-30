//
//  main.cpp
//  progtestulohy2.0
//
//  Created by Adam Zvada on 05.12.16.
//  Copyright © 2016 Adam Zvada. All rights reserved.
//

#include <iostream>
#include <cstdlib>

using namespace std;

struct Task {
    long id;
    long timeToStart;       //Hour when you start working on the task
    long deadline;          //Hour when you finsih the task
    long numPoints;         //Number of pints which u get from finishing the task
    Task * prev;
};

struct Result {
    long numFinalPoints;
    long numTasks;
    long * path;
    Task * predecssor;
    Result(Result * result);
    Result();
};


Result::Result(Result * result) {
    numFinalPoints = result->numFinalPoints;
    numTasks = result->numTasks;
}

Result::Result() {
    numTasks = 0;
    numFinalPoints = 0;
    predecssor = NULL;
}


long binarySearch(Task ** tasks, long index, long numTasks) {
    
    long lo = 0, hi = index - 1;
    
    while (lo <= hi) {
        long mid = (lo + hi) / 2;
        if (tasks[mid]->deadline <= tasks[index]->timeToStart) {
            if (tasks[mid + 1]->deadline <= tasks[index]->timeToStart) {
                lo = mid + 1;
            } else {
                return mid;
            }
        } else {
            hi = mid - 1;
        }
    }
    
    return -1;
}

void getPath(Task * task, long *& arr, long & size) {
    if (task->prev == NULL) {
        arr[0] = task->id;
        size = 1;
        return;
    }
    getPath(task->prev, arr, size);
    arr[size] = task->id;
    size++;
}

Result * getSolution(Task ** tasks, long numTasks) {
    
    Result * res = new Result();
    res->numTasks = 0;
    res->numFinalPoints = 0;
    res->predecssor = NULL;
    
    Result * tableRes = new Result[numTasks];
    
    tableRes[0].numFinalPoints = tasks[0]->numPoints;
    tableRes[0].numTasks = 1;
    tableRes[0].predecssor = tasks[0];
    tasks[0]->prev = NULL;
    
    for (long i = 1; i < numTasks; i++) {
        Result actualRes;
        actualRes.numFinalPoints = tasks[i]->numPoints;
        actualRes.numTasks++;
        actualRes.predecssor = tasks[i];
        tasks[i]->prev = NULL;
        
        long prevRes = binarySearch(tasks, i, numTasks);
        
        if (prevRes != -1) {
            actualRes.numFinalPoints += tableRes[prevRes].numFinalPoints;
            actualRes.numTasks += tableRes[prevRes].numTasks;
            actualRes.predecssor = tasks[i];
            
            tasks[i]->prev = tableRes[prevRes].predecssor;
        }
        
        if (actualRes.numFinalPoints < tableRes[i-1].numFinalPoints) {
            tableRes[i] = tableRes[i-1];
            
        } else {
            tableRes[i].numFinalPoints = actualRes.numFinalPoints;
            tableRes[i].numTasks = actualRes.numTasks;
            tableRes[i].predecssor = actualRes.predecssor;
        }
    }
    res = &tableRes[numTasks-1];
    
    
    long * arr = new long[res->numTasks];
    long size = 0;
    getPath(res->predecssor, arr, size);
    
    res->path = arr;
    
    //delete [] table;
    
    return res;
}

void swap(Task *& a, Task *& b) {
    Task * tmp = b;
    b = a;
    a = tmp;
}

void swapRes(long & a, long & b) {
    long tmp = b;
    b = a;
    a = tmp;
}


void quickSort(Task ** tasks, long left, long right) {
    if(left < right){
        long boundary = left;
        for(long i = left + 1; i < right; i++){
            if(tasks[i]->deadline < tasks[left]->deadline){
                swap(tasks[i], tasks[++boundary]);
            }
        }
        swap(tasks[left], tasks[boundary]);
        quickSort(tasks, left, boundary);
        quickSort(tasks, boundary + 1, right);
    }
}

void quickSortID(long * res, long left, long right) {
    if(left < right){
        long boundary = left;
        for(long i = left + 1; i < right; i++){
            if(res[i] < res[left]){
                swapRes(res[i], res[++boundary]);
            }
        }
        swapRes(res[left], res[boundary]);
        quickSortID(res, left, boundary);
        quickSortID(res, boundary + 1, right);
    }
}

int compareTask(const void * a, const void * b) {
    long taskA = (*(Task**)a)->deadline;
    long taskB = (*(Task**)b)->deadline;
    
    return (int)(taskA - taskB);
}

int compareID(const void * a, const void * b) {
    return (int)(*(long*)a - *(long*)b);
}

void print(Task ** tasks, long numTasks) {
    for (long i = 0; i < numTasks; i++) {
        cout << tasks[i]->timeToStart << " " << tasks[i]->deadline << " " << tasks[i]->numPoints << endl;
    }
}


int main(int argc, const char * argv[]) {
    
    long numTasks;
    scanf("%ld", &numTasks);
    
    Task ** tasks = new Task*[numTasks];
    
    for (long i = 0; i < numTasks; i++) {
        Task * task = new Task();
        scanf("%ld %ld %ld", &task->timeToStart, &task->deadline, &task->numPoints);
        task->id = i;
        tasks[i] = task;
    }
    
    qsort(tasks, numTasks, sizeof(Task*), compareTask);
    
//        cout << "-------------" << endl;
//        print(tasks, numTasks);
//        cout << "-------------" << endl;
    
    Result * res = getSolution(tasks, numTasks);
    
    qsort(res->path, res->numTasks, sizeof(long), compareID);
    
    cout << res->numFinalPoints << endl;
    for (long i = 0; i < res->numTasks; i++) {
        cout << res->path[i] << " ";
    }
    cout << endl;
    
    for (long i = 0; i < numTasks; i++) {
        delete tasks[i];
    }
    delete [] tasks;
    
    //delete res;
    
    return 0;
}
