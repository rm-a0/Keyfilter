//xrepcim00
//16.10.2023

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_LENGTH 101

//checks how many characters are in the search input
void charCounter(char input[], int *output)
{
    int counter = 0;

    for (int i = 0; input[i] != 0; i++)
        {
            if ((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z') || input[i] == ' ')
            {
                counter ++;
            }
        }
    *output = counter;
}

//converts string to upper case
void upperCaseConverter(char *input)
{
    int length;
    charCounter(input, &length);

    for (int i = 0; i < length; i++)
    {
        if (input[i] > 'Z')
        {
            input[i] = input[i] - ('a' - 'A');
        }
    }
}

int main(int argc, char *argv[])
{
    
    int searchCounter = 0;
    int keyCounter = 0;
    int adressLineCounter = 0;
    int adressCharCounter = 0;
    
    //used for breaking loops
    //condition one counts how many lines in text file are different from the search input
    int conditionOne = 0;
    //condition two ensures that there wont be duplicated characters in allowedKeys array
    int conditionTwo = 0;
    //condition three ensures that when there is only one adress avaliable, that adress will be printed out
    int conditionThree = 0;
    
    char appendKey[] = "0";
    char allowedKeys[MAX_LENGTH] = "";
    char adressList[MAX_LENGTH] = "";
    char searchInput[MAX_LENGTH] = "";
    char matchingAdress[MAX_LENGTH] = "";

    if (isatty(STDIN_FILENO))
    {
	    fprintf(stderr, "No file specified \n");
	    return 1;
    }

    if(argc < 2)
    {
        strcat(searchInput, "");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            //if there is multi word input append all arguments into one string and add spaces between them
            strcat(searchInput, argv[i]);

            //ensures that there is no empty space after last argument
            if (argc - 1 != i)
            {
                strcat(searchInput, " ");
            }
        }
    }


    //converts search input into upper case
    upperCaseConverter(searchInput);

    //checks if search input is correct
    for (int i = 0; i < (int)sizeof(searchInput); i++)
    {
        if ((searchInput[i] > 'Z' || searchInput[i] < 'A') && searchInput[i] != 0 && searchInput[i] != ' ')
        {
            fprintf(stderr, "Incorrect input\n");
            return 1;
        }
    }
    //checks how many characters are in the search input
    charCounter(searchInput, &searchCounter);

    //scans text from txt file until it encounters blank line or breaks condition one
    //condition one breaks when we find match
    while(fgets(adressList, MAX_LENGTH, stdin))
    {
        adressLineCounter ++;

        //converts all characters in line to upper case
        upperCaseConverter(adressList);

        //checks how many characters are in the line
        charCounter(adressList, &adressCharCounter);
        
        //checks if the characters from text file and seacrh input are identical
        for (int i = 0; i < searchCounter + 1; i++)
        {
            if (searchInput[i] == adressList[i])
            {
                continue;
            }
            else
            {
                //checks if the position of i is equal to number of characters in search input (ensures that only consecutive characters are appended)
                //appends a character that doesnt match search input and breaks cycle 
                if (i == searchCounter)
                {
                    appendKey[0] = adressList[i];

                    //checks how many characters are in the allowedKeys before appending them
                    charCounter(allowedKeys, &keyCounter);

                    //before appending characters checks for duplicates
                    for (int i = 0; i < keyCounter; i++)
                    {
                        if(allowedKeys[i] == appendKey[0])
                        {
                            conditionTwo = 1;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    //appends character if it doesnt have duplicate
                    if (conditionTwo == 0)
                    {
                        strcat(allowedKeys, appendKey);
                        //appends adress to new array, so when there is only one adress avaliable, print it out
                        strcat(matchingAdress, adressList);
                        //adds one to condition three
                        //condition three ensures that when there is only one adress avaliable, that adress will be printed out
                        conditionThree ++;
                        break;
                    }
                    //doesnt append character, resets condition, breaks cycle
                    else
                    {
                        //condition three ensures that when there is only one adress avaliable, that adress will be printed out
                        conditionThree ++;
                        //resets condition two
                        conditionTwo = 0;
                        break;
                    }
                }
                //if there is no match adds 1 to condition 3 and breaks cycle
                //condition one counts how many lines in text file are different from the search input
                else
                {
                    conditionOne ++;
                    break;
                }
            } 
        }
    }

    //checks how many characters are in the allowedKeys before sorting them
    charCounter(allowedKeys, &keyCounter);
    
    //sorts string
    for (int i = 0; i < keyCounter; i++)
    {
        for (int j = 0; j < keyCounter - 1; j++)
        {
            if (allowedKeys[j] > allowedKeys[j + 1])
            {
                char temp = allowedKeys[j];
                allowedKeys[j] = allowedKeys[j + 1];
                allowedKeys[j + 1] = temp;
            }
        }
    }
    //checks if there is only one adress avaliable for search input
    if (conditionThree == 1)
    {
        printf("Found: %s\n", matchingAdress);
    }  
    //checks if there are characters in allowedKeys
    else if (keyCounter > 0 && conditionThree != 1)
    {
        printf("Enable: %s\n", allowedKeys);
    }    
    //condition one counts how many lines are different from the search input
    else if (adressLineCounter == conditionOne)
    {
        printf("Not found\n");
    }

    return 0;
}
