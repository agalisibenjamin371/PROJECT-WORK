#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TEXT 200000
#define MAX_WORDS 1000
#define WORD_LEN 50

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

void progressBar();
void wordAnalyzer(char text[], char targetWord[], char filename[]);

int main()
{
    char text[MAX_TEXT];
    char word[WORD_LEN];
    char filename[200];
    int choice;

    while(1)
    {
        printf(CYAN "\n========== WORD ANALYZER ==========\n" RESET);
        printf("1. Manual Input\n");
        printf("2. Load Text File\n");
        printf("3. Quit\n");
        printf("Choose option: ");

        if(scanf("%d",&choice)!=1)
        {
            printf(RED "Invalid input.\n" RESET);
            while(getchar()!='\n');
            continue;
        }

        getchar();

        if(choice == 3)
        {
            printf(GREEN "Exiting program...\n" RESET);
            break;
        }

        if(choice == 1)
        {
            printf(YELLOW "\nEnter text:\n" RESET);
            fgets(text,MAX_TEXT,stdin);
            strcpy(filename,"Manual Input");
        }

        else if(choice == 2)
        {
            printf("Enter file path or file name:\n");
            fgets(filename,200,stdin);

            filename[strcspn(filename,"\n")] = 0;

            FILE *file = fopen(filename,"r");

            if(file == NULL)
            {
                printf(RED "File not found. Returning to menu...\n" RESET);
                continue;
            }

            size_t read = fread(text,1,MAX_TEXT-1,file);
            text[read] = '\0';

            fclose(file);
        }

        else
        {
            printf(RED "Invalid option.\n" RESET);
            continue;
        }

        progressBar();

        printf(GREEN "\nEnter word to search: " RESET);
        scanf("%s",word);

        wordAnalyzer(text,word,filename);

        printf("\n");
        printf("1. Run Again\n");
        printf("2. Return to Main Menu\n");
        printf("3. Quit\n");

        int next;
        printf("Choose option: ");
        scanf("%d",&next);
        getchar();

        if(next == 3)
            break;
        else if(next == 1)
            continue;
        else
            continue;
    }

    return 0;
}

void progressBar()
{
    printf(YELLOW "\nAnalyzing Text...\n" RESET);

    for(int i=0;i<=100;i+=10)
    {
        printf("\rProgress: [");

        int bars = i/10;

        for(int j=0;j<bars;j++)
            printf("#");

        for(int j=bars;j<10;j++)
            printf(" ");

        printf("] %d%%",i);

        fflush(stdout);

        for(long k=0;k<70000000;k++);
    }

    printf("\n");
}

void wordAnalyzer(char text[], char targetWord[], char filename[])
{
    int letters=0,spaces=0,words=0,sentences=0,occurrences=0;

    char tempWord[WORD_LEN];
    int index=0;

    char wordList[MAX_WORDS][WORD_LEN];
    int freq[MAX_WORDS]={0};
    int uniqueCount=0;

    for(int i=0;targetWord[i];i++)
        targetWord[i]=tolower(targetWord[i]);

    for(int i=0;text[i]!='\0';i++)
    {
        char c=text[i];

        if(isalpha(c))
            letters++;

        if(isspace(c))
            spaces++;

        if(c=='.'||c=='!'||c=='?')
            sentences++;

        if(isalpha(c))
        {
            tempWord[index++]=tolower(c);
        }
        else
        {
            if(index>0)
            {
                tempWord[index]='\0';
                words++;

                if(strcmp(tempWord,targetWord)==0)
                    occurrences++;

                int found=0;

                for(int j=0;j<uniqueCount;j++)
                {
                    if(strcmp(wordList[j],tempWord)==0)
                    {
                        freq[j]++;
                        found=1;
                        break;
                    }
                }

                if(!found && uniqueCount<MAX_WORDS)
                {
                    strcpy(wordList[uniqueCount],tempWord);
                    freq[uniqueCount]=1;
                    uniqueCount++;
                }

                index=0;
            }
        }
    }

    printf(CYAN "\n===== ANALYSIS RESULT =====\n" RESET);

    printf("Source: %s\n",filename);
    printf("Letters: %d\n",letters);
    printf("Spaces: %d\n",spaces);
    printf("Words: %d\n",words);
    printf("Sentences: %d\n",sentences);
    printf("Occurrences of \"%s\": %d\n",targetWord,occurrences);

    printf("\nTop 5 Most Frequent Words:\n");

    for(int i=0;i<5 && i<uniqueCount;i++)
    {
        int maxIndex=i;

        for(int j=i+1;j<uniqueCount;j++)
        {
            if(freq[j]>freq[maxIndex])
                maxIndex=j;
        }

        int tempFreq=freq[i];
        freq[i]=freq[maxIndex];
        freq[maxIndex]=tempFreq;

        char tempStr[WORD_LEN];
        strcpy(tempStr,wordList[i]);
        strcpy(wordList[i],wordList[maxIndex]);
        strcpy(wordList[maxIndex],tempStr);

        printf("%s : %d\n",wordList[i],freq[i]);
    }
}