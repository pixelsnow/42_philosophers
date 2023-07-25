#include "philosophers.h"

// Function to join philosopher threads

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
 */
void	join_philosopher_threads(t_party *party)
{
	unsigned int	i;

	for (i = 0; i < party->number_of_philosophers; i++)
	{
		printf("Joining philosopher thread %u\n", i);
		if (pthread_join(party->philosophers[i].thread, NULL) != SUCCESS)
		{
			printf("Failed to join philosopher thread %u\n", i);
		}
		printf("Joined philosopher thread %u -> SUCCESS\n", i);
	}
}

/**
 * @brief Joins the monitoring thread.
 *
 * This function is responsible for joining the monitoring thread created by
 * the party. It calls `pthread_join` on the `party->monitoring_thread`. If an
 * error occurs during joining, it prints an error message.
 *
 * @param party Pointer to the `t_party` struct holding party information
 */
void	join_monitoring_thread(t_party *party)
{
	printf("Joining monitoring thread\n");
	if (pthread_join(party->monitoring_thread, NULL) != SUCCESS)
	{
		printf("Failed to join monitoring thread\n");
	}
	printf("Joined monitoring thread -> SUCCESS\n");
}

// Main thread that runs the party and starts all the threads
/**
 * @brief	Function to run the philosopher party and start all the threads
 * 
 * This function is the main thread that organizes the philosopher party and
 * starts all the philosopher threads along with the monitoring thread. It
 * locks the `party->guard` mutex to ensure that all threads start
 * simultaneously and then proceeds to start the philosopher threads using the
 * `start_philosopher` function. After starting all the threads, it unlocks the
 * `party->guard` mutex and waits for a short period to ensure that the threads
 * have been successfully started before proceeding to the next step.
 * 
 * The main thread then proceeds to start the monitoring thread using the
 * `start_monitoring` function. It prints the message "Main thread waiting..."
 * and waits briefly using `usleep(1000)` to allow the monitoring thread and
 * philosopher threads to execute their routines and interact with each other.
 * After waiting, the main thread locks the `party->party_going_on` mutex,
 * which will be unlocked by the monitoring thread when it is done. This is
 * done to prevent the main thread from exiting prematurely.
 * 
 * Note: There is an issue in the code where the main thread does not wait for
 * the monitoring and philosopher threads to stop properly. It currently uses
 * `usleep(1000)` to wait for a brief period, but this is not a robust solution
 * and may cause synchronization problems. A more reliable solution is needed
 * to ensure that the main thread waits for all threads to finish before
 * proceeding to cleanup.
 *
 * @param party	Pointer to the `t_party` struct holding party information
 * @return int	`SUCCESS` if the party runs successfully, otherwise `ERROR`
 */
int	run_party(t_party *party)
{
	unsigned int	i;

	// Start all threads simultaneously
	pthread_mutex_lock(&(party->guard));
	// (Make this a separate function maybe)
	i = 0;
	while (i < party->number_of_philosophers)
	{
		if (start_philosopher(party, i) == ERROR)
		{
			//How do we set the end of party here?
			pthread_mutex_unlock(&(party->guard));
			break ;
		}
		i++;
	}
	start_monitoring(party);
	printf("unlocking guard...\n");
	pthread_mutex_unlock(&(party->guard));
	/* Issue now is that main thread doesn't wait for monitoring and philosophers to stop.
	if we remove usleep(1000) Main snatches party->party_going_on before monitoring gets to it
	and therefore gets to cleanup while party is still ongoing. */
	//printf("Main thread waiting...\n");
	//usleep(1000);
	//pthread_mutex_lock(&(party->party_going_on));
	//pthread_mutex_unlock(&(party->party_going_on));
	printf("Main thread waiting...\n");
	join_philosopher_threads(party);
	join_monitoring_thread(party);
	printf("Main thread continuing...\n");
	/* I'm wondering if the solution is that monitoring should trigger cleanup and everything instead,
	and the main thread should just be allowed to die in peace? Or is that illegal */
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
	t_party	party;

	if (parse_args(&party, ac, av))
	{
		return (ERROR);
	}
	// Init
	if (prepare_party(&party))
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
