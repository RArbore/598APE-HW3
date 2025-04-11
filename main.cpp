#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#include <sys/time.h>

float tdiff(struct timeval *start, struct timeval *end) {
  return (end->tv_sec-start->tv_sec) + 1e-6*(end->tv_usec-start->tv_usec);
}

unsigned long long seed = 100;

unsigned long long randomU64() {
  seed ^= (seed << 21);
  seed ^= (seed >> 35);
  seed ^= (seed << 4);
  return seed;
}

typedef float NUMBER_TYPE;

NUMBER_TYPE randomNum()
{
   unsigned long long next = randomU64();
   next >>= (64 - 26);
   unsigned long long next2 = randomU64();
   next2 >>= (64 - 26);
   return ((next << 27) + next2) / (NUMBER_TYPE)(1LL << 53);
}

int nplanets;
int timesteps;
NUMBER_TYPE dt;
NUMBER_TYPE G;

int main(int argc, const char** argv){
   if (argc < 2) {
      printf("Usage: %s <nplanets> <timesteps>\n", argv[0]);
      return 1;
   }
   nplanets = atoi(argv[1]);
   timesteps = atoi(argv[2]);
   dt = 0.001;
   G = 6.6743;

   NUMBER_TYPE* mass = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* x = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* y = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* vx = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* vy = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* next_x = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   NUMBER_TYPE* next_y = (NUMBER_TYPE*)malloc(sizeof(NUMBER_TYPE) * nplanets);
   for (int i=0; i<nplanets; i++) {
      mass[i] = randomNum() * 10 + 0.2;
      x[i] = ( randomNum() - 0.5 ) * 100 * pow(1 + nplanets, 0.4);
      y[i] = ( randomNum() - 0.5 ) * 100 * pow(1 + nplanets, 0.4);
      next_x[i] = x[i];
      next_y[i] = y[i];
      vx[i] = randomNum() * 5 - 2.5;
      vy[i] = randomNum() * 5 - 2.5;
   }

   struct timeval start, end;
   gettimeofday(&start, NULL);
   for (int i=0; i<timesteps; i++) {
      #pragma omp parallel for
      for (int i=0; i<nplanets; i++) {
         #pragma omp simd
         for (int j=0; j<nplanets; j++) {
            NUMBER_TYPE dx = x[j] - x[i];
            NUMBER_TYPE dy = y[j] - y[i];
            NUMBER_TYPE distSqr = dx*dx + dy*dy + 0.0001;
            NUMBER_TYPE invDist = mass[i] * mass[j] / sqrt(distSqr);
            NUMBER_TYPE invDist3 = invDist * invDist * invDist;
            vx[i] += dt * dx * invDist3;
            vy[i] += dt * dy * invDist3;
         }
         next_x[i] = dt * vx[i] + x[i];
         next_y[i] = dt * vy[i] + y[i];
      }

      std::swap(x, next_x);
      std::swap(y, next_y);
      // printf("x=%f y=%f vx=%f vy=%f\n", planets[nplanets-1].x, planets[nplanets-1].y, planets[nplanets-1].vx, planets[nplanets-1].vy);
   }
   gettimeofday(&end, NULL);
   printf("Total time to run simulation %0.6f seconds, final location %f %f\n", tdiff(&start, &end), x[nplanets-1], y[nplanets-1]);
   free(mass);
   free(x);
   free(y);
   free(vx);
   free(vy);
   free(next_x);
   free(next_y);

   return 0;   
}
