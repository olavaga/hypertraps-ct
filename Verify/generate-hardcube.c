#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _N 8
#define RND drand48()
#define _NH 1000

double expsample(double lambda)
{
  return -log(RND)/lambda;
}

void outputstr(FILE *fp, int ref)
{
  switch(ref)
    {
    case 0: fprintf(fp, "0 0 0\n"); break;
    case 1: fprintf(fp, "0 0 1\n"); break;
    case 2: fprintf(fp, "0 1 0\n"); break;
    case 3: fprintf(fp, "0 1 1\n"); break;
    case 4: fprintf(fp, "1 0 0\n"); break;
    case 5: fprintf(fp, "1 0 1\n"); break;
    case 6: fprintf(fp, "1 1 0\n"); break;
    case 7: fprintf(fp, "1 1 1\n"); break;
    }
}

int main(void)
{
  double edges[_N*_N];
  int i, j;
  int state, oldstate;
  int t;
  double tc, tau;
  double sampled[_NH];
  double cumsum[_NH];
  double chance[_NH];
  double r;
  int n;
  int step;
  int tmpt;
  double total;
  double scores[300];
  int run;
  double samp1, samp2, oldsamp;
  FILE *fp, *fptime;

  srand48(45);

  for(i = 0; i < _N; i++)
    {
      for(j = 0; j < _N; j++)
	{
	  // edge going from i to j
	  edges[i*_N+j] = 0;
	}
    }
  edges[0*_N+1] = RND;
  edges[0*_N+2] = RND;
  edges[1*_N+3] = RND;
  edges[2*_N+3] = RND;
  edges[0*_N+4] = RND;
  edges[1*_N+5] = RND;
  edges[4*_N+5] = RND;
  edges[2*_N+6] = RND;
  edges[4*_N+6] = RND;
  edges[3*_N+7] = RND;
  edges[5*_N+7] = RND;
  edges[6*_N+7] = RND;

  fp = fopen("synth-hardcube.txt", "w");
  for(i = 0; i < _N; i++)
    {
      for(j = 0; j < _N; j++)
	fprintf(fp, "%i %i %f\n", i, j, edges[i*_N+j]);
    }
  fclose(fp);

  // first get probability of being in state 1 at time t by sampling trajectories
  fp = fopen("synth-hardcube-data.txt", "w");
  fptime = fopen("synth-hardcube-time.txt", "w");
  for(n = 0; n < 1000; n++)
    {
      tc = 0; state = 0;
      samp1 = RND*2;
      samp2 = samp1+RND*5;

      for(;tc < 10000;)
	{
	  total = 0; cumsum[0] = 0;
	  for(j = 0; j < _N; j++)
	    {
	      chance[j] = edges[state*_N+j];
	      cumsum[j] = (j == 0 ? 0 : cumsum[j-1])+chance[j];
	      total += chance[j];
	    }
	  if(total == 0) tau = 10000;
	  else tau = expsample(total);

	  if(samp1 != -1 && tc+tau > samp1)
	    {
	      oldstate = state;
	      oldsamp = samp1;
	      samp1 = -1;
	    }
	  if(samp2 != -1 && tc+tau > samp2)
	    {
      	      outputstr(fp, oldstate);
	      outputstr(fp, state);
	      fprintf(fptime, "%f\n", samp2-oldsamp);
	      samp2 = -1;
	    }
	  printf("%i %i %f\n", n, state, tau);
	  if(total == 0) break;
	  r = RND;
	  for(j = 0; cumsum[j]/total < r; j++);
	  state = j;
	  tc += tau;
	}
    }

  fclose(fp);
  fclose(fptime);
    
  return 0;
}
