#include "philosophers.h"

// Temporary replacement
int	parse_args_old(t_party	*party, int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (ERROR);
	party->number_of_philosophers = atoi(av[1]);
	party->time_to_die = atoi(av[2]) * 1000;
	party->time_to_eat = atoi(av[3]) * 1000;
	party->time_to_sleep = atoi(av[4]) * 1000;
	// What if 0?
	if (ac > 5)
		party->number_of_meals_needed = atoi(av[5]);
	else
		party->number_of_meals_needed = -1;
	return (SUCCESS);
}

// Currently useless
int	quit_gracefully(t_party	*party)
{
	(void) party;
	return (ERROR);
}

// Getting current time in microseconds
unsigned long long	get_current_time(void)
{
	struct timeval tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000000 + tp.tv_usec);
}