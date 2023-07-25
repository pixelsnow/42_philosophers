#include "philosophers.h"

//
///**
// * @brief Function to acquire both forks
// *
// * @param philosopher
// */
//void	take_forks(t_philosopher *philosopher)
//{
//	int	count;
//
//	/* Are the loop, count,
//		if conditions use valid? If lock prevents to go further ino
// * thread, it hangs until own fork is unlocked, then wait for second,
//	and never
// * go through the while loop again */
//	count = 0;
//	while (count < 2)
//	{
//		if (pthread_mutex_lock(philosopher->fork_own) == SUCCESS)
//		{
//			printf("Philo [%d] has taken a fork [own]\n",
//					(int)philosopher->index);
//			count++;
//		}
//		if (pthread_mutex_lock(philosopher->fork_borrowed) == SUCCESS)
//		{
//			printf("Philo [%d] has taken a fork [borrowed]\n",
//					(int)philosopher->index);
//			count++;
//		}
//		usleep(100);
//	}
//}

// Combined eat, sleep, and think function
void	eat_sleep_think(t_philosopher *philosopher)
{
	//take_forks(philosopher);
	pthread_mutex_lock(philosopher->fork_own);
	printf("Philo [%d] has taken a fork [own]\n", (int)philosopher->index);
	pthread_mutex_lock(philosopher->fork_borrowed);
	printf("Philo [%d] has taken a fork [borrowed]\n", (int)philosopher->index);
	//pthread_mutex_lock(&(philosopher->party->printing));
	philosopher->time_last_ate = get_current_time();
	philosopher->meal_count += 1;
	//pthread_mutex_unlock(&(philosopher->party->printing));
	printf("Philo [%d] is eating\n", (int)philosopher->index);
	//May need to replace usleep with custom usleep()
	usleep(philosopher->party->time_to_eat);
	pthread_mutex_unlock(philosopher->fork_own);
	pthread_mutex_unlock(philosopher->fork_borrowed);
	printf("Philo [%d] is sleeping\n", (int)philosopher->index);
	//May need to replace usleep with custom usleep()
	usleep(philosopher->party->time_to_sleep);
	printf("Philo [%d] is thinking\n", (int)philosopher->index);
}
void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher		*philosopher;
	unsigned long long	curr_time;
	int					someone_dead;


	someone_dead = 0;
	philosopher = (t_philosopher *)philosopher_data;
	// Ensure synchronization with monitoring thread
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	// Additional checks before the main loop
	if (philosopher->index % 2 == 0)
	{
		printf("Philo [%d] is thinking\n", (int)philosopher->index);
		usleep(philosopher->party->time_to_eat / 10);
	}
	while (1)
	{
		// Perform the eat_sleep_think routine
		eat_sleep_think(philosopher);
		// Check if philosopher died
		curr_time = get_current_time();
		if (curr_time
			- philosopher->time_last_ate > philosopher->party->time_to_die)
		// >= or > ?
		{
			printf("Philo [%d] starved to death\n", (int)philosopher->index);
			pthread_mutex_lock(&(philosopher->party->dying));
			philosopher->party->someone_dead = 1;
			pthread_mutex_unlock(&(philosopher->party->dying));
			// Signal the monitoring thread to check and quit if necessary
            pthread_mutex_lock(&(philosopher->party->dying));
            pthread_mutex_unlock(&(philosopher->party->dying));
			break ;
		}
		// Check if someone is dead (with proper synchronization)
		//pthread_mutex_lock(&(philosopher->party->dying));
		//someone_dead = philosopher->party->someone_dead;
		//pthread_mutex_unlock(&(philosopher->party->dying));
		if (someone_dead)
		{
			printf("Philo [%d] detects someone is dead and quits\n",
					(int)philosopher->index);
			break ;
		}
		// Check if this philosopher has eaten enough (with proper synchronization)
		if (philosopher->party->number_of_meals >= 0 &&
			philosopher->meal_count >= philosopher->party->number_of_meals)
		{
			printf("Philo [%d] had enough meals\n", (int)philosopher->index);
			pthread_mutex_lock(&(philosopher->party->reporting_enough_meals));
			philosopher->party->number_of_philosophers_fed += 1;
			pthread_mutex_unlock(&(philosopher->party->reporting_enough_meals));
			break ;
		}
	}
	return (NULL);
}
//void	*philosopher_routine(void *philosopher_data)
//{
//	t_philosopher		*philosopher;
//	unsigned long long	curr_time;
//
//	philosopher = (t_philosopher *)philosopher_data;
//	// Just garbage printfs
//	printf("Philo [%d] routine on\n", (int)philosopher->thread);
//	printf("time_last_ate: %llu\n", philosopher->time_last_ate);
//	printf("Philo [%d] waiting for guard...\n", (int)philosopher->thread);
//	// Syncing threads
//	pthread_mutex_lock(&(philosopher->party->guard));
//	pthread_mutex_lock(&(philosopher->party->dying));
//	philosopher->time_last_ate = get_current_time();
//	pthread_mutex_unlock(&(philosopher->party->dying));
//	pthread_mutex_unlock(&(philosopher->party->guard));
//	printf("Philo [%d] let in by guard...\n", (int)philosopher->thread);
//	if (philosopher->index % 2 == 0)
//	{
//		printf("is thinking\n");
//		usleep(philosopher->party->time_to_eat / 10);
//	}
//	while (1)
//	{
//		//eat_sleep_think(philosopher);
//		//pthread_mutex_lock(&(philosopher->party->dying));
//		//if (philosopher->party->someone_dead)
//		//{
//		//	pthread_mutex_unlock(&(philosopher->party->dying));
//		//	break ;
//		//}
//		//pthread_mutex_unlock(&(philosopher->party->dying));
//		// 1. Check time since last meal and die if starved
//		curr_time = get_current_time();
//		if (curr_time
//			- philosopher->time_last_ate > philosopher->party->time_to_die)
//		{
//			// Idk if these time calculations are actually correct
//			printf("Philo [%d] starved to death\n", (int)philosopher->thread);
//			pthread_mutex_lock(&(philosopher->party->dying));
//			philosopher->party->someone_dead = 1;
//			pthread_mutex_unlock(&(philosopher->party->dying));
//			break ;
//		}
//		// Probably unnecessary,
//		//since monitoring can check this and join all threads
//		// 2. If there was a death, stop
//		pthread_mutex_lock(&(philosopher->party->dying));
//		if (philosopher->party->someone_dead)
//		{
//			printf("Philo [%d] detects someone is dead and quits\n",
//			(int)philosopher->thread);
//			pthread_mutex_unlock(&(philosopher->party->dying));
//			break ;
//		}
//		pthread_mutex_unlock(&(philosopher->party->dying));
//		// 3. If this philo ate enough, stop
//		if (philosopher->party->number_of_meals >= 0
//			&& philosopher->meal_count >= philosopher->party->number_of_meals)
//		{
//			printf("Philo [%d] had enough meals\n", (int)philosopher->thread);
//			pthread_mutex_lock(&(philosopher->party->reporting_enough_meals));
//			philosopher->party->number_of_philosophers_fed += 1;
//			pthread_mutex_unlock(&(philosopher->party->reporting_enough_meals));
//			break ;
//		}
//	}
//	return (NULL);
//}
//
//////////////////////////////////////////////////////////////////////////////////
////void	eat_sleep_think(t_philosopher	*philosopher)
////{
////	// EAT
////	// delay if even
////	if (philosopher->index % 2 == 0)
////		usleep(philosopher->party->time_to_eat / 10);
////	pthread_mutex_lock(philosopher->fork_own);
////	pthread_mutex_lock(philosopher->fork_borrowed);
////	printf("Philo [%d] started eating\n", (int)philosopher->thread);
////	philosopher->time_last_ate = get_current_time();
////	philosopher->meal_count += 1;
////	usleep(philosopher->party->time_to_eat);
////	pthread_mutex_unlock(philosopher->fork_own);
////	pthread_mutex_unlock(philosopher->fork_borrowed);
////	// SLEEP
////	usleep(philosopher->party->time_to_sleep);
////}
////
////void	*philosopher_routine(void *philosopher_data)
////{
////	t_philosopher		*philosopher;
////	unsigned long long	curr_time;
////
////	philosopher = (t_philosopher *)philosopher_data;
////	// Just garbage printfs
////	printf("Philo [%d] routine on\n", (int)philosopher->thread);
////	printf("time_last_ate: %llu\n", philosopher->time_last_ate);
////	printf("Philo [%d] waiting for guard...\n", (int)philosopher->thread);
////	// Syncing threads
////	pthread_mutex_lock(&(philosopher->party->guard));
////	pthread_mutex_unlock(&(philosopher->party->guard));
////	printf("Philo [%d] let in by guard...\n", (int)philosopher->thread);
////	while (1)
////	{
////		// 1. Check time since last meal and die if starved
////		curr_time = get_current_time();
////		if (curr_time - philosopher->time_last_ate
////			> philosopher->party->time_to_die)
////		{
////			// Idk if these time calculations are actually correct
////			printf("Philo [%d] starved to death\n",
	//				(int)philosopher->thread);
////			pthread_mutex_lock(&(philosopher->party->dying));
////			philosopher->party->someone_dead = 1;
////			pthread_mutex_unlock(&(philosopher->party->dying));
////			break ;
////		}
////
////		// Probably unnecessary,
/////		since monitoring can check this and join all threads
////		// 2. If there was a death, stop
////		pthread_mutex_lock(&(philosopher->party->dying));
////		if (philosopher->party->someone_dead)
////		{
////			printf("Philo [%d] detects someone is dead and quits\n",
//////			(int)philosopher->thread);
////			pthread_mutex_unlock(&(philosopher->party->dying));
////			break ;
////		}
////		pthread_mutex_unlock(&(philosopher->party->dying));
////
////		// 3. If this philo ate enough, stop
////		if (philosopher->party->number_of_meals >= 0
////			&& philosopher->meal_count >= philosopher->party->number_of_meals)
////		{
////			printf("Philo [%d] had enough meals\n",
////					(int)philosopher->thread);
////			pthread_mutex_lock(&(philosopher->party->reporting_enough_meals));
////			philosopher->party->number_of_philosophers_fed += 1;
////			pthread_mutex_unlock(&(philosopher->party->reporting_enough_meals));
////			break ;
////		}
////		// TODO: figure out proper eat_sleep_think routine
////		eat_sleep_think(philosopher);
////	}
////	printf("Philo [%d] quitting\n", (int)philosopher->thread);
////	return (NULL);
////}