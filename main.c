#include <stdio.h>
#include <stdlib.h>

//Funktion für Ausgabe bei ungültiger Eingabe:
void invalidInput()
{
    printf("\nInput invalid! Try again:");
}

//---------------------Structs---------------------------------------
//Enumeration zur Definition des Typs eines Waggons:
enum type {passenger = 80, sleeper = 83, diner = 68}; //Die Zahlen sind die jeweiligen großen Anfangsbuchstaben in der ASCII-Tabelle & werden für die "void printCarriage(carriage wagon)" verwendet

//Struktur um einen Waggon zu speichern
struct carriage
{
    enum type type;
    int capacity;
} typedef carriage;

//Struktur um einen Zug zu speichern
struct train
{
    carriage carriages[10];
    int length;
} typedef train;

//-------------------Funktionen zur Ausgabe:-------------------------

//Funktion zur Ausgabe eines Waggons:
void printCarriage(carriage wagon)
{
    printf("[%c:%03d]", wagon.type, wagon.capacity);
}

//Funktion zur Ausgabe eines Zuges:
void printTrain(train *zug)
{

    printf("\nTrain: ");
    if(zug->length == 0)
    {
        printf("No Carriages!"); //Ausgabe, wenn der Zug noch keine Wagons hat
    }
    else
    {
        for(int i = 1; i < zug->length; i++)            //Ausgabe aller Wagons vor dem letzten Wagon
        {
            printf("%d:", i-1);
            printCarriage(zug->carriages[i-1]);
            printf("-");
        }
        printf("%d:", zug->length-1);                   //Ausgabe des letzten Wagons (ohne Bindestrich)
        printCarriage(zug->carriages[zug->length-1]);
        printf(" Length: %d", zug->length);
    }
}

//----------------------Eingabefunktionen:---------------------------

//Funktion für Eingabemenü:
char getMenu(char eingabe)
{

    printf("\nChoose Action: print train (p), new carriage (n), print stats (s) or exit (x):");
    scanf(" %c", &eingabe);

    while(eingabe != 'p' && eingabe != 'n' && eingabe != 's' && eingabe != 'x')
    {
        invalidInput();
        scanf(" %c", &eingabe);
    }

    return eingabe;
}

//Funktion zum Erfragen eines Wagontyps & dessen Kapazität:
struct carriage getCarriage()
{
    carriage wagon;
    char eingelesenertype;
    printf("\nChoose type of carriage: passenger (p), sleeper (s) or diner (d):");
    scanf(" %c", &eingelesenertype);

    while(eingelesenertype != 'p' && eingelesenertype != 's' && eingelesenertype != 'd')
    {
        invalidInput();
        scanf(" %c", &eingelesenertype);
    }

    if(eingelesenertype == 'p')
    {
        wagon.type = passenger;
    }
    else if(eingelesenertype == 's')
    {
        wagon.type = sleeper;
    }
    else if(eingelesenertype == 'd')
    {
        wagon.type = diner;
    }

    printf("\nChoose capacity (20 - 130):");
    scanf("%d", &wagon.capacity);

    while(wagon.capacity < 20 || wagon.capacity > 130)
    {
        invalidInput();
        scanf("%d", &wagon.capacity);
    }
    return wagon;
}

//Eingabe der Position eines Waggons:
int getPosition(int length)
{
    int position = 0;
    printf("\nChoose position for the new carriage (0-%d):", length);
    scanf("%d", &position);

    while(position < 0 || position > length)
    {
        invalidInput();
        scanf("%d", &position);
    }
    return position;
}

//-----------Funktion zum Einfügen eines Waggons in den Zug-------------------
int insertCarriage(train *zug, int position, carriage wagon)
{

    if(zug->length == 10)
    {
        return -1;
    }
    else if(position < 0 || position > zug->length)
    {
        return -2;
    }
    else if(wagon.type == sleeper && (position < 2 || zug->carriages[position-1].type != passenger || zug->carriages[position-2].type != passenger))
    {
        return -3;
    }
    else
    {
        for(int i = zug->length-1; i >= position; i--)   //bestehende Wagons hinter dem neu eingefügten Wagon um 1 Position nach hinten verschieben
        {
            zug->carriages[i+1] = zug->carriages[i];
        }
        zug->carriages[position] = wagon;   //neuen Wagon an der eingegebenen Position einfügen
        zug->length++;                      //Länge des Zuges um 1 erhöhen
        return 0;
    }
}

//-----------------Funktionen für die Statistik-------------------------------
int sumCapacity(train *zug, enum type typ)
{
    int summe = 0;

    for(int i = 0; i < zug->length; i++)
    {
        if(zug->carriages[i].type == typ)
        {
            summe += zug->carriages[i].capacity;
        }
    }
    return summe;
}

void printTrainStats(train*zug)
{
    printf("\nCapacities:");
    printf("\n  Passenger: %d", sumCapacity(zug, passenger));
    if(sumCapacity(zug, passenger) > 100)
    {
        printf(" - invalid");
    }
    printf("\n  Sleeper: %d", sumCapacity(zug, sleeper));
    printf("\n  Diner: %d", sumCapacity(zug, diner));
}
//---------------------------------Main---------------------------------------

int main()
{
    char eingabe; //Variable für "char getMenu(char eingabe)" & die if-Verzweigung in der Main
    train zug;
    zug.length = 0;

    do
    {

        eingabe = getMenu(eingabe);

        if(eingabe == 'p')
        {
            printTrain(&zug);
        }
        if(eingabe == 'n')
        {
            int neuerwagon = insertCarriage(&zug, getPosition(zug.length), getCarriage());
            if(neuerwagon == -1)
            {
                printf("\nError: Train too long!");
            }
            else if(neuerwagon == -2)
            {
                printf("\nError: Position not possible!");
            }
            else if(neuerwagon == -3)
            {
                printf("\nError: Sleeper only possible directly after two passenger carriages!");
            }
        }
        if(eingabe == 's')
        {
            printTrain(&zug);
            printTrainStats(&zug);
        }

    }
    while (eingabe != 'x');


    return 0;
}
