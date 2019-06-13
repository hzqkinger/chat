#include "test2.hpp"

int main()
{
	A *pa = new A;
	pa->ptt();

	pthread_exit(NULL);
	return 0;
}
