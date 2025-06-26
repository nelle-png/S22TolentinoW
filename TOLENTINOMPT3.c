///// VERY VERY INCOMPLETE
/*
	NEEDED AS OF JUNE 26, 2025 11:30PM:
	- Edit words
	- Delete words
	- Fix playtime (wont run properly)

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h> // purely for color coding

#define LENGTH 200
#define PASSWORD "mygame"


/***
FUNCTION PROTOTYPES:

void addWordsManual(gameData wordlist[], int* entryCount)
void editWords(gameData wordlist[], int* entryCount)
void deleteWords(gameData wordlist[], int* entryCount)
void adminMenu(gameData wordlist[], int* entryCount)
void mainmenu(gameData wordlist[], int* entryCount)
void quitGame()

int isDuplicate(gameData wordlist[], int count, char word[])
char randomLetterGenerator(char *word)

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
	float nScore;
};

struct playerDataTag {
	char strName[LENGTH];
	int nLevel;      // Default level count starts at 1
	int nLives;      // Default lives start as 3 
	int nScore;      // Default score starts at 0
	int nSavedGames; // New field to track saves
};

typedef struct gameDataTag gameData;
typedef struct playerDataTag playerData;

// checks for duplicate words [NEEDS CLARIFICATION if clues must be checked as well]
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

// main play function yey
void playTime(playerData *player, gameData wordlist[], int* entryCount) {
    float points = 0;
    int nLevelIn = 0;
    int len = 0;
    int found = 0;
    int playedRounds = 0;
    char strAnswer[LENGTH];
    char cLetter;

    printf("Please input player username: ");
    scanf(" %[^\n]", player->strName);

    gameCheck(player);

    // Check if there are words available
    if (*entryCount > 0) {
        while (player->nLives > 0 && playedRounds < *entryCount) {
            found = 0;
            nLevelIn = 0; // Reset search from beginning each time

            // Find appropriate word based on level progression
            while (!found && nLevelIn < *entryCount) {
                len = strlen(wordlist[nLevelIn].strWord);

                // Simplified level progression
                if ((player->nLevel <= 3 && len <= 4) ||
                    (player->nLevel > 3 && player->nLevel <= 5 && len <= 8) ||
                    (player->nLevel > 5)) {
                    found = 1;
                } else {
                    nLevelIn++;
                }
            }

            if (found) {
                cLetter = randomLetter(wordlist[nLevelIn].strWord);

                printf("\nLEVEL %d\n", player->nLevel);
                printf("CLUE: %s\n", wordlist[nLevelIn].strClue);
                printf("LETTER: %c\n", cLetter);

                printf("Enter your guess: ");
                scanf(" %[^\n]", strAnswer);
                
                playedRounds++;

                if (strcmp(wordlist[nLevelIn].strWord, strAnswer) == 0) {
                    printf("Correct!\n");

                    // Calculate points based on level
                    if (player->nLevel <= 3) {
                        points = 2.5;
                    } else if (player->nLevel <= 5) {
                        points = 4.0;
                    } else {
                        points = 7.0;
                    }

                    player->nScore += points;
                    player->nLevel++;
                } else {
                    colorRed();
                    printf("Incorrect. The correct word was: %s\n", wordlist[nLevelIn].strWord);
                    resetColor();
                    player->nLives--;
                }

                printf("Score: %d | Lives: %d\n", player->nScore, player->nLives);
                nLevelIn++; // Move to next word for next round
            }
        }

        if (playedRounds > 0) {
            if (player->nLives == 0) {
                colorRed();
                printf("\nGAME OVER :(\n");
                resetColor();
            } else {
                colorGreen();
                printf("\nCONGRATULATIONS! :3\n");
                resetColor();
            }
        } else {
            printf("No suitable words found for gameplay!\n");
        }
    } else {
        printf("No words available to play! Please add words first.\n");
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
			printf("|              [3] RESET PLAYER DATABASE     |\n");
			colorCyan();
			printf("|              [4] SAVED GAMES               |\n");
			colorCyan();
			printf("|              [5] EXIT                      |\n");
			resetColor();
			printf("+--------------------------------------------+\n\n\n");

			printf("Enter choice: ");
			scanf("%d", &nChoice);

			if (nChoice == 1) {
				addWordsManual(wordlist, entryCount);
			} else if (nChoice == 2 || nChoice == 3 || nChoice == 4) {
				quitGame(); // placeholder
			} else if (nChoice != 5) {
				printf("Invalid choice. Try again.\n");
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

		if (nChoice == 1) {
			adminMenu(wordlist, entryCount);
		} else if (nChoice == 2) {
			playTime(&player, wordlist, entryCount);
		} else if (nChoice == 3 || nChoice == 4) {
			quitGame(); // placeholder
		} else if (nChoice != 5) {
			printf("Invalid choice. Try again.\n");
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
