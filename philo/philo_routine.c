/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:12:53 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/04 21:24:24 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_return_value	philo_eat(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->fork_own);
	print_whats_happening(philosopher, "has taken a fork");
	pthread_mutex_lock(philosopher->fork_borrowed);
	print_whats_happening(philosopher, "has taken a fork");
	pthread_mutex_lock(&philosopher->time_last_ate_lock);
	philosopher->time_last_ate = get_current_time();
	pthread_mutex_unlock(&philosopher->time_last_ate_lock);
	print_whats_happening(philosopher, "is eating");
	if (custom_usleep(philosopher->party->time_to_eat, philosopher->party)
		!= SUCCESS)
	{
		pthread_mutex_unlock(philosopher->fork_own);
		pthread_mutex_unlock(philosopher->fork_borrowed);
		return (SOMEONE_DIED);
	}
	pthread_mutex_unlock(philosopher->fork_own);
	pthread_mutex_unlock(philosopher->fork_borrowed);
	pthread_mutex_lock(&philosopher->meal_count_lock);
	philosopher->meal_count++;
	pthread_mutex_unlock(&philosopher->meal_count_lock);
	return (SUCCESS);
}

static t_return_value	philo_sleep(t_philosopher *philosopher)
{
	print_whats_happening(philosopher, "is sleeping");
	return (custom_usleep(philosopher->party->time_to_sleep,
			philosopher->party));
}

static t_return_value	eat_sleep_think(t_philosopher *philosopher)
{
	t_return_value	ret_val;

	if (philosopher->party->number_of_philosophers == 1)
		return (SINGLE_PHILO_CASE);
	ret_val = philo_eat(philosopher);
	if (ret_val != SUCCESS)
		return (ret_val);
	ret_val = philo_sleep(philosopher);
	if (ret_val != SUCCESS)
		return (ret_val);
	print_whats_happening(philosopher, "is thinking");
	return (SUCCESS);
}

void	*philosopher_routine(void *philosopher_data)
{
	t_philosopher	*philosopher;
	t_return_value	ret_val;

	philosopher = (t_philosopher *)philosopher_data;
	pthread_mutex_lock(&(philosopher->party->guard));
	pthread_mutex_unlock(&(philosopher->party->guard));
	print_whats_happening(philosopher, "is thinking");
	if (philosopher->index % 2 == 1)
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
			break ;
	}
	return (NULL);
}
