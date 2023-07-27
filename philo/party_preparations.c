#include "philosophers.h"

static t_return_value	prepare_philosopher(t_party	*party, unsigned int i)
{
	party->philosophers[i].fork_own = &party->forks[i];
	party->philosophers[i].fork_borrowed
		= &party->forks[(i + 1) % party->number_of_philosophers];
	party->philosophers[i].meal_count = 0;
	party->philosophers[i].party = party;
	if (pthread_mutex_init(&(party->philosophers[i].meal_update), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	return (SUCCESS);
}

t_return_value prepare_party(t_party *party)
{
	unsigned int		i;
	//unsigned long long	curr_time;
	
	party->someone_dead = 0;
	party->number_of_philosophers_fed = 0;
	party->philosophers = malloc(sizeof(t_philosopher) * party->number_of_philosophers);
	if (party->philosophers == NULL)
		return (MALLOC_FAIL);
	party->forks = malloc(sizeof(pthread_mutex_t) * party->number_of_philosophers);
	if (party->forks == NULL)
		return (MALLOC_FAIL);

	// Init philosopers
	i = 0;
	while (i < party->number_of_philosophers)
	{
		prepare_philosopher(party, i);
		i++;
	}
	// Init all mutexes
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (pthread_mutex_init(&(party->forks[i]), NULL) != SUCCESS)
			return (MUTEX_FAIL);
		i++;
	}
	if (pthread_mutex_init(&(party->guard), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->printing), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->dying), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->reporting_enough_meals), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->party_going_on), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	return (SUCCESS);
}
