#include "philosophers.h"
#include <stdlib.h>

int	parse_args(t_party	*party, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (ERROR);
	party->number_of_philosophers = atoi(av[1]);
	party->time_to_die = atoi(av[2]);
	party->time_to_eat = atoi(av[3]);
	party->time_to_sleep = atoi(av[4]);
	// What if 0?
	if (ac > 5)
		party->number_of_meals = atoi(av[5]);
	return (SUCCESS);
}

int	quit_gracefully(t_party	*party)
{
	(void) party;
	return (ERROR);
}

unsigned long long	get_current_time(t_party *party)
{
	struct timeval tp;

	if (gettimeofday(&tp, NULL))
	{
		quit_gracefully(party);
		return (0);
		// Confusing because 0 not 1, double check
	}
	return (tp.tv_sec * 1000000 + tp.tv_usec);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	int				*idptr;

	philosopher = (t_philosopher *)philosopher_data;
	idptr = (int *)&philosopher->thread;
	printf("Thread [");
	printf("%d", (int)philosopher->thread);
	printf("] routine on\n");
	printf("time_last_ate: %llu\n", philosopher->time_last_ate);
	printf("number_of_philosophers: %i\n",
		philosopher->party->number_of_philosophers);
	printf("Before lock\n");
	pthread_mutex_lock(&(philosopher->party->guard));
	printf("Before unlock\n");
	pthread_mutex_unlock(&(philosopher->party->guard));
	/* while (1)
	{
		if ()
		if (philosopher->party->someone_dead)
		{
			break;
		}
		if (philosopher->party->number_of_meals >= 0
			&& philosopher->meal_count >= philosopher->party->number_of_meals)
		{
			break;
		}

	} */
	printf("Thread [");
	printf("%d", (int)philosopher->thread);
	printf("] routine is on\n");
	pthread_mutex_lock(philosopher->fork_own);
	printf("Thread [");
	printf("%d", (int)philosopher->thread);
	printf("] took own fork\n");
	pthread_mutex_unlock(philosopher->fork_own);
	return (NULL);
}

void	prepare_philosopher(t_party	*party, unsigned int i)
{
	printf("prepare_philosopher\n");
	party->philosophers[i].fork_own = &party->forks[i];
	party->philosophers[i].fork_borrowed
		= &party->forks[(i + 1) % party->number_of_philosophers];
	party->philosophers[i].meal_count = 10;
	party->philosophers[i].dead = 0;
	party->philosophers[i].party = party;
}

void	start_philosopher(t_party	*party, unsigned int i)
{
	// REMEMEBER: returns -1 on error, 0 on success
	printf("start_philosopher\n");
	pthread_create(
		&(party->philosophers[i].thread), 
		NULL, 
		philosopher_routine,
		(void *)&(party->philosophers[i]));
}

int prepare_party(t_party	*party)
{
	unsigned int		i;
	unsigned long long	curr_time;
	
	// Init and malloc
	// TODO: protect mallocs
	printf("prepare_party\n");
	party->someone_dead = 0;
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
	pthread_mutex_init(&(party->guard), NULL);
	// TODO: add print mutex
	
	// Start all threads simultaneously
	
	pthread_mutex_lock(&(party->guard));
	i = 0;
	while (i < party->number_of_philosophers)
	{
		start_philosopher(party, i);
		printf("i++\n");
		i++;
	}
	printf("unlocking guard...\n");
	pthread_mutex_unlock(&(party->guard));

	// Save starting timestamps
	curr_time = get_current_time(party);
	if (!curr_time)
		return (quit_gracefully(party));
	i = 0;
	while (i < party->number_of_philosophers)
	{
		party->philosophers[i].time_last_ate = curr_time;
		i++;
	}
	
	return (SUCCESS);
}

void	monitor_party(t_party	*party)
{
	printf("monitor_party\n");
	(void) party;
}

void	clean_up(t_party	*party)
{
	unsigned int	i;

	printf("clean_up\n");
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_join(party->philosophers[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_destroy(&(party->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(party->guard));
	free(party->philosophers);
	free(party->forks);
}

int	main(int ac, char **av)
{
	t_party	party;
	
	if (parse_args(&party, ac, av))
	{
		return (ERROR);
	}
	if (prepare_party(&party))
	{
		return (ERROR);
	}
	monitor_party(&party);
	clean_up(&party);
	return(SUCCESS);
}
