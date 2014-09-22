////Pong JHL1138 
//Justin please note I had a help with this. learned a ton!!  
//// 08/21/2014
/*Notes 
Keep everything together makes everything easyer to find and fix.
Pass by reference rather than a pointer this is sweet and clean. (ball& b) or whatever in the function pa 
becuase we can't use classes Make everthing in functions

*/
#include "AIE.h"
#include <iostream>

//INIT SCREEN 
int iScreenWidth = 800;
int iScreenHeight = 600;
const char* gameTitle = "PONGish";
const char ESC = '\0x27';

//
const char* score1 = "Score<1>";
const char* score2 = "Score<2>";
const char* hScore = "High Score";
const char* numCredits = "99";
const char* enterPlay = "Press Enter to Play";
const char* menuScreen = "Menu";
const char* pong = "Pong";
const char* gQuit = "Esc to Quit";


///SET SCREEN SHIT
const int iScreenRight = 799; 
const int iScreenLeft = 1;
const int iScreenTop = 599;
const int iscreenBottom = 1;
const int iPaddleInset = iScreenWidth*0.05;

///....MENU ENUM WORKING
enum GAMESTATE  //Game Menu Shit day of week
{
	eMAIN_MENU,
	eGAME_PLAY,
	eEND,
};
/*
BUILT A STRUCT WITH EVERYTHING I NEED TO PASS LATER
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

*/

void BallOnce(ball& b){
	b.iUpMovementExtreme = iScreenHeight;
	b.iDownMovementExtreme = iscreenBottom;
	b.iLeftMovementExtreme = iScreenLeft;
	b.iRightMovementExtreme = iScreenRight;
}

void BallForGame(ball& b){
	b.iSpriteID = CreateSprite(b.ballLoc, b.fWidth, b.fHeight, true);
}

///SET BALL FOR THE GAME PLAY
void BallForRally(ball& b){
	b.y = (b.iUpMovementExtreme + b.iDownMovementExtreme) / 2.f;
	b.x = (b.iRightMovementExtreme + b.iLeftMovementExtreme) / 2.f;
}

//DRAW THE BALL 
void Draw(ball& b){
	DrawSprite(b.iSpriteID);
}

/////SIDES OF PADDLES
enum side {
	LEFT, RIGHT 
};

//PLAYER STRUCT (function in a struct is really a class)
struct player{
	side s; 
	unsigned int iSpriteID;
	const char* ballLoc = "./images/ball.png";
	const char* paddleLoc = "./images/paddle.png";
	int fWidth = 20;
	int fHeight = 60;
	float x = 0;
	float y = 0;
	float xStrikingFace; //offset of paddle
	float speed = 500.f;
	unsigned int SetPosition;
	unsigned int iMoveUpKey;
	unsigned int iMoveDownKey;
	unsigned int iUpMovementExtreme;
	unsigned int iDownMovementExtreme;
	int cPointsWon; 
	const char* numScore1 = "00000";
	const char* numScore2 = "00000";
	const char* numHScore = "00000";
};


/*//WALL BOUNCE
HAD HELP HERE BUT IT WORKS
*/
float BounceAmount(ball& b, player& p){ //BALL STRUCT PAS BY REF. PLAYER STRUCT PASS BY REF.  
	const float halfHeight = p.fHeight / 2.0f;  //VAR FOR CAL FROM CENTER OF PADDLE
	if (b.y > (p.y + halfHeight)) return 0; //  
	if (b.y < (p.y - halfHeight)) return 0; // 
	const float bounce = b.x - p.xStrikingFace; 
	if ((LEFT == p.s) && (bounce < 0)){
		return bounce;
	}
	if ((RIGHT == p.s) && (bounce > 0)){
		return bounce;
	}
	return 0;
}
 /*
 
 */
bool RallyOverMove(ball& b, float a_fTimeStep, player& pLeft, player& pRight){
	float xNew = b.x + (a_fTimeStep * b.xSpeed);
	if (xNew > b.iRightMovementExtreme){
		pLeft.cPointsWon++;
		return true;
	}
	if (xNew < b.iLeftMovementExtreme){
		pRight.cPointsWon++;
		return true;
	}
	// PADDLE COLLISION
	float xBounce = BounceAmount(b, pLeft);
	if (0 == xBounce){
		xBounce = BounceAmount(b, pRight);
	}
	if (0 != xBounce){
		xNew = xNew - (2.f * xBounce);
		b.xSpeed = -b.xSpeed;
	}

	// WALL BOUNCE
	float yNew = b.y + (a_fTimeStep * b.ySpeed); 
	float yBounce = yNew - b.iUpMovementExtreme;
	if (yBounce > 0){
		yNew = b.iUpMovementExtreme - yBounce;
		b.ySpeed = -b.ySpeed;
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
PLAYER ONE USE PLAYER STRUCT AND SIDE ENUM PASS AS REF TO p AND s
THEN USE STRIKING FACE TO OFFSET X POS, 
*/
void PlayerOnce(player& p, side s){
	p.s = s; //PLAYER SIDE = 
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
		// report error but do something rEASONABLE (fall through);
	case LEFT:
		p.x = iScreenLeft + iPaddleInset;
		p.xStrikingFace = p.x + 20;
		p.iMoveDownKey = 'A';
		p.iMoveUpKey = 'Q';
		break;
	}
}

void PlayerForGame(player& p){
	p.y = (p.iUpMovementExtreme + p.iDownMovementExtreme) / 2.f;
	p.iSpriteID = CreateSprite(p.paddleLoc, p.fWidth, p.fHeight, true);
	MoveSprite(p.iSpriteID, p.x, p.y);
	DrawSprite(p.iSpriteID);
	p.cPointsWon = 0;
}
void Draw(player& p){
	DrawSprite(p.iSpriteID);
}

void iSpriteID(player& p, const char* location, int paddleW, int paddleH){
	p.paddleLoc = location;
	p.fWidth = paddleW;
	p.fHeight = paddleH;
}
void SetPosition(player& p, float a_x, float a_y){
	p.x = a_x;
	p.y = a_y;
}


void SetSize(player& p, float a_fWidth, float a_fHeight){
	p.fWidth = a_fWidth;
	p.fHeight = a_fHeight;
}


void SetMovementKey(player& p, unsigned int a_moveUp, unsigned int a_moveDown){
	p.iMoveUpKey = a_moveUp;
	p.iMoveDownKey = a_moveDown;
}

void SetMovementExtreme(player& p, unsigned a_UpExtreme, unsigned int a_DownExtreme){
	p.iUpMovementExtreme = a_UpExtreme;
	p.iDownMovementExtreme = a_DownExtreme;

}
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
player playerOne;
player playerTwo;
ball theBall;

GAMESTATE eCurrentState = eMAIN_MENU; ///SET enum start

void UpdateMainMenu(); // FUNCTION PROTOTYPE
void UpdateGameState(float a_deltaTime); // FUNCTION PROTOTYPE
void UpdateGameEnd(); // FUNCTION PROTOTYPE
float MoveBall();
void MovePlayer();

/////////////////////MAIN
int main(int argc, char* argv[]){
	Initialise(800, 600, false, "PONGish"); //Window Name
	SetBackgroundColour(SColour(0, 0, 24, 255)); // Screen Color
	PlayerOnce(playerOne, LEFT); 
	PlayerOnce(playerTwo, RIGHT);
	BallOnce(theBall);

	// Loop
	do
	{

		ClearScreen();
		float deltaTime = GetDeltaTime(); //set time to independant of CPU

		switch (eCurrentState)
		{

		case eMAIN_MENU:
			UpdateMainMenu();
			break;
		case eGAME_PLAY:
			UpdateGameState(deltaTime);
			break;
		case eEND:
			break;
		default:
			break;
		}
	} while (!FrameworkUpdate());

	/// Destroy Sprites here



	Shutdown();

	return 0;
}

/////////////////////MENU

void UpdateMainMenu() {
	
	DrawString(pong, iScreenWidth * 0.465f, iScreenHeight * 0.65f, SColour(25, 125, 152, 255));
	DrawString(enterPlay, iScreenWidth * 0.35f, iScreenHeight * 0.5f);
	DrawString(menuScreen, iScreenWidth * 0.46f, iScreenHeight * 0.60f);
	DrawString(hScore, iScreenWidth * 0.42f, iScreenHeight * 0.55f);
	DrawString(gQuit, iScreenWidth * 0.42f, iScreenHeight * 0.45f);



	if (IsKeyDown(257)) //257 is enter key
	{
		eCurrentState = eGAME_PLAY;
	}
	else if (IsKeyDown(033))
	{
		Shutdown();
	}
}

//////GAME PLAY

void UpdateGameState(float a_deltaTime){
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

			////////Score Table stuff
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
		} while (!FrameworkUpdate() && (eCurrentState == eGAME_PLAY) && !endRally); //do this until
	} while (!FrameworkUpdate() && (eCurrentState == eGAME_PLAY)); // then do this 
	DestroySprite(playerOne.iSpriteID); //get rid of player
	DestroySprite(playerTwo.iSpriteID); //get rid of player
	// Shutdown();

}


/////////////////////GAME OVER BACK TO MENU
void UpdateGameEnd(){

	if (IsKeyDown(KEY_ESC)) // ESC KEY
	{
		eCurrentState = eMAIN_MENU;
	}

}


void MovePlayer(){

	playerOne.paddleLoc;
	playerTwo.paddleLoc;


	playerOne.iSpriteID;
	playerTwo.iSpriteID;


	playerOne.fHeight = 60;
	playerTwo.fHeight = 60;

	playerOne.fWidth = 20;
	playerTwo.fWidth = 20;


	///////PLAYERS 
	do{
		DrawSprite(playerOne.iSpriteID);
		DrawSprite(playerTwo.iSpriteID);

		MoveSprite(playerOne.iSpriteID, playerOne.x, playerOne.y);
		MoveSprite(playerTwo.iSpriteID, playerTwo.x, playerTwo.y);


	} while (true);
}
