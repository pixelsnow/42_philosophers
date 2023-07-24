#include "philosophers.h"

void	start_philosopher(t_party	*party, unsigned int i)
{
	// REMEMEBER: returns -1 on error, 0 on success
	printf("start_philosopher\n");
	party->philosophers[i].index = i;
	//TODO: add fail check
	pthread_create(
		&(party->philosophers[i].thread), 
		NULL, 
		philosopher_routine,
		(void *)&(party->philosophers[i]));
}

void	start_monitoring(t_party	*party)
{
	printf("monitor_party\n");
	//TODO: add fail check
	pthread_create(
		&(party->monitoring_thread),
		NULL,
		monitoring_routine,
		(void *)&(party->monitoring_thread));
}