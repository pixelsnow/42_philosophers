#include "philosophers.h"

// Combined eat, sleep, and think function
void	eat_sleep_think(t_philosopher *philosopher)
{
	//take_forks(philosopher);
	pthread_mutex_lock(philosopher->fork_own);
	//printf("Philo [%d] has taken a fork [own]\n", (int)philosopher->index);
	print_whats_happening(philosopher, "has taken a fork [own], WAITING for second fork...");
	pthread_mutex_lock(philosopher->fork_borrowed);
	//printf("Philo [%d] has taken a fork [borrowed]\n", (int)philosopher->index);
	print_whats_happening(philosopher, "has taken a fork [borrowed], GOT BOTH FORKS");
	//pthread_mutex_lock(&(philosopher->party->printing));
	pthread_mutex_lock(&philosopher->meal_update);
	philosopher->meal_count++;
	philosopher->time_last_ate = get_current_time();
	pthread_mutex_unlock(&philosopher->meal_update);
	//pthread_mutex_unlock(&(philosopher->party->printing));
	print_whats_happening(philosopher, "is eating");
	// Update meal info
	//May need to replace usleep with custom usleep()
	custom_usleep(philosopher->party->time_to_eat, philosopher->party);
	pthread_mutex_unlock(philosopher->fork_own);
	pthread_mutex_unlock(philosopher->fork_borrowed);
	print_whats_happening(philosopher, "finished eating");
	print_whats_happening(philosopher, "is sleeping");
//	printf("Philo [%d] is sleeping\n", (int)philosopher->index);
	//May need to replace usleep with custom usleep()
	custom_usleep(philosopher->party->time_to_sleep, philosopher->party);
	print_whats_happening(philosopher, "finished sleeping");
	print_whats_happening(philosopher, "is thinking");
	//printf("Philo [%d] is thinking\n", (int)philosopher->index);
}
void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher		*philosopher;
/* 	unsigned long long	curr_time;*/
	int					someone_dead;

	someone_dead = 0; 
	philosopher = (t_philosopher *)philosopher_data;
	// Ensure synchronization with monitoring thread
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	// Additional checks before the main loop
	printf("philo_routing before while loop thread: party->someone_dead = %i\n", philosopher->party->someone_dead);
	if (philosopher->index % 2 == 0)
	{
		printf("Philo [%d] is thinking\n", (int)philosopher->index);
		custom_usleep(philosopher->party->time_to_eat / 10, philosopher->party);
	}
	while (1)
	{
		// Perform the eat_sleep_think routine
		/* WE CANNOT monitor for the death here like this,
		it doesn't work because of eat_sleep_think taking a long time.
		Must move all this to monitoring */
		eat_sleep_think(philosopher);
		// Check if philosopher died
/* 		pthread_mutex_lock(&(philosopher->party->dying));
		printf("philo_routing inside while loop thread: party->someone_dead = %i\n", philosopher->party->someone_dead);
		pthread_mutex_lock(&(philosopher->party->dying));
		curr_time = get_current_time();
		if (curr_time
			- philosopher->time_last_ate >= philosopher->party->time_to_die)
		{
			printf("Philo [%d] starved to death\n", (int)philosopher->index);
			pthread_mutex_lock(&(philosopher->party->dying));
			philosopher->party->someone_dead = 1;
			pthread_mutex_unlock(&(philosopher->party->dying));
			// Signal the monitoring thread to check and quit if necessary
			pthread_mutex_lock(&(philosopher->party->dying)); // unneeded lines
			pthread_mutex_unlock(&(philosopher->party->dying));
			break ;
		} */
		// Check if someone is dead (with proper synchronization)
		pthread_mutex_lock(&(philosopher->party->dying));
		someone_dead = philosopher->party->someone_dead;
		pthread_mutex_unlock(&(philosopher->party->dying));
		if (someone_dead)
		{
			printf("Philo [%d] detects someone is dead and quits\n",
					(int)philosopher->index);
			break ;
		}
		// Check if this philosopher has eaten enough (with proper synchronization)
/* 		if (philosopher->party->number_of_meals >= 0 &&
			philosopher->meal_count >= philosopher->party->number_of_meals)
		{
			printf("Philo [%d] had enough meals\n", (int)philosopher->index);
			pthread_mutex_lock(&(philosopher->party->reporting_enough_meals));
			philosopher->party->number_of_philosophers_fed += 1;
			pthread_mutex_unlock(&(philosopher->party->reporting_enough_meals));
			break ;
		} */
	}
	return (NULL);
}
