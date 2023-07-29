#include "philosophers.h"


static t_return_value	someone_starved(t_party *party)
{
	unsigned int		i;
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
			return (SOMEONE_DIED);
		}
		i++;
	}
	return (LIFE_GOES_ON);
}

static t_return_value	everyone_is_fed(t_party *party)
{
	unsigned int	i;
	int				philo_meal_count;

	if (party->number_of_meals < 0)
		return (LIFE_GOES_ON);
	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_lock(&(party->philosophers[i].meal_update));
		philo_meal_count = party->philosophers[i].meal_count;
		pthread_mutex_unlock(&(party->philosophers[i].meal_update));
		if (philo_meal_count < party->number_of_meals)
			return (LIFE_GOES_ON);
		i++;
	}
	return (EVERYONE_IS_FED);
}

void	*monitoring_routine(void *party_data)
{
	t_party	*party;

	party = (t_party *)party_data;
	while (1)
	{
		if (someone_starved(party) == SOMEONE_DIED)
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			break ;
		}
		if (everyone_is_fed(party) == EVERYONE_IS_FED)
		{
			pthread_mutex_lock(&(party->dying));
			party->someone_dead = 1;
			pthread_mutex_unlock(&(party->dying));
			break ;
		}
		custom_usleep(party->time_to_die / 10, party);
	}
	return (NULL);
}
