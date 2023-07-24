#include "philosophers.h"

// Cleans all threads, mutexes and memory
void	clean_up(t_party	*party)
{
	unsigned int	i;

	printf("clean_up\n");

	// Clean threads
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_join(party->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(party->monitoring_thread, NULL);

	// Clean mutexes
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_destroy(&(party->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(party->guard));
	pthread_mutex_destroy(&(party->printing));
	pthread_mutex_destroy(&(party->dying));
	pthread_mutex_destroy(&(party->reporting_enough_meals));
	pthread_mutex_destroy(&(party->party_going_on));

	// Free memory
	free(party->philosophers);
	free(party->forks);
}