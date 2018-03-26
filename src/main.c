/*
 * main.c
 *
 *  Created on: Mar 24, 2018
 *      Author: akshit
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#define WEEK
//#define WEEK2
//#define WEEK3
//#define MONTH
//#define DEBUG printf

/* Places of the game */
typedef enum {
	TAVERN,
	YARD,
	ROUND_TABLE
}Place_t;

/* Attribute for each knight */
typedef struct _knight{
	uint32_t xp;
	int32_t sta;
	uint32_t round_table_hours;
}Knight_t;

/* Definitions */
#define NUM_OF_KNIGHTS	(13) /* Email 1: changed num of knights from 6 to 12 */ /* Email 6: + 1 for king */
#define NUM_OF_HOURS	(24)
#define NUM_OF_DAYS		(1)

/* Email 5: Compile time switches to run the game more if desired */
#ifdef WEEK
#define NUM_OF_DAYS		(7)
#endif
#ifdef WEEK2
#define NUM_OF_DAYS		(14)
#endif
#ifdef WEEK3
#define NUM_OF_DAYS		(21)
#endif
#ifdef MONTH
#define NUM_OF_DAYS		(30)
#endif

/* Function Declarations */
static inline void PrintXP(Knight_t *knight);
static inline void AllocateBonus(Knight_t *knight, uint32_t count);
static inline uint32_t CountNumOfKnightsWithGoodXp(Knight_t *knight);
static inline uint8_t isBonusAllowed(uint32_t count);
static inline void UpdateKnights(Knight_t *knight);

int main()
{
	/* Allocate memory space for number of knights and init to 0 */
	Knight_t *knight = (Knight_t*)calloc(sizeof(Knight_t),NUM_OF_KNIGHTS);

	/* Flag to check which knight has negative stamina
	 * Currently only supporting upto max 64 knights
	 * */
    uint64_t knight_flag = 0;


    for(uint32_t day = 0; day < NUM_OF_DAYS; day++)
    {
    	/* ASSUMPTION: After each day knights gain 2 stamina pts (Just to make game little fun) as they will rest
    	 * This also initialize knights with some stamina to start with (They are all charged up) to trainS
    	 * */
    	UpdateKnights(knight);

    	/* Process the game each hour */
		for(uint32_t hour = 0; hour < NUM_OF_HOURS; hour++)
		{
			for (int knight_id = 0; knight_id < NUM_OF_KNIGHTS; knight_id++)
			{
				uint8_t which_place = rand() % 3;
				if(which_place == TAVERN)
				{
					knight[knight_id].sta += 1;
				}
				else if(which_place == YARD)
				{
					/*Email 4: No Xp for entire day if they try to train more */
					if(knight[knight_id].sta < 0)
					{
						/* set the bit corresponding to knight id */
						knight_flag |= (1 << knight_id);
					}
					/*
					 * email 2: knight should not earn xp for entire day when stamina is below 0
					 * & ideally stamina should not go below zero
					 */
					else if(knight[knight_id].sta >= 0 &&
							!(knight_flag & (1 << knight_id)) &&
							knight[knight_id].round_table_hours > 2)
					{
						knight[knight_id].sta -= 1;
						knight[knight_id].xp += 1;
					}

					/* Debug print */
#ifdef DEBUG
					DEBUG("[DEBUG] At %d Hour in %d Knight: %d, Stamina: %d, XP: %d, Round table hours: %d flag: %u\n",
							i,which_place,knight_id,knight[knight_id].sta,knight[knight_id].xp,
							knight[knight_id].round_table_hours, knight_flag);
#endif
				}
				else /* Round Table */
				{
					knight[knight_id].round_table_hours += 1;
				}
			}
		}

		/* Count number of knights with XP greater than num of knights/2 */
		uint32_t count = CountNumOfKnightsWithGoodXp(knight);

		/* Allocate bonus xp based on criteria */
		if(isBonusAllowed(count))
		{
			AllocateBonus(knight,count);
		}

		/* Print total XP after each day */
		printf("After Day %d\n",day+1);
		PrintXP(knight);
		printf("\n");
    }

	/* Total Xp earned */
    printf("Total XP Earned in the Game:\n");
	PrintXP(knight);

	/* Free the memory space */
	free(knight);
	return 0;
}

/* Utility Function to update knights after each day */
static inline void UpdateKnights(Knight_t *knight)
{
	for(uint32_t knight_id = 0; knight_id < NUM_OF_KNIGHTS; knight_id++)
	{
		knight[knight_id].sta += 2;
		knight[knight_id].round_table_hours = 0;
	}
}

/* Utility Function to allocate Bonus XP */
static inline void AllocateBonus(Knight_t *knight, uint32_t count)
{
	for(int i = 0; i < NUM_OF_KNIGHTS; i++)
	{
		/* Email 3: Bonus XP system modified for n knights */
		if(count == NUM_OF_KNIGHTS/3)
		{
			if(knight[i].xp >= NUM_OF_KNIGHTS/2)
				knight[i].xp += 5;
		}
		else if(count == NUM_OF_KNIGHTS/2)
		{
			if(knight[i].xp >= NUM_OF_KNIGHTS/2)
				knight[i].xp += 10;
		}
		else if(count == (3*NUM_OF_KNIGHTS)/4)
		{
			if(knight[i].xp >= NUM_OF_KNIGHTS/2)
				knight[i].xp += 20;
		}
		else if(count == NUM_OF_KNIGHTS)
		{
			knight[i].xp += 30;
		}
	}
}

/* Utility Funciton to print total xp */
static inline void PrintXP(Knight_t *knight)
{
	uint32_t sum_xp = 0;
	/* Print total xp of all knights */
	for(uint32_t i = 1;i < NUM_OF_KNIGHTS; i++)
	{
		sum_xp += knight[i].xp;

		/* Debug print */
#ifdef DEBUG
		DEBUG("[DEBUG] Knight %d: XP:%u\tSTA:%d\n", i, knight[i].xp, knight[i].sta);
#endif
	}
	printf("Total xp: %d\n", sum_xp);
	printf("King's xp: %d\n", knight[0].xp);
}

/* Utility Function to count number of knights eligible for bonus */
static inline uint32_t CountNumOfKnightsWithGoodXp(Knight_t *knight)
{
	unsigned int count = 0;
	for(uint32_t i = 0; i < NUM_OF_KNIGHTS; i++)
	{
		if(knight[i].xp >= NUM_OF_KNIGHTS/2)
		{
			count += 1;
		}
	}
	return count;
}

/* Utility Function to check if bonus is to be allocated or not */
static inline uint8_t isBonusAllowed(uint32_t count)
{
	return (count == NUM_OF_KNIGHTS/3 ||
			count == NUM_OF_KNIGHTS/2 ||
			count == (3*NUM_OF_KNIGHTS)/4 ||
			count == NUM_OF_KNIGHTS
			);
}
