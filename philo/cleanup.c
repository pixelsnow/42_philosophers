/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:10:50 by vvagapov          #+#    #+#             */
/*   Updated: 2023/08/04 21:27:09 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_memory(t_party	*party)
{
	if (party->philosophers)
		free(party->philosophers);
	if (party->forks)
		free(party->forks);
}

void	destroy_mutexes(t_party	*party)
{
	unsigned int	i;

	i = 0;
	while (i < party->number_of_philosophers)
	{
		pthread_mutex_destroy(&(party->forks[i]));
		pthread_mutex_destroy(&(party->philosophers[i].meal_count_lock));
		pthread_mutex_destroy(&(party->philosophers[i].time_last_ate_lock));
		i++;
	}
	pthread_mutex_destroy(&(party->guard));
	pthread_mutex_destroy(&(party->party_over_lock));
}

t_return_value	handle_error(t_party *party, t_return_value	ret_val)
{
	if (ret_val == MALLOC_FAIL)
	{
		free_memory(party);
	}
	else if (ret_val == MUTEX_FAIL)
	{
		destroy_mutexes(party);
		free_memory(party);
	}
	else if (ret_val == THREAD_FAIL)
	{
		join_philosopher_threads(party);
		join_monitoring_thread(party);
	}
	return (ret_val);
}

void	clean_up(t_party	*party)
{
	destroy_mutexes(party);
	free_memory(party);
}
