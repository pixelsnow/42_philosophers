/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   party_preparations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:12:38 by vvagapov          #+#    #+#             */
/*   Updated: 2023/07/29 22:39:39 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_return_value	prepare_philosopher(t_party	*party, unsigned int i)
{
	party->philosophers[i].fork_own = &party->forks[i];
	party->philosophers[i].fork_borrowed
		= &party->forks[(i + 1) % party->number_of_philosophers];
	party->philosophers[i].meal_count = 0; // TODO: replace with number_of_meals
	party->philosophers[i].party = party;
	if (pthread_mutex_init(&(party->philosophers[i].meal_update), NULL) != SUCCESS)
	 	return (MUTEX_FAIL);
	return (SUCCESS);
}

t_return_value allocate_memory(t_party *party)
{
	party->philosophers = malloc(sizeof(t_philosopher) *
		party->number_of_philosophers);
	if (party->philosophers == NULL)
		return (MALLOC_FAIL);
	party->forks = malloc(sizeof(pthread_mutex_t) *
		party->number_of_philosophers);
	if (party->forks == NULL)
		return (MALLOC_FAIL);
	return (SUCCESS);
}

t_return_value prepare_party(t_party *party)
{
	unsigned int		i;
	t_return_value		ret_val;
	
	ret_val = allocate_memory(party);
	if (ret_val != SUCCESS)
		return (ret_val);

	party->someone_dead = 0;
	
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (prepare_philosopher(party, i) != SUCCESS
				|| pthread_mutex_init(&(party->forks[i]), NULL) != SUCCESS)
			return (MUTEX_FAIL);
		i++;
	}
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (pthread_mutex_init(&(party->forks[i]), NULL) != SUCCESS)
			return (MUTEX_FAIL);
		i++;
	}
	if (pthread_mutex_init(&(party->guard), NULL) != SUCCESS)
		return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->dying), NULL) != SUCCESS)
		return (MUTEX_FAIL);
	if (pthread_mutex_init(&(party->printing), NULL) != SUCCESS)
		return (MUTEX_FAIL);
	return (SUCCESS);
}
