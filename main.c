#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct state {
	int *board;
	int emptyPos;
	int count;
	int moves[100];
};


struct node {
	int *board;
	struct node* next;
};

struct queue {
	int head;
	int tail;
	struct state *states;
};

struct queue createQueue(int capacity) {
	struct queue queue;
	queue.states = malloc(capacity * sizeof(struct state));
	queue.head = 0;
	queue.tail = 0;
	return queue;

}

bool emptyQueue(struct queue *queue) {
	return queue->head == queue->tail;
}

void enqueue(struct queue *queue, struct state board){
	queue->states[queue->tail] = board;
	queue->tail++;
}

struct state dequeue(struct queue *queue) {
	struct state board = queue->states[queue->head];
	queue->head++;
	return board;
}

int hash(int key[], int capacity, int k) {
	int h = 0;
	int pow = 1;
	for(int i = 0; i < (k * k); i++) {
		h += (key[i] * pow) % capacity;
		pow *= 31;
	}
	return ((h % capacity) + capacity) % capacity;
}


void insert(int board[], struct node** hashtable, int capacity, int k) {
	int index = hash(board, capacity, k);
	struct node* comp = hashtable[index];
	while (comp != NULL) {
		if (memcmp(comp->board, board, k * k * sizeof(int)) == 0) {
		       return;
		}
		comp = comp->next;
	}

	struct node* newNode = malloc(sizeof(struct node));

	newNode->board = malloc(k * k * sizeof(int));
		
	memcpy(newNode->board, board, k * k * sizeof(int));

	newNode->next = hashtable[index];
	hashtable[index] = newNode;
}

bool solvable(int board[], int k) {
	int inversions = 0;
	int zeroidx;
	for (int i = 0; i < (k * k); i++) {
		if (board[i] == 0) {
			zeroidx = i;
		}
	}

	for (int i = 0; i < (k * k) - 1; i++) {
		for (int j = i + 1; j < (k * k); j++) {
			if (board[i] != 0 && board[j] != 0 && board[i] > board[j]) {
				inversions++;
			}
		}
	}

	if (k % 2 == 1) {
		if (inversions % 2 == 1) {
			return false;
		}
	}

	int rowidx = zeroidx / k;

	if (k % 2 == 0) {
		if ((inversions + rowidx) % 2 == 0) {
			return false;
		}
	}

	return true;
}

bool invalidPos(int pos, int k, int zeroPos, int i) {
	if (pos < 0) {
		return true;
	}
	if (pos >= k * k) {
		return true;
	}
	if (i == 0 && zeroPos % k == k - 1) {
		return true;
	}
	if (i == 1 && zeroPos % k == 0) {
		return true;
	}
	return false;
}

bool answer(int board[], int k) {
	int correctBoard[k * k];
	int idx = 0;
	for (int i = 1; i < (k * k); i++) {
		correctBoard[idx] = i;
		idx++;
	}

	correctBoard[(k*k) - 1] = 0;

	if (memcmp(board, correctBoard, k * k * sizeof(int)) == 0) {
        	return true;
        }

	return false;
}


struct state bfs(int board[], int k, struct node** table, int capacity) {
	struct queue queue = createQueue(100000);
	int pos[4] = {1, -1, k, -k};
	struct state beg;
	beg.board = malloc(k * k * sizeof(int));
	memcpy(beg.board, board, k * k * sizeof(int));
	beg.count = 0;

	for (int i = 0; i < (k * k); i++) {
        	if (beg.board[i] == 0) {
                beg.emptyPos = i;
             }
        }

	enqueue(&queue, beg);
	insert(beg.board, table, capacity, k);

	while (!emptyQueue(&queue)) {
		struct state curr = dequeue(&queue);

		if (answer(curr.board, k)) {
			free(queue.states);
			return curr;
		}

		for (int i = 0; i < 4; i++) {
			int swpPos = curr.emptyPos + pos[i];
			if (invalidPos(swpPos, k, curr.emptyPos, i)) {
				continue;
			}

			struct state newBoard;
			newBoard.board = malloc(k * k * sizeof(int));

			memcpy(newBoard.board, curr.board, k * k * sizeof(int));
			newBoard.board[curr.emptyPos] = newBoard.board[swpPos];
			int val = newBoard.board[swpPos];
			newBoard.board[swpPos] = 0;
			newBoard.emptyPos = swpPos;
			memcpy(newBoard.moves, curr.moves, curr.count * sizeof(int));
			newBoard.moves[curr.count] = val;
			newBoard.count = curr.count + 1;

			if (answer(newBoard.board, k)) {
                	return newBoard;
            		}		

			int hashIdx = hash(newBoard.board, capacity, k);

			bool found = false;

			struct node* comp = table[hashIdx];
        		while (comp != NULL) {
                		if (memcmp(comp->board, newBoard.board, k * k * sizeof(int)) == 0) {
                       			found = true;
					break;
                		}		
                	comp = comp->next;

        		}

			if (found == false) {
				enqueue(&queue, newBoard);
				insert(newBoard.board, table, capacity, k);
			} else {
				free(newBoard.board);
			}
		}

		free(curr.board);
	}
}







int main(int argc, char **argv)
{
	FILE *fp_in,*fp_out;
	
	fp_in = fopen(argv[1], "r");
	if (fp_in == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	fp_out = fopen(argv[2], "w");
	if (fp_out == NULL){
		printf("Could not open a file.\n");
		return -1;
	}
	
	char *line = NULL;
	size_t lineBuffSize = 0;
	ssize_t lineSize;
	int k;
	
	getline(&line,&lineBuffSize,fp_in);//ignore the first line in file, which is a comment
	fscanf(fp_in,"%d\n",&k);//read size of the board
	//printf("k = %d\n", k); //make sure you read k properly for DEBUG purposes
	getline(&line,&lineBuffSize,fp_in);//ignore the second line in file, which is a comment

	int initial_board[k*k];//get kxk memory to hold the initial board
	for(int i=0;i<k*k;i++)
		fscanf(fp_in,"%d ",&initial_board[i]);
	//printBoard(initial_board, k);//Assuming that I have a function to print the board, print it here to make sure I read the input board properly for DEBUG purposes
	fclose(fp_in);

	////////////////////
	// do the rest to solve the puzzle
	////////////////////

	//once you are done, you can use the code similar to the one below to print the output into file
	//if the puzzle is NOT solvable use something as follows
	if (!solvable(initial_board, k)) {

		fprintf(fp_out, "#moves\n");
		fprintf(fp_out, "no solution\n");
		return 0;
	}


	int capacity;
	if (k == 3) {
		capacity = pow(2, 12);
	} else if (k == 4) {
		capacity = pow(2, 14);
	} else if (k == 5) {
		capacity = pow(2, 16);
	} else {
		capacity = pow(2, 20);
	}

	struct node** hashtable = calloc(capacity, sizeof(struct node*));

	struct state answers = bfs(initial_board, k, hashtable, capacity);

	int numberOfMoves = answers.count;

	int* move = answers.moves;

	
	//if it is solvable, then use something as follows:
	fprintf(fp_out, "#moves\n");
	//probably within a loop, or however you stored proper moves, print them one by one by leaving a space between moves, as below
	for(int i=0; i<numberOfMoves; i++) {
		fprintf(fp_out, "%d ", move[i]);
        }

	fclose(fp_out);

	free(answers.board);
	for (int i = 0; i < capacity; i++) {
        	struct node* current = hashtable[i];
        	while (current != NULL) {
        	struct node* temp = current;
            	current = current->next;
            	free(temp->board);
            	free(temp);
        }
    }

    free(hashtable);

	return 0;

}
