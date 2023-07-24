#include "philosophers.h"

void	*monitoring_routine(void *party_data)
{
	t_party	*party;

	party = (t_party *)party_data;
	printf("Monitoring thread is on\n");
	// This lock is supposed to hold the main thread while the party is ongoing
	// but currently the problem is main thread needs to be held off with usleep
	pthread_mutex_lock(&(party->party_going_on));
	while (1)
	{
		// BROKEN! party->someone_dead has garbage value inside.
		// But supposedly we check if someone died.
		pthread_mutex_lock(&(party->dying));
		if (party->someone_dead)
		{
			printf("Monitoring detected DEATH: %u\n", party->someone_dead);
			pthread_mutex_unlock(&(party->dying));
			break;
		}
		else
			pthread_mutex_unlock(&(party->dying));
		
		// NOT TESTED at all. We check if everyone is fed already
		if (party->number_of_meals >= 0)
		{
			pthread_mutex_lock(&(party->reporting_enough_meals));
			// number_of_meals_needed doesn't need a mutex Lionel said
			if (party->number_of_philosophers_fed >= party->number_of_philosophers)
			{
				printf("Monitoring detected DEATH: %u\n", party->someone_dead);
				pthread_mutex_unlock(&(party->reporting_enough_meals));
				break;
			}
			pthread_mutex_unlock(&(party->reporting_enough_meals));
		}
	}
	// Prevent further printing
	// (currently not working because we don't use this mutex anywhere else)
	pthread_mutex_lock(&(party->printing));

	// Let the main thread continue
	printf("Monitoring thread is off\n");
	pthread_mutex_unlock(&(party->party_going_on));
	return (NULL);
}
