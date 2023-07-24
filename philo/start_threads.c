#include "philosophers.h"

t_return_value	start_philosopher(t_party	*party, unsigned int i)
{
	// REMEMEBER: returns -1 on error, 0 on success
	printf("start_philosopher\n");
	// Sheree has a personal philo init mutex eaten_mutex
	party->philosophers[i].index = i;
	if (pthread_create(
		&(party->philosophers[i].thread), 
		NULL, 
		philosopher_routine,
		(void *)&(party->philosophers[i])) != 0)
		{
			printf("Failed to create a philo thread\n");
			return (ERROR);
		}
	return (SUCCESS);
}

t_return_value	start_monitoring(t_party	*party)
{
	printf("monitor_party\n");
	//TODO: add fail check
	if (pthread_create(
		&(party->monitoring_thread),
		NULL,
		monitoring_routine,
		(void *)&(party->monitoring_thread)) != 0)
		{
			printf("Failed to create a monitoring thread\n");
			return (ERROR);
		}
	return (SUCCESS);
}