/////
/*
	RANDOM LETTER VERSION
	NEEDED AS OF JULY 18, 2025 
	- my data.txt file keeps getting rewritten and its getting way too annoying ( T o T )
	- Needs more testing on broken/invalid inputs
	- pretty much everything is almost fixed, just need clarification on some parts of the specs
	- test cases started!!!
	- MORE COMMENTS!!!!


EXTRA RESOURCES:
https://www.geeksforgeeks.org/c/scanf-and-fscanf-in-c/ [Returning quantities of inputs with fscanf]

https://stackoverflow.com/questions/34874347/what-does-an-asterisk-in-a-scanf-format-specifier-mean [scanf with *, reading a character but not assigning it to a variable]
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h> // purely for rand function, was also used in term 1 space invaders mp

#define LENGTH 200
#define PASSWORD "mygame"


/***
FUNCTION PROTOTYPES:

int isDuplicate(gameData wordlist[], int n, char word[]) 
void addWordsManual(gameData wordlist[], int* entryCount) 
void addFromFile(gameData wordlist[], int* entryCount)
void editWords(gameData wordlist[], int* entryCount)
void deleteWords(gameData wordlist[], int *entryCount)
void gameCheck(playerData *player) 
char randomLetter(char *word) 
void saveGame(playerData *player)
void playTime(playerData *player, gameData wordlist[], int* entryCount) 
void sortHighScores(playerData player[], int* entryCount)
void displayHighScores(playerData player[], int* entryCount)
void quitGame() 
void adminMenu(gameData wordlist[], int* entryCount) 
void mainMenu(gameData wordlist[], int* entryCount) 


***/

// This function changes the text color to red 
void colorRed() { printf("\033[1;31m"); }
// This function changes the text color to yellow
void colorYellow() { printf("\033[1;33m"); }
// This function changes the text color to blue
void colorBlue() { printf("\033[0;34m"); }
// This function changes the text color to purple
void colorPurple() { printf("\033[0;35m"); }
// This function changes the text color to green
void colorGreen() { printf("\033[0;32m"); }
// This function changes the text color to cyan
void colorCyan() { printf("\033[0;36m"); }
// This function resets the colors to default whenever called 
void resetColor() { printf("\033[0m"); }

// structures
struct gameDataTag {
	char strWord[LENGTH];
	char strClue[LENGTH];
};

struct playerDataTag {
	char strName[LENGTH]; // For player username
	int nLevel;      // Default level count 
	int nLives;      // Default lives
	float nScore;      // Default score 
	int nAnswered; // Default questions answered 
	int nSavedGames; // New field to track saves
};

typedef struct gameDataTag gameData;
typedef struct playerDataTag playerData;


/// HELPER FUNCTIONS:

// checks for duplicate words
int isDuplicate(gameData wordlist[], int n, char word[]) {
	int nDuplicate = 0;
	int i;
	for (i = 0; i < n; i++) {
		if (strcmp(wordlist[i].strWord, word) == 0) {
			nDuplicate++;
		}
	}
	return nDuplicate;
}

// character comparison function to help compare and equalize words even with mismatched cases
int wordValidator(char* word1, char* word2){
	int i = 0;
	int wordEqual = 1; // assume the words are always equal at first input
	int len1 = strlen(word1);
	int len2 = strlen(word2);
	char char1; // for each character
	char char2; 
	
	// iterate through every letter and convert to lower case for compartison
	if(len1 != len2){
		wordEqual = 0;
	}
	
	while(word1[i] && word2[i] && wordEqual){
		char1 = word1[i];
		char2 = word2[i];
		
		if(char1 >= 'A' && char1 <= 'Z'){
			char1 += 32; // convert every character in word 1 to lowercase
		}
		
		if(char2 >= 'A' && char2 <= 'Z'){
			char2 += 32; // convert every character in word 2 to lowercase
		}
		
		if(char1 != char2){
			wordEqual = 0; // words are no longer equal once a single character from each does not match
		}
		
		i++;
	}
	
	// 
	if(word1[i] != word2[i]){
		wordEqual = 0;
	}
	
	return wordEqual;
}

// saves the modified word data, to be called after editWords and deleteWords
void saveWordlist(gameData wordlist[], int entryCount){
	FILE *fData = fopen("data.txt", "w");
	int i;
	if(fData != NULL){
		for(i = 0; i < entryCount; i++){
			fprintf(fData, "%s\n0\n%s\n", wordlist[i].strWord, wordlist[i].strClue);
		}
		fclose(fData);
	}
}

/// MAIN FUNCTIONS:

// allows user to add words
void addWordsManual(gameData wordlist[], int* entryCount) {
	char addPrompt = 'y';
	char inputtedWord[LENGTH];

	printf("You have selected MANUAL ADDITION!\n");

	while (addPrompt == 'y' || addPrompt == 'Y') {
		printf("Please add a word: ");
		scanf(" %[^\n]", inputtedWord);

		if (isDuplicate(wordlist, *entryCount, inputtedWord) > 0) {
			printf("Duplicate word. Please enter a different one\n");
		} else {
			strcpy(wordlist[*entryCount].strWord, inputtedWord);

			printf("Please add a clue: ");
			scanf(" %[^\n]", wordlist[*entryCount].strClue);

			(*entryCount)++;

			printf("Added a new word and clue!\n");
		}

		printf("Do you want to add another word? [y/n]: ");
		scanf(" %c", &addPrompt);

		if (addPrompt != 'Y' && addPrompt != 'N' && addPrompt != 'y' && addPrompt != 'n') {
			printf("Invalid choice.\n");
		}
	}
}

// adds words from file input
void addFromFile(gameData wordlist[], int* entryCount){
	FILE *fAdd = NULL; 
	char filename[50];
	char word[LENGTH]; // keeps words from inputted file
	char clue[LENGTH]; // keeps clues from inputted file
	int openedFile = 0;
	int nInputted = 1; // ensures that line reading is successful for all 3 required inputs (word, 1/0 indicator/)
	int nUsed = 0; // 1 if used, 0 if not yet
	int nAdded = 0;
	int nDuplicate = isDuplicate(wordlist, *entryCount, word);
	
	printf("Enter a file to import: ");
	scanf(" %[^\n]", filename);
	strcat(filename, ".txt"); // adds a .txt to ensure that the program is processing a text file even if user inputs a non-textfile format
	
	fAdd = fopen(filename, "r");
	openedFile = (fAdd != NULL);
	
	if(openedFile){
		nInputted = (fscanf(fAdd, " %[^\n]", word) == 1); // only 1 word can be read at a time
		nInputted = nInputted && (fscanf(fAdd, " %d", &nUsed) == 1); // only 1 used flag can be read at a time
		nInputted = (fscanf(fAdd, " %[^\n]", clue) == 1); // only 1 clue can be read at a time
		
		while(nInputted && *entryCount < 200){
			if(nDuplicate > 0){
				printf("Duplicate word. Please enter a different one\n");
			} else {
				strcpy(wordlist[*entryCount].strWord, word);
				strcpy(wordlist[*entryCount].strClue, clue);
				(*entryCount)++;
				nAdded++;
				printf("Added entries successfully!\n");
			}
			
			// reading the next set of words
			nInputted = (fscanf(fAdd, " %[^\n]", word) == 1); // only 1 word can be read at a time
			nInputted = nInputted && (fscanf(fAdd, " %d", &nUsed) == 1); // only 1 used flag can be read at a time
			nInputted = (fscanf(fAdd, " %[^\n]", clue) == 1); // only 1 clue can be read at a time
		}
		fclose(fAdd);
	}
	if(openedFile){
		printf("Added %d new words from file %s\n", nAdded, filename);
	} else {
		printf("ERROR!\n");
	}
}

/// displays all words in struct wordlist[] and gives the user the option to select an index and edit the word in that index
void editWords(gameData wordlist[], int* entryCount){
	int editIndex;
	int editSelect;
	int i;
	int validInput = 0;
	char newWord[LENGTH];
	char newClue[LENGTH];
	
	if(*entryCount > 0)
	{
	
		printf("INDEX - WORD - CLUE\n");
		
		for(i = 0; i < *entryCount; i++){
			printf("%d - %s - %s\n", i+1, wordlist[i].strWord, wordlist[i].strClue);
		}
		printf("\n");
		
		validInput = 0;
		while(!validInput){
			printf("Do you wish to edit a WORD [0] or a CLUE [1]?\n");
			scanf("%d", &editSelect);
			
			if(editSelect == 1 || editSelect == 0){
				validInput = 1;
			}
			
			scanf("%*[^\n]"); 
			scanf("%*c");
		}
		
		validInput = 0;
		while(!validInput){
			printf("Enter the index of the entry you wish to edit (1 - %d): ", *entryCount);
			scanf("%d", &editIndex);
			editIndex--; // zero-based indexing
			
			if(editIndex >= 0 && editIndex < *entryCount){
				validInput = 1;
			}
			
			scanf("%*[^\n]"); 
			scanf("%*c");
		}
		
		if(editSelect == 0){	
			printf("Selected word: %s\n", wordlist[editIndex].strWord);
			printf("New word: ");
			scanf(" %[^\n]", newWord);
			if(newWord[0]){
				strcpy(wordlist[editIndex].strWord, newWord);
			}
		} else if(editSelect == 1){
			printf("Selected clue: %s\n", wordlist[editIndex].strClue);
			printf("New clue: ");
			scanf(" %[^\n]", newClue);
			
			if(newClue[0]){
				strcpy(wordlist[editIndex].strClue, newClue);
			}
		} else {
			printf("Invalid entry. Please enter either 0 for WORDS and 1 for CLUES");
		}
		printf("Updated entry %d: %s - %s\n", editIndex, 
		wordlist[editIndex].strWord, wordlist[editIndex].strClue);
	} else {
		printf("No available entries for editing :(\n");
	}
	saveWordlist(wordlist, *entryCount);
}

// deletes a word (copypaste from term2 mp)
void deleteWords(gameData wordlist[], int *entryCount){
    int index;
    int i;

    if (*entryCount == 0) {
        printf("No entries to delete.\n");
    } else {
       printf("INDEX - WORD - CLUE\n");
		
		for(i = 0; i < *entryCount; i++){
			printf("%d - %s - %s\n", i+1, wordlist[i].strWord, wordlist[i].strClue);
		}
		printf("\n");
        
        printf("Enter the number of the entry to delete (1-%d): ", *entryCount);
        scanf("%d", &index);

        if (index >= 1 && index <= *entryCount) {
            index--; // Convert to zero-based index
            i = index;

            // Shift only if it's not the last entry
            while (i < *entryCount - 1) {
                wordlist[i] = wordlist[i + 1];
                i++;
            }

            (*entryCount)--; // Reduce the total count
            printf("Selected entry was deleted successfully.\n");
        } else {
            printf("Invalid entry number.\n");
        }
    }
	saveWordlist(wordlist, *entryCount);
}

// this will check if player has previous saved data
void gameCheck(playerData *player) {
	int nSaves = player->nSavedGames;
	char choice;

	if (nSaves > 0) {
		printf("Saved game found for %s.\n", player->strName);
		printf("Level: %d, Lives: %d, Score: %d\n", player->nLevel, player->nLives, player->nScore);
		printf("Continue saved game? (Y/N): ");
		scanf(" %c", &choice);

		if (choice == 'N' || choice == 'n') {
			player->nLevel = 1;
			player->nLives = 3;
			player->nScore = 0;
			player->nSavedGames = 0;
			player->nAnswered = 0;
			
			printf("Starting anew!\n");
		} else {
			printf("Continuing where we left off. . .\n");
		}
	} else {
		printf("No saves available. Starting new game for %s.\n", player->strName);
		player->nLevel = 1;
		player->nLives = 3;
		player->nScore = 0;
		player->nSavedGames = 0;
		player->nAnswered = 0;
	}
}

// this will pull a random letter from a given word 
char randomLetter(char *word) {
	int len = strlen(word);
	int letterIndex = 0;
	char c = '\0';

	if (len > 0) {
		letterIndex = rand() % len;
		c = word[letterIndex];

		// if letter is in lowercase then gawing uppercase before it's returned
		if (c >= 'a' && c <= 'z') {
			c -= 32; // convert to uppercase manually
		}
	} 
	
	return c;
}

// saves the game
void saveGame(playerData *player, gameData wordlist[], int* entryCount){
    FILE *fGame = fopen("saved.txt", "w");;
    FILE *fData = fopen("data.txt", "w");
	int i;
	int used; 

	if(fGame){
		fprintf(fGame, "%s|%d|%d|%.1f\n", player->strName, player->nLevel, player->nLives, player->nScore);
		fclose(fGame);
	}

	if(fData){
		for(i = 0; i < *entryCount; i++){
			if(i >= (*entryCount - player->nAnswered)){
				used = 1; 
				fprintf(fData, "%s\n%d\n%s\n", wordlist[i].strWord, used, wordlist[i].strClue);
			}
		}
		fclose(fData);
	}
   
}

// main game function yeyy
void playTime(playerData *player, gameData wordlist[], int* entryCount) {
    gameData temp; // will be used for keeping track of words used
    FILE *scoresFile = NULL;
    float points = 0;
    int currentWord;
    int wordLength;
    int foundWord;
    char strAnswer[LENGTH];
    char cLetter;
    char cSave = 'y'; // saving even before the game ends (for the sake of loading saved games)
    char saveCode[LENGTH];
    int i;

    printf("Please input player username: ");
    scanf(" %49[^\n]", player->strName);

    gameCheck(player);
	
    if (*entryCount > 0 && (cSave == 'y' || cSave == 'Y')) {
        while (player->nLives > 0) {
            foundWord = 0;
            currentWord = -1; // initialize to invalid index

            i = 0;
			while (i < *entryCount && foundWord == 0) {
                wordLength = strlen(wordlist[i].strWord); 
                    
                if (player->nLevel <= 3 && wordLength <= 4){ 
				// +2.5 points for the first 3 levels/questions, words will be at most 4 characters
                    currentWord = i;
                    foundWord = 1;
                    points = 2.5;
                } else if (player->nLevel <= 5 && wordLength >= 5 && wordLength <= 8){
				// +4.0 points for the next 2 levels/questions (lvl 4 and 5), words will be at most 8 characters
                    currentWord = i;
                    foundWord = 1;
                    points = 4.0;
                } else if (player->nLevel >= 6 && wordLength > 9) { 
				// +7.0 points for succeeding levels (all after 5) using words with more than 9 characters
                    currentWord = i;
                    foundWord = 1;
                    points = 7.0;
				}
				i++;
            }

           // Game status
            if (!foundWord) {
                colorRed();
                printf("No words available for Level %d!\n", player->nLevel);
                resetColor();
                player->nLives = 0;  // End game if no valid words
            } else {
                cLetter = randomLetter(wordlist[currentWord].strWord); 

                printf("\nLEVEL %d\n", player->nLevel);
                printf("WORD LENGTH: %d\n", wordLength); 
                printf("CLUE: %s\n", wordlist[currentWord].strClue); 
                printf("LETTER: %c\n", cLetter);

                printf("Enter your guess: ");
                scanf(" %49[^\n]", strAnswer);
                
				// if the word is correct 
				/// NOTE: strcmp cannot directly be used here because of the case-insensitive requirement :<
                if (wordValidator(wordlist[currentWord].strWord, strAnswer)) { 
                    printf("Correct!\n");
                    player->nScore += points; 
                    player->nLevel++;
                    player->nAnswered++;
                    
                    // indicates used word
                    temp = wordlist[currentWord];
                    wordlist[currentWord] = wordlist[*entryCount-1];
                    wordlist[*entryCount-1] = temp;
                    (*entryCount)--;
                } 
                else {
                    colorRed();
                    printf("Incorrect. Try that again..\n");
                    resetColor();
                    player->nLives--;
                }

                printf("Score: %.1f | Lives: %d\n", player->nScore, player->nLives);  
				printf("Do you want to continue? [Y/N]: ");
				scanf(" %c", &cSave);
				
				if(cSave == 'N' || cSave == 'n'){
                    saveGame(player, wordlist, entryCount);
                    printf("Game saved!\n");
					
					printf("Enter your own specialized code to save your score!: ");
					scanf(" %[^\n]", saveCode);
					
					scoresFile = fopen("scores.txt", "a");
					if(scoresFile != NULL){
						fprintf(scoresFile, "%s\n%.1f\n%d\n\n", player->strName, player->nScore, player->nAnswered);
						fclose(scoresFile);
					}
                }
            }
        }

		// FIXED SINCE LAST INQUIRY: 
        if (player->nLives == 0) {
            colorYellow();
            printf("\nGAME OVER :(\n");
			printf("The correct word was: %s\n", wordlist[currentWord].strWord);
            resetColor();
            colorGreen();
            printf("Your total score was %.1f!\n", player->nScore);
            resetColor();
			
            saveGame(player, wordlist, entryCount);
            printf("Game saved!\n");
            
            printf("Enter your own specialized code to save your score!: ");
            scanf(" %[^\n]", saveCode);
            
            scoresFile = fopen("scores.txt", "a");
            if(scoresFile != NULL){
                fprintf(scoresFile, "%s\n%.1f\n%d\n\n", player->strName, player->nScore, player->nAnswered);
                fclose(scoresFile);
            }
        }
    } else {
        printf("No words available to play! Please add words first.\n");
    }
}

// sorting function using selection sort 
void sortHighScores(playerData player[], int* entryCount){
	int i;
	int j;
	int nHigh = 0;	
	playerData nTemp;
	
	for(i = 0; i < *entryCount - 1; i++){
		nHigh = i;
		for(j = i+1; j < *entryCount; j++){
			if(player[j].nScore > player[nHigh].nScore){
				nHigh = j;
			}
		}
		
		if(nHigh != i){
			nTemp = player[i];
			player[i] = player[nHigh];
			player[nHigh] = nTemp;
		}
	}
}

// displays the high scores
void displayHighScores(){
	FILE *fHighScores = NULL;
	playerData scores[100]; // an array of data for 100 scorers
	int count = 0; 
	int i;
	
	fHighScores = fopen("scores.txt", "r");
    if(fHighScores != NULL) {
        
        while(count < 100 && fscanf(fHighScores, " %[^|]|%f|%d ",  scores[count].strName, &scores[count].nScore, &scores[count].nAnswered)) {
            count++;
        }
        fclose(fHighScores);
    }
	
	sortHighScores(scores, &count);
	
	printf("PLAYER   |   CORRECT ANSWERS   |   HIGH SCORE\n");
	for(i = 0; i < count; i++){
		printf("%s   %d   %.1f\n", scores[i].strName, scores[i].nAnswered, scores[i].nScore);
	}
	
	if(count == 0){
		printf("No scores added yet~\n");
	}
}

// loads saves with lives remaining
void loadSave(playerData *player, gameData wordlist[], int* entryCount, int* loaded){
   FILE *fGame = NULL;
   FILE *fData = NULL;
   playerData savedPlayers[101];
   int count = 0; // counts the players encountered
   int choice = 0;
   int valid = 0; 
   int i;
   int opened = 0;
   int loading = 1; // tracks if the file is supposed to load
   int contread = 1; // keep reading unless stated otherwise
   
   fGame = fopen("saved.txt", "r");
   if(fGame != NULL){
	   opened = 1;
   } else {
	   printf("Error encountered while opening file :(");
   }
   
   if(opened){
	   while(count < 101 && loading){
		   if(fscanf(fGame, "%[^|]|%d|%d|%f\n", savedPlayers[count].strName, &savedPlayers[count].nLevel, &savedPlayers[count].nLives, &savedPlayers[count].nScore) == 4){
			   // if all 4 needed data are scanned, add to the player counter
			   count++;
		   } else {
			   loading = 0;
		   }
	   }
	   fclose(fGame);
   }
   
   if(count == 0){
	   printf("No saved games found :(");
   } else {
	   printf("PLAYER NUMBER | USERNAME | LEVEL | LIVES | SCORE\n");
	   for(i = 0; i < count; i++){
		   printf("%d | %s | %d | %d | %.2f\n", i+1, player[i].strName, player[i].nLevel, player[i].nLives, player[i].nScore);
	   }
	   
	   printf("Enter player number: ");
	   scanf("%d",&choice);
	   
	   if(choice >= 1 && choice <= count){
		   valid = 1;
	   } else {
		   printf("No such player exists...:\\n");
	   }
	   
	   if(valid){
		   // loading the player's data
		   *player = savedPlayers[choice-1];
		   player->nAnswered = 0;
		   *entryCount = 0;
		   
		   fData = fopen("data.txt", "r"); // read the word data
		   if(fData != NULL){
			   opened = 1;
		   }
		   
		   if(opened){
			   contread = 1;
			   while(*entryCount < 200 && contread){
				   if (fscanf(fData, "%[^\n] %*d %[^\n] %*[^\n]", wordlist[*entryCount].strWord, wordlist[*entryCount].strClue) == 2) {
                        (*entryCount)++;
                    } else {
                        contread = 0;
                    }
			   }
			   fclose(fData);
		   }
		   *loaded = 1;
		   printf("Game loaded successfully :D\n");
	   }
   }
}

// this will quit the game
void quitGame() {
	printf("Thank you for your time!\n");
}

// admin menu
void adminMenu(gameData wordlist[], int* entryCount) {
	int nChoice;
	char passwordvalid[50];

	printf("STOP!\n Before you continue, please enter the developer's intended password: ");
	scanf("%s", passwordvalid);

	if (strcmp(PASSWORD, passwordvalid) == 0) {
		do {
			printf("+--------------------------------------------+\n");
			printf("|~~~  Welcome to GUESSING WORD GAME     ~~~|\n");
			printf("|           You are in ADMIN OPTIONS         |\n");
			colorGreen();
			printf("|              [1] MANUAL WORD ADDITION      |\n");
			colorYellow();
			printf("|              [2] ADD WORDS FROM FILE       |\n");
			colorCyan();
			printf("|              [3] EDIT WORDS                |\n");
			colorCyan();
			printf("|              [4] DELETE WORDS              |\n");
			colorCyan();
			printf("|              [5] EXIT                      |\n");
			resetColor();
			printf("+--------------------------------------------+\n\n\n");

			printf("Enter choice: ");
			scanf("%d", &nChoice);
			
			switch(nChoice){
				case 1: 
					addWordsManual(wordlist, entryCount);
					break;
				case 2: 
					addFromFile(wordlist, entryCount);
					break;
				case 3: 
					editWords(wordlist, entryCount);
					break;
				case 4: 
					deleteWords(wordlist, entryCount);
					break;
				case 5:
					quitGame();
					break;
				default: printf("Invalid choice. Try again.\n");
			}
		} while (nChoice != 5);
	} else {
		printf("Wrong password. Try again!\n");
	}
}

// main game menu
void mainMenu(gameData wordlist[], int* entryCount) {
	int nChoice;
	int loaded = 0;
	playerData player;

	do {
		printf("+--------------------------------------------+\n");
		printf("|~~~  Welcome to GUESSING WORD GAME     ~~~|\n");
		printf("|                  Options:                  |\n");
		colorGreen();
		printf("|              [1] ADMIN                     |\n");
		colorYellow();
		printf("|              [2] PLAY                      |\n");
		colorCyan();
		printf("|              [3] HIGH SCORE                |\n");
		colorCyan();
		printf("|              [4] SAVED GAMES               |\n");
		colorCyan();
		printf("|              [5] EXIT                      |\n");
		resetColor();
		printf("+--------------------------------------------+\n\n\n");

		printf("Enter choice: ");
		scanf("%d", &nChoice);

		switch(nChoice){
			case 1: 
				adminMenu(wordlist, entryCount);
				break;
			case 2: 
				playTime(&player, wordlist, entryCount);
				break;
			case 3: 
				displayHighScores();
				break;
			case 4: 
				loadSave(&player, wordlist, entryCount, &loaded);
				if(loaded){
					playTime(&player, wordlist, entryCount);
				}
				break;
			case 5:
				quitGame();
				break;
			default: printf("Invalid choice. Try again.\n");
		}
	} while (nChoice != 5);
}

// main
int main() {
	gameData wordlist[200];
	int entryCount = 0;

	mainMenu(wordlist, &entryCount);

	return 0;
}
