#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#include<stdio.h>
#include<math.h>

#define ScWidth 1400
#define ScHeight 700
#define OpWidth 400

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 77
#define KEY_RIGHT 75
#define SPACEBAR 32

using namespace std;

// Dispaly Gameover screen and reset score
void GameOver();

// Draw game board and score / control area
void DrawBoard();

// if enemy hit player this function returns 1 
int EnemyHitPlayer();

// if bullet hit enemy this function returns 1 
int BulletHitEnemy();

// function to convert degree to radian
static double d2r(double degree);

// Player Variables
int pSize = 20, step = 20;// player size, player speed
double px = 1000 - pSize, py = 300; // player X, player Y

// Bullet Variables
// bullets (x,y,visible/hidden) , bi- bullet index
int bullet[30][3], bi = 0;

// Enemy Variables
// Enemy X, Enemy Y, Enemy Speed
double enemy[3][3] = {0,260,10,0,320,10,0,390,10};
int ei = 1; // enemy index

// Other Variables
char option, txtScore[5];
int score = 0;

int main(){
	// to make sure generation of random numbers
	srand((unsigned) time(0));
	
	// Initialize Graphics
	int gd = DETECT, gm, err, tmp;
	initgraph( & gd, & gm, "C:\\tc\\bgi");
  	
  	// Initialize bullets with 0 as postion and speed
  	for(int i=0; i<30; i++){
  		bullet[bi][0] = bullet[bi][1] = bullet[bi][2] = 0;
  	}
  	
	while(1){ // for keeping game playing
		
		// Draws game board and score/controls area
		DrawBoard();
		
		// if any key is pressed
		if( kbhit()){
			option = getch(); // catch key in option variable
			if( option == KEY_UP){ // if up key is pressed update player position
				if( py > 200 ){
					py -= step;
				}
			}else if( option == KEY_DOWN){ // if down key is pressed update player position
				if( py < 450 ){
					py += step;
				}
			}
			else if(option == SPACEBAR){ // if key is spacebar shoot bullet
				 bullet[bi][0] = 1000; // set X position of bullet
				 bullet[bi][1] = py+pSize*1.5; // set Y position of bullet
				 bullet[bi][2] = 1; // set speed of bullet
				 
				 // shoot bullet until 30 bullets
				 if( bi < 29)
				 	 bi++;
		 	     else
		 	     	 bi = 0; // other wise restart resuing bullet from 1st
			}
		}
		
		// Draw Bullets 
		for(int i=0; i< 30; i++){
			if( bullet[i][2]!=0 )
				circle(bullet[i][0], bullet[i][1], 5);
			if( bullet[i][0] >= 0){ // increment bullet position until it hits boundry
				bullet[i][0]-=10;
			}else{
				bullet[i][2]=0; // reset if it hits boundry
			}
		}
		
		
		// If any of the enemies hit player
		if( EnemyHitPlayer() == 1 ){
			GameOver(); // display game over screen
			
			// Reset Enemy Positions
			enemy[0][0] = 0; 
			enemy[0][1] = 260;
			enemy[0][2] = 8+rand()%8;
			enemy[1][0] = 0; 
			enemy[1][1] = 320;
			enemy[1][2] = 8+rand()%8;
			enemy[2][0] = 0; 
			enemy[2][1] = 390;
			enemy[2][2] = 8+rand()%8;
		} 
		 
		
		for(int i=0; i<ei; i++){
			// Drawing enemey
			setcolor(YELLOW);
			setfillstyle(1,YELLOW);
			bar(enemy[i][0],enemy[i][1],enemy[i][0]+40,enemy[i][1]+20);
			
			// update enemy position
			enemy[i][0] += enemy[i][2];
			
			// if enemy goes beyond screen reset enemy position
			if( enemy[i][0] >= 1000 ){
				enemy[i][0] = 0; 
				enemy[i][2] = 8+rand()%8;
			}
			
			// if first enemy goes beyond 300px from left generate next enemy
			if( enemy[i][0] >= 300 && enemy[i][0] < 310 )
				if( ei < 3 )
					ei++;
		}
		
		if( BulletHitEnemy() == 1 ){
			score++; // display game over screen
		}
		
		// drawing player
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		bar(px, py, px+pSize, py+pSize*3);
		bar(px-pSize, py+pSize, px, py+pSize*2);
		
		 
		delay(40);
		cleardevice();
	}
 
	
	getch();
	closegraph();
}


void GameOver(){
	cleardevice(); // clear screen
	
	setcolor(WHITE);
	// Print Game Over Text
	settextstyle(4,0,9);
	outtextxy(300,300,"Game Over");

	settextstyle(4,0,2);
	outtextxy(500,420,"Press any key to restart...");
	getch();
	score = 0; // reset score
} 

void DrawBoard(){	

	setcolor(WHITE);
	
	// draw board
	rectangle(0,0,ScWidth-OpWidth,ScHeight);
	rectangle(ScWidth-OpWidth,0,ScWidth,ScHeight);
	
	// print score
	// itoa function converts number to character array
	settextstyle(4,0,9);
	if( score<10 )
		outtextxy(ScWidth-270, 250, itoa(score,txtScore,10));
	else if( score>=10 )
		outtextxy(ScWidth-320, 250, itoa(score,txtScore,10));
	
	settextstyle(4,0,1);
	outtextxy(ScWidth-375, 500, "Controls: ");
	outtextxy(ScWidth-300, 550, "Up Arrow - Move Upward");
	outtextxy(ScWidth-300, 600, "Down Arrow - Move Downward");
	outtextxy(ScWidth-300, 650, "Spacebar - Fire");
}

// returns 1 if enemy hits player
int EnemyHitPlayer(){
	for(int i=0; i< ei; i++){
		if( enemy[i][0] >= px-pSize && enemy[i][0] <= px+pSize ){
			if( (enemy[i][1] <= py+pSize*3 && enemy[i][1] >= py) ||   
				(enemy[i][1]+20 >= py && enemy[i][1]+20 <= py) )
			{
				return 1;
			}
		}
	}
	return 0;
}

// return 1 if bullet hits enemy
int BulletHitEnemy(){ 
	for(int i=0; i<30; i++){  
		if(bullet[i][2] == 1){ 
		    for(int j=0; j<ei; j++){ 
				if( enemy[j][0]+40 >= bullet[i][0]-10 && enemy[j][0] <= bullet[i][0]+10 ){ 
					if( bullet[i][1] >= enemy[j][1] && bullet[i][1] <= enemy[j][1]+20 ){
						enemy[j][0] = 0;
						bullet[i][2] = 0; 
						return 1;
					}
				}
			}	
		} 
	}	
	return 0;
}

// Convert degree to radian
static double d2r(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
} 
