///// VERY VERY INCOMPLETE
/*
	NEEDED AS OF JULY 1, 2025 12:50PM:
	- File processing works but accidentally rewriting original files (ToT)
	- Unsure if guesses by user must be case-sensitive if randomizer is Not case-sensitive
	- MORE COMMENTS!!!!


EXTRA RESOURCES:
https://www.geeksforgeeks.org/c/scanf-and-fscanf-in-c/ [Returning quantities of inputs with fscanf]
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
			printf("%d - %s - %s\n", i, wordlist[i].strWord, wordlist[i].strClue);
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
			printf("Enter the index of the entry you wish to edit (0 - %d): ", *entryCount-1);
			scanf("%d", &editIndex);
			
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
	
}

// delete functionplay
void deleteWords(gameData wordlist[], int *entryCount){
    int index;
    int i;

    if (*entryCount == 0) {
        printf("No entries to delete.\n");
    } else {
       printf("INDEX - WORD - CLUE\n");
		
		for(i = 0; i < *entryCount; i++){
			printf("%d - %s - %s\n", i, wordlist[i].strWord, wordlist[i].strClue);
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
/// help this makes me uncofmy
void saveGame(playerData *player, gameData wordlist[], int* entryCount){
	FILE *fGame = NULL;
	FILE *fData = NULL;
	playerData allPlayers[100]; // an array of data for 100 players
	int count = 0;
	int found = 0;
	int userData;
	int used = 0;
	int i;

	// read pre-existing game data from previous save file
	fGame = fopen("savefile.txt", "r");
	if(fGame != NULL){ // if the file opens, 
		userData = fscanf(fGame, "%s|%d|%d|%f|%d\n", allPlayers[count].strName, &allPlayers[count].nLives, &allPlayers[count].nLevel, &allPlayers[count].nScore, &allPlayers[count].nAnswered);
		while(userData == 5 && count < 99){
			count++;
		}
		fclose(fGame);
	}
	
	// Iterates through the playerData list nad compares previously existing usernames with newly inputted ones 
	for(i = 0; i < count && !found; i++){
		if(strcmp(allPlayers[i].strName, player->strName) == 0){
			allPlayers[i] = *player;
			found = 1;
		}
	}
	
	// adds a new player save if player is not found
	if(!found && count < 100){
		allPlayers[count++] = *player;
	}
	
	// saves new player data into the save file
	fGame = fopen("savefile.txt", "w");
	if(fGame != NULL){
		for(i = 0; i < count; i++){
			fprintf(fGame, "%s | %d | %d | %f | %d\n", allPlayers[i].strName, &allPlayers[i].nLives, &allPlayers[i].nLevel, &allPlayers[i].nScore, &allPlayers[i].nAnswered); ///// SMTH IS DEFINITELY WRONG!!!!!
			
			if(userData < 0){
				printf("No data detected for %s :/", allPlayers[i].strName);
			}
		}
		fclose(fGame);
		printf("User %s's game saved successfully!", player->strName);
	} else {
		printf("Error saving game data :(\n");
	}
	
	fData = fopen("data.txt", "w"); ///// SMTH IS DEFINITELY WRONG!!!!!
	if(fData != NULL){
		for(i = 0; i < *entryCount; i++){
			if(i >= (*entryCount - player->nAnswered)){ // total words - words successfully answeed by the user
				used = 1;
			}
			fprintf(fData, "%s\n%d\n%s\n\n", wordlist[i].strWord, used, wordlist[i].strClue);
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
	char cSave;
	int continueSearch; 
	int i;

    printf("Please input player username: ");
    scanf(" %49[^\n]", player->strName);

    gameCheck(player);

    if (*entryCount > 0) {
        while (player->nLives > 0) {
            foundWord = 0;
			continueSearch = 1;
			
            for(i = 0; i < *entryCount && continueSearch; i++) {
                wordLength = strlen(wordlist[i].strWord);
                
                if(player->nLevel <= 3 && wordLength <= 4) {
                    currentWord = i;
                    foundWord = 1;
                    points = 2.5;
					continueSearch = 0;
                } else if (player->nLevel <= 5 && wordLength <= 8) {
                    currentWord = i;
                    foundWord = 1;
                    points = 4.0;
					continueSearch = 0;
                } else if(player->nLevel >= 6 && wordLength >= 9) { 
                    currentWord = i;
                    foundWord = 1;
                    points = 7.0;
					continueSearch = 0;
                }
            }

            if (foundWord) {
                cLetter = randomLetter(wordlist[currentWord].strWord); 

                printf("\nLEVEL %d\n", player->nLevel);
                printf("WORD LENGTH: %d\n", wordLength); 
                printf("CLUE: %s\n", wordlist[currentWord].strClue); 
                printf("LETTER: %c\n", cLetter);

                printf("Enter your guess: ");
                scanf(" %49[^\n]", strAnswer);
				
                if (strcmp(wordlist[currentWord].strWord, strAnswer) == 0) { 
                    printf("Correct!\n");
                    (player->nScore) += points; 
                    player->nLevel++;
					player->nAnswered++;
                    
					// indicates used word
					temp = wordlist[currentWord];
					wordlist[currentWord] = wordlist[*entryCount-1];
                    wordlist[*entryCount-1] = temp;
                    (*entryCount)--;
					
                } else {
                    colorRed();
                    printf("Incorrect. The correct word was: %s\n", wordlist[currentWord].strWord);
                    resetColor();
                    player->nLives--;
                }

                printf("Score: %.1f | Lives: %d\n", player->nScore, player->nLives);  
				
				printf("Do you want to save your progess? [Y/N] ");
				scanf(" %c", &cSave);
				if(cSave == 'Y' || cSave == 'y'){
					saveGame(player, wordlist, entryCount);
					printf("Game saved!\n");
				}
            } else {
                colorRed();
                printf("No words available for this level!\n");
                resetColor();
                player->nLives--;  // Deduct life when no word found
            }
        }

        if (player->nLives == 0) {
            colorYellow();
            printf("\nGAME OVER :(\n");
            resetColor();
            colorGreen();
            printf("Your total score was %.1f!\n", player->nScore);
            resetColor();
			
			scoresFile = fopen("scores.txt", "a");
			if(scoresFile != NULL){
				fprintf(scoresFile, "%s | %.1f | %d\n", player->strName, player->nScore, player->nAnswered);
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
	
	printf("PLAYER   |   CORRECT ANSWERS   |   HIGH SCORE");
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
void displayHighScores(playerData player[], int* entryCount){
	FILE *fHighScores = NULL;
	playerData scores[100]; // an array of data for 100 scorers
	int scoreQuantity;
	int count = 0; 
	int i;
	
	fHighScores = fopen("scores.txt", "r");
	if(fHighScores != NULL){
		scoreQuantity = fscanf(fHighScores, " %[^n] | %f | %d\n", scores[count].strName, &scores[count].nScore, &scores[count].nAnswered);
		while(scoreQuantity == 3 && count < 100){
			count++;
		}
		fclose(fHighScores);
	}
	
	sortHighScores(scores, &count);
	
	printf("PLAYER   |   CORRECT ANSWERS   |   HIGH SCORE");
	for(i = 0; i < *entryCount; i++){
		printf("%s   %d   %d", player[i].strName, player[i].nAnswered, player[i].nScore);
	}
	
	if(count == 0){
		printf("No scores added yet~\n");
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
				displayHighScores(&player, entryCount);
				break;
			case 4: 
				saveGame(&player, wordlist, entryCount);
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
