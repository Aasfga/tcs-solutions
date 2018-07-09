#include<stdint.h>
#include<stdlib.h>
#include<stack>
#include<iostream>
#include<assert.h>


extern "C" void thread_switch(uint64_t *old_t, uint64_t *new_t);

typedef void (*thread)();

//RBX 0
//RSP 1
//RBP 2
//R12 3
//R13 4
//R14 5
//R15 6

#define SIZE 5 * 1024

struct thread_state
{
    uint64_t *mem = new uint64_t[SIZE]; 
    uint64_t *reg = new uint64_t[7];    
    unsigned long msg;                          
    bool used = false;                          
    bool waiting = false;                      
};

uint8_t crr_id = 0;
std::stack<uint8_t> stack;
thread_state threads[256];

int find_empty()
{
    for(int i = 1; i < 256; i++)
    {
        if(!threads[i].used)
            return i;
    }

    return -1;
}

void clean()
{
    thread_state *state = threads + crr_id;
    state->used = false;
    int old_id = crr_id;
    int new_id = stack.top();
    stack.pop();
    crr_id = new_id;
    thread_switch(threads[old_id].reg, threads[new_id].reg);
}

uint64_t* align_mem(uint64_t *ptr)
{
    uint64_t p = reinterpret_cast<uint64_t>(ptr);
    int r = p % 16;

    return reinterpret_cast<uint64_t*>(p - r - 8);
}

extern "C" 
{
    // get current thread ID or 0 if not in any thread
    uint8_t thread_id()
    {
        return crr_id;
    }
    //
    // // create a thread and return its ID
    uint8_t thread_start(thread t)
    {
        int id = find_empty();
        int old_id = crr_id;
        if(id == -1)
            return 0;   
        thread_state *state = threads + id;
        state->used = true;
        state->waiting = false;
        uint64_t *mem = align_mem(state->mem + SIZE - 1);
        *mem = reinterpret_cast<uint64_t>(clean);
        mem--;
        *mem = reinterpret_cast<uint64_t>(t);
        state->reg[1] = reinterpret_cast<uint64_t>(mem);

        crr_id = id;
        stack.push(old_id);
        thread_switch(threads[old_id].reg, threads[id].reg);

        return id;
    }

    //
    // // send a number to thread t_id, noop if error
    void thread_send(uint8_t id, unsigned long value)
    {
        if(!threads[id].used || !threads[id].waiting)
            return;
        threads[id].waiting = false;
        stack.push(crr_id);
        crr_id = id;
        threads[id].msg = value;
        thread_switch(threads[stack.top()].reg, threads[id].reg);
        return;
    }
    //
    // // stop thread and wait for a number, noop if not in a thread
    unsigned long thread_receive()
    {
        int next_id;
        if(crr_id == 0)
            return 0;
        if(stack.size() == 0)
            next_id = 0;
        else
        {
            next_id = stack.top();
            stack.pop();
        }
        int old_id = crr_id;
        threads[old_id].waiting = true;
        crr_id = next_id;
        thread_switch(threads[old_id].reg, threads[next_id].reg);

        return threads[old_id].msg;
    }
}
