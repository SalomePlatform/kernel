#include <mpi.h>

int main(int argc, char** argv)
{
#ifdef OPEN_MPI
  return 1;
#elif defined(MPICH)
  return 2;
#elif defined(LAM_MPI)
  return 3;
#else
  return 0;
#endif
}
