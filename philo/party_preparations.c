#include "philosophers.h"

static void	prepare_philosopher(t_party	*party, unsigned int i)
{
	printf("prepare_philosopher\n");
	party->philosophers[i].fork_own = &party->forks[i];
	party->philosophers[i].fork_borrowed
		= &party->forks[(i + 1) % party->number_of_philosophers];
	party->philosophers[i].meal_count = 0;
	party->philosophers[i].party = party;
}

int prepare_party(t_party *party)
{
	unsigned int		i;
	//unsigned long long	curr_time;
	
	// Init and malloc
	// TODO: protect mallocs
	printf("prepare_party\n");
	party->someone_dead = 0;
	party->number_of_philosophers_fed = 0;
	party->philosophers = malloc(sizeof(t_philosopher)
			* party->number_of_philosophers);
	party->forks = malloc(sizeof(pthread_mutex_t)
			* party->number_of_philosophers);

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
		pthread_mutex_init(&(party->forks[i]), NULL);
		i++;
	}
	if (pthread_mutex_init(&(party->guard), NULL) != SUCCESS)
		printf("\nGuard mutex init has failed\n");
	if (pthread_mutex_init(&(party->printing), NULL) != SUCCESS)
		printf("\nPrinting mutex init has failed\n");
	if (pthread_mutex_init(&(party->dying), NULL) != SUCCESS)
		printf("\nDying mutex init has failed\n");
	if (pthread_mutex_init(&(party->reporting_enough_meals), NULL) != SUCCESS)
		printf("\nReporting_enough_meals mutex init has failed\n");
	if (pthread_mutex_init(&(party->party_going_on), NULL) != SUCCESS)
		printf("\nParty_going_on mutex init has failed\n");

	// time setting probably shouldn't be here
	//curr_time = get_current_time();
	//i = 0;
	//while (i < party->number_of_philosophers)
	//{
	//	party->philosophers[i].time_last_ate = curr_time;
	//	i++;
	//}
	return (SUCCESS);
}
