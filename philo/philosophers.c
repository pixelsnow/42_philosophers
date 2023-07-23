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
	if (ac > 5)
		party->number_of_times_each_philosopher_must_eat = atoi(av[5]);
	return (SUCCESS);
}

int	quit_gracefully(t_party	*party)
{
	(void) party;
	return (ERROR);
}

void print_thread_id(pthread_t id)
{
    size_t i;
    for (i = sizeof(i); i; --i)
        printf("%02x", *(((unsigned char*) &id) + i - 1));
}


void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)philosopher_data;
	printf("time_last_ate: %llu\n", philosopher->time_last_ate);
	printf("dead: %i\n", philosopher->dead);
	printf("number_of_philosophers: %i\n",
		philosopher->party->number_of_philosophers);
	printf("Before lock\n");
	pthread_mutex_lock(&(philosopher->party->guard));
	printf("Before unlock\n");
	pthread_mutex_unlock(&(philosopher->party->guard));
	printf("Thread [");
	print_thread_id(philosopher->thread);
	printf("] routine is on\n");
	pthread_mutex_lock(philosopher->fork_own);
	printf("Thread [");
	print_thread_id(philosopher->thread);
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
	party->philosophers[i].dead = 1;
	party->philosophers[i].party = party;
}

void	start_philosopher(t_party	*party, unsigned int i)
{
	// REMEMEBER: returns -1 on error, 0 on success
	printf("start_philosopher\n");
	pthread_create(
		&(party->philosophers[i].thread), 
		NULL, 
		philosopher_routine(&(party->philosophers[i])),
		&(party->philosophers[i]));
}

int prepare_party(t_party	*party)
{
	unsigned int	i;
	struct timeval tp;
	
	// TODO: protect mallocs
	printf("prepare_party\n");
	party->philosophers = malloc(sizeof(t_philosopher)
			* party->number_of_philosophers);
	party->forks = malloc(sizeof(pthread_mutex_t)
			* party->number_of_philosophers);
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_init(&(party->forks[i]), NULL);
		i++;
	}
	i = 0;
	while (i < party->number_of_philosophers)
	{
		prepare_philosopher(party, i);
		i++;
	}
	i = 0;
	while (i < party->number_of_philosophers)
	{
		party->philosophers[i].time_last_ate = tp.tv_sec * 1000000 + tp.tv_usec;
		printf("time is %llu\n", party->philosophers[i].time_last_ate);
		i++;
	}
	pthread_mutex_init(&(party->guard), NULL);
	pthread_mutex_lock(&(party->guard));
	i = 0;
	while (i < party->number_of_philosophers)
	{
		start_philosopher(party, i);
		printf("i++\n");
		i++;
	}
	printf("quit_gracefully...\n");
	pthread_mutex_unlock(&(party->guard));
	
	if (gettimeofday(&tp, NULL))
		return (quit_gracefully(party));
	
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
