#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>

typedef void (*thread) ();

// get current thread ID or 0 if not in any thread
uint8_t thread_id();

// create a thread and return its ID
uint8_t thread_start(thread);

// send a number to thread t_id, noop if error
void thread_send(uint8_t t_id, unsigned long value);

// stop thread and wait for a number, noop if not in a thread
unsigned long thread_receive();

#endif
