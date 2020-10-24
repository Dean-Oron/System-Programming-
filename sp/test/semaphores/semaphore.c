/******************************************************************************/
/* 					Semaphor				  					  			  */
/*					Description -  Threads - Semaphre excercise				  */
/*					Autor    -     Dean Oron								  */
/*					Date     - 	   04.05.20									  */
/*					Reviewer - 	   Amir Paz									  */
/*					Open Lab 86											  	  */
/******************************************************************************/
#include <stdio.h>     
#include <string.h>    
#include <semaphore.h> 
#include <sys/stat.h>	
#include <fcntl.h>		

#define INIT_VALUE (100)
#define WORD_SIZE (10)

static void SemDown(sem_t *main_sem, int ammount);
static void SemUp(sem_t *main_sem, int ammount);
int prev_sem = INIT_VALUE;

int main(int argc, char *argv[])
{
	sem_t *main_sem = NULL;
	int sem_value = 0;
	int dif = 0;
	char input;
	int ammount = 0;

	main_sem = sem_open(argv[argc - 1], O_CREAT, S_IRWXU, INIT_VALUE);

	sem_getvalue(main_sem, &sem_value);
	printf("The semaphore value at start is %d\n", sem_value);

	while (1)
	{
		printf("Insert 'D' to decrease, 'U' to increase or 'X' for exit or 'undo'\n");
		scanf("\n%c", &input);

		switch (input)
		{
			case 'D':
				sem_getvalue(main_sem, &sem_value);
				printf("Insert the number to decrease semaphore value %d\n", sem_value);
				scanf("\n%d", &ammount);
				SemDown(main_sem, ammount);
				
				break;
	
			case 'U':
				sem_getvalue(main_sem, &sem_value);
				printf("Insert the number to increase semaphore value %d\n", sem_value);
				scanf("\n%d", &ammount);
				SemUp(main_sem, ammount);
				break;
				
			case 'X':
				sem_getvalue(main_sem, &sem_value);
				dif = INIT_VALUE - sem_value;

				if (0 < dif)
				{
					SemUp(main_sem, dif);
				}
				else
				{
					SemDown(main_sem, - dif);
				}
				
				sem_getvalue(main_sem, &sem_value);
				printf("The semaphore value at exit is %d\n", sem_value);
				sem_close(main_sem);
				sem_unlink(argv[argc - 1]);
				
				return (0);
				
			default:
				printf("Wrong value\n\n");
				break;
		}
	}

	return (0);
}

static void SemDown(sem_t *main_sem, int ammount)
{
	while (ammount)
	{
		sem_wait(main_sem);
		--ammount;
	}
}

static void SemUp(sem_t *main_sem, int ammount)
{
	while (ammount)
	{
		sem_post(main_sem);
		--ammount;
	}
}

