/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:12:53 by vvagapov          #+#    #+#             */
/*   Updated: 2023/07/29 23:43:34 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_return_value	eat_sleep_think(t_philosopher *philosopher)
{
	// TODO: refactor this
	if (philosopher->party->number_of_philosophers != 1)
	{
		pthread_mutex_lock(philosopher->fork_own);
		print_whats_happening(philosopher, "has taken a fork");
		pthread_mutex_lock(philosopher->fork_borrowed);
		print_whats_happening(philosopher, "has taken a fork");
		// Should this be 2 different mutexes instad?
		pthread_mutex_lock(&philosopher->meal_update);
		philosopher->time_last_ate = get_current_time();
		pthread_mutex_unlock(&philosopher->meal_update);
		print_whats_happening(philosopher, "is eating");
		if (custom_usleep(philosopher->party->time_to_eat, philosopher->party)
				!= SUCCESS)
		{
			printf("philo %d detected that someone died\n", philosopher->index + 1);
			pthread_mutex_unlock(philosopher->fork_own);
			pthread_mutex_unlock(philosopher->fork_borrowed);
			return (SOMEONE_DIED);
		}
		pthread_mutex_unlock(philosopher->fork_own);
		pthread_mutex_unlock(philosopher->fork_borrowed);
		pthread_mutex_lock(&philosopher->meal_update);
		philosopher->meal_count++;
		pthread_mutex_unlock(&philosopher->meal_update);
		print_whats_happening(philosopher, "is sleeping");
		if (custom_usleep(philosopher->party->time_to_sleep, philosopher->party)
				!= SUCCESS)
		{
			printf("philo %d detected that someone died\n", philosopher->index + 1);
			return (SOMEONE_DIED);
		}
		print_whats_happening(philosopher, "is thinking");
		return (SUCCESS);
	}
	return (SINGLE_PHILO_CASE);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	int				someone_dead;
	t_return_value	ret_val;

	someone_dead = 0;
	philosopher = (t_philosopher *)philosopher_data;
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	print_whats_happening(philosopher, "is thinking");
	if (philosopher->index % 2 == 0)
		custom_usleep(philosopher->party->time_to_eat / 10, philosopher->party);
	while (1)
	{
		ret_val = eat_sleep_think(philosopher);
		if (ret_val == SINGLE_PHILO_CASE)
		{
			print_whats_happening(philosopher, "has taken a fork");
			break ;
		}
		if (ret_val == SOMEONE_DIED)
		{
			break ;
		}
	}
	return (NULL);
}
