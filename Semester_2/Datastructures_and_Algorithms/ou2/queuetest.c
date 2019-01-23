/*
 * Datastrukturer & Algoritmer (C)
 * Spring 17
 * Ou2
 * 
 * Filename: 	queuetest.c
 * Author: 		Jonas Sjödin
 * Cs-user: 	id16jsn
 * 
 * Description: Testing an implemented queue.
 * Date: 		2017-02-13
 * 
 * Input: 		None
 * Output: 		Test Results
 */




#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void testQueueEmpty();
void testQueueAdd();
void testQueueDelete();
void testQueueOrder();
void testQueueFront();
void testQueueFrontOrder();

/*
 * Function: main()
 *
 * Description: Prints info about incoming test and runs the tests.
 *
 * Input: NONE.
 * Output: Runs tests which prints if the tests are successful or not.
 */
int main(void)
{	
	fprintf(stderr, "1. Is queue empty?\n");
	testQueueEmpty();
	
	fprintf(stderr, "2. Does add value work?\n");
	testQueueAdd();
	
	fprintf(stderr, "3. Does delete value work?\n");
	testQueueDelete();
	
	fprintf(stderr, "4. Is value entered at front and deleted ");
	fprintf(stderr, "at back working, and does front work?\n");
	testQueueOrder();
	
	fprintf(stderr, "5. Does front work?\n");
	testQueueFront();
	
	fprintf(stderr, "6. Does front check first value?\n");
	testQueueFrontOrder();
	
	
	return 0;
}

/*
 * Function: testQueueEmpty()
 * 
 * Description: Tests if a queue is empty after it has been
 * emptied.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 * Successful if empty and Failed if it has a value.
 */
void testQueueEmpty(){
	queue *queue1 = queue_empty();
	queue_setMemHandler(queue1, free);

	//If the queue is empty the test has suceeded.
	
	if (queue_isEmpty(queue1)){
		printf("Successful!\n");
	}
	else{
		queue_free(queue1);
		printf("Failed!\n");
		printf("The queue isn't created empty. Either doesn't queue_empty or ");
		printf("queue_isEmpty work correctly\n");
		
		exit(1);
	}
	
	queue_free(queue1);
	printf("\n");
}

/*
 * Function: testQueueAdd()
 * 
 * Description: Tests if a queue is not empty after a value has 
 * been added.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 * Successful if it has a value and Failed if empty.
 */
void testQueueAdd(){
	
		
	queue *queue1 = queue_empty();
	queue_setMemHandler(queue1, free);
	
	int *value1 = malloc(sizeof(int));
	
	queue_enqueue(queue1, value1);

	//If the queue is not empty the queue has succeeded.
	if (queue_isEmpty(queue1)){
		printf("Failed!\n");
		printf("Can't add a value. ");
		printf("The queue_enqueue function does not work.\n");
		
		exit(1);
	}
	else{
		printf("Successful!\n");
	}
	
	queue_free(queue1);
	printf("\n");
}

/*
 * Function: testQueueDelete()
 * 
 * Description: Tests if the queue is empty if a value is added to an empty 
 * queue and then the first value of the queue is deleted.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 */ 
void testQueueDelete(){
	
	
	queue *queue1 = queue_empty();
	queue_setMemHandler(queue1, free);
	
	int *value1 = malloc(sizeof(int));
	
	queue_enqueue(queue1, value1);
	
	if (queue_isEmpty(queue1)){
		printf("Queue is empty and shouldn't be empty");
		queue_free(queue1);
		exit(0);
	}
	
	queue_dequeue(queue1);

	
	
	//printf("2. Does add value work?				");
	
	
	//If the queue is empty the test has succeeded.
	if (queue_isEmpty(queue1)){
		printf("Successful!\n");
		
	}
	else{
		printf("Failed!\n");
		printf("Can't delete a value. ");
		printf("The queue_dequeue function does not work.\n");
	
		exit(1);
	}
	
	queue_free(queue1);
	printf("\n");

}

/*
 * Function: testQueueORder()
 * 
 * Description: If you have two identical NONempty queues. If you add a 
 * value to one of the queues and then deletes the first value in the queue.
 * Then you delete the first value in the queue and then add the same value as 
 * above. If both queues are the same now then the test has suceeded.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 */
void testQueueOrder(){
	
	
	queue *queue1 = queue_empty();
	queue *queue2 = queue_empty();
	queue_setMemHandler(queue1, free);
	queue_setMemHandler(queue2, free);
	
	int *value1 = malloc(sizeof(int));
	int *value2 = malloc(sizeof(int));
	int *value3 = malloc(sizeof(int));
	int *value4 = malloc(sizeof(int));
	
	*value1 = 1;
	*value2 = 9;
	*value3 = 1;
	*value4 = 9;

	queue_enqueue(queue1, value1);
	queue_enqueue(queue2, value3);

	queue_enqueue(queue1, value2);
	
	if (queue_isEmpty(queue1) || queue_isEmpty(queue2)){
		printf("Queue is empty and shouldn't be empty");
		queue_free(queue1);
		queue_free(queue2);
		exit(0);
	}
	
	queue_dequeue(queue1);
	queue_dequeue(queue2);
	
	
	queue_enqueue(queue2, value4);	
	

	//If the queues are the same the test has succeeded.
	if (queue_isEmpty(queue2)){
		printf("Queue is empty and shouldn't be empty");
		exit(0);
	} 
	
	if (*(int*)queue_front(queue1) == *(int*)queue_front(queue2)){
		printf("Successful!\n");
	}
	else{
		printf("Failed!\n");
		printf("One of the queue_enqueue and/or queue_dequeue does not work");
		printf(" as it should. \nThe queue_enqueue function doesn't add a ");
		printf("value at the last place of the queue \nand/or queue_dequeue ");
		printf("doesn't delete the first value in the queue. \nqueue_front");
		printf(" might also not start at all. This test however works even");
		printf(" tho \nqueue_front doesnt check the value at the front of");
		printf(" queue but checks a random position.\n");
		
		exit(1);
	}
	
	queue_free(queue1);
	queue_free(queue2);	
	printf("\n");
	
}

/*
 * Function: testQueueFront()
 * 
 * Description: If you have an empty queue which you add a value to. The test 
 * succeeds if the first value of the que is the same as the one that was 
 * recently added.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 */
void testQueueFront(){
		
	
	queue *queue1 = queue_empty();
	queue_setMemHandler(queue1, free);
	
	int *value1 = malloc(sizeof(int));
	*value1 = 1;
	
	queue_enqueue(queue1, value1);
	
	if (queue_isEmpty(queue1)){
		printf("Queue is empty and shouldn't be empty");
		queue_free(queue1);
		exit(0);
	}

	//If the queues first value is the same as value1 the test has succeeded.
	if (queue_front(queue1) == value1){
		printf("Successful!\n");
	}
	else{
		printf("Failed!\n");
		printf("The queue_front function doesn't work.\n");
		
		exit(1);
	}
	
	queue_free(queue1);
	printf("\n");
}

/*
 * Function: testQueueOrder()
 * 
 * Description: If you have an empty queue which you add a value to. The test 
 * succeeds if the first value of the que is the same as the one that was 
 * recently added.
 * 
 * Input: NONE.
 * Output: Prints info if test failed or succeeded.
 */
void testQueueFrontOrder(){
	
	
	queue *queue1 = queue_empty();
	queue *queue2 = queue_empty();
	queue_setMemHandler(queue1, free);
	queue_setMemHandler(queue2, free);	

	int *value1 = malloc(sizeof(int));
	int *value2 = malloc(sizeof(int));
	int *value3 = malloc(sizeof(int));
	*value1 = 1;
	*value2 = 2;
	*value3 = 1;

	queue_enqueue(queue1, value1);
	queue_enqueue(queue2, value3);
	
	queue_enqueue(queue1, value2);
	
	if (queue_isEmpty(queue1) || queue_isEmpty(queue2)){
		printf("Queue is empty and shouldn't be empty");
		queue_free(queue1);
		queue_free(queue2);
		exit(0);
	}
	//If they have the same value at the front, test has succeeded.
	if (*(int*)queue_front(queue1) == *(int*)queue_front(queue2)){
		printf("Successful!\n");
	}
	else{
		printf("Failed!\n");
		printf("queue_front doesn't check the value at the front of the ");
		printf("queue.\n");
		exit(1);
	}	

	queue_free(queue1);
	queue_free(queue2);
	printf("\n");
}