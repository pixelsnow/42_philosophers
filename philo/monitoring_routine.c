#include "philosophers.h"

void	*monitoring_routine(void *party_data)
{
	t_party	*party;
	int		someone_dead;
	int		all_fed;

	party = (t_party *)party_data;
	printf("Monitoring thread is on\n");
	// The monitoring thread waits for the party to start
	pthread_mutex_lock(&(party->party_going_on));
	while (1)
	{
		// Check if someone is dead (with proper synchronization)
		pthread_mutex_lock(&(party->dying));
		someone_dead = party->someone_dead;
		pthread_mutex_unlock(&(party->dying));
		if (someone_dead)
		{
			printf("Monitoring detected DEATH: %u\n", party->someone_dead);
			break ;
		}
		// Check if everyone is fed already (with proper synchronization)
		if (party->number_of_meals >= 0)
		{
			pthread_mutex_lock(&(party->reporting_enough_meals));
			all_fed = party->number_of_philosophers_fed >= party->number_of_philosophers;
			pthread_mutex_unlock(&(party->reporting_enough_meals));
			if (all_fed)
			{
				printf("Monitoring detected all philosophers are fed\n");
				break ;
			}
		}
		// Delay between checks (with proper synchronization)
		usleep(party->time_to_die / 10);
	}

    // Set the someone_dead flag to stop and quit the party
    pthread_mutex_lock(&(party->dying));
    party->someone_dead = 1;
    pthread_mutex_unlock(&(party->dying));

    // Signal the main thread to continue and quit the party
    pthread_mutex_unlock(&(party->party_going_on));

    // Prevent further printing
    pthread_mutex_lock(&(party->printing));

    // Let the main thread continue
    printf("Monitoring thread is off\n");
	return (NULL);
}
//void	*monitoring_routine(void *party_data)
//{
//	t_party	*party;
//
//	party = (t_party *)party_data;
//	printf("Monitoring thread is on\n");
//	// This lock is supposed to hold the main thread while the party is ongoing
//	// but currently the problem is main thread needs to be held off with usleep
//	pthread_mutex_lock(&(party->party_going_on));
//	usleep(0.7 * party->time_to_die); //NEED TO REPLACE WITH custom usleep fct
//	while (1)
//	{
//		// BROKEN! party->someone_dead has garbage value inside.
//		// But supposedly we check if someone died.
//		pthread_mutex_lock(&(party->dying));
//		if (party->someone_dead)
//		{
//			printf("Monitoring detected DEATH: %u\n", party->someone_dead);
//			pthread_mutex_unlock(&(party->dying));
//			break ;
//		}
//		else
//			pthread_mutex_unlock(&(party->dying));
//
//		// NOT TESTED at all. We check if everyone is fed already
//		if (party->number_of_meals >= 0)
//		{
//			pthread_mutex_lock(&(party->reporting_enough_meals));
//			// number_of_meals_needed doesn't need a mutex Lionel said
//			if (party->number_of_philosophers_fed >= party->number_of_philosophers)
//			{
//				printf("Monitoring detected DEATH: %u\n", party->someone_dead);
//				pthread_mutex_unlock(&(party->reporting_enough_meals));
//				break ;
//			}
//			pthread_mutex_unlock(&(party->reporting_enough_meals));
//		}
//	}
//	// Prevent further printing
//	// (currently not working because we don't use this mutex anywhere else)
//	pthread_mutex_lock(&(party->printing));
//
//	// Let the main thread continue
//	printf("Monitoring thread is off\n");
//	pthread_mutex_unlock(&(party->party_going_on));
//	return (NULL);
//}
//