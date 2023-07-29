#include "philosophers.h"

void	free_memory(t_party	*party)
{
	if (party->philosophers)
		free(party->philosophers);
	if (party->forks)
		free(party->forks);
}

void	destroy_mutexes(t_party	*party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_destroy(&(party->forks[i]));
		pthread_mutex_destroy(&(party->philosophers[i].meal_update));
		i++;
	}
	pthread_mutex_destroy(&(party->guard));
	pthread_mutex_destroy(&(party->dying));
	pthread_mutex_destroy(&(party->reporting_enough_meals));
}

t_return_value	quit_gracefully(t_party *party, t_return_value	ret_val)
{
	if (ret_val == MALLOC_FAIL)
	{
		free_memory(party);
	}
	else if (ret_val == MUTEX_FAIL)
	{
		destroy_mutexes(party);
		free_memory(party);
	}
	else if (ret_val == THREAD_FAIL)
	{
		// no, it can't be done here because we don't know the last index, or does it matter?
		join_philosopher_threads(party, party->number_of_philosophers - 1);
	}
	return (ret_val);
}

void	clean_up(t_party	*party)
{
	destroy_mutexes(party);
	free_memory(party);
}
