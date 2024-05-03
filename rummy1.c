// C PROJECT(RUMMY GAME)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define NUM_PLAYERS 2
#define NUM_CARDS 54
#define HAND_SIZE 13
#define POS 4

int top = -1;
int r1 = 0;
int r2 = 0;
int pr1 = 0;
int pr2 = 0;

typedef struct
{
    char suit;
    int rank;
} Card;

typedef struct
{
    Card cards[HAND_SIZE + 1];
    int size;
} Hand;

Card playerrummy1[POS][POS];
Card playerrummy2[POS][POS];
Card rummy[POS];
Card old[POS];
Card drop;
Card open;

void push(Card cards[], int i, char suits, int ranks)
{
    cards[i].suit = suits;
    cards[i].rank = ranks;
}

Card pop(Card deck[])
{
    return deck[top--];
}

void clearScreen()
{
    system(CLEAR);
}

void initializeDeck(Card deck[])
{
    char suits[] = {'h', 'd', 'c', 's'};
    int ranks[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    int i = 0;
    for (int suitIndex = 0; suitIndex < 4; suitIndex++)
    {
        for (int rankIndex = 0; rankIndex < 13; rankIndex++)
        {
            push(deck, i, suits[suitIndex], ranks[rankIndex]);
            top++;

            i++;
        }
    }
    for (int k = 0; k < 2; k++)
    {
        push(deck, i, 'j', 0);
        top++;
        i++;
    }

    // for (int j = 0; j <= top; j++)
    // {
    //     printf("%c%d ", deck[j].suit, deck[j].rank);
    // }
}

void shuffleDeck(Card deck[])
{
    srand(time(NULL));
    for (int i = NUM_CARDS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }

    // printf("after shuffling...\n\n");

    // for (int j = 0; j <= top; j++)
    // {
    //     printf("%c%d ", deck[j].suit, deck[j].rank);
    // }
}

void dealCards(Card deck[], Hand hands[], int numPlayers)
{

    for (int i = 0; i < HAND_SIZE; i++)
    {
        for (int j = 0; j < numPlayers; j++)
        {
            hands[j].cards[i] = pop(deck);
            hands[j].size++;
        }
    }
}

void displayHand(Hand *hand)
{
    for (int i = 0; i < hand->size; i++)
    {
        printf("%c%d ", hand->cards[i].suit, hand->cards[i].rank);
    }
    printf("\n");
}

void displayCard(Card cards[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%c%d ", cards[i].suit, cards[i].rank);
    }
    printf("\n");
}

int compareCards(const void *a, const void *b)
{
    Card *cardA = (Card *)a;
    Card *cardB = (Card *)b;

    // First, compare by suits (alphabetically)
    int suitComparison = cardA->suit - cardB->suit;
    if (suitComparison != 0)
    {
        return suitComparison;
    }

    // If suits are the same, compare by rank
    return cardA->rank - cardB->rank;
}

// Function to sort a player's hand
void sortHand(Hand *hand)
{
    qsort(hand->cards, hand->size, sizeof(Card), compareCards);
}

void sortCard(Card cards[], int n)
{
    qsort(cards, n, sizeof(Card), compareCards);
}

int find(Hand *hands)
{
    printf("\n");
    printf("Enter the card to be dropped : ");
    scanf(" %c%d", &drop.suit, &drop.rank);
    int flag;
    for (int i = 0; i < hands->size; i++)
    {
        if (drop.suit == hands->cards[i].suit && drop.rank == hands->cards[i].rank)
        {
            flag = 1;
            return i;
        }
        else
        {
            flag = -1;
        }
    }
    if (flag == -1)
    {
        printf("\033[1;31m");
        printf("\nThe Card Doesn't Exist");
        printf("\033[0m");
        printf("\n");
        return find(hands);
    }
}

Card delete(Hand *hands, int index)
{
    for (int i = 0; i < hands->size; i++)
    {
        if (i == index)
        {
            open = hands->cards[index];
            // printf("The Open Card is  %c%d\n", open.suit, open.rank);
            for (int j = index; j < hands->size - 1; j++)
            {
                hands->cards[j] = hands->cards[j + 1];
            }
            hands->size--;
            break;
        }
    }
    return open;
}

int pureRummy(Card rummy[], int n)
{
    char e = rummy[0].suit;
    int i = rummy[0].rank;
    int c = 0, c1 = 0;
    for (int k = 0; k < n; k++)
    {
        if (rummy[k].suit == e)
        {
            c++;
        }
        else
        {
            return -1;
        }

        if (rummy[k].rank == i % 13 || rummy[k].rank == 13)
        {
            printf("%d ", i);
            i++;
            c1++;
        }
        else
        {
            return -1;
        }
    }
    if (c == n && c1 == n)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
int subRummy(Card rummy[], int n)
{
    int q = 0, r = 0;
    char e = rummy[0].suit;
    while (e == 'j')
    {
        e = rummy[q++].suit;
    }
    int i = rummy[0].rank;
    while (i == 0)
    {
        i = rummy[r++].rank;
    }
    int c = 0, joker = 0;
    if (rummy[0].suit == 'j' && rummy[0].rank == 0)
    {
        i--;
    }
    for (int k = 0; k < n; k++)
    {

        if (rummy[k].suit == e)
        {
            c++;
        }
        else if (rummy[k].suit == 'j' && rummy[k].rank == 0)
        {
            joker++;
        }
        else
        {

            return -1;
        }

        if (rummy[k].rank == i % 13 || rummy[k].rank == 13)
        {

            i++;
            c++;
        }
        else if (rummy[k].suit == 'j' && rummy[k].rank == 0)
        {

            i++;
        }
        else
        {

            return -1;
        }
    }

    if (n == c - joker)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
int Mokka(Card rummy[], int n)
{
    int c = 0, c1 = 0;
    char suits[] = {'h', 'c', 'd', 's'};
    int i = rummy[0].rank;
    for (int j = 0; j < n; j++)
    {
        if (rummy[j].rank == i)
        {
            c++;
        }
        else
        {
            return -1;
        }

        for (int k = 0; k < 4; k++)
        {
            if (rummy[j].suit == suits[k])
            {
                suits[k] = '\0';
                c1++;
            }
        }
    }
    if (c == n && c1 == n)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int subMokka(Card rummy[], int n)
{
    int c1 = 0, r = 0;
    int i = rummy[0].rank;
    char suits[] = {'h', 'c', 'd', 's'};
    while (i == 0)
    {
        i = rummy[r++].rank;
    }
    int c = 0, joker = 0;
    printf("i value %d\n", i);
    for (int j = 0; j < n; j++)
    {
        if (rummy[j].rank == i)
        {
            c++;
        }
        else if (rummy[j].suit == 'j' && rummy[j].rank == 0)
        {
            joker++;
            c++;
            continue;
        }
        else
        {
            // printf("fool");
            return -1;
        }

        for (int k = 0; k < 5; k++)
        {
            if (rummy[j].suit == suits[k])
            {
                suits[k] = '\0';
                c1++;
            }
            else if (rummy[j].suit == 'j')
            {
                continue;
            }
        }
    }
    printf("c = %d,c1 = %d,joker = %d", c, c1, joker);
    if (c == n && c1 + joker == n)
    {
        return 1;
    }
    else
    {
        // printf("fool1");
        return -1;
    }
}

int checkarray(Card arr1[], Card arr2[], int n)
{
    int c = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr1[i].suit == arr2[i].suit && arr1[i].rank == arr2[i].rank)
        {
            c++;
        }
    }
    if (c == n)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void dis2d(Card arr[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%c%d ", arr[i][j].suit, arr[i][j].rank);
        }
    }
}

void removeCardRummy()
{
}

int checkInput(Hand *hands, Card rummy[], int n)
{
    int flag;
    for (int q = 0; q < n; q++)
    {
        for (int i = 0; i < hands->size; i++)
        {
            if (rummy[q].suit == hands->cards[i].suit && rummy[q].rank == hands->cards[i].rank)
            {
                flag = 1;
                break;
            }
            else
            {
                flag = -1;
                continue;
            }
        }
    }
    return flag;
}
int rummyRepeat(int num)
{
    int flag = 0;
    for (int i = 0; i < POS; i++)
    {
        for (int k = 0; k < POS; k++)
        {
            old[k] = playerrummy1[i][k];
        }

        int v = checkarray(rummy, old, num);
        if (v == -1)
        {
            for (int p = 0; p < num; p++)
            {
                playerrummy1[i][p] = rummy[p];
            }
            continue;
        }
        else
        {
            flag = -1;
            break;
        }
    }
}
void result(int r)
{
    if (r == 0)
    {
        r1++;
        printf("\033[1;32m");
        printf("\nYou have collected a Rummy set : %d\n", r1);
        printf("\033[0m");
    }
    else
    {
        r2++;
        printf("\033[1;32m");
        printf("\nYou have collected a Rummy set : %d\n", r2);
        printf("\033[0m");
    }
}
void printGradientText(const char *text, int startColor, int endColor)
{
    int colorStep = (endColor - startColor) / (int)strlen(text);

    for (int i = 0; i < (int)strlen(text); ++i)
    {
        printf("\033[38;5;%dm%c", startColor + i * colorStep, text[i]);
    }
    printf("\033[0m\n");
}
void verify(Hand *hands, Card rummy[], int r)
{
    int num = 0;
    int x, y;
    do
    {
        printf("1. 4 cards rummy\n2. 3 cards rummy\n\nOption : ");
        scanf("%d", &x);
        switch (x)
        {
        case 1:
            num = 4;
            break;

        case 2:
            num = 3;
            break;
        }
    } while (x != 1 && x != 2);

    do
    {
        printf("1.Pure rummy\n2.Sub rummy\n3.Mokka\n4.Sub Mokka\n\nOption : ");
        scanf("%d", &y);

        printf("Enter the cards to be checked for rummy :");
        for (int i = 0; i < num; i++)
        {
            scanf(" %c%d", &rummy[i].suit, &rummy[i].rank);
        }
        if (checkInput(&hands[r], rummy, num) == 1)
        {
            switch (y)
            {
            case 1:

                if (pureRummy(rummy, num) == 1)
                {
                    if (rummyRepeat(num) != -1)
                    {
                        result(r);
                    }
                    else
                    {
                        printf("\033[1;36m");
                        printf("\nThis Rummy already exist ! Try another\n");
                        printf("\033[0m");
                    }
                }
                else
                {
                    printf("Not a Rummy");
                }

                break;

            case 2:
                if (subRummy(rummy, num) == 1)
                {
                    if (rummyRepeat(num) != -1)
                    {
                        result(r);
                    }
                    else
                    {
                        printf("\033[1;36m");
                        printf("\nThis Rummy already exist ! Try another\n");
                        printf("\033[0m");
                    }
                }
                else
                {
                    printf("Not a Rummy");
                }
                break;

            case 3:
                if (Mokka(rummy, num) == 1)
                {
                    if (rummyRepeat(num) != -1)
                    {
                        result(r);
                    }
                    else
                    {
                        printf("\033[1;36m");
                        printf("\nThis Rummy already exist ! Try another\n");
                        printf("\033[0m");
                    }
                }
                else
                {
                    printf("Not a Rummy");
                }
                break;

            case 4:
                if (subMokka(rummy, num) == 1)
                {
                    if (rummyRepeat(num) != -1)
                    {
                        result(r);
                    }
                    else
                    {
                        printf("\033[1;36m");
                        printf("\nThis Rummy already exist ! Try another\n");
                        printf("\033[0m");
                    }
                }
                else
                {
                    printf("Not a Rummy");
                }
                break;
            }
        }
        else
        {
            printf("\033[1;36m");
            printf("\nEnter the Cards in Your Hands!\n");
            printf("\033[0m");
        }

    } while (y != 1 && y != 2 && y != 3 && y != 4);
}

int main()
{
    Card *deck = malloc(NUM_CARDS * sizeof(Card));
    printf("\n");
    printf("\033[1;36m");
    printf("\t\t\t\t_________________________________\t\t\t\n");
    printf("\n");
    printf("\t\t\t\t|\tTIME TO PLAY RUMMY\t|");
    printf("\n");
    printf("\t\t\t\t_________________________________\t\t\t\n");

    printf("\033[0m");
    printf("\n");
    initializeDeck(deck);
    // printf("top = %d\n", top);
    shuffleDeck(deck);

    Hand *hands = malloc(NUM_PLAYERS * sizeof(Hand));

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        hands[i].size = 0;
    }

    dealCards(deck, hands, NUM_PLAYERS);

    // printf("Rummy Game\n\n");

    int x, n = 0;
    open = pop(deck);

    do
    {

        printf("\033[1;34m");
        printf("\nPlayer%d's turn\n", n + 1);
        printf("\033[0m");

        do
        {
            printf("\n1.Show card\n2.Sort card\n3.Check\n4.Start Your Game\n5.End Game\n\nOption : ");
            scanf("%d", &x);
            switch (x)
            {
            case 1:
                printf("\n");
                printf("\033[1;34m");
                printf("SHOW CARD\n");
                printf("\033[0m");
                displayHand(&hands[n]);
                break;
            case 2:
                printf("\n");
                printf("\033[1;34m");
                printf("SORTED CARDS\n");
                printf("\033[0m");
                sortHand(&hands[n]);
                displayHand(&hands[n]);
                break;
            case 3:
                verify(&hands[n], rummy, n);
                if (r1 == 2)
                {
                    const char *message1 = "Player 1 Wins! Congratulations!";
                    int startColor = 36; // ANSI color code for cyan
                    int endColor = 35;   // ANSI color code for magenta

                    // Print the message with a gradient color effect
                    printGradientText(message1, startColor, endColor);
                    // printf("\nPlayer 1 WON...!");
                }
                else if (r2 == 2)
                {
                    const char *message2 = "Player 1 Wins! Congratulations!";
                    int startColor = 36; // ANSI color code for cyan
                    int endColor = 35;   // ANSI color code for magenta

                    // Print the message with a gradient color effect
                    printGradientText(message2, startColor, endColor);
                    // printf("Player 2 WON...!");
                }
                else
                {
                    continue;
                }
                break;
            case 5:
                exit(0);
            }

        } while (x != 4);

        // clearScreen();

        printf("\033[1;33m");
        printf("\nThe Open Card is %c%d\n", open.suit, open.rank);
        printf("\033[0m");

        printf("\nWhat do you want :\n1.Open Card\n2.Deck Card\n\nOption : ");
        int z;
        scanf("%d", &z);
        switch (z)
        {
        case 2:

            hands[n].cards[hands[n].size++] = pop(deck);
            displayHand(&hands[n]);
            break;

        case 1:

            hands[n].cards[hands[n].size++] = open;
            displayHand(&hands[n]);
            break;
        }

        int dropIndex = find(&hands[n]);
        // printf("%d", dropIndex);
        open = delete (&hands[n], dropIndex);

        sortHand(&hands[n]);
        displayHand(&hands[n]);
        printf("\n");
        printf("\033[1;34m");
        printf("\nFinal status : %d and %d\n", r1, r2);
        printf("\033[0m");

        n++;

        if (n > 1)
        {
            n = 0;
        }
        clearScreen();
    } while (1);

    free(deck);
    free(hands);

    return 0;
}
