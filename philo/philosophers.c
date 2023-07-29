/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvagapov <vvagapov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 22:10:43 by vvagapov          #+#    #+#             */
/*   Updated: 2023/07/29 22:15:03 by vvagapov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	set_party_start_time(t_party *party)
{
	unsigned int	i;
	
	party->party_start_time = get_current_time();
	i = 0;
	while (i < party->number_of_philosophers)
	{
		party->philosophers[i].time_last_ate = party->party_start_time;
		i++;
	}
}

/*
 * This function runs the main party process:
 *		- starts philosopher threads and monitoring thread
 *		- joins all the threads when they are done running
 * Guard mutex is used to synchronise the threads.
 */
static t_return_value	run_party(t_party *party)
{
	pthread_mutex_lock(&(party->guard));
	if (start_philosophers(party) != SUCCESS || start_monitoring(party) != SUCCESS)
	{
		pthread_mutex_unlock(&(party->guard));
		return (THREAD_FAIL);
	}
	set_party_start_time(party);
	pthread_mutex_unlock(&(party->guard));
	if (join_monitoring_thread(party) != SUCCESS ||
		join_philosopher_threads(party, party->number_of_philosophers) != SUCCESS)
	{
		return (JOIN_FAIL);
	}
	return (SUCCESS);
}

/*
 * This is the main function of the program. It initializes the `t_party`
 * struct, parses the command-line arguments using `parse_args`, prepares the
 * party using `prepare_party`, runs the party using `run_party`, and finally
 * cleans up the resources using `clean_up`.
 */
int	main(int ac, char **av)
{
	t_party			party;
	t_return_value	ret_val;
	// rename to status or result

	ret_val = parse_args(&party, ac, av);
	if (ret_val != SUCCESS)
		return (ret_val);
	ret_val = prepare_party(&party);
	if (ret_val != SUCCESS)
		return (quit_gracefully(&party, ret_val));
	ret_val = run_party(&party);
	if (ret_val != SUCCESS)
	{
		join_philosopher_threads(&party, party.number_of_philosophers);
		join_monitoring_thread(&party);
	}
	clean_up(&party);
	return (SUCCESS);
}
