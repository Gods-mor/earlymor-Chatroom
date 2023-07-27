#pragma once

class TaskQueue {
   public:
    void (*function)(void* arg);
    void* arg;
};