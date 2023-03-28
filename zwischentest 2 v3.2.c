#include <stdio.h>
#include <ctype.h>
enum type{p,s,d};
struct carriage
{
    enum type type;
    int pass_nr;
};
struct train
{
    struct carriage carriages[10];
    int lenght;
};
int current_lenght(struct train *);
void printCarriage(struct carriage);
void printTrain(struct train *, int);
char getMenu();
int getPosition(struct train *);
int insertCarriage(struct train *train, int position, struct carriage carriage);
struct carriage getCarriage();
void printCarriage(struct carriage);
int sumCapacity(struct train *, enum type);
void printTrainStats(struct train *);
int main()
{
    printf("\n");
    struct train train;
    // fill everything with 0 in pass_nr to avoid random numbers and be able to count the lenght later on
    for (int i = 0; i < 10; i++)
    {
        train.carriages[i].pass_nr = 0;
    }
    // create a pointer for the train
    struct train *zeiger;
    zeiger = &train;
    int a = 1;
    while (a == 1)
    {
        char input = getMenu();
        if (input == 'p')
        {
            printTrain(zeiger, current_lenght(zeiger));
        }
        if (input == 'n')
        {
            int exit = insertCarriage(zeiger, getPosition(zeiger), getCarriage());
            if (exit == -1)
            {
                printf("Error: Train too long!\n");
            }
            else if (exit == -2)
            {
                printf("Error: Position not possible!\n");
            }
            else if (exit == -3)
            {
                printf("Error: Sleeper only possible directly after two passenger carriages!\n");
            }
        }
        if (input == 's')
        {
            printTrainStats(zeiger);
        }
        if (input == 'x')
        {
            return 0;
        }
    }
}

char getMenu()
{
    printf("Choose Action: print train (p), new carriage (n), print stats (s) or exit (x):\n");
    char input;
    scanf(" %c", &input);
    while (input != 'p' && input != 'n' && input != 's' && input != 'x')
    {
        printf("Input invalid! Try again:\n");
        scanf(" %c", &input);
    }
    return input;
}
struct carriage getCarriage()
{
    char input_type;
    printf("Choose type of carriage: passenger (p), sleeper (s) or diner (d): \n");
    do
    {
        scanf(" %c", &input_type);
        if (input_type != 'p' && input_type != 's' && input_type != 'd')
        {
            printf("Input invalid! Try again:\n");
        }
    } while (input_type != 'p' && input_type != 's' && input_type != 'd');
    int input_capacity = 20;
    printf("Choose capacity (20 - 130): \n");
    do
    {
        scanf(" %d", &input_capacity);
        if (input_capacity < 20 || 130 < input_capacity)
        {
            printf("Input invalid! Try again:\n");
        }
    } while (input_capacity < 20 || 130 < input_capacity);
    input_type = toupper(input_type);
    struct carriage carriage = {input_type, input_capacity};
    return carriage;
}
int getPosition(struct train *train)
{
    int position;
    int lenght = current_lenght(train);

    printf("Choose position for the new carriage (0-%d): \n", lenght);
    do
    {
        scanf(" %d", &position);
        if (position > lenght || position < 0)
        {
            printf("Input invalid! Try again:\n");
        }
    } while (position > lenght || position < 0);
    return position;
}
int insertCarriage(struct train *train, int position, struct carriage carriage)
{
    // get current lenght of the train.
    int lenght = current_lenght(train);

    // check for errors/conditions before start
    if (lenght > 9)
    {
        return -1; // in main function transform to meaning.
    }
    else if (position > lenght)
    {
        return -2; // in main function transform to meaning.
    }
    else if (carriage.type == 'S')
    {
        if (position < 2)
        {
            return -3; // in main function transform to meaning.
        }
        else if (train->carriages[position - 1].type != 'P' || train->carriages[position - 2].type != 'P')
        {
            return -3; // in main function transform to meaning.
        }
    }
        // check if train has a carriage in that position
        if (train->carriages[position].pass_nr == 0)
        {
            train->carriages[position].pass_nr = carriage.pass_nr;
            train->carriages[position].type = carriage.type;
        } // if not, move the carriages and insert the new one
        else if (train->carriages[position].pass_nr != 0)
        {
            for (int i = lenght; i >= position; i--)
            {
                 if (i == 0)
                    {
                        break;
                    }
                train->carriages[i] = train->carriages[i - 1];
            }
            train->carriages[position].pass_nr = carriage.pass_nr;
            train->carriages[position].type = carriage.type;
        }
    return 0;
}
void printTrain(struct train *train, int lenght)
{
    if (current_lenght(train) == 0)
    {
        printf("Train: No Carriages!\n");
    }
    else
    {
        printf("Train: ");
        for (int i = 0; i < lenght; i++)
        {
            if (i == lenght - 1)
            {
                printf("%d:", i);
                printCarriage(train->carriages[i]);
            }
            else
            {
                printf("%d:", i);
                printCarriage(train->carriages[i]);
                printf("-");
            }
        }
        printf(" Length: %d\n", lenght);
    }
}

int current_lenght(struct train *train)
{
    int lenght = 0;

    while (train->carriages[lenght].pass_nr != 0)
    {
        lenght++;
        if (lenght == 10)
        {
            break;
        }
    }
    return lenght;
}
void printCarriage(struct carriage carriage)
{
    if (carriage.pass_nr < 100)
    {
        printf("[%c:0%d]", carriage.type, carriage.pass_nr);
    }
    else
    {
        printf("[%c:%d]", carriage.type, carriage.pass_nr);
    }
}
int sumCapacity(struct train *train, enum type type)
{
    int sum = 0;
    for (int i = 0; i < current_lenght(train); i++)
    {
        if (train->carriages[i].type == type)
        {
            sum += train->carriages[i].pass_nr;
        }
    }
    return sum;
}
void printTrainStats(struct train *train)
{
    printTrain(train, current_lenght(train));
    printf("Capacities:\n");
    if (sumCapacity(train, 'P') >= 120)
    {
        printf("  Passenger: %d - invalid\n", sumCapacity(train, 'P'));
    }
    else
    {
        printf("  Passenger: %d\n", sumCapacity(train, 'P'));
    }
    printf("  Sleeper: %d\n", sumCapacity(train, 'S'));
    printf("  Diner: %d\n", sumCapacity(train, 'D'));
}