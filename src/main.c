/*
 * main.c
 *
 *  Created on: Mar 24, 2018
 *      Author: akshit
 */

#include <stdio.h>
#include <stdlib.h>

/* Places of the game */
typedef enum {
	TAVERN,
	YARD
}Place_t;

/* Attribute for each knight */
typedef struct _knight{
	unsigned int xp;
	int sta;
	Place_t place;
}Knight_t;

/* Number of knights */
#define NUM_OF_KNIGHTS			(6)
#define NUM_OF_HOURS			(24)

int main()
{
	/* Allocate memory space for number of knights and init to 0*/
	Knight_t *knight = (Knight_t*)calloc(sizeof(Knight_t),NUM_OF_KNIGHTS);

	/* Process the game each hour */
	for(int i = 0; i < NUM_OF_HOURS; i++)
	{
		for (int knight_id = 0; knight_id < NUM_OF_KNIGHTS; knight_id++)
		{
			unsigned int which_place = rand() % 2;
			if(which_place == TAVERN)
			{
				knight[knight_id].sta += 1;
			}
			else
			{
				knight[knight_id].sta -= 1;
				knight[knight_id].xp += 1;
			}
		}
	}

	/* Count number of knights with XP greater than 3*/
	unsigned int count = 0;
	for(int i = 0; i < NUM_OF_KNIGHTS; i++)
	{
		if(knight[i].xp > 2){
			count += 1;
		}
	}

	/* Allocate bonus xp based on criteria */
	for(int i = 0; i < NUM_OF_KNIGHTS; i++)
	{
		if(count == 3)
		{
			if(knight[i].xp > 2)
				knight[i].xp += 5;
		}
		else if(count == 5)
		{
			if(knight[i].xp > 2)
				knight[i].xp += 10;
		}
		else if(count == 6)
		{
			knight[i].xp += 20;
		}
	}

	unsigned int sum_xp = 0;
	/* Print total xp of all knights */
	for(int i = 0;i < NUM_OF_KNIGHTS; i++)
	{
		sum_xp += knight[i].xp;

		/* Debug print */
		printf("Knight %d: XP:%u\tSTA:%d\n", i + 1, knight[i].xp, knight[i].sta);
	}
	printf("Total xp: %d\n", sum_xp);
	return 0;
}
