/******************************************************************************/
/* 				Sorting Threads            		                              */
/*				Description - Multiple counting sort simultanusly             */
/*				Author	 -    Dean Oron								          */
/*				Date	 - 	  19.05.20							              */
/*				Reviewer - 	  Lior Cohen					                  */
/*				Open Lab 86											          */
/******************************************************************************/\

#include <stdio.h>	/*printf()*/
#include <string.h> /* strlen */
#include <stdlib.h>	/* malloc() free()*/
#include <pthread.h>/*thread_funcs*/
#include <time.h> 	/* clock(), clock_t, time_t, time() */

#define LINEAR_TEST
/* #define THREAD_TEST */
#define MAX_CHARS (256)
#define DICT_SIZE (102305)
#define MAX_WORD_SIZE (23)
#define THREADS_NUM (5)

char **LoadDict(char *path);
void CountingSort(int *arr, int size, int lefter, int upper);
void* thread_func(void *arg);

int *Merge2SortedArray(int *arr1, int *arr2, int arr1_size, int arr2_size);

int main()
{
    char **dictionary = NULL;
    int i = 0;
    char *path = "/usr/share/dict/american-english";
    int arr1[DICT_SIZE];
    int arr2[DICT_SIZE];
    int arr3[DICT_SIZE];
    int arr4[DICT_SIZE];
    int arr5[DICT_SIZE];
    int *final_arr = (int *)malloc(sizeof(int) * (DICT_SIZE * 5));
	int *sorted1_arr = (int *)malloc(sizeof(int) * (DICT_SIZE * 2));
	int *sorted2_arr = (int *)malloc(sizeof(int) * (DICT_SIZE * 2));
	int *sorted3_arr = (int *)malloc(sizeof(int) * (DICT_SIZE * 4));
	clock_t before;
	clock_t after;
	double elapsed = 0;

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;
    
    dictionary = LoadDict(path);

    for(i = 0; i < DICT_SIZE; ++i)
    {
        arr1[i] = strlen(dictionary[i]);
        arr2[i] = strlen(dictionary[i]);
        arr3[i] = strlen(dictionary[i]);
        arr4[i] = strlen(dictionary[i]);
        arr5[i] = strlen(dictionary[i]);
    }
#ifdef THREAD_TEST
	before = clock();
    pthread_create(&thread1, NULL, thread_func, arr1);
    pthread_create(&thread2, NULL, thread_func, arr2);
    pthread_create(&thread3, NULL, thread_func, arr3);
    pthread_create(&thread4, NULL, thread_func, arr4);
    pthread_create(&thread5, NULL, thread_func, arr5); 

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
#endif /*THREAD_TEST*/

#ifdef LINEAR_TEST
before = clock();
 CountingSort(arr1, DICT_SIZE, 0, 23);
 CountingSort(arr2, DICT_SIZE, 0, 23);
 CountingSort(arr3, DICT_SIZE, 0, 23);
 CountingSort(arr4, DICT_SIZE, 0, 23);
 CountingSort(arr5, DICT_SIZE, 0, 23);

#endif /*LINEAR_TEST*/
 
 
    sorted1_arr = Merge2SortedArray(arr1, arr2, DICT_SIZE, DICT_SIZE);
    sorted2_arr = Merge2SortedArray(arr3, arr4, DICT_SIZE, DICT_SIZE);
	sorted3_arr = Merge2SortedArray(sorted1_arr, sorted2_arr, (DICT_SIZE * 2), (DICT_SIZE * 2));
	final_arr =  Merge2SortedArray(sorted3_arr, arr5, (DICT_SIZE * 4), DICT_SIZE);
	
    for(i = 0; i < (DICT_SIZE * 5); ++i)
    {
        printf("%d\n", final_arr[i]);
    }

	free(final_arr);
	free(sorted1_arr);
	free(sorted2_arr);
	free(sorted3_arr);
	
    return(0);
}

char **LoadDict(char *path)
{
	FILE *dict;
	char **words_arr = NULL;
	char buff[MAX_WORD_SIZE];
	size_t i = 0;
	
	words_arr = (char **)malloc(sizeof(char *) * DICT_SIZE);
	dict = fopen(path, "r");
	
	if (!dict)
	{
		return (NULL);
	}
	
	for(i = 0; i < DICT_SIZE; ++i)
	{	
        fscanf(dict, "%s", buff);
		words_arr[i] = (char *)malloc(MAX_WORD_SIZE);
		strcpy(words_arr[i], buff);
	}
	
	return (words_arr);
}

void CountingSort(int *arr, int size, int lefter, int upper)
{
	int i = 0;
	int *count_arr;
	int *sorted_arr;
	int count_arr_size = upper - lefter + 1;
	
	count_arr = (int *)calloc(count_arr_size, sizeof(int));
	sorted_arr = (int *)calloc(size, sizeof(int));
	
	/* filling counting array */
	for (i = 0; i < size; ++i)
	{
		++count_arr[arr[i]];
	}
	
	/* add previous to counting array elements */
	for (i = 1; i < count_arr_size; ++i)
	{
		count_arr[i] += count_arr[i - 1];
	}
	
	/* fill sorted array */
	for (i = 0; i < size; ++i)
	{
		sorted_arr[count_arr[arr[i]] - 1] = arr[i];
		--count_arr[arr[i]];
	}
	
	/* assigned sorted elements to original arr */
	for (i = 0; i < size; ++i)
	{
		arr[i] = sorted_arr[i];
	}
	
	free(count_arr);
	count_arr = NULL;
	
	free(sorted_arr);
	sorted_arr = NULL;
}

void* thread_func(void *arg)
{
    int *arr = (int *)arg;
    CountingSort(arr, DICT_SIZE, 0, 23);
    return(NULL);
}

int *Merge2SortedArray(int *arr1, int *arr2, int arr1_size, int arr2_size)
{
	int i = 0;		/* arr1 loop counter */
	int j = 0;		/* arr2 loop counter */
	int k = 0;		/* merge_array loop counter */
	
	int *merged_array = (int *)malloc(sizeof(int) *(arr1_size + arr2_size));

	/* merge temp arrays back into arr[].
		while there are elements at the TWO subarrays compare values of first 
		index at the two subarrays and copy the smallest value to arr[]*/
	while (i < arr1_size && j < arr2_size)
	{
		if (arr1[i] <= arr2[j])
		{
			merged_array[k] = arr1[i];
			++i;
		}
		else
		{
			merged_array[k] = arr2[j];
			++j;
		}
		++k;
	}
	
	/* copy the remaining elements of top_arr/ bottom_arr if there are*/
	while (i < arr1_size)
	{
		merged_array[k] = arr1[i];
		++i;
		++k;
	}
	while (j < arr2_size)
	{
		merged_array[k] = arr2[j];
		++j;
		++k;
	}

	return (merged_array);
}
