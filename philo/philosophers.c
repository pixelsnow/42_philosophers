#include "philosophers.h"

// Main thread that runs the party and starts all the threads
int run_party(t_party *party)
{
	unsigned int		i;

	// Start all threads simultaneously
	pthread_mutex_lock(&(party->guard));
	// (Make this a separate function maybe)
	i = 0;
	while (i < party->number_of_philosophers)
	{
		start_philosopher(party, i);
		i++;
	}
	start_monitoring(party);
	
	printf("unlocking guard...\n");
	pthread_mutex_unlock(&(party->guard));

	/* Issue now is that main thread doesn't wait for monitoring and philosophers to stop.
	if we remove usleep(1000) Main snatches party->party_going_on before monitoring gets to it
	and therefore gets to cleanup while party is still ongoing. */
	printf("Main thread waiting...\n");
	usleep(1000);
	pthread_mutex_lock(&(party->party_going_on));
	pthread_mutex_unlock(&(party->party_going_on));
	printf("Main thread continuing...\n");
	/* I'm wondering if the solution is that monitoring should trigger cleanup and everything instead,
	and the main thread should just be allowed to die in peace? Or is that illegal */
	
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_party	party;
	
	if (parse_args_old(&party, ac, av))
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
	return(SUCCESS);
}
