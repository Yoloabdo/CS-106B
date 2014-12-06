/*
 * File: Boggle.cpp
 * ----------------
 * Name: Abdulrhman Mohammed Eaita
 * Section: Self learning
 * This file is the main starter file for Assignment #4, Boggle.
 * Version : 1.0
 */
#include <iostream>
#include <string>
#include "gboggle.h"
#include "grid.h"
#include "set.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
#include "strlib.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

struct cubeT
{
  int row;
  int col;
};
int compareCube(cubeT cu1, cubeT cu2){
  if (cu1.row == cu2.row && cu1.col == cu2.col) return 0;
  else if(cu1.row > cu2.col) return 1;
  else return -1;
}
/* Function prototypes */

void welcome();
void giveInstructions();
void InitGameBoard(Grid<char> & cubes);
void gamePlay(Grid<char> & grid, Lexicon & language);
bool checkWordInBoard(Grid<char> &gameBoard, string word, int Col);
bool followChars(int row, int col, string word, Grid<char> & gameBoard, Set<cubeT> usedCubes);
Vector<cubeT> findNextBlock(int x, int y, Grid<char> & gameBoard, char ch, Set<cubeT> usedCubes);
void pcGameStarter(Grid<char> & gameBoard, Lexicon & lexicon, Set<string> & pcChoosenWords,
                            Set<string> & userChoosenWords, int col = 0);
void allPossibleWordsFinder(Grid<char> & gameBoard, Lexicon & lexicon, Set<string> & pcChoosenWords,
                           Set<string> & userChoosenWords, Set<cubeT> & usedCubes,
                           string word, int row, int col);
bool usedSlot(Vector <Vector<int> > usedCubes, int i, int j);
void computerPlayTurn(Grid<char> & grid, Lexicon & language, Set<string> userChoosenWords);
void humanPlayTurn(string word, Grid<char> & grid, Lexicon & language, Set<string> userChoosenWords);


Vector<cubeT> nextGoodBlock(Grid<char> & gameBoard, Lexicon & lexicon, string word, int row, int col);
/* Main program */

int main() {
   GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
   Grid<char> gameCubes;
   Lexicon english("EnglishWords.dat");
   initGBoggle(gw);
   welcome();
   giveInstructions();
   InitGameBoard(gameCubes);
   gamePlay(gameCubes, english);

   return 0;
}

/*
*Function: pcGameStarter
*Usage: pcGameStarter();
*----------------------------------------------------------
*Initalizing Pc game and start looking for the words in the grid through looping rows and recursivly columns.
*/

void pcGameStarter(Grid<char> & gameBoard, Lexicon & lexicon, Set<string> & pcChoosenWords,
                    Set<string> & userChoosenWords, int col){

  Set<cubeT> usedCubes(compareCube);

  if (col > gameBoard.numCols()) return;

  for (int rowToTry = 0; rowToTry < gameBoard.numRows(); ++rowToTry)
  {
    if (gameBoard.inBounds(rowToTry, col))
    {
      string word;
      word += gameBoard[rowToTry][col];
      cubeT cube;
      cube.row = rowToTry;
      cube.col = col;
      usedCubes.add(cube);
      allPossibleWordsFinder(gameBoard, lexicon, pcChoosenWords, userChoosenWords,usedCubes, word, rowToTry, col);
    }
  }
  return pcGameStarter(gameBoard, lexicon, pcChoosenWords, userChoosenWords, ++col);
}

/*
*Function: allPossibleWordsFinder
*Usage: allPossibleWordsFinder();
*----------------------------------------------------------
*Helper for pcGameStarter, which recursively searching for every available word starting
*with specific cube in the grid.
*/
void allPossibleWordsFinder(Grid<char> & gameBoard, Lexicon & lexicon, Set<string> & pcChoosenWords,
                           Set<string> & userChoosenWords, Set<cubeT> & usedCubes, 
                           string word, int row, int col){
  if(!lexicon.containsPrefix(word))   return;

  foreach(cubeT cube in nextGoodBlock(gameBoard, lexicon, word, row, col)){
    if (!usedCubes.contains(cube))
    {
      usedCubes.add(cube);

      string newWord = word + gameBoard[cube.row][cube.col];
      if (newWord.length() > 3 && lexicon.contains(newWord) && !userChoosenWords.contains(newWord))
      {
        pcChoosenWords.add(newWord);
      }
      else {
        allPossibleWordsFinder(gameBoard, lexicon, pcChoosenWords, userChoosenWords, 
          usedCubes, newWord, cube.row, cube.col);
        usedCubes.remove(cube);
      }
    }
  }
}

/*
*Function: nextGoodBlock
*Usage: Vector<cubeT> nextCubes = nextGoodBlock();
*----------------------------------------------------------
*Helper for allPossibleWordsFinder function, returns a vector of cubes for 
*each possible word compination in the grid.
*/
Vector<cubeT> nextGoodBlock(Grid<char> & gameBoard, Lexicon & lexicon, string word, int row, int col){
  Vector<cubeT> result;

  for (int limitedRow = row - 1; limitedRow < row + 2; ++limitedRow)
  {
    for (int limitedCol = col - 1; limitedCol < row + 2; ++limitedCol)
    {
      if (gameBoard.inBounds(limitedRow, limitedCol) && ((limitedRow != row) || (limitedCol != col)) && 
          (lexicon.containsPrefix(word + gameBoard[limitedRow][limitedCol])) )
      {
        cubeT nextPosition;
        nextPosition.row = limitedRow;
        nextPosition.col = limitedCol;
        result.add(nextPosition);
      }
    }
  }
  return result;
}


/* USER Functions SECTION */ 

/* Function: checkWordInBoard()
*  Usage: bool checked = checkWordInBoard();
*  ------------------------------------------
*  This function is the main function of finiding the word from the player on board
*  it checks for the word recursively, by going through cols and rows, following the
*  word if the first letter was found, trying all possible letters around it, and continue
*  the lookup if it didn't found it. the optional param is Row, which define if the func
*  is following the normal lookup or following a word. 
*/

bool checkWordInBoard(Grid<char> &gameBoard, string word, int Col){

  if (Col > gameBoard.numCols()) return false;
  for (int rowToTry = 0; rowToTry < gameBoard.numRows(); ++rowToTry)
  {
      if (gameBoard.inBounds(rowToTry, Col))
      {
        cubeT cube;
        cube.row = rowToTry;
        cube.col = Col;
        if (gameBoard[cube.row][cube.col] == word[0])
        {
          highlightCube(cube.row, cube.col, true);
          Set<cubeT> usedCubes(compareCube);
          usedCubes.add(cube);
          if (followChars(cube.row, cube.col, word.substr(1), gameBoard, usedCubes))
          {
            highlightCube(cube.row, cube.col, false);
            return true;
          }
          highlightCube(cube.row, cube.col, false);
        }
    }
  }
  return checkWordInBoard(gameBoard, word, ++Col);
}

/*
*Function: followChars
*Usage: bool availableInBoard = followChars();
*----------------------------------------------------------
*Recursion function that traces chars of the word in the cubes surronding 
* that pos.
*/

bool followChars(int row, int col, string word, Grid<char> & gameBoard, Set<cubeT> usedCubes){
  if (word.length() == 0) return true;

  foreach(cubeT cube in findNextBlock(row, col, gameBoard, word[0], usedCubes)){
    usedCubes.add(cube);
    pause(300);
    highlightCube(cube.row, cube.col, true);
    if(followChars(cube.row, cube.col, word.substr(1), gameBoard, usedCubes)){
      highlightCube(cube.row, cube.col, false);
      return true;
    }
    else{
      usedCubes.remove(cube);
      highlightCube(cube.row, cube.col, false);
    }
  }
  return false;
}

/*
*Function: findNextBlock
*Usage: Vector<cubeT> availableBlocks = findNextBlock();
*----------------------------------------------------------
*Checking surronding cubes for a defined char, returns a vector of possible
*solutions, excluding used cubes.
*/
Vector<cubeT> findNextBlock(int x, int y, Grid<char> & gameBoard, char ch, Set<cubeT> usedCubes){
  Vector<cubeT> aroundCubes;
  for (int i = x - 1; i < x + 2; ++i)
  {
    for (int j = y - 1; j < y + 2; ++j)
    {
      if (gameBoard.inBounds(i, j))
      {
        cubeT cube;
        cube.row = i;
        cube.col = j;
        if (!usedCubes.contains(cube) && gameBoard[cube.row][cube.col] == ch)
        {
          aroundCubes.add(cube);
        }
      }
    }
  }
  return aroundCubes;
}

/*
* Function: computerPlayTurn
* Usage: computerPlayTurn();
* ----------------------
* Starting computer turn in game, and finishing the game or intialzing a new one.
*/
void computerPlayTurn(Grid<char> & grid, Lexicon & language, Set<string> userChoosenWords){
  Set<string> pcWords;
  pcGameStarter(grid, language, pcWords, userChoosenWords);
  foreach(string word in pcWords){
    recordWordForPlayer(word, COMPUTER);
  }
  cout << "Computer wins, would you like to play another round? y or n: ";
  if (getLine()[0] == 'y')
  {
    InitGameBoard(grid);
  }else{
     cout << "Thanks for playing the Boggle game." << endl;
     pause(500);
     exit(0);
  }
}

/*
* Function: humanPlayTurn
* Usage: humanPlayTurn();
* ----------------------
* Starting user turn in game, and checking his inputs to game standards.
* Displaying diffrent message with each error for better UX.
*/
void humanPlayTurn(string word, Grid<char> & grid, Lexicon & language, Set<string> userChoosenWords){
  if (word.length() < 4)
  {
     cout  << "Not acceptable, please enter more than 4 letters words!" << endl;
  } else {
   if (!userChoosenWords.contains(word))
   {
    if (language.contains(word))
    {
      if (checkWordInBoard(grid, word, 0))
      {
        userChoosenWords.add(word);
        recordWordForPlayer(word, HUMAN);
      }else{
        cout << "not available on the board! Please enter another one" << endl;
      }
    }else{
      cout << "Not an English word, please enter another one!" << endl;
    }
   }else{
      cout << "you've entred this word before." << endl;
   }
  }
 return;
}
/*
* Function: gamePlay
* Usage: gamePlay();
* ----------------------
* Setting the game for bothe players, calling needed functions for each turn.
*/

void gamePlay(Grid<char> & grid, Lexicon & language){
 Set<string> userChoosenWords;
 cout << "Now it's your turn, please start writing words that's more than 4 letters, blank when you're done! " << endl;
  while(true) {
    string word = getLine("word: ");
    word = toUpperCase(word);
    if (word.empty()){
      computerPlayTurn(grid, language, userChoosenWords);
    }
    else{
      humanPlayTurn(word, grid, language, userChoosenWords);
    }
  }

}
/*
* Function: InitGameBoard
* Usage: InitGameBoard()
* ------------------
* intiate the game board, takes the size as user input and check if it's the right
* size for the game, then starts to shuffle the board for the game and draw it.
*/
void InitGameBoard(Grid<char> & cubes){
   int sizeOfGame;
   while(true) {
       sizeOfGame = getInteger("Please specify if you want 4 or 5 game board?");
       if (sizeOfGame == 4 || sizeOfGame == 5){
          cubes.resize(sizeOfGame, sizeOfGame);
          break;
       }
   }
   drawBoard(sizeOfGame, sizeOfGame);
   Vector <string> initCubes;
   for (int i = 0; i < sizeOfGame * sizeOfGame; ++i)
   {
      initCubes.add(STANDARD_CUBES[i]);
   }

   for (int i = 0; i < initCubes.size(); ++i)
   {
      int randomIndex = randomInteger(i, initCubes.size() -1);
      string tmpString = initCubes[i];
      initCubes[i] = initCubes[randomIndex];
      initCubes[randomIndex] = tmpString;
   }
   int count = 0;
   for (int i = 0; i < sizeOfGame; ++i){
    for (int j = 0; j < sizeOfGame; ++j)
      {
        cubes[i][j] = initCubes[count++].at(randomInteger(0, 5));
        labelCube(i, j, cubes[i][j]);
      }
   }
}



/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
   cout << "Welcome!  You're about to play an intense game ";
   cout << "of mind-numbing Boggle.  The good news is that ";
   cout << "you might improve your vocabulary a bit.  The ";
   cout << "bad news is that you're probably going to lose ";
   cout << "miserably to this little dictionary-toting hunk ";
   cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
   cout << endl;
   cout << "The boggle board is a grid onto which I ";
   cout << "I will randomly distribute cubes. These ";
   cout << "6-sided cubes have letters rather than ";
   cout << "numbers on the faces, creating a grid of ";
   cout << "letters on which you try to form words. ";
   cout << "You go first, entering all the words you can ";
   cout << "find that are formed by tracing adjoining ";
   cout << "letters. Two letters adjoin if they are next ";
   cout << "to each other horizontally, vertically, or ";
   cout << "diagonally. A letter can only be used once ";
   cout << "in each word. Words must be at least four ";
   cout << "letters long and can be counted only once. ";
   cout << "You score points based on word length: a ";
   cout << "4-letter word is worth 1 point, 5-letters ";
   cout << "earn 2 points, and so on. After your puny ";
   cout << "brain is exhausted, I, the supercomputer, ";
   cout << "will find all the remaining words and double ";
   cout << "or triple your paltry score." << endl << endl;
   cout << "Hit return when you're ready...";
   getLine();
}

// [TODO: Fill in the rest of the code]
