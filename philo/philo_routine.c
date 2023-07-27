#include "philosophers.h"

static t_return_value	eat_sleep_think(t_philosopher *philosopher)
{
	if (philosopher->party->number_of_philosophers != 1)
	{
		pthread_mutex_lock(philosopher->fork_own);
		print_whats_happening(philosopher, "has taken a fork");
		pthread_mutex_lock(philosopher->fork_borrowed);
		print_whats_happening(philosopher, "has taken a fork");
		pthread_mutex_lock(&philosopher->meal_update);
		philosopher->meal_count++;
		philosopher->time_last_ate = get_current_time();
		pthread_mutex_unlock(&philosopher->meal_update);
		print_whats_happening(philosopher, "is eating");
		custom_usleep(philosopher->party->time_to_eat, philosopher->party);
		pthread_mutex_unlock(philosopher->fork_own);
		pthread_mutex_unlock(philosopher->fork_borrowed);
		print_whats_happening(philosopher, "is sleeping");
		custom_usleep(philosopher->party->time_to_sleep, philosopher->party);
		print_whats_happening(philosopher, "is thinking");
		return (SUCCESS);
	}
	return (ERROR);
}

void * philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	int				someone_dead;

	someone_dead = 0;
	philosopher = (t_philosopher *)philosopher_data;
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	print_whats_happening(philosopher, "is thinking");
	if (philosopher->index % 2 == 0)
		custom_usleep(philosopher->party->time_to_eat / 10, philosopher->party);
	while (1)
	{
		if (eat_sleep_think(philosopher) == ERROR)
		{
			print_whats_happening(philosopher, "has taken a fork");
			break ;
		}
		pthread_mutex_lock(&(philosopher->party->dying));
		someone_dead = philosopher->party->someone_dead;
		pthread_mutex_unlock(&(philosopher->party->dying));
		if (someone_dead != 0)
			break ;
	}
	return (NULL);
}
