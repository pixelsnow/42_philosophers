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

	party = (t_party *)party_data;
	while (1)
	{
		if (someone_starved(party))
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			printf("Monitoring detected DEATH\n");
			break ;
		}
		if (everyone_is_fed(party))
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			printf("Monitoring detected ALL PHILOS ARE FED\n");
			break ;
		}
		custom_usleep(party->time_to_die / 10, party);
	}
    pthread_mutex_lock(&(party->printing));
	return (NULL);
}
