#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HAND_BUFFER 256
#define HAND_SIZE 5

typedef enum 
{
    fiveofakind = 7,
    fourofakind = 6,
    fullhouse = 5,
    threeofakind = 4,
    twopair = 3,
    onepair = 2,
    highcard = 1
} type;

typedef struct 
{
    char cards[HAND_SIZE];
    int bid;
    type handtype;
} hand;

static int HANDSBUFFER = HAND_BUFFER;
static int HANDSCOUNT = 0;
static hand *hands;

int getcardvalue(char card)
{
    if(isdigit(card))
        return card - '0';
    switch (card) {
    case 'A':
        return 14;
    case 'K':
        return 13;
    case 'Q':
        return 12;
    case 'J':
        return 11;
    case 'T':
        return 10;
    }
    return 0;
}

void addhand(hand h)
{
    if (HANDSBUFFER <= HANDSCOUNT)
    {
        HANDSBUFFER *= 2;
        hands = realloc(hands, HANDSBUFFER * sizeof(hand));
    }
    hands[HANDSCOUNT] = h;
    HANDSCOUNT++;
}

char worthmore(hand *h1, hand *h2)
{
    if (h1->handtype > h2->handtype)
        return 1;
    if (h1->handtype < h2->handtype)
        return 0;
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (getcardvalue(h1->cards[i]) > getcardvalue(h2->cards[i]))
            return 1;
        if (getcardvalue(h1->cards[i]) < getcardvalue(h2->cards[i]))
            return 0;
    }
    return 1;
}

hand **orderhands() {
    hand **sortedhands = malloc(HANDSCOUNT * sizeof(hand *));
    sortedhands[0] = &hands[0];
    int sortedcount = 1;
    for (int i = 1; i < HANDSCOUNT; i++)
    {
        int checkpos = 0;
        while (sortedcount != checkpos && worthmore(&hands[i], sortedhands[checkpos]))
        {
            checkpos++;
        }
        if (sortedcount != checkpos)
            memmove(&sortedhands[checkpos+1], &sortedhands[checkpos], (sortedcount - checkpos)*sizeof(hand *));
        sortedhands[checkpos] = &hands[i];
        sortedcount++;
    }

    return sortedhands;
}

void sethandtype(hand *hand)
{
    char cardcount[HAND_SIZE] = {0};
    for (int i = 0; i < HAND_SIZE; i++)
    {
        char current = hand->cards[i];
        for (int j = 0; j < HAND_SIZE; j++)
            if (hand->cards[j] == current)
                cardcount[i]++;
    }
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 5)
        {
            hand->handtype = fiveofakind;
            return;
        }
    }
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 4)
        {
            hand->handtype = fourofakind;
            return;
        }
    }
    char hasthree = 0;
    char hastwo = 0;
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 3)
            hasthree = 1;
        if (cardcount[i] == 2)
            hastwo = 1;
        if (hasthree && hastwo)
        {
            hand->handtype = fullhouse;
            return;
        }
    }
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 3)
        {
            hand->handtype = threeofakind;
            return;
        }
    }
    char firstpair = '\0';
    char hasfirstpair = 0;
    char hassecondpair = 0;
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 2 && !hasfirstpair)
        {
            firstpair = hand->cards[i];
            hasfirstpair = 1;
        }
        else if (cardcount[i] == 2 && hasfirstpair && hand->cards[i] != firstpair)
            hassecondpair = 1;
        if (hasfirstpair && hassecondpair)
        {
            hand->handtype = twopair;
            return;
        }
    }
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (cardcount[i] == 2)
        {
            hand->handtype = onepair;
            return;
        }
    }
    hand->handtype = highcard;
}

void parseinput(FILE *file)
{
    hand h;
    while (fscanf(file, "%c%c%c%c%c %d\n", &h.cards[0], &h.cards[1], &h.cards[2], &h.cards[3], &h.cards[4], &h.bid) == 6)
        addhand(h);
}

void parsefile(char *filepath)
{
    FILE *file;

    file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", filepath);
        exit(0);
    }
    
    hands = malloc(HANDSBUFFER * sizeof(hand));
    
    parseinput(file);

    for (int i = 0; i < HANDSCOUNT; i++)
        sethandtype(&hands[i]);

    hand **sortedlist = orderhands();

    long sum = 0;
    for (int i = 0; i < HANDSCOUNT; i++)
        sum += sortedlist[i]->bid * (i+1);

    printf("Sum of sorted hands: %ld\n", sum);

    fclose(file);
    free(hands);
    free(sortedlist);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Give input file as an argument.");
        exit(0);
    }

    parsefile(argv[1]);

    exit(0);
}
