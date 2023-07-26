#include "philosophers.h"

/**
 * @brief Start a philosopher thread.
 *
 * This function is responsible for creating and starting a new thread for a
 * philosopher with the specified index `i`. It sets the philosopher's index
 * and then creates a new thread using `pthread_create`, passing the
 * `philosopher_routine` function as the thread's entry point. The philosopher's
 * data is passed to the thread as the argument using `&(party->philosophers[i])`.
 * If the thread creation is successful, the function returns `SUCCESS`, otherwise,
 * it prints an error message and returns `ERROR`.
 * 
 * @param party A pointer to the `t_party` struct representing the philosopher party.
 * @param i The index of the philosopher to start the thread for.
 * @return Returns `SUCCESS` if the thread is created and started successfully, otherwise `ERROR`.
 */
t_return_value	start_philosopher(t_party	*party, unsigned int i)
{
	// REMEMEBER: returns -1 on error, 0 on success
//	printf("start_philosopher\n");
	party->philosophers[i].index = i;
	if (pthread_create(
		&(party->philosophers[i].thread), 
		NULL, 
		philosopher_routine,
		(void *)&(party->philosophers[i])) != 0)
		{
			printf("Failed to create a philo thread\n");
			return (ERROR);
		}
	return (SUCCESS);
}

/**
 * @brief Start the monitoring thread.
 *
 * This function is responsible for creating and starting the monitoring thread
 * for the philosopher party. It uses `pthread_create` to create a new thread,
 * passing the `monitoring_routine` function as the thread's entry point. The
 * `party->monitoring_thread` is passed as the argument to the thread using
 * `(void *)&(party->monitoring_thread)`. If the thread creation is successful,
 * the function returns `SUCCESS`, otherwise, it prints an error message and
 * returns `ERROR`.
 * 
 * @param party A pointer to the `t_party` struct representing the philosopher party.
 * @return Returns `SUCCESS` if the monitoring thread is created and started successfully, otherwise `ERROR`.
 */
t_return_value	start_monitoring(t_party	*party)
{
//	printf("monitor_party\n");
	if (pthread_create(
		&(party->monitoring_thread),
		NULL,
		monitoring_routine,
		(void *)party) != 0)
		{
			printf("Failed to create a monitoring thread\n");
			return (ERROR);
		}
	return (SUCCESS);
}