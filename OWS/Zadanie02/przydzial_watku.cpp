#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <pthread.h>

int main() {
    cpu_set_t cpuset;
    pthread_t thread;
    int s, jj;

    int id = omp_get_thread_num();
    thread = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(id, &cpuset);

    s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

    for(jj = 0; jj < sizeof(cpu_set_t); jj++)
        if(CPU_ISSET(jj, &cpuset));

    printf("Ustalona maska powinowactwa dla procesu %d wynosi %d\n", id, jj);
}