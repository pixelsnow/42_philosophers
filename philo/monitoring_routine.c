#include "philosophers.h"
/**
 * @brief Checks if any philosopher has starved.
 *
 * This function checks each philosopher in the party and determines if any
 * philosopher has starved. It calculates the time since the philosopher last ate
 * and compares it with the time_to_die value to detect starvation.
 *
 * @param party A pointer to the t_party structure representing the philosopher party.
 * @return 1 if any philosopher has starved, 0 otherwise.
 */
int someone_starved(t_party	*party)
{
	unsigned int	i;
	unsigned long long	curr_time;
	unsigned long long	time_philo_last_ate;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		curr_time = get_current_time();
		pthread_mutex_lock(&(party->philosophers[i].meal_update));
		time_philo_last_ate = party->philosophers[i].time_last_ate;
		pthread_mutex_unlock(&(party->philosophers[i].meal_update));
		if (curr_time - time_philo_last_ate >= party->time_to_die)
		{
			print_whats_happening(party->philosophers, "died");
			return (1);
		}
		i++;
	}
	return (0);
}
/**
 * @brief Checks if every philosopher has reached the number of meals specified.
 *
 * This function checks each philosopher's meal count and compares it with the
 * number_of_meals value to determine if every philosopher has eaten the specified
 * number of meals.
 *
 * @param party A pointer to the t_party structure representing the philosopher party.
 * @return 1 if every philosopher has eaten the specified number of meals or if the
 *         number_of_meals value is negative, 0 otherwise.
 */
int	everyone_is_fed(t_party	*party)
{
	unsigned int	i;
	int philo_meal_count;

	if (party->number_of_meals < 0)
		return (0);
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_lock(&(party->philosophers[i].meal_update));
		philo_meal_count = party->philosophers[i].meal_count;
		pthread_mutex_unlock(&(party->philosophers[i].meal_update));
		if (philo_meal_count < party->number_of_meals)
			return (0);
		i++;
	}
	return (1);
}

void	*monitoring_routine(void *party_data)
{
	t_party	*party;
/* 	int		someone_dead; */

	party = (t_party *)party_data;
	printf("Monitoring thread is on\n");
	// The monitoring thread waits for the party to start
	pthread_mutex_lock(&(party->party_going_on));
/* 	printf("Monitoring_routine before while loop thread: party->someone_dead = %i\n",
			party->someone_dead); */
	while (1)
	{
		// Check if someone is dead (with proper synchronization)
		// printf("Monitoring_routine inside while loop thread, loop start\n");
		/* pthread_mutex_lock(&(party->dying));
		someone_dead = party->someone_dead;
		pthread_mutex_unlock(&(party->dying)); */
		if (someone_starved(party))
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			printf("Monitoring detected DEATH\n");
			break ;
		}
		// Check if everyone is fed already (with proper synchronization)
		if (everyone_is_fed(party))
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			/* pthread_mutex_lock(&(party->reporting_enough_meals));
			all_fed = party->number_of_philosophers_fed >= party->number_of_philosophers;
			printf("Monitoring number_of_philosophers_fed: %u\n", party->number_of_philosophers_fed);
			printf("Monitoring number_of_philosophers: %u\n", party->number_of_philosophers);
			pthread_mutex_unlock(&(party->reporting_enough_meals));
			printf("All fed? %i\n", all_fed); */
			printf("Monitoring detected ALL PHILOS ARE FED\n");
			break ;
		}
		// Delay between checks (with proper synchronization)
		custom_usleep(party->time_to_die / 10, party);
	}

    // Prevent further printing
    pthread_mutex_lock(&(party->printing));
    // Let the main thread continue
	pthread_mutex_unlock(&(party->party_going_on));
    printf("Monitoring thread is off\n");
	return (NULL);
}
