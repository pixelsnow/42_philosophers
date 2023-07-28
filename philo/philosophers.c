#include "philosophers.h"

/**
 * @brief Joins all philosopher threads.
 *
 * This function is responsible for joining all the philosopher threads created
 * by the party. It iterates through the `party->philosophers` array and calls
 * `pthread_join` on each philosopher's thread. If an error occurs during
 * joining, it prints an error message indicating which philosopher thread
 * failed to join.
 *
 * @param party Pointer to the `t_party` struct holding party information
 * @return Returns SUCCESS if all philosopher threads are joined successfully,
	otherwise FAILED.
 */
static t_return_value	join_philosopher_threads(t_party *party)
{
	unsigned int	i;

	for (i = 0; i < party->number_of_philosophers; i++)
	{
		if (pthread_join(party->philosophers[i].thread, NULL) != SUCCESS)
		{
			printf("Failed to join philosopher thread %u\n", i);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

/**
 * @brief Joins the monitoring thread.
 *
 * This function is responsible for joining the monitoring thread created by
 * the party. It calls `pthread_join` on the `party->monitoring_thread`. If an
 * error occurs during joining, it prints an error message.
 *
 * @param party Pointer to the `t_party` struct holding party information
 * @return Returns SUCCESS if all philosopher threads are joined successfully,
	otherwise FAILED.
 */
static t_return_value	join_monitoring_thread(t_party *party)
{
	if (pthread_join(party->monitoring_thread, NULL) != SUCCESS)
	{
		printf("Failed to join monitoring thread\n");
		return (ERROR);
	}
	return (SUCCESS);
}

static int	run_party(t_party *party)
{
	unsigned int	i;

	pthread_mutex_lock(&(party->guard));
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (start_philosopher(party, i) == ERROR)
		{
			pthread_mutex_unlock(&(party->guard));
			break ;
		}
		i++;
	}
	party->party_start_time = get_current_time();
	i = 0;
	while (i < party->number_of_philosophers)
	{
		party->philosophers[i].time_last_ate = party->party_start_time;
		i++;
	}
	start_monitoring(party);
	pthread_mutex_unlock(&(party->guard));
	join_monitoring_thread(party);
	join_philosopher_threads(party);
	return (SUCCESS);
}

/**
 * @brief Entry point of the program.
 *
 * This is the main function of the program. It initializes the `t_party`
 * struct, parses the command-line arguments using `parse_args`, prepares the
 * party using `prepare_party`, runs the party using `run_party`, and finally
 * cleans up the resources using `clean_up`.
 *
 * @param ac Number of command-line arguments
 * @param av Array of command-line argument strings
 * @return int `SUCCESS` if the program runs successfully, otherwise `ERROR`
 */
int	main(int ac, char **av)
{
	t_party			party;
	t_return_value	ret_val;

	ret_val = parse_args(&party, ac, av);
	if (ret_val != SUCCESS)
	{
		return (ret_val);
	}
	// Init
	if (prepare_party(&party) != SUCCESS)
	{
		// prepare_party currenty actually always returns success (i think)
		return (ERROR);
	}
	if (run_party(&party))
	{
		// run_party currenty actually always returns success (i think)
		return (ERROR);
	}
	clean_up(&party);
	return (SUCCESS);
}
