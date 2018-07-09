#include <stdio.h>
#include <assert.h>
#include "threads.h"

uint8_t next;

void add1() {
    uint8_t local_next = next;
    unsigned long v = thread_receive() + 1;
    thread_send(local_next, v);
    thread_send(local_next, v + 1000); // ignored
    thread_send(20 + local_next, v); // ignored
    printf("Thread %u sent %lu to %u\n", thread_id(), v, local_next);
}

int main() {
    thread_receive(); // ignored
    assert(thread_id() == 0);
    for (next = 0; next < 10; next++)
        thread_start(add1);
    assert(thread_id() == 0);
    thread_receive(); // ignored
    thread_send(10, 1000);
    assert(thread_id() == 0);
    return 0;
}
