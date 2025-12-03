#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===== Card definition ===== */

typedef struct {
    int value;      /* 1-13 (Ace-King) */
    char suit;      /* 'H', 'D', 'C', 'S' */
} Card;

/* ===== Stack for deck ===== */

typedef struct StackNode {
    Card card;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
} Stack;

void initStack(Stack *s) {
    s->top = NULL;
}

int isStackEmpty(Stack *s) {
    return s->top == NULL;
}

void push(Stack *s, Card c) {
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    if (node == NULL) {
        printf("Memory allocation error in push.\n");
        exit(1);
    }
    node->card = c;
    node->next = s->top;
    s->top = node;
}

Card pop(Stack *s) {
    Card empty;
    StackNode *temp;

    empty.value = 0;
    empty.suit = 'X';

    if (s->top == NULL) {
        printf("Stack underflow.\n");
        return empty;
    }
    temp = s->top;
    empty = temp->card;
    s->top = temp->next;
    free(temp);
    return empty;
}

/* ===== Queue for cards played in a round ===== */

typedef struct QueueNode {
    Card card;
    int playerIndex; /* which player played this card */
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
}

int isQueueEmpty(Queue *q) {
    return q->front == NULL;
}

void enqueue(Queue *q, Card c, int playerIndex) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL) {
        printf("Memory allocation error in enqueue.\n");
        exit(1);
    }
    node->card = c;
    node->playerIndex = playerIndex;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

QueueNode *dequeue(Queue *q) {
    QueueNode *temp;
    if (q->front == NULL) {
        return NULL;
    }
    temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

/* ===== Circular queue for rotating player turns ===== */

typedef struct PlayerTurnNode {
    int playerIndex;
    struct PlayerTurnNode *next;
} PlayerTurnNode;

typedef struct {
    PlayerTurnNode *current;
} PlayerTurnQueue;

void initPlayerTurnQueue(PlayerTurnQueue *ptq) {
    ptq->current = NULL;
}

void addPlayerTurn(PlayerTurnQueue *ptq, int playerIndex) {
    PlayerTurnNode *node = (PlayerTurnNode *)malloc(sizeof(PlayerTurnNode));
    PlayerTurnNode *temp;
    if (node == NULL) {
        printf("Memory allocation error in addPlayerTurn.\n");
        exit(1);
    }
    node->playerIndex = playerIndex;
    if (ptq->current == NULL) {
        ptq->current = node;
        node->next = node; /* circular */
    } else {
        temp = ptq->current;
        while (temp->next != ptq->current) {
            temp = temp->next;
        }
        temp->next = node;
        node->next = ptq->current;
    }
}

int getNextPlayer(PlayerTurnQueue *ptq) {
    int index;
    if (ptq->current == NULL) {
        return -1;
    }
    index = ptq->current->playerIndex;
    ptq->current = ptq->current->next;
    return index;
}

void freePlayerTurnQueue(PlayerTurnQueue *ptq) {
    PlayerTurnNode *temp;
    PlayerTurnNode *start;
    if (ptq->current == NULL) {
        return;
    }
    start = ptq->current;
    temp = start->next;
    while (temp != start) {
        PlayerTurnNode *next = temp->next;
        free(temp);
        temp = next;
    }
    free(start);
    ptq->current = NULL;
}

/* ===== Linked list for round results ===== */

typedef struct ResultNode {
    int roundNumber;
    char winnerName[50];
    int winnerScoreAfterRound;
    struct ResultNode *next;
} ResultNode;

typedef struct {
    ResultNode *head;
} ResultList;

void initResultList(ResultList *list) {
    list->head = NULL;
}

void addResult(ResultList *list, int roundNumber, const char *winnerName, int winnerScore) {
    ResultNode *node = (ResultNode *)malloc(sizeof(ResultNode));
    ResultNode *temp;

    if (node == NULL) {
        printf("Memory allocation error in addResult.\n");
        exit(1);
    }
    node->roundNumber = roundNumber;
    strcpy(node->winnerName, winnerName);
    node->winnerScoreAfterRound = winnerScore;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = node;
    }
}

void printResults(ResultList *list) {
    ResultNode *temp = list->head;
    if (temp == NULL) {
        printf("No rounds played.\n");
        return;
    }
    printf("\n=== Round Results ===\n");
    while (temp != NULL) {
        printf("Round %d: Winner = %s (score after round: %d)\n",
               temp->roundNumber, temp->winnerName, temp->winnerScoreAfterRound);
        temp = temp->next;
    }
}

void freeResultList(ResultList *list) {
    ResultNode *temp = list->head;
    while (temp != NULL) {
        ResultNode *next = temp->next;
        free(temp);
        temp = next;
    }
    list->head = NULL;
}

/* ===== Binary Search Tree for players by score ===== */

typedef struct PlayerNode {
    char name[50];
    int score;
    struct PlayerNode *left;
    struct PlayerNode *right;
} PlayerNode;

PlayerNode *createPlayerNode(const char *name, int score) {
    PlayerNode *node = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (node == NULL) {
        printf("Memory allocation error in createPlayerNode.\n");
        exit(1);
    }
    strcpy(node->name, name);
    node->score = score;
    node->left = node->right = NULL;
    return node;
}

PlayerNode *insertPlayer(PlayerNode *root, const char *name, int score) {
    if (root == NULL) {
        return createPlayerNode(name, score);
    }
    if (score < root->score) {
        root->left = insertPlayer(root->left, name, score);
    } else {
        root->right = insertPlayer(root->right, name, score);
    }
    return root;
}

void printPlayersInOrder(PlayerNode *root) {
    if (root == NULL) {
        return;
    }
    printPlayersInOrder(root->left);
    printf("Player: %s | Score: %d\n", root->name, root->score);
    printPlayersInOrder(root->right);
}

void freePlayerTree(PlayerNode *root) {
    if (root == NULL) {
        return;
    }
    freePlayerTree(root->left);
    freePlayerTree(root->right);
    free(root);
}

/* ===== Helpers for card and game logic ===== */

const char *valueToString(int value) {
    static char buf[3];

    switch (value) {
        case 1:  return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default:
            if (value < 10) {
                buf[0] = (char)('0' + value);
                buf[1] = '\0';
            } else {
                buf[0] = (char)('0' + value / 10);
                buf[1] = (char)('0' + value % 10);
                buf[2] = '\0';
            }
            return buf;
    }
}

const char *suitToString(char suit) {
    switch (suit) {
        case 'H': return "Hearts";
        case 'D': return "Diamonds";
        case 'C': return "Clubs";
        case 'S': return "Spades";
        default:  return "Unknown";
    }
}

void printCard(Card c) {
    printf("%s of %s", valueToString(c.value), suitToString(c.suit));
}

/* Create a standard 52-card deck in an array */
void createDeck(Card deck[], int *size) {
    int values[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    char suits[4] = {'H','D','C','S'};
    int i, j;
    int index = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 13; j++) {
            deck[index].value = values[j];
            deck[index].suit = suits[i];
            index++;
        }
    }
    *size = index;
}

/* Simple Fisher–Yates shuffle */
void shuffleDeck(Card deck[], int size) {
    int i;
    for (i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

int main(void) {
    int numPlayers;
    char playerNames[4][50];
    int scores[4];
    Card deck[52];
    int deckSize;
    Stack deckStack;
    ResultList resultList;
    PlayerNode *playerTree = NULL;
    PlayerTurnQueue turnQueue;
    int round, maxRounds;
    int i;

    printf("=== Card Game with Data Structures ===\n\n");
    printf("Enter number of players (2–4): ");
    if (scanf("%d", &numPlayers) != 1 || numPlayers < 2 || numPlayers > 4) {
        printf("Invalid number of players.\n");
        return 1;
    }

    for (i = 0; i < numPlayers; i++) {
        printf("Enter name for player %d: ", i + 1);
        scanf("%49s", playerNames[i]);
        scores[i] = 0;
    }

    /* Initialize random seed */
    srand((unsigned int)time(NULL));

    /* Create & shuffle deck */
    createDeck(deck, &deckSize);
    shuffleDeck(deck, deckSize);

    /* Push deck to stack (top of stack = last card in array) */
    initStack(&deckStack);
    for (i = 0; i < deckSize; i++) {
        push(&deckStack, deck[i]);
    }

    /* Initialize result list and turn queue */
    initResultList(&resultList);
    initPlayerTurnQueue(&turnQueue);

    for (i = 0; i < numPlayers; i++) {
        addPlayerTurn(&turnQueue, i);
    }

    /* Decide max rounds (e.g. 5 or limited by deck) */
    maxRounds = 5;
    if (maxRounds * numPlayers > deckSize) {
        maxRounds = deckSize / numPlayers;
    }

    printf("\nStarting game with %d players and %d rounds.\n", numPlayers, maxRounds);

    for (round = 1; round <= maxRounds; round++) {
        Queue roundQueue;
        int bestPlayer = -1;
        int bestValue = -1;
        int tie = 0; /* 1 if tie */

        initQueue(&roundQueue);

        printf("\n--- Round %d ---\n", round);

        /* Each player draws a card using the circular player queue */
        for (i = 0; i < numPlayers; i++) {
            int pIndex = getNextPlayer(&turnQueue);
            Card c;

            if (isStackEmpty(&deckStack)) {
                printf("Deck is empty. Stopping game.\n");
                break;
            }

            c = pop(&deckStack);
            enqueue(&roundQueue, c, pIndex);

            printf("%s draws: ", playerNames[pIndex]);
            printCard(c);
            printf("\n");
        }

        /* Determine winner for this round */
        while (!isQueueEmpty(&roundQueue)) {
            QueueNode *qn = dequeue(&roundQueue);
            int cardValue = qn->card.value;
            int pIndex = qn->playerIndex;

            if (cardValue > bestValue) {
                bestValue = cardValue;
                bestPlayer = pIndex;
                tie = 0;
            } else if (cardValue == bestValue) {
                tie = 1;
            }

            free(qn);
        }

        if (bestPlayer == -1) {
            printf("No cards played this round.\n");
            break;
        }

        if (tie) {
            printf("Round %d result: Tie. No points awarded.\n", round);
        } else {
            scores[bestPlayer] += 100;
            printf("Round %d winner: %s (+100 points)\n",
                   round, playerNames[bestPlayer]);
            addResult(&resultList, round, playerNames[bestPlayer], scores[bestPlayer]);
        }
    }

    /* Insert players into BST by final score */
    for (i = 0; i < numPlayers; i++) {
        playerTree = insertPlayer(playerTree, playerNames[i], scores[i]);
    }

    /* Print results and final ranking */
    printResults(&resultList);

    printf("\n=== Final ranking (by score, BST in-order) ===\n");
    printPlayersInOrder(playerTree);

    /* Cleanup */
    freeResultList(&resultList);
    freePlayerTree(playerTree);
    freePlayerTurnQueue(&turnQueue);

    /* Free any remaining cards in stack */
    while (!isStackEmpty(&deckStack)) {
        (void)pop(&deckStack);
    }

    printf("\nGame over.\n");
    return 0;
}

