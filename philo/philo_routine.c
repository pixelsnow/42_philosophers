#include "philosophers.h"

void	eat_sleep_think(t_philosopher	*philosopher)
{
	// EAT
	// delay if even
	if (philosopher->index % 2 == 0)
		usleep(philosopher->party->time_to_eat / 10);
	pthread_mutex_lock(philosopher->fork_own);
	pthread_mutex_lock(philosopher->fork_borrowed);
	printf("Philo [%d] started eating\n", (int)philosopher->thread);
	philosopher->time_last_ate = get_current_time();
	philosopher->meal_count += 1;
	usleep(philosopher->party->time_to_eat);
	pthread_mutex_unlock(philosopher->fork_own);
	pthread_mutex_unlock(philosopher->fork_borrowed);
	// SLEEP
	usleep(philosopher->party->time_to_sleep);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher		*philosopher;
	unsigned long long	curr_time;

	philosopher = (t_philosopher *)philosopher_data;
	// Just garbage printfs
	printf("Philo [%d] routine on\n", (int)philosopher->thread);
	printf("time_last_ate: %llu\n", philosopher->time_last_ate);
	printf("Philo [%d] waiting for guard...\n", (int)philosopher->thread);
	// Syncing threads
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	printf("Philo [%d] let in by guard...\n", (int)philosopher->thread);
	while (1)
	{
		// 1. Check time since last meal and die if starved
		curr_time = get_current_time();
		if (curr_time - philosopher->time_last_ate
			> philosopher->party->time_to_die)
		{
			// Idk if these time calculations are actually correct
			printf("Philo [%d] starved to death\n", (int)philosopher->thread);
			pthread_mutex_lock(&(philosopher->party->dying));
			philosopher->party->someone_dead = 1;
			pthread_mutex_unlock(&(philosopher->party->dying));
			break;
		}

		// Probably unnecessary, since monitoring can check this and join all threads
		// 2. If there was a death, stop
		pthread_mutex_lock(&(philosopher->party->dying));
		if (philosopher->party->someone_dead)
		{
			printf("Philo [%d] detects someone is dead and quits\n", (int)philosopher->thread);
			pthread_mutex_unlock(&(philosopher->party->dying));
			break;
		}
		pthread_mutex_unlock(&(philosopher->party->dying));

		// 3. If this philo ate enough, stop
		if (philosopher->party->number_of_meals >= 0
			&& philosopher->meal_count >= philosopher->party->number_of_meals)
		{
			printf("Philo [%d] had enough meals\n", (int)philosopher->thread);
			pthread_mutex_lock(&(philosopher->party->reporting_enough_meals));
			philosopher->party->number_of_philosophers_fed += 1;
			pthread_mutex_unlock(&(philosopher->party->reporting_enough_meals));
			break;
		}
		// TODO: figure out proper eat_sleep_think routine
		eat_sleep_think(philosopher);
	}
	printf("Philo [%d] quitting\n", (int)philosopher->thread);
	return (NULL);
}