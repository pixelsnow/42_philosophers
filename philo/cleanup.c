#include "philosophers.h"

void	clean_up(t_party	*party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_join(party->philosophers[i].thread, NULL);
		pthread_mutex_destroy(&(party->philosophers[i].meal_update));
		i++;
	}
	pthread_join(party->monitoring_thread, NULL);
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
	free(party->philosophers);
	free(party->forks);
}
