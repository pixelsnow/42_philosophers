#include "philosophers.h"

/**
 * Start a philosopher thread.
 *
 * This function is responsible for creating and starting a new thread for a
 * philosopher with the specified index `i`. It sets the philosopher's index
 * and then creates a new thread using `pthread_create`, passing the
 * `philosopher_routine` function as the thread's entry point. The philosopher's
 * data is passed to the thread as the argument using `&(party->philosophers[i])`.
 */
t_return_value	start_philosopher(t_party	*party, unsigned int i)
{
	party->philosophers[i].index = i;
	if (pthread_create(&(party->philosophers[i].thread), NULL, \
		philosopher_routine, (void *)&(party->philosophers[i])) != 0)
	{
		printf("Failed to create a philo thread\n");
		return (THREAD_FAIL);
	}
	return (SUCCESS);
}

/**
 * Start the monitoring thread.
 *
 * This function is responsible for creating and starting the monitoring thread
 * for the philosopher party. It uses `pthread_create` to create a new thread,
 * passing the `monitoring_routine` function as the thread's entry point. The
 * `party->monitoring_thread` is passed as the argument to the thread using
 * `(void *)&(party->monitoring_thread)`. If the thread creation is successful,
 * the function returns `SUCCESS`, otherwise, it prints an error message and
 * returns `ERROR`.
 */
t_return_value	start_monitoring(t_party	*party)
{
	if (pthread_create(&(party->monitoring_thread), NULL, \
		monitoring_routine, (void *)party) != 0)
	{
		printf("Failed to create a monitoring thread\n");
		return (THREAD_FAIL);
	}
	return (SUCCESS);
}