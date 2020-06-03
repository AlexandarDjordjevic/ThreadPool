# ThreadPool

This is a simple Thread Pool Library.

## Usage
---
```
void Task1(){
    ...
}

void Task2(std::string msg){
    ...
}

ThreadPool::ThreadPool threadPool;
threadPool.create(2); //create and pause threads
threadPool.enqueTask(Task1); //add task 1 to queue
threadPool.enqueTask(Task2, "This is message from task 2"); //add task 2 to queue
threadPool.destroy(); //destroy queue with tasks, and destroy all started threads
```

### Prerequisites
---
* CMake version 3.8 or newer
* C++ 14

#### Build 
---
Building via CMake  