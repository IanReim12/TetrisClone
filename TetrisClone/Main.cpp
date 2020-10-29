/*
=======================================================================
Tetris Clone
Ian Reim


------Controls-----------
Arrow Keys to move tertris block
Z key to rotate block
-------------------------
=======================================================================
*/




#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include  <iostream>
#include  <Windows.h>
#include <thread>
#include <vector>

using namespace std;

wstring tetrisBlock[7]; //there are 7 different blocks in tetris

//playing field dimentions
const int fieldWidth = 12;
const int fieldHeight = 18;
const int nScreenWidth = 80;
const int nScreenHeight = 30;
unsigned char *pField = nullptr;

bool bKey[4];
bool zKeyHeld = true;


int difficulty = 20;
int difficultyCounter = 0; //counts game tics
bool pieceDown = false;
int pieceCounter = 0;
int score;
vector<int> lines;






int Rotate(int px, int py, int rotationIndex)
{
	int pi = 0;
	switch (rotationIndex % 4)
	{
	case 0: // 0 degrees			// 0  1  2  3
		pi = py * 4 + px;			// 4  5  6  7
		break;						// 8  9 10 11
									//12 13 14 15

	case 1: // 90 degrees			//12  8  4  0
		pi = 12 + py - (px * 4);	//13  9  5  1
		break;						//14 10  6  2
									//15 11  7  3

	case 2: // 180 degrees			//15 14 13 12
		pi = 15 - (py * 4) - px;	//11 10  9  8
		break;						// 7  6  5  4
									// 3  2  1  0

	case 3: // 270 degrees			// 3  7 11 15
		pi = 3 - py + (px * 4);		// 2  6 10 14
		break;						// 1  5  9 13
	}								// 0  4  8 12

	return pi;
}


bool DoesPieceFit(int blockID, int rotationIndex, int topLeftX, int topLeftY)
{

	for (int i = 0; i < 4; i ++)

		for (int j = 0; j < 4; j++)
		{

			int currentIndex = Rotate(i, j, rotationIndex); //gets current index

			int fieldIndex = (topLeftY + j) * fieldWidth + (topLeftX + i);//into field


			if (topLeftX + i >= 0 && topLeftX + i < fieldWidth)
			{

				if (topLeftY + j >= 0 && topLeftY + j < fieldHeight)
				{
					if (tetrisBlock[blockID][currentIndex] == L'X' && pField[fieldIndex] != 0)
						return false; //only pass false on hit
				}

			}

		}

	return true;

}







int main()
{
	bool gameRunning = false; //used for gameloop

	int currPieceID = 0;
	int currPieceRotation = 0;
	int currPieceX = fieldWidth / 2;
	int currPieceY = 0;





	//Windows function used to create cmd line screen buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	HWND console = GetConsoleWindow();

	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);

	//resize console window
	MoveWindow(console, 0, 0, 0, 0, TRUE);
	MoveWindow(console, 0, 0, 675, 600, TRUE);

	


	//Create entries for each shape 
	tetrisBlock[0].append(L"..X."); //each block entry is a 4x4, representing the block and its empty spaces
	tetrisBlock[0].append(L"..X.");
	tetrisBlock[0].append(L"..X.");
	tetrisBlock[0].append(L"..X.");
	
	tetrisBlock[1].append(L"..X."); 
	tetrisBlock[1].append(L".XX.");
	tetrisBlock[1].append(L".X..");
	tetrisBlock[1].append(L"....");

	tetrisBlock[2].append(L".X.."); 
	tetrisBlock[2].append(L".XX.");
	tetrisBlock[2].append(L"..X.");
	tetrisBlock[2].append(L"....");

	tetrisBlock[3].append(L"...."); 
	tetrisBlock[3].append(L".XX.");
	tetrisBlock[3].append(L".XX.");
	tetrisBlock[3].append(L"....");

	tetrisBlock[4].append(L"..X."); 
	tetrisBlock[4].append(L".XX.");
	tetrisBlock[4].append(L"..X.");
	tetrisBlock[4].append(L"....");

	tetrisBlock[5].append(L"...."); 
	tetrisBlock[5].append(L".XX.");
	tetrisBlock[5].append(L"..X.");
	tetrisBlock[5].append(L"..X.");

	tetrisBlock[6].append(L"...."); 
	tetrisBlock[6].append(L".XX.");
	tetrisBlock[6].append(L".X..");
	tetrisBlock[6].append(L".X..");

	pField = new unsigned char[fieldWidth*fieldHeight]; // Create play field buffer
	for (int x = 0; x < fieldWidth; x++) // Board Boundary
		for (int y = 0; y < fieldHeight; y++)
			pField[y*fieldWidth + x] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0;

	while (!gameRunning) //game loop
	{

		//Game Timing============================
		std::this_thread::sleep_for(50ms);
		difficultyCounter++;
		pieceDown = (difficultyCounter == difficulty); //one line bool
		
		//Input==================================
		for (int k = 0; k < 4; k++)
		{

			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		}
		//Game Logic=============================

		if (bKey[0]) //player presses Right
		{

			if (DoesPieceFit(currPieceID, currPieceRotation, currPieceX + 1, currPieceY))
			{
				currPieceX = currPieceX + 1;
			}


		}

		if (bKey[1]) //player presses left
		{

			if (DoesPieceFit(currPieceID, currPieceRotation, currPieceX - 1, currPieceY))
			{
				currPieceX = currPieceX - 1;
			}


		}

		if (bKey[2]) //player presses down
		{

			if (DoesPieceFit(currPieceID, currPieceRotation, currPieceX , currPieceY + 1))
			{
				currPieceY = currPieceY + 1;
			}


		}



		

		if (bKey[3])
		{

			if (zKeyHeld && DoesPieceFit(currPieceID, currPieceRotation + 1, currPieceX, currPieceY))
			{

				currPieceRotation = currPieceRotation + 1;

				zKeyHeld = false;
			}


		}
		else
		{
			zKeyHeld = true;
		}
			

		if (pieceDown)
		{
			// Update difficulty every 50 pieces
			difficultyCounter = 0;
			pieceCounter++;
			if (pieceCounter % 50 == 0)
				if (difficulty >= 10) difficulty--;

			// Test if piece can be moved down
			if (DoesPieceFit(currPieceID, currPieceRotation, currPieceX, currPieceY + 1))
				currPieceY++; // It can, so do it!
			else
			{
				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetrisBlock[currPieceID][Rotate(px, py, currPieceRotation)] != L'.')
							pField[(currPieceY + py) * fieldWidth + (currPieceX + px)] = currPieceID + 1;

				// Check for lines
				for (int py = 0; py < 4; py++)
					if (currPieceY + py < fieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < fieldWidth - 1; px++)
							bLine &= (pField[(currPieceY + py) * fieldWidth + px]) != 0; //checks if theres an empty space in the field array, representing a break in what would be a line

						if (bLine)//if the above statement fails
						{
							// Remove Line, set to =
							for (int px = 1; px < fieldWidth - 1; px++)
								pField[(currPieceY + py) * fieldWidth + px] = 8;
							lines.push_back(currPieceY + py);
						}
					}

				score += 25;

				if (!lines.empty()) score += (1 << lines.size()) * 100;//scoring




				//Select next peice to be dropped
				currPieceX = fieldWidth / 2;
				currPieceY = 0;
				currPieceRotation = 0;
				currPieceID = rand() % 7;


				//game over
				gameRunning = !DoesPieceFit(currPieceID, currPieceRotation, currPieceX, currPieceY);

			}

			
		}



		

		//Render Output==========================



		//Draw Field
		for (int x = 0; x < fieldWidth; x++)
			for (int y = 0; y < fieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 35)] = L" ABCDEFG=#"[pField[y*fieldWidth + x]];




		//Draw Active Piece
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				if (tetrisBlock[currPieceID][Rotate(x, y, currPieceRotation)] == L'X')
					screen[(currPieceY + y + 2) * nScreenWidth + (currPieceX + x + 35)] = currPieceID + 65;//for letter representation.

		//Draw score
		swprintf_s(&screen[2 * nScreenWidth + fieldWidth + 6], 16, L"SCORE: %8d", score);


		if (!lines.empty())
		{
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay a bit
			
			for (auto &v : lines)
				for (int px = 1; px < fieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
						pField[py * fieldWidth + px] = pField[(py - 1) * fieldWidth + px];
					pField[px] = 0;
				}


			
			lines.clear();
		}



		//Display Field
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	


	CloseHandle(hConsole);
	cout << "Game Over!! Score:" << score << endl;
	//system("pause");
	return 0;
	

}