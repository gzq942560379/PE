#include<iostream>
#include<x86intrin.h>
#include<stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <iomanip>

using namespace std;

//#define __AVX__
//read reference cycles
//regardless of turbo, power-saving, or clock-stopped idle
uint64_t rdtsc(){
    return __rdtsc();
}
//get time
double get_time(struct timespec *start,
  struct timespec *end)
{
  return end->tv_sec - start->tv_sec +
    (end->tv_nsec - start->tv_nsec) * 1e-9;
}

const int n = 1000000;
#if __AVX512F__
  const int vwidth = 64;
#elif __AVX__
  const int vwidth = 32;
#endif
  typedef float floatv __attribute__((vector_size(vwidth), aligned(4)));

static long perf_event_open(struct perf_event_attr *hw_event, 
  pid_t pid, int cpu, int group_fd, unsigned long flags) {
  int ret;
  ret = syscall(SYS_perf_event_open, hw_event, pid, cpu,
                group_fd, flags);
  return ret;
}

//jb = 4, for example
void axpy_simd(floatv *X, floatv a, floatv c, long m){
  for(uint32_t j = 0; j < m; j += 4) {
    for(uint64_t i = 0; i < n; i++) {
      for(uint32_t jj = j; jj < j + 4; jj++) {
        X[jj] = a * X[jj] + c;
      }
    }
  }
}



int main() 
{
  struct perf_event_attr pe;
  int fd;
  char buf[4096];
  memset(&pe, 0, sizeof(pe));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(pe);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;
  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }

  const int L = sizeof(floatv) / sizeof(float);
  cout << "SIMD width: " << L << endl;
  floatv a, c, x[17];
  for(int i = 0; i < L; i++)
    a[i] = c[i]= 1.0;
  for(int i = 0; i < 16; i++)
    x[i] = a;

  uint64_t start_cycle, end_cycle;
  struct timespec start, end;
  //header
  cout << setw(20) << "chains," << "\t" << setw(20) << "cycles/iter," \
  << "\t" << "flops/cycle" << endl;
  
  for(int i = 4; i <= 16; i+=4 ) {
    //time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    //cpu ref clocks
    start_cycle = rdtsc();
    //cpu core clocks
    ioctl(fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);
    axpy_simd(x, a, c, i);
    ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);
    end_cycle = rdtsc();
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double used_time = get_time(&start, &end);
    uint64_t used_cycles= end_cycle - start_cycle;
    double flops = 2.0 * L * n * (i);
    uint64_t cpu_clocks;
    if(read(fd, &cpu_clocks, sizeof(cpu_clocks)) == -1) {
       cout << "read cpu clocks error" << endl;
    };
    cout << setw(20) << i << ",\t" << setw(20) << 1.0 * cpu_clocks / n \
    << ",\t" << flops / cpu_clocks << endl;
  }  
  //in case that compiler optimized
  return (int)x[0][0];
}
