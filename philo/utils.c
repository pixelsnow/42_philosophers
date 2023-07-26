#include "philosophers.h"

// Temporary replacement
int	parse_args_old(t_party *party, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (ERROR);
	party->number_of_philosophers = atoi(av[1]);
	party->time_to_die = atoi(av[2]) * 1000;
	party->time_to_eat = atoi(av[3]) * 1000;
	party->time_to_sleep = atoi(av[4]) * 1000;
	// What if 0?
	if (ac > 5)
		party->number_of_meals = atoi(av[5]);
	else
		party->number_of_meals = -1;
	return (SUCCESS);
}

// Currently useless
int	quit_gracefully(t_party *party)
{
	(void)party;
	return (ERROR);
}

void	custom_usleep(unsigned long long duration, t_party *party)
{
	unsigned long long	start_time;

	start_time = get_current_time();
	while (get_current_time() - start_time < duration)
	{
		pthread_mutex_lock(&(party->dying));
		if (party->someone_dead != 0)
		{
			pthread_mutex_unlock(&(party->dying));
			break ;
		}
		pthread_mutex_unlock(&(party->dying));
		usleep(500);
	}
}

void	print_whats_happening(t_philosopher *philosopher, char *event)
{
	unsigned long long	timestamp;

	timestamp = get_current_time() - philosopher->party->party_start_time;
	timestamp /= 1000ULL; // // cast to ULL to avoid potential int overflow
	//printf("inside print_whats_happening\n");
	pthread_mutex_lock(&(philosopher->party->dying));
	//printf("inside print_whats_happening LOCKED\n");
	//if (philosopher->party->someone_dead)
	// Will need to adjust that when someone_dead is functional
	printf("%llu\t%d\t%s\n", timestamp, philosopher->index + 1, event); // make sure philosopher index is fine for output
	pthread_mutex_unlock(&(philosopher->party->dying)); 
	//printf("inside print_whats_happening UNLOCKED \n");
}

// Getting current time in microseconds
unsigned long long	get_current_time(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return ((unsigned long long)tp.tv_sec * 1000000ULL + tp.tv_usec);
	// cast to ULL to avoid potential int overflow
}