/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts
learned. I have constructed the functions and their respective algorithms and corresponding code by myself. The
program was run, tested, and debugged by my own efforts. I further certify that I have not copied in part or whole or
otherwise plagiarized the work of other students and/or persons.

							Winelle R. Tolentino, DLSU ID# 12414913

EXTRA RESOURCES:
- https://www.geeksforgeeks.org/c/scanf-and-fscanf-in-c/ [Returning quantities of inputs with fscanf]
- https://stackoverflow.com/questions/34874347/what-does-an-asterisk-in-a-scanf-format-specifier-mean [scanf with *, reading a character but not assigning it to a variable]
- https://www.youtube.com/watch?v=dG1HBSArgjM [Countdown Timer | C Programming Example by Portfolio Courses]
- https://www.reddit.com/r/C_Programming/comments/jogazf/what_the_hell_is_nc/ [Some "%^\n" explanations (initially learned from prof inquiry)]
-  CCPROG 2 Course Notes

*********************************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h> // purely for rand function and color coding, was also used in term 1 space invaders mp
#include <time.h> // for timer
#define LENGTH 200
#define PASSWORD "mygame" // developer password for mygame

// ANSI color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define WHITE  "\e[0;37m"


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

// The gameDataTag structure will act as the database and store words/their used flags/clues
struct gameDataTag {
	char strWord[LENGTH];
	int wordUsed;
	char strClue[LENGTH];
};

// The playerDataTag structure will act as the player database to store unique information on players
struct playerDataTag {
	char strName[LENGTH]; // For player username
	char strCode[LENGTH]; // for code 
	int nLevel;      // Default level count 
	int nLives;      // Default lives
	float nScore;      // Default score 
	int nAnswered; // Default questions answered 
	int nSavedGames; // Tracks saves
};

typedef struct gameDataTag gameData;
typedef struct playerDataTag playerData;


/// HELPER FUNCTIONS: ///

/* isDuplicate returns the number of duplicated words upon input
	@param wordlist - the struct that accesses game data (details for the word)
	@param n - the current number of entries in the wordlist array  

	@return nDuplicate if there are any duplicate words
*/
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

/* wordValidator compares and equalizes words even with mismatched cases
	@param word1 - array to store first word; loop iterates through it and compares each character
	@param word2 - array to store second word; loop iterates through it and compares each character

	@return wordEqual returns words that are equalized/valid 
*/
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

/* saveWordlist saves the modified word data, to be called after every function that modifies the wordlist 
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void saveWordlist(gameData wordlist[], int entryCount){
	FILE *fData = fopen("data.txt", "w");
	int i;
	if(fData != NULL){
		for(i = 0; i < entryCount; i++){
			fprintf(fData, "%s\n%d\n%s\n\n", wordlist[i].strWord, wordlist[i].wordUsed, wordlist[i].strClue);
		}
		fclose(fData);
	}
}

/* gameCheck reads from the file saved.txt and reinitializes the contents of the playerData struct if saved is not found
	@param player - accesses the playerData struct 
	
	return saveFound 
*/
int gameCheck(playerData *player) {
    FILE *fGame = fopen("saved.txt", "r");
    int saveFound = 0;
    playerData tempPlayer;
    char savedName[LENGTH];
    
    /* Initialize player defaults */
    player->nLevel = 1;
    player->nLives = 3;
    player->nScore = 0;
    player->nAnswered = 0;

    if (fGame != NULL) {
        while (fscanf(fGame, " %[^\n]", savedName) == 1) {
            if (fscanf(fGame, "%[^\n]", tempPlayer.strCode) == 1 &&
				fscanf(fGame, "%d", &tempPlayer.nLevel) == 1 && 
                fscanf(fGame, "%d", &tempPlayer.nLives) == 1 && 
                fscanf(fGame, "%f", &tempPlayer.nScore) == 1) {
                
                if (strcmp(savedName, player->strName) == 0) {
                    saveFound = 1;
                    strcpy(player->strCode, tempPlayer.strCode);
                    player->nLevel = tempPlayer.nLevel;
                    player->nLives = tempPlayer.nLives;
                    player->nScore = tempPlayer.nScore;
                }
            }
            fscanf(fGame, "%*[^\n]"); // skips newline
            fscanf(fGame, "%*c");
        }
        fclose(fGame);
    }
    return saveFound;
}

/* randomLetter extracts a random character from a given word
	@param player - accesses the playerData struct
	
	return letter returns the extracted random letter
*/
char randomLetter(char *word) {
	int len = strlen(word);
	int letterIndex = 0;
	char letter = '\0';

	if (len > 0) {
		letterIndex = rand() % len;
		letter = word[letterIndex];

		// if letter is in lowercase then gawing uppercase before it's returned
		if (letter >= 'a' && letter <= 'z') {
			letter -= 32; // convert to uppercase manually
		}
	} 
	
	return letter;
}

/* BONUS: saveAchievements quantifies user achievements based on their score.
	@param player - accesses the playerData struct
	@param mode - the indicated mode depending on the player's chosen mode to play
*/
void saveAchievements(playerData *player, char *mode) {
    FILE *fAchievements = fopen("achievements.txt", "a");
    
	if(fAchievements != NULL){
		if(player->nScore >= 100) {
        fprintf(fAchievements, "  %s | Word Master - Get a score of 100!\n", player->strName);
        printf(MAGENTA "@ ACHIEVEMENT: Word Master! \n" RESET);
		} else if(player->nScore >= 80) {
			fprintf(fAchievements, "  %s | Word Proficient - Get a score of 80!\n", player->strName);
			printf(CYAN "@ ACHIEVEMENT: Word Proficient! \n" RESET);
		} else if(player->nScore >= 50) {
			fprintf(fAchievements, "  %s | Word Competent - Get a score of 50!\n", player->strName);
			printf(CYAN "@ ACHIEVEMENT: Word Competent! \n" RESET);
		} else if(player->nScore >= 25) {
			fprintf(fAchievements, "  %s | Word Beginner - Get a score of 25!\n", player->strName);
			printf(CYAN "@ ACHIEVEMENT: Word Beginner! \n" RESET);
		} else if (player->nScore >= 10) {
			fprintf(fAchievements, "  %s | Word Rookie - Get a score of 10!\n", player->strName);
			printf(CYAN "@ ACHIEVEMENT: Word Rookie! \n" RESET);
		}
		
		if(strcmp(mode, "NORMAL") == 0) {
			fprintf(fAchievements, "%s | Normal Mode Novice - Normal Mode Attempted\n", player->strName);
			printf(GREEN "$ ACHIEVEMENT: Normal Mode Novice! \n" RESET);
		} else if(strcmp(mode, "HARD") == 0) {
			fprintf(fAchievements, "%s | Infinite void - Limbo Mode Attempted\n", player->strName);
			printf(GREEN "$ ACHIEVEMENT: Infinite void! \n" RESET);
		}
		
		fclose(fAchievements);
	} else {
		printf("Could not detect achievements file... :<\n");
	}
}

/* BONUS: jumbledWord will pull a random word from the word database and jumble it for the user to guess soon
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
	@param jumbledWord - the jumbled word
	
	pre-condition: this function ignores the point system
*/
void jumbledWord(gameData wordlist[], int entryCount, char *jumbledWord, char *originalWord) {
    int random;
    int length;
    int i, j;
    char temp;
    
    if(entryCount > 0) {
        random = rand() % entryCount;
        strcpy(originalWord, wordlist[random].strWord); // Store original word
        strcpy(jumbledWord, originalWord); // Copy to jumble
        length = strlen(jumbledWord);
        
        // Jumble the word
        for(i = 0; i < length; i++) {
            j = rand() % length;
            temp = jumbledWord[i];
            jumbledWord[i] = jumbledWord[j];
            jumbledWord[j] = temp;
        }
    } else {
        jumbledWord[0] = '\0';
        originalWord[0] = '\0';
    }
}

/// MAIN FUNCTIONS: ///

/* addWordsManual allows the user to manually add words to the wordlist array
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void addWordsManual(gameData wordlist[], int* entryCount) {
	char addPrompt = 'y';
	char inputtedWord[LENGTH];

	printf("You have selected MANUAL ADDITION!\n");

	while (addPrompt == 'y' || addPrompt == 'Y') {
		printf("Please add a word: ");
		scanf(" %[^\n]", inputtedWord);

		if (isDuplicate(wordlist, *entryCount, inputtedWord) > 0) {
			printf(RED"Duplicate word. Please enter a different one\n"RESET);
		} else {
			strcpy(wordlist[*entryCount].strWord, inputtedWord);
			wordlist[*entryCount].wordUsed = 0; // initialize a word as unused the moment its inputted by user
			
			printf("Please add a clue: ");
			scanf(" %[^\n]", wordlist[*entryCount].strClue);
			(*entryCount)++;

			printf(CYAN"Added a new word and clue!\n"RESET);
		}

		printf("Do you want to add another word? [y/n]: ");
		scanf(" %c", &addPrompt);

		if (addPrompt != 'Y' && addPrompt != 'N' && addPrompt != 'y' && addPrompt != 'n') {
			printf(RED"Invalid choice.\n"RESET);
		}
	}
	saveWordlist(wordlist, *entryCount);
}

/* addFromFile allows the user to add words from file
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void addFromFile(gameData wordlist[], int* entryCount){
	FILE *fAdd = NULL; 
	char filename[50];
	char word[LENGTH]; // keeps words from inputted file
	char clue[LENGTH]; // keeps clues from inputted file
	int openedFile = 0;
	int nAdded = *entryCount;
	int nUsed = 0; // 1 if used, 0 if not yet
	
	printf("Enter a file to import: ");
	scanf(" %[^\n]", filename);
	strcat(filename, ".txt"); // adds a .txt to ensure that the program is processing a text file even if user inputs a non-textfile format
	
	fAdd = fopen(filename, "r");
	openedFile = (fAdd != NULL);
	
	if(openedFile){
		while(*entryCount < 200 && fscanf(fAdd, " %[^\n]", word) == 1 && fscanf(fAdd, " %d", &nUsed) == 1 && fscanf(fAdd, " %[^\n]", clue) == 1){
			if(!isDuplicate(wordlist, *entryCount, word)){
				strcpy(wordlist[*entryCount].strWord, word);
				strcpy(wordlist[*entryCount].strClue, clue);
				wordlist[*entryCount].wordUsed = nUsed;
				(*entryCount)++;
				nAdded++;
			}
			fscanf(fAdd, "%*[\n]"); // Skip empty lines
		}
		fclose(fAdd);
	}
	if(openedFile){
		printf(YELLOW"Added %d new words from file %s\n"RESET, nAdded, filename);
	} else {
		printf(RED"ERROR!\n"RESET);
	}
}

/* showWords displays all words in struct wordlist[]
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void viewWords(gameData wordlist[], int* entryCount){
	int i;
	
	if(*entryCount > 0)
	{
		printf(MAGENTA"INDEX " RESET " - " CYAN " WORD " RESET " - " YELLOW " CLUE\n" RESET);
		
		for(i = 0; i < *entryCount; i++){
			printf(MAGENTA"%d " RESET  " - " CYAN " %s " RESET " - " YELLOW " %s\n "RESET, i+1, wordlist[i].strWord, wordlist[i].strClue);
		}
		printf("\n");
	} else {
		printf(RED"No words available right now :(\n\n"RESET);
	}
}

/* editWords displays all words in struct wordlist[] and gives the user the option to select an index and edit the word or clue in that index
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void editWords(gameData wordlist[], int* entryCount){
	int editIndex;
	int editSelect;
	int i;
	int validInput = 0;
	char newWord[LENGTH];
	char newClue[LENGTH];
	
	if(*entryCount > 0)
	{
	
		printf(MAGENTA"INDEX" RESET "-" CYAN "WORD" RESET "-" YELLOW "CLUE\n" RESET);
		
		for(i = 0; i < *entryCount; i++){
			printf(MAGENTA "%d" RESET  " - " CYAN "%s" RESET " - " YELLOW"%s\n"RESET, i+1, wordlist[i].strWord, wordlist[i].strClue);
		}
		printf("\n");
		
		validInput = 0;
		while(!validInput){
			printf("Do you wish to edit a" CYAN " WORD [0]" RESET " or a" BLUE "CLUE [1] " RESET "?\n");
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
			printf(CYAN"Selected word: %s\n"RESET, wordlist[editIndex].strWord);
			printf("New word: ");
			scanf(" %[^\n]", newWord);
			if(newWord[0]){
				strcpy(wordlist[editIndex].strWord, newWord);
			}
		} else if(editSelect == 1){
			printf(BLUE"Selected clue: %s\n"RESET, wordlist[editIndex].strClue);
			printf("New clue: ");
			scanf(" %[^\n]", newClue);
			
			if(newClue[0]){
				strcpy(wordlist[editIndex].strClue, newClue);
			}
		} else {
			printf(RED"Invalid entry. Please enter either 0 for WORDS and 1 for CLUES"RESET);
		}
		printf("Updated entry %d: %s - %s\n", editIndex+1, 
		wordlist[editIndex].strWord, wordlist[editIndex].strClue);
	} else {
		printf("No available entries for editing :(\n");
	}
	saveWordlist(wordlist, *entryCount);
}

/* deleteWords displays all words in struct wordlist[] and gives the user the option to select an index and delete the word *and* its clue in that index
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void deleteWords(gameData wordlist[], int *entryCount){
    int index;
    int i;

    if (*entryCount == 0) {
        printf("No entries to delete.\n");
    } else {
       printf(MAGENTA"INDEX" RESET " - " CYAN "WORD" RESET " - " YELLOW "CLUE\n" RESET);
		
		for(i = 0; i < *entryCount; i++){
			printf(MAGENTA"%d" RESET  " - " CYAN "%s" RESET " - " YELLOW "%s\n"RESET, i+1, wordlist[i].strWord, wordlist[i].strClue);
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

/* saveGame saves the player data into saved.txt
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void saveGame(playerData *player, gameData wordlist[], int* entryCount) {
    FILE *fSave = NULL;
    
	//append new player data to saved.txt
    fSave = fopen("saved.txt", "a");
    if(fSave != NULL) {
        fprintf(fSave, "\n%s\n%s\n%d\n%d\n%.1f\n",
               player->strName,
			   player->strCode,
               player->nLevel,
               player->nLives,
               player->nScore);
        fclose(fSave);
    }
	
	// wordlist state is always called/updated and saved
    saveWordlist(wordlist, *entryCount);
}

/* playTime serves as the main game function. It calls the helper functions to check/save the game, as well as give random characters from a certain word and the word length for that word for the player to guess. It will keep running said game until the player either chooses to exit or loses all three lives.
    @param player - the struct that accesses player data (information about the player and their played/saved games)
    @param wordlist - the struct that accesses game data (details for the word)
    @param entryCount - the number of entries in wordlist
*/
void playTime(playerData *player, gameData wordlist[], int* entryCount, int loadedGame) {
    FILE *scoresFile = NULL;
    FILE *fData = NULL;
    float points = 0;
    int currentWord = -1;
    int wordLength = 0;
    int foundWord = 0;
    int startedGame = 0; 
    char strAnswer[LENGTH];
    char cLetter;
    char cSave = 'y';
    int breakout = 0;
    int i;
    char choice;
    char mode[200] = "NORMAL"; // FOR BONUS FEATURE: Achievement system

    // if no saves were found:
    if(!loadedGame && !gameCheck(player)){
         /* Get username */
        printf("Please input player username: ");
        scanf(" %257[^\n]", player->strName);
        
        /* Get the specialized password */
        printf("Enter player's specialized code: ");
        scanf(" %257[^\n]", player->strCode);
        
        printf("Started a new game for" MAGENTA "%s!\n" RESET, player->strName);
        player->nLevel = 1;
        player->nLives = 3;
        player->nScore = 0;
        player->nAnswered = 0;
    } else if (loadedGame){
        printf("\nSaved game found for" MAGENTA" %s\n"RESET, player->strName);
        printf(YELLOW "Level: %d " RESET " | " RED " Lives: %d " RESET " | " MAGENTA " Score: %.1f\n" RESET, 
              player->nLevel, player->nLives, player->nScore);
        
        printf("Continue this game? [Y/N]: ");
        printf(RED"[Y will continue, N will start anew] "RESET);
        scanf(" %c", &choice);
        
        if (choice != 'Y' && choice != 'y') {
            // Reset to new game
            printf("Started a new game for %s!\n", player->strName);
            player->nLevel = 1;
            player->nLives = 3;
            player->nScore = 0;
            player->nAnswered = 0;
        }
    }

    // Load wordlist
    fData = fopen("data.txt", "r");
    if (fData) {
        *entryCount = 0;
        while (*entryCount < 200 && 
               fscanf(fData, " %[^\n]", wordlist[*entryCount].strWord) == 1 &&
               fscanf(fData, "%d", &wordlist[*entryCount].wordUsed) == 1 &&
               fscanf(fData, " %[^\n]", wordlist[*entryCount].strClue) == 1) {
            (*entryCount)++;
            fscanf(fData, "%*[\n]");
        }
        fclose(fData);
    }

    // Main game loop - runs while player has lives and hasn't chosen to exit
    if (*entryCount > 0) {
        while (player->nLives > 0 && !breakout) {
            foundWord = 0;
            currentWord = -1; // initialize to invalid index

            // FIRST PASS: Try to find a word that matches current level requirements
            for(i = 0; i < *entryCount && !foundWord; i++){
                if(wordlist[i].wordUsed == 0){
                    wordLength = strlen(wordlist[i].strWord); 
                    
                    // LEVEL 1-3: Words with at most 4 characters (2.5 points)
                    if (player->nLevel <= 3 && wordLength <= 4){ 
                        currentWord = i;
                        foundWord = 1;
                        points = 2.5;
                    } 
                    // LEVEL 4-5: Words with 5-8 characters (4.0 points)
                    else if (player->nLevel >= 4 && player->nLevel <= 5 && wordLength >= 5 && wordLength <= 8){
                        currentWord = i;
                        foundWord = 1;
                        points = 4.0;
                    } 
                    // LEVEL 6+: Words with more than 9 characters (7.0 points)
                    else if (player->nLevel >= 6 && wordLength > 9) { 
                        currentWord = i;
                        foundWord = 1;
                        points = 7.0;
                    }
                }
            }

            // SECOND PASS: If no perfect match found, use any available word
            if(!foundWord) {
                for(i = 0; i < *entryCount && !foundWord; i++){
                    if(wordlist[i].wordUsed == 0){
                        currentWord = i;
                        foundWord = 1;
                        wordLength = strlen(wordlist[i].strWord);
                        // Assign points based on word length as fallback
                        if(wordLength <= 4) points = 2.5;
                        else if(wordLength <= 8) points = 4.0;
                        else points = 7.0;
                    }
                }
            }

            // Handle no word found
            if (!foundWord) {
                printf( RED "No words available!\n" RESET, player->nLevel);
                player->nLives = 0;  // End game if no valid words
                breakout = 1;        // Flag to exit the loop
            }

            // Only proceed with the game if a valid word was found
            if (foundWord) {
                startedGame = 1; 

                // Mark word as used and get random letter
                wordlist[currentWord].wordUsed = 1;
                cLetter = randomLetter(wordlist[currentWord].strWord); 

                // Display game info
                printf(YELLOW "\nLEVEL %d\n" RESET, player->nLevel);
                printf(GREEN "WORD LENGTH: %d\n" RESET, wordLength); 
                printf(BLUE "CLUE: %s\n" RESET, wordlist[currentWord].strClue); 
                printf(CYAN "LETTER: %c\n" RESET, cLetter);

                // Get player's guess
                printf(MAGENTA "Enter your guess: " RESET);
                scanf(" %257[^\n]", strAnswer);
                
                // Check if answer is correct (case-insensitive)
                if (wordValidator(wordlist[currentWord].strWord, strAnswer)) { 
                    printf(GREEN "Correct!\n" RESET);
                    player->nScore += points; 
                    player->nLevel++;
                    player->nAnswered++;
                    
                    // Save wordlist state after correct answer
                    saveWordlist(wordlist, *entryCount);
                } 
                else {
                    printf(RED "Incorrect. Try that again..\n" RESET);
                    player->nLives--;
                }

                // Display current stats
                printf(MAGENTA "Score: %.1f " RESET " | " RED " Lives: %d\n" RESET, player->nScore, player->nLives);  
                
                // Save/Continue prompt
                printf("Do you want to continue? ["GREEN"Y"RESET"/"RED"N"RESET"]: ");
                scanf(" %c", &cSave);
                
                // Handle player's choice
                if(cSave == 'N' || cSave == 'n'){
                    // Save game state before exiting
                    saveGame(player, wordlist, entryCount);
                    saveAchievements(player, mode);
                    
                    // Record score to high scores
                    scoresFile = fopen("scores.txt", "a");
                    if(scoresFile != NULL){
                        fprintf(scoresFile, "%s\n%.1f\n%d\n\n", player->strName, player->nScore, player->nAnswered);
                        fclose(scoresFile);
                    }
                    breakout = 1;
                }
            }
        }

        // Game over handling (only if the game actually started)
        if (player->nLives == 0 && startedGame) {
            printf(BLUE"\nGAME OVER :(\n"RESET);
            if (foundWord) {
                printf("The correct word was:" GREEN "%s\n" RESET, wordlist[currentWord].strWord);
            }
            printf("Your total score was" CYAN "%.1f!\n" RESET, player->nScore);
            
            // Final save
            saveGame(player, wordlist, entryCount);
            saveAchievements(player, mode);
            
            // Record final score
            scoresFile = fopen("scores.txt", "a");
            if(scoresFile != NULL){
                fprintf(scoresFile, "%s\n%.1f\n%d\n\n", player->strName, player->nScore, player->nAnswered);
                fclose(scoresFile);
            }
        }
    } else {
        printf(RED"No words available to play! Please add words first.\n"RESET);
    }
}

/* sortHighScores employs a simple selection sort algorithm to sort 
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
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

/* resetSavedGames will open saved.txt and clear everything in it.
	pre-condition: this will rewrite everything in saved.txt and leave it blank,
*/
void resetSavedGames(){
    FILE *fSaved = fopen("saved.txt", "w");
	
    if (fSaved == NULL) {
        printf(RED "Error clearing saved games :(\n" RESET);
    } else {
        fclose(fSaved);
        printf(GREEN "Cleared all saved games :D\n" RESET);
    }
}

/* resetHighScore will open scores.txt and clear everything in it.
	pre-condition: this will rewrite everything in scores.txt and leave it blank,
*/
void resetHighScore(){
    FILE *fSaved = fopen("scores.txt", "w");
	
    if (fSaved == NULL) {
        printf(RED "Error clearing high scores :(\n" RESET);
    } else {
        fclose(fSaved);
        printf(GREEN "Cleared all high scores :D\n" RESET);
    }
}


/* displayHighScores will read from the file scores.txt and display all high scores inside of it
	pre-condition: a separate function of sortHighScores is called upon to aid in sorting
*/
void displayHighScores(){
	FILE *fHighScores = NULL;
	playerData scores[100]; // an array of data for 100 scorers
	int count = 0; 
	int i;
	
    fHighScores = fopen("scores.txt", "r");
    if (fHighScores != NULL) {
        while (count < 101 && fscanf(fHighScores, " %257[^\n]", scores[count].strName) == 1 && fscanf(fHighScores, "%f", &scores[count].nScore) == 1 && fscanf(fHighScores, "%d", &scores[count].nAnswered) == 1) {
        count++;
        fscanf(fHighScores, "%*[\n]");  // Reads empty newlines but skips them
        }
        fclose(fHighScores);
    }
	
	sortHighScores(scores, &count);
	
	printf(WHITE "+-----------------------------------------------------+\n" RESET);
	printf(WHITE "|" CYAN "                     HIGH SCORES                     " WHITE "|\n" RESET);
	printf(WHITE "+-----------------------+---------------+-------------+\n" RESET);
	printf(WHITE "|" MAGENTA " PLAYER               " WHITE "|" CYAN " CORRECT ANSWERS " WHITE "|" YELLOW " HIGH SCORE " WHITE "|\n" RESET);
	printf(WHITE "+-----------------------+--------------+--------------+\n" RESET);

	for(i = 0; i < count; i++) {
		printf(WHITE "|" MAGENTA " %-20s " WHITE "|" CYAN " %14d " WHITE "|" YELLOW " %9.1f " WHITE "|\n" RESET,
			   scores[i].strName, scores[i].nAnswered, scores[i].nScore);
	}

	printf(WHITE "+-----------------------+---------------+-------------+\n" RESET);
	
	if(count == 0){
		printf("|                No scores added yet~                 | \n");
		printf(WHITE "+-----------------------+---------------+-------------+\n\n" RESET);
	}
}

/* loadSave will read from the files saved.txt and data.txt to load and continue saved scores and levels
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
	
	return 1 if a saved game is detected, to be loaded in. 0 if not.
*/
int loadSave(playerData *player, gameData wordlist[], int* entryCount) {
    FILE *fGame = fopen("saved.txt", "r");
    FILE *fData = fopen("data.txt", "r");
    playerData savedPlayers[100];
	char strPassword[LENGTH];
    int count = 0;
    int choice;
    int i;
    int loaded = 0;

    if (fGame != NULL && fData != NULL) {
        // Read all saves
        while(count < 100 && !feof(fGame)) {
            if(fscanf(fGame, " %[^\n]", savedPlayers[count].strName) == 1 &&
				fscanf(fGame, " %[^\n]", savedPlayers[count].strCode) == 1 &&
               fscanf(fGame, "%d", &savedPlayers[count].nLevel) == 1 &&
               fscanf(fGame, "%d", &savedPlayers[count].nLives) == 1 &&
               fscanf(fGame, "%f", &savedPlayers[count].nScore) == 1) {
                count++;
                fscanf(fGame, "%*[^\n]"); 
                fscanf(fGame, "%*c");
            }
        }
        
        if(count > 0) {
            // Display saves
            printf(CYAN"\nSAVED GAMES:\n"RESET);
            printf(CYAN "INDEX " RESET " | " MAGENTA " USER " RESET " | " CYAN " LEVEL " RESET " | " MAGENTA "SCORE\n");
            for(i = 0; i < count; i++) {
                printf(CYAN"%d " RESET " | " MAGENTA " %s " RESET " | " CYAN " Level %d "RESET " | " MAGENTA " %.1f pts\n" RESET, 
                      i+1, savedPlayers[i].strName, 
                      savedPlayers[i].nLevel, savedPlayers[i].nScore);
            }
            
            // Get selection
            printf("\nSelect game to load (1-%d) or 0 to cancel: ", count);
            scanf("%d", &choice);
            
            if(choice >= 1 && choice <= count) {
				while(getchar() != '\n');
                printf("Please enter said user's password to continue: ");
				scanf("%[^\n]", strPassword);
				
                if(strcmp(savedPlayers[choice-1].strCode, strPassword) == 0) {
                    strcpy(player->strName, savedPlayers[choice-1].strName);
					strcpy(player->strCode, savedPlayers[choice-1].strCode);
					player->nLevel = savedPlayers[choice-1].nLevel;
					player->nLives = savedPlayers[choice-1].nLives;
					player->nScore = savedPlayers[choice-1].nScore;
					loaded = 1;
                    
                    // Reload wordlist
                    *entryCount = 0;
                    while(*entryCount < 200 && 
                          fscanf(fData, " %[^\n]", wordlist[*entryCount].strWord) == 1 &&
                          fscanf(fData, "%d", &wordlist[*entryCount].wordUsed) == 1 &&
                          fscanf(fData, " %[^\n]", wordlist[*entryCount].strClue) == 1) {
                        (*entryCount)++;
                        fscanf(fData, "%*[^\n]");
                        fscanf(fData, "%*c");
                    }
                    printf("\nGame loaded successfully!\n");
                } else {
                    printf(RED "\nERROR: Incorrect password. Try again.\n" RESET);
                }
            }
        } else {
            printf("No saved games found!\n");
        }
        fclose(fGame);
        fclose(fData);
    } else {
        printf("Failed to open save files!\n");
    }
	
	return loaded;
}

/* 
	quitGame will quit the game if selected. It also displays a thank you message.
*/
void quitGame() {
	printf(MAGENTA "Thank you :D <3 !\n" RESET);
}

/***
	BONUS FEATURES:
	- Added an additional help feature (simple display)
	- Added a simple timer function 
	- Added an achievement save system that awards the players depending on their level/score
	- Added a print achievement function that displays said achievements
	- Added an infinite/limbo mode, where users play infinitely with no quits unless they give up
	- Added a function that saves limbo mode scores
	- Added a function that prints limbo mode scores
***/


/* BONUS: hellTimer is a built-in timer using time.h. It counts down to 0 from a given number of seconds.
	@param seconds - the number of seconds left in the timer
	
	pre-condition: The number of seconds is to be initialized/set in the function hellTimer is called
*/
void limboTimer(int seconds){
	clock_t stop; // clock ticks we can expect for a second
	
	while(seconds > 0){
		printf(CYAN "\r%02d" RESET, seconds);
		fflush(stdout);
		stop = clock() + CLOCKS_PER_SEC;
		// will stop running once clock returns a number greater than stop
		while(clock() < stop){ }
		seconds--;
	}
	
	printf(GREEN"\nGO!\n" RESET);
}

/* BONUS: displayAchievements will read from the file achievements.txt and display all achievements inside of it
	pre-condition: a separate function of sortHighScores is called upon to aid in sorting
*/
void displayAchievements() {
    FILE *fAchievements = fopen("achievements.txt", "r");
    char achievement[256]; 
    
    
   printf("\n+-------------------------------------------------------------+\n");
	printf("|" YELLOW "                        ACHIEVEMENTS!                        " RESET "|\n");
	printf("|-------------------------------------------------------------|\n");
	printf("|" CYAN "           Congratulations to these players:                 " RESET "|\n\n");


    
	// scanning an entire line
    if(fAchievements != NULL) {
        while(fgets(achievement, sizeof(achievement), fAchievements)) {
            printf(YELLOW"  %s"RESET, achievement);
        }
        fclose(fAchievements);
		printf("\n+-------------------------------------------------------------+\n");
    } 
    else {
        printf("|   " RED "No achievements yet... :(" RESET "                                |\n");
		printf("\n+-------------------------------------------------------------+\n");

    }
}

/* BONUS: limboMode serves as a BONUS feature. It works similar to the playtime function, but does not give the option to save game results early. All words, regardless of used flag, are used/reused and shuffled. After all, this is meant to mimic limbo.
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
	
	pre-condition: There are no options to save the game early or continue pre-existing games. All words in the wordlist are also used and shuffled, disregarding the used flag.
*/
void limboMode(playerData *player, gameData wordlist[], int* entryCount) {
    /* Variable Declarations */
    FILE *flimboSave = NULL;
    FILE *fData = NULL;
    float points = 0;
	int seconds = 3;
    char strAnswer[LENGTH];
    char limboWord[LENGTH];
	char originalWord[LENGTH]; 

    printf("Welcome to" RED "LIMBO" RESET", challenger >:D\n");
    printf(BLUE"Enter your name... if you dare!: "RESET);
    scanf(" %[^\n]", player->strName);
    
    // Initialize player stats
    player->nLevel = 1;
    player->nLives = 3;
    player->nScore = 0;
    player->nAnswered = 0;

    // Load wordlist from file
    fData = fopen("data.txt", "r");
    if (fData) {
        *entryCount = 0;
        while (*entryCount < 200 && 
               fscanf(fData, " %[^\n]", wordlist[*entryCount].strWord) == 1 &&
               fscanf(fData, "%d", &wordlist[*entryCount].wordUsed) == 1 &&
               fscanf(fData, " %[^\n]", wordlist[*entryCount].strClue) == 1) {
            (*entryCount)++;
            fscanf(fData, "%*[\n]");
        }
        fclose(fData);
    }
	
	printf("Get ready in... ");
	limboTimer(seconds);
    
    if (*entryCount > 0) {
        while (player->nLives > 0) {
                    
            // Set points by level
            if (player->nLevel <= 5) { 
                points = 2.0;
            } else if (player->nLevel <= 10) {
                points = 5.0;
            } else { 
                points = 7.5;
            }
			
            // Display level info
            printf(YELLOW"\nLEVEL %d\n"RESET, player->nLevel);
            jumbledWord(wordlist, *entryCount, limboWord, originalWord);
            printf(BLUE"JUMBLED: %s\n"RESET, limboWord);
            printf("Enter your guess: ");
            scanf(" %[^\n]", strAnswer);

            if (wordValidator(originalWord, strAnswer)) {
                printf(GREEN "\nCorrect!\n" RESET);
                player->nScore += points;
                player->nLevel++;
                player->nAnswered++;
            } else {
                printf(RED "\nYou messed up >:)\n" RESET);
                printf("Correct word was: %s\n", originalWord); 
                player->nLives--;
            }
            
            printf(MAGENTA"Score: %.1f" RESET " | " RED "Lives: %d\n" RESET, player->nScore, player->nLives);
        }

        // Game over message
        printf(CYAN"\nGAME OVER! You ran out of lives.\n"RESET);
        printf(GREEN "Final Score: %.1f " RESET " | Reached Level: " MAGENTA "%d\n" RESET, 
              player->nScore, player->nLevel);

        // Save results
        flimboSave = fopen("limbo.txt", "a");
        if (flimboSave != NULL) {
            fprintf(flimboSave, "%s\n%d\n%.1f\n\n", 
                   player->strName, 
                   player->nLevel, 
                   player->nScore);
            fclose(flimboSave);
        }
    } else {
        printf(RED "No available words to play >:/\n" RESET);
    }
}

/* BONUS: limboModeSave saves player data from limbo mode into limbo.txt
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void limboModeSave(playerData *player, gameData wordlist[], int* entryCount){
	FILE *fLimbo = NULL;
	
	//append new player data to saved.txt
    fLimbo = fopen("limbo.txt", "a");
    if(fLimbo != NULL) {
        fprintf(fLimbo, "%s\n%d\n%.1f\n\n",
               player->strName,
               player->nLevel,
               player->nScore);
        fclose(fLimbo);
    }
	
	// wordlist state is always called/updated and saved
    saveWordlist(wordlist, *entryCount);
}

/* limboModeDisplay will read from the file limbomode.txt and display all high scores inside of it. Basically a copy of the displayHighScores function, but for limbo mode.
	pre-condition: a separate function of sortHighScores is called upon to aid in sorting
*/
void limboModeDisplay(){
	FILE *fLimbo = NULL;
	playerData scores[100]; // an array of data for 100 scorers
	int count = 0; 
	int i;
	
     fLimbo = fopen("limbo.txt", "r");
    if (fLimbo != NULL) {
        while (count < 100 && 
               fscanf(fLimbo, " %[^\n]", scores[count].strName) == 1 && 
			   fscanf(fLimbo, " %d", &scores[count].nLevel) == 1 &&
			   fscanf(fLimbo, "%f", &scores[count].nScore) == 1) {
            count++;
            fscanf(fLimbo, "%*[\n]"); // skip newline
        }
        fclose(fLimbo);
    }
    
    sortHighScores(scores, &count);
    
    printf(WHITE "+---------------------------------------------+\n" RESET);
	printf(WHITE "|" CYAN "              LIMBO MODE SCORES              " WHITE "| \n" RESET);
	printf(WHITE "+-----------------------+---------------------+\n" RESET);
	printf(WHITE "|" MAGENTA " PLAYER                " WHITE "|" YELLOW "    HIGH SCORE       " WHITE "| \n" RESET);
	printf(WHITE "+-----------------------+---------------------+\n" RESET);

	for(i = 0; i < count; i++) {
		printf(WHITE "|" MAGENTA " %-20s " WHITE " |" YELLOW " %9.1f          " WHITE " |\n" RESET, 
			   scores[i].strName, scores[i].nScore);
	}
	printf(WHITE "+-----------------------+---------------------+\n" RESET);
    
    if(count == 0) {
        printf(RED"No scores added yet~\n"RESET);
    }
}

/// MENU FUNCTIONS ///

/* 
	creditsMenu will print credits 
*/

void creditsMenu(){
	printf("+---------------------------------------------------------------+\n");
	printf("|           " YELLOW "Welcome to my guessing word game!" RESET "            |\n");
	printf("|                                                               |\n");
	printf("| This was formulated by " RED "Winelle R. Tolentino" RESET " from " CYAN "Section S22" RESET " |\n");
	printf("| of " GREEN "Ms. Jackylyn Beredo's CCPROG2 class!" RESET "                       |\n");
	printf("|                                                               |\n");
	printf("| This project was completed in a span of " BLUE "3 weeks" RESET "             |\n");
	printf("| (not including the several bonus features added)!             |\n");
	printf("|                                                               |\n");
	// printf("| Special thanks to my playtesters " MAGENTA "(name here)" RESET "              |\n"); /// i dont have any yet
	printf("| Check out my cool " RED "LIMBO MODE (Infinite)" RESET " — limitless fun!     |\n");
	printf("|                                                               |\n");
	printf("|       " YELLOW "Thank you for playing this game! I hope you have fun <3" RESET "     |\n");
	printf("+---------------------------------------------------------------+\n\n");

}

/* adminMenu accesses the interface for the admin menu, wherein users can access the different admin options
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void adminMenu(gameData wordlist[], int* entryCount) {
    int nChoice;
    char passwordvalid[50];
    int inputResult;
    int passwordCorrect = 0;

    printf("STOP!\nBefore you continue, please enter the developer's intended password: ");
    scanf("%s", passwordvalid);

    passwordCorrect = (strcmp(PASSWORD, passwordvalid) == 0);

    if (passwordCorrect) {
        do {
			printf("+-------------------------------------------------------+\n");
			printf("|         ~~~ Welcome to " MAGENTA "GUESSING" RESET " " CYAN "WORD" RESET " " YELLOW "GAME" RESET " ~~~         |\n");
			printf("|" YELLOW  "                 You are in ADMIN OPTIONS               " RESET "|\n");
			printf("|" MAGENTA "                 [1] MANUAL WORD ADDITION               " RESET "|\n");
			printf("|" CYAN    "                 [2] ADD WORDS FROM FILE                " RESET "|\n");
			printf("|" BLUE    "                 [3] VIEW WORDS                         " RESET "|\n");
			printf("|" GREEN   "                 [4] EDIT WORDS                         " RESET "|\n");
			printf("|" BLUE    "                 [5] DELETE WORDS                       " RESET "|\n");
			printf("|" CYAN    "                 [6] RESET SAVED GAMES                  " RESET "|\n");
			printf("|" MAGENTA "                 [7] RESET HIGH SCORE                   " RESET "|\n");
			printf("|" WHITE   "                 [8] EXIT                               " RESET "|\n");
			printf("+-------------------------------------------------------+\n\n");


            printf("Enter choice: ");
            inputResult = scanf("%d", &nChoice);

            if (inputResult == 1) {
                switch(nChoice) {
                    case 1: 
                        addWordsManual(wordlist, entryCount);
                        break;
                    case 2: 
                        addFromFile(wordlist, entryCount);
                        break;
                    case 3: 
						viewWords(wordlist, entryCount);
                        break;
					case 4:
                        editWords(wordlist, entryCount);
                        break;
                    case 5: 
                        deleteWords(wordlist, entryCount);
                        break;
                    case 6:
                        resetSavedGames();
                        break;
                    case 7:
                        resetHighScore();
                        break;
                    case 8:
                        break; // Will exit from the loop condition
                    default: 
                        printf(RED"Invalid choice. Please enter a number between 1-7.\n"RESET);
                }
            } else {
                printf("Invalid input. Please enter a number between 1-7.\n");
                while (getchar() != '\n'); // Clear input buffer
                nChoice = 0; // Force another iteration
            }
        } while (nChoice != 8);
    } else {
        printf("Wrong password. Try again!\n");
    }
}

/* mainMenu accesses the interface for the main menu, wherein users can access the different user options
	@param player - the struct that accesses player data (information about the player and their played/saved games)
	@param wordlist - the struct that accesses game data (details for the word)
	@param entryCount - the number of entries in wordlist
*/
void mainMenu(gameData wordlist[], int* entryCount) {
    int nChoice;
    playerData player;
    int inputResult;
	printf("               U  ___ u   ____     ____          ____      _      __  __  U _____ u  _    \n");
	printf(" __        __    \\\"/_ \\/U |  _\"\\ u |  _\"\\      U /\"___|uU  /\"\\  uU|' \\/ '|u\\| ___\"|/U|\"|u  \n");
	printf(" \\\"\\      /\"/    | | | | \\| |_) |//| | | |     \\| |  _ / \\/ _ \\/ \\| |\\/| |/ |  _|\"  \\| |/  \n");
	printf(" /\\ \\ /\\ / /\\.-,_| |_| |  |  _ <  U| |_| |\\     | |_| |  / ___ \\  | |  | |  | |___   |_|   \n");
	printf("U  \\ V  V /  U\\_)-\\___/   |_| \\_\\  |____/ u      \\____| /_/   \\_\\ |_|  |_|  |_____|  (_)   \n");
	printf(".-,_\\ /\\ /_,-.     \\\\     //   \\\\_  |||_         _)(|_   \\\\    >><<,-,,-.   <<   >>  |||_  \n");
	printf(" \\_)-'  '-(_/     (__)   (__)  (__)(__)_)       (__)__) (__)  (__)(./  \\.) (__) (__)(__)_)  \n");


    do {
        printf("+-------------------------------------------------------+\n");
		printf("|         ~~~ Welcome to " MAGENTA "GUESSING" RESET " " CYAN "WORD" RESET " " YELLOW "GAME" RESET " ~~~         |\n");
		printf("|" CYAN "                      Options:                         " RESET "|\n");
		printf("|" RED    "                 [1] CREDITS                           " RESET "|\n");
		printf("|" YELLOW "                 [2] ADMIN                             " RESET "|\n");
		printf("|" GREEN  "                 [3] PLAY                              " RESET "|\n");
		printf("|" BLUE   "                 [4] HIGH SCORES                       " RESET "|\n");
		printf("|" GREEN  "                 [5] SAVED GAMES                       " RESET "|\n");
		printf("|" YELLOW "                 [6] ACHIEVEMENTS                      " RESET "|\n");
		printf("|" RED    "                 [7] LIMBO MODE                        " RESET "|\n");
		printf("|" WHITE  "                 [8] EXIT                              " RESET "|\n");
		printf("+-------------------------------------------------------+\n\n");


		
        printf("Enter choice: ");
        inputResult = scanf("%d", &nChoice);

        if (inputResult == 1) {
            switch(nChoice) {
                case 1:
                    creditsMenu();
                    break;
                case 2:
                    adminMenu(wordlist, entryCount);
                    break;
                case 3: 
                    playTime(&player, wordlist, entryCount, 0);
                    break;
                case 4: 
                    displayHighScores();
					limboModeDisplay();
                    break;
                case 5: 
                    if(loadSave(&player, wordlist, entryCount)) {
                        playTime(&player, wordlist, entryCount, 1);
                    }
                    break;
                case 6:
                    displayAchievements();
                    break;
                case 7:
                    limboMode(&player, wordlist, entryCount);
                    break;
                case 8:
                    quitGame();
                    break;
                default: 
                    printf(RED"Invalid choice. Please enter a number between 1-8.\n"RESET);
            }
        } else {
            printf(RED"Invalid input. Please enter a number between 1-8.\n"RESET);
            // Clear the input buffer
            while (getchar() != '\n'); // This consumes all characters until newline
        }
    } while (nChoice != 8);
}

// main
int main() {
	gameData wordlist[200];
	int entryCount = 0;

	mainMenu(wordlist, &entryCount);

	return 0;
}
