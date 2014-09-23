////Pong JHL1138 
//
//// 08/21/2014

/* JUSTIN 
I SPENT SUNDAY WITH A FRIEND WHO HELPED ME WITH MUCH OF CODE BELLOW. I WANT TO BE CLEAR ABOUT THAT UPFRONT HE WALKED ME THROUGH A LOT OF THIS. I HAVE MADE TONS OF NOTES
SO YOU ARE CLEAR ON MY UNDERSTANDING OF WHERE I NEED HELP. I HAVE LEARNED A LOT BUT STILL HAVE A LOSE GRASP OF THE MANY OF THE CONCEPTS WE HAVE COVERED. 
I HAVE NOTED THIS IN THE CODE WHERE RELIVENT. SO YOU CAN GRADE ME APPROPRIATELY.

THAT SAID I HAVE A BIGGER PROBLEM THAN THE GRADE OF THIS PROJECT. 
I DO NOT FEEL COMFORTABLE ASKING YOU FOR HELP IN CLASS BECAUSE YOU ARE ALWAYS BERATING STUDENTS IN FRONT OF OTHER STUDENTS!
THIS STARTED DAY ONE AND I TRYED TO OFFER YOU A SOLUTION THAT WOULD WORK FOR ME BUT IT WASN'T NOTED.
THIS MIGHT BE HOW IT WORKS IN THE "REAL WORLD" BUT THIS IS A CLASS ROOM.
THE CURRENT ENVIROMENT DOES NOT PROVIDE A CODUCIVE LEARNING ENVIROMENT FOR ME.
LETS CHAT MORE ABOUT THIS TODAY AND SEE IF THERE IS A SOLUTION THAT WORKS FOR BOTH OF US. 
OTHERWISE I WILL NEED TO DROP YOUR CLASS.

NOTES
Keep everything together makes everything easyer to find and fix.
Pass by reference rather than a pointer this is sweet and clean. (ball& b)  in the function Parameter THIS IS CLEVER BUT NEED TO UNDERSTAND BETTER.
because we can't use classes make everthing in functions.

*/
#include "AIE.h"
#include <iostream>
#include <fstream>

//INIT SCREEN 
int iScreenWidth = 800;
int iScreenHeight = 600;
const char* gameTitle = "PONGish";
const char ESC = '\0x27';
const char* score1 = "Score<1>";
const char* score2 = "Score<2>";
const char* hScore = "High Score";



/*
NEED TO FIGURE OUT HOW TO PASS VAR INTO SCORE AND THEN DRAW THAT TO SCREEN.
*/

const char* enterPlay = "Press Enter to Play";
const char* menuScreen = "Menu";
const char* pong = "Pong";
const char* gQuit = "Space Bar to Quit";

/*
SCREEN SIZE
PADDLE INSET
*/
const int iScreenRight = 799;
const int iScreenLeft = 1; 
const int iScreenTop = 599;
const int iscreenBottom = 1;
const int iPaddleInset = iScreenWidth*0.05;

/*
GAMESTATE FOR MENU
*/

enum GAMESTATE  
{
	eMAIN_MENU,
	eGAME_PLAY,
	eEND,
};
/*
BUILT A STRUCT WITH EVERYTHING I NEED TO PASS LATER.
ADDED A FEW THINGS AS I WENT
*/

struct ball
{
	unsigned int iSpriteID;
	const char* ballLoc = "./images/ball.png";
	int fWidth = 20;
	int fHeight = 20;
	float x = 0;
	float y = 0;
	float xSpeed = 80.f; //for better bounce
	float ySpeed = 200.f; //for better bounce
	unsigned int iUpMovementExtreme;
	unsigned int iDownMovementExtreme;
	unsigned int iRightMovementExtreme;
	unsigned int iLeftMovementExtreme;
};

/*
BALL STUFF MOVEMENT EXTREMES LATER FOR COLLISION
*/

void BallOnce(ball& b){
	b.iUpMovementExtreme = iScreenHeight; //ball call struct up ext = height of screen
	b.iDownMovementExtreme = iscreenBottom; // etc
	b.iLeftMovementExtreme = iScreenLeft; // etc
	b.iRightMovementExtreme = iScreenRight; //etc
}

/*
BALL SPRITEID, CREATIVE THE BALL, LOC, W, H 
*/

void BallForGame(ball& b){ //function for game ball, call bal struct PASS THROUGH REFERCE ( ball is b ) SIMPLE ENOUGH
	b.iSpriteID = CreateSprite(b.ballLoc, b.fWidth, b.fHeight, true); //createSprite get location , width and height
}
/*
///SET BALL  FOR THE GAME PLAY

*/

void BallForRally(ball& b){ //function for game play
	b.y = (b.iUpMovementExtreme + b.iDownMovementExtreme) / 2.f; //ball can only go so far up
	b.x = (b.iRightMovementExtreme + b.iLeftMovementExtreme) / 2.f; //ball can only go so far down
}

//DRAW THE BALL

void Draw(ball& b){ //function for drawing the ball
	DrawSprite(b.iSpriteID); //drawSprite ball iSpriteID
}

/////SIDES OF PADDLES
enum side { //need enum for side of paddle
	LEFT, RIGHT // set left and right
};

/*//PLAYER STRUCT 
SO AS I UNDERSTAND IT FUCNTIONS IN A STRUCT IS A "CLASS" AND YOU SAID NOT TO USE CLASSES YET
SO I REWROTE ALL OF THIS WITH STRUCTS WITH NO FUCTIONS. 

*/

struct player{
	side s; // ENUM FOR SIDES
	unsigned int iSpriteID; //PLAYER ID
	const char* ballLoc = "./images/ball.png"; //BALL LOCATION
	const char* paddleLoc = "./images/paddle.png"; // PADDLE LOCATION
	int fWidth = 20; //PADDLE SIZE
	int fHeight = 60; //PADDLE SIZE
	float x = 0; // PLAYER x
	float y = 0;//  PLAYER Y
	float xStrikingFace; //offset of paddle
	float speed = 500.f; //SPEED
	unsigned int SetPosition; //POSITION
	unsigned int iMoveUpKey; //UP
	unsigned int iMoveDownKey; //DOWN
	unsigned int iUpMovementExtreme; // iScreenHeight
	unsigned int iDownMovementExtreme; //iscreenBottom
	char cPointsWon; // use atoi to convert to int 
	const char* numScore1 = "00000"; //NEED HELP HERE NOT SURE HOW TO PASS  
	const char* numScore2 = "00000"; //NEED HELP HERE
	const char* numHScore = "00000";  //NEED HELP HERE
};

/*//WALL BOUNCE
WOULD HAVE BEEN NICE IF WE HAD GONE OVER THE THEORY OF COLLISION 
SOONER THEN THE DAY BEFORE THE PROJECT IS DO. THIS IS MAKING SENSE 
BUT I DON'T THINK I WOULD HAVE GOT HERE WITHOUT HELP. 

*/

float BounceAmount(ball& b, player& p){ //BALL STRUCT PAS BY REF. PLAYER STRUCT PASS BY REF.  
	const float halfHeight = p.fHeight / 2.0f;  //VAR FOR CAL FROM CENTER OF PADDLE
	if (b.y > (p.y + halfHeight)) return 0; //  BALL Y > HALFHEIGHT SWING AND A MISS
	if (b.y < (p.y - halfHeight)) return 0; //  SAME
	const float bounce = b.x - p.xStrikingFace; //CHEAT FOR X COLLISION
	if ((LEFT == p.s) && (bounce < 0)){ //LEFT HIT SIDE BOUNCE
		return bounce;
	}
	if ((RIGHT == p.s) && (bounce > 0)){ //RIGHT HIT SIDE BOUNCE 
		return bounce; //RETURN FLOAT VAL
	}
	return 0;
}

 /*
 HOLY BALLS THIS WAS A BITCH! 
 AGAIN THANKFULLY I HAVE A FRIEND THAT KNOWS THIS STUFF OTHERWISE THIS WOULDN'T
 HAVE GOTTEN THIS FAR. I DO NOT FEEL I AM GETTING THE SUPPORT I NEED IN CLASS!!!!!

 I NEED MORE INSTRUCTION HERE. AGAIN I HAD HELP HERE AND I HAVE A LOSE GRASP OF THE CONCEPTS HERE 
 PRE-RECORDED LECTURES DO NOT GIVE METHOD!!! I HAVE COMMENTED MY UNDERSTAND. 
 WE SHOULD CHAT MORE ABOUT THIS TO MAKE SURE I GET IT. 
 */

bool RallyOverMove(ball& b, float a_fTimeStep, player& pLeft, player& pRight){ // 
	float xNew = b.x + (a_fTimeStep * b.xSpeed); //make new float to hold ball pass speed
	if (xNew > b.iRightMovementExtreme){ //if ball(xNew) is greater the right side of screen 
		pLeft.cPointsWon++; //Left player point increase by one NEED TO UNDERSTAND HOW TO PASS THIS TO  SCORE VARIABLE AND DRAW THAT TO SCREEN 
		return true; // return as true and add point 
	}
	if (xNew < b.iLeftMovementExtreme){ //if ball is less then Left Side of Screen
		pRight.cPointsWon++; // add Point to player Right  NEED TO UNDERSTAND THIS BETTER
		return true; //return as true and add point
	}
	// BOUNCE MATHS NEED THIS LATER
	float xBounce = BounceAmount(b, pLeft); //new float bounce amount bounce amount ball player left
	if (0 == xBounce){ // if bounce == TO XBOUNCE
		xBounce = BounceAmount(b, pRight); //
	}
	if (0 != xBounce){  //NEED BETTER UNDERSTANDING HERE
		xNew = xNew - (2.f * xBounce);  // NEED BETTER UNDERSTANDING HERE
		b.xSpeed = -b.xSpeed; //BALL SPEED
	}

	/* WALL BOUNCE MATHS
	// NEED TO UNDERSTAND THIS BETTER AS I UNDERSTAND IT WE TRACK BALL MOVEMENT SO IF IT GOES PASSED 
	THE EXTREME MOVEMENT WE SUBTRACT IT BACK AND KEEP BALL ON THE SCREEN USING YBOUNCE
	*/
	float yNew = b.y + (a_fTimeStep * b.ySpeed); //NEW Y VAR = BALL STRUCT Y (PASS SPEED)
	float yBounce = yNew - b.iUpMovementExtreme;  //DO IT AGAIN SET NEW VAR SET UP EXTREME
	if (yBounce > 0){ // NOW IF Y BOUNCE IS GREATER THE 0
		yNew = b.iUpMovementExtreme - yBounce; //
		b.ySpeed = -b.ySpeed; //
	}
	else {
		yBounce = b.iDownMovementExtreme - yNew;
		if (yBounce > 0) {
			yNew = b.iDownMovementExtreme + yBounce;
			b.ySpeed = -b.ySpeed;
		}
	}
	b.x = xNew;
	b.y = yNew;
	MoveSprite(b.iSpriteID, b.x, b.y);
	return false;
}

/* 
 
 SET UP THE PLAYERS 

*/

void PlayerOnce(player& p, side s){ //
	p.s = s; 
	p.iUpMovementExtreme = iScreenHeight;
	p.iDownMovementExtreme = iscreenBottom;
	switch (s){
	case RIGHT:
		p.x = iScreenRight - iPaddleInset;
		p.xStrikingFace = p.x - 20;
		p.iMoveDownKey = 'L';
		p.iMoveUpKey = 'P';
		break;
	default:
		
	case LEFT:
		p.x = iScreenLeft + iPaddleInset;
		p.xStrikingFace = p.x + 20;
		p.iMoveDownKey = 'A';
		p.iMoveUpKey = 'Q';
		break;
	}
}

/*
PLAYER INFORMATION

*/

void PlayerForGame(player& p){
	p.y = (p.iUpMovementExtreme + p.iDownMovementExtreme) / 2.f;
	p.iSpriteID = CreateSprite(p.paddleLoc, p.fWidth, p.fHeight, true);
	MoveSprite(p.iSpriteID, p.x, p.y);
	DrawSprite(p.iSpriteID);
	p.cPointsWon = 0;
}

/*
DRAW PLAYER
*/

void Draw(player& p){
	DrawSprite(p.iSpriteID);
}

/*
SETUP SPRITEID
*/

void iSpriteID(player& p, const char* location, int paddleW, int paddleH){
	p.paddleLoc = location;
	p.fWidth = paddleW;
	p.fHeight = paddleH;
}


/*
SET POSITION OF PLAYER VAR
*/

void SetPosition(player& p, float a_x, float a_y){
	p.x = a_x;
	p.y = a_y;
}

//SET SIZE OF PLAYER VAR 
void SetSize(player& p, float a_fWidth, float a_fHeight){
	p.fWidth = a_fWidth;
	p.fHeight = a_fHeight;
}

// PLAYER MOVEMENT KEYS VAR
void SetMovementKey(player& p, unsigned int a_moveUp, unsigned int a_moveDown){
	p.iMoveUpKey = a_moveUp;
	p.iMoveDownKey = a_moveDown;
}

//ETC...

void SetMovementExtreme(player& p, unsigned a_UpExtreme, unsigned int a_DownExtreme){
	p.iUpMovementExtreme = a_UpExtreme;
	p.iDownMovementExtreme = a_DownExtreme;

}

///MOVE

void Move(player& p, float a_fTimeStep){
	if (IsKeyDown(p.iMoveUpKey))
	{
		p.y += a_fTimeStep * p.speed;
		if (p.y > (p.iUpMovementExtreme - p.fHeight*.5f))
		{
			p.y = (p.iUpMovementExtreme - p.fHeight*.5f);
		}
	}
	if (IsKeyDown(p.iMoveDownKey))
	{
		p.y -= a_fTimeStep * p.speed;
		if (p.y < (p.iDownMovementExtreme + p.fHeight*.5f))
		{
			p.y = (p.iDownMovementExtreme + p.fHeight*.5f);
		}
	}
	MoveSprite(p.iSpriteID, p.x, p.y);
}

//Sprite Globals

player playerOne;  // PLAYERONE 
player playerTwo; //PLAYERTWO
ball theBall; //THEBALL

GAMESTATE eCurrentState = eMAIN_MENU; ///SET GAME START STATE

void UpdateMainMenu(); // FUNCTION PROTOTYPE
void UpdateGameState(float a_deltaTime); // FUNCTION PROTOTYPE
void UpdateGameEnd(); // FUNCTION PROTOTYPE
float MoveBall();// FUNCTION PROTOTYPE
void MovePlayer();// FUNCTION PROTOTYPE

/////////////////////MAIN

int main(int argc, char* argv[]){
	Initialise(800, 600, false, "PONGish"); //INIT SCREEN AND PASS SIZE AND NAME
	SetBackgroundColour(SColour(0, 0, 24, 255)); // Screen Color
	PlayerOnce(playerOne, LEFT); //ONLY NEED TO DO THIS ONCE THEN EVERYTHING MOVES
	PlayerOnce(playerTwo, RIGHT); //ONLY NEED TO DO THIS ONCE THEN EVERYTHING MOVES
	BallOnce(theBall); ////ONLY NEED TO DO THIS ONCE THEN EVERYTHING MOVES

	// Loop
	do
	{
		ClearScreen();
		float deltaTime = GetDeltaTime(); //SET TIME INDEPENDANT OF CPU
		switch (eCurrentState)
		{

		case eMAIN_MENU: //MENU
			UpdateMainMenu();
			break;
		case eGAME_PLAY: //GAME PLAY
			UpdateGameState(deltaTime);
			break;
		case eEND: // END
			break;
		default:
			break;
		}
	} while (!FrameworkUpdate()); //UNTIL NO FRAMEWORK

	Shutdown(); //SHUT DOWN 

	return 0; //RETURN FALSE RIGHT????
}

/*
MENU FUNCTION

*/

void UpdateMainMenu() {
	
	DrawString(pong, iScreenWidth * 0.465f, iScreenHeight * 0.65f, SColour(25, 125, 152, 255));
	DrawString(enterPlay, iScreenWidth * 0.35f, iScreenHeight * 0.5f);
	DrawString(menuScreen, iScreenWidth * 0.46f, iScreenHeight * 0.60f);
	DrawString(hScore, iScreenWidth * 0.42f, iScreenHeight * 0.55f);
	DrawString(gQuit, iScreenWidth * 0.42f, iScreenHeight * 0.45f);


	if (IsKeyDown(257)) //PRESS ENTER KEY
	{
		eCurrentState = eGAME_PLAY; // JUMP TO GAME PLAY
	}
	else if (IsKeyDown(ESC)) //PRESS ESC CLOSE GAME  STILL NEED TO GET WHY THIS ISN'T WORKING
	{
		Shutdown(); //THIS SHOULD CLOSE PROGRAM???
	}
}

/*
GAME PLAY FUNCTION
THIS IS A MEAT OF THE GAME
BECAUSE WE DID ALL THAT WORK THIS LOOK NICE
*/

void UpdateGameState(float a_deltaTime){ //PASS SPEED
	PlayerForGame(playerOne);
	PlayerForGame(playerTwo);
	BallForGame(theBall);
	//Game Loop
	do
	{
		bool endRally = false;
		BallForRally(theBall);
		do{
			ClearScreen();
			float deltaTime = GetDeltaTime();

			///// Paddles
			Move(playerOne, deltaTime);
			Move(playerTwo, deltaTime);
			endRally = RallyOverMove(theBall, deltaTime, playerOne, playerTwo);
			Draw(playerOne);
			Draw(playerTwo);
			Draw(theBall);
			////ball  

			////////DRAW SCORE TO SCREEN 
			DrawString(score1, iScreenWidth *.075f, iScreenHeight - 2, SColour(25, 125, 152, 255));
			DrawString(score2, iScreenWidth *.75f, iScreenHeight - 2, SColour(25, 125, 152, 255));
			DrawString(hScore, iScreenWidth *.40f, iScreenHeight - 2);


			/////////////Graphics
			DrawLine(50, 0, 50, iScreenHeight, SColour(255, 255, 255, 255));
			DrawLine(750, 0, 750, iScreenHeight, SColour(255, 255, 255, 255));

			if (IsKeyDown(' ' /*KEY_ESC*/)) // ESC Key
			{
				eCurrentState = eMAIN_MENU;
			}
		} while (!FrameworkUpdate() && (eCurrentState == eGAME_PLAY) && !endRally); //STRAIGHT FORWARD ENOUGH
	} while (!FrameworkUpdate() && (eCurrentState == eGAME_PLAY)); // then do this 
	DestroySprite(playerOne.iSpriteID); //get rid of player
	DestroySprite(playerTwo.iSpriteID); //get rid of player
	// Shutdown();

}


///GAME OVER BACK TO MENU

void UpdateGameEnd(){

	if (IsKeyDown(ESC)) // ESC KEY NOT SURE WHY THIS ISN'T WORKING
	{
		Shutdown();
	}

}

///PLAYER FUNCTION

void MovePlayer(){

	playerOne.paddleLoc;
	playerTwo.paddleLoc;


	playerOne.iSpriteID;
	playerTwo.iSpriteID;


	playerOne.fHeight = 60;
	playerTwo.fHeight = 60;

	playerOne.fWidth = 20;
	playerTwo.fWidth = 20;


	///////PLAYER DRAW AND MOVE SPRITE B.S.

	do{
		DrawSprite(playerOne.iSpriteID);
		DrawSprite(playerTwo.iSpriteID);

		MoveSprite(playerOne.iSpriteID, playerOne.x, playerOne.y);
		MoveSprite(playerTwo.iSpriteID, playerTwo.x, playerTwo.y);


	} while (true);
}
