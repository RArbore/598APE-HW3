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

double randomDouble()
{
   unsigned long long next = randomU64();
   next >>= (64 - 26);
   unsigned long long next2 = randomU64();
   next2 >>= (64 - 26);
   return ((next << 27) + next2) / (double)(1LL << 53);
}

int nplanets;
int timesteps;
double dt;
double G;

int main(int argc, const char** argv){
   if (argc < 2) {
      printf("Usage: %s <nplanets> <timesteps>\n", argv[0]);
      return 1;
   }
   nplanets = atoi(argv[1]);
   timesteps = atoi(argv[2]);
   dt = 0.001;
   G = 6.6743;

   double* mass = (double*)malloc(sizeof(double) * nplanets);
   double* x = (double*)malloc(sizeof(double) * nplanets);
   double* y = (double*)malloc(sizeof(double) * nplanets);
   double* vx = (double*)malloc(sizeof(double) * nplanets);
   double* vy = (double*)malloc(sizeof(double) * nplanets);
   double* next_x = (double*)malloc(sizeof(double) * nplanets);
   double* next_y = (double*)malloc(sizeof(double) * nplanets);
   for (int i=0; i<nplanets; i++) {
      mass[i] = randomDouble() * 10 + 0.2;
      x[i] = ( randomDouble() - 0.5 ) * 100 * pow(1 + nplanets, 0.4);
      y[i] = ( randomDouble() - 0.5 ) * 100 * pow(1 + nplanets, 0.4);
      next_x[i] = x[i];
      next_y[i] = y[i];
      vx[i] = randomDouble() * 5 - 2.5;
      vy[i] = randomDouble() * 5 - 2.5;
   }

   struct timeval start, end;
   gettimeofday(&start, NULL);
   for (int i=0; i<timesteps; i++) {
      for (int i=0; i<nplanets; i++) {
         for (int j=0; j<nplanets; j++) {
            double dx = x[j] - x[i];
            double dy = y[j] - y[i];
            double distSqr = dx*dx + dy*dy + 0.0001;
            double invDist = mass[i] * mass[j] / sqrt(distSqr);
            double invDist3 = invDist * invDist * invDist;
            vx[i] += dt * dx * invDist3;
            vy[i] += dt * dy * invDist3;
         }
         next_x[i] += dt * vx[i];
         next_y[i] += dt * vy[i];
      }

      memcpy(x, next_x, sizeof(double) * nplanets);
      memcpy(y, next_y, sizeof(double) * nplanets);
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
