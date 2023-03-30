//https://www.circuitbasics.com/how-to-setup-an-led-matrix-on-the-arduino/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <stdbool.h>
#include <stdio.h>
#include<stdlib.h>

#include "DHT.h"

#define MAX_ROWS 32
#define MAX_COLS 8

#define MAX_WAIT 2000
#define MIN_WAIT 100

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

int pinCS = 10;
//Din connects to pin 11
//CLK connects to pin 13

int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 4;

int incomingByte = 0; // for incoming serial data

int wait=500; //delay (ms).

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);


//Custom Functions

//bool arr[MAX_ROWS][MAX_COLS]; //use this when randomly filling matrix.

bool arr[MAX_ROWS][MAX_COLS] = {
{0,0,0,0,0,0,0,0}, //row1
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}, //row 4
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}, //row8
{0,0,0,1,1,0,0,0},
{0,0,1,1,0,0,0,0},
{0,0,0,1,0,0,0,0},
{0,0,0,0,0,0,0,0}, //row 12
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0}, //row 16
{0,0,0,0,1,0,1,0},
{0,0,0,0,1,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0}, //row 20
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,1,0,0},
{0,0,0,0,1,0,0,0},
{0,0,0,0,0,0,0,0}, //row 24
{0,0,0,1,0,1,0,0},
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0}, //row 28
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,0,0,0}, //row 32 (last)
};


//Start-up configuration for the board
void init_screen(bool arr[MAX_ROWS][MAX_COLS]){
  
  matrix.fillScreen(0); //clear screen
  for(int chip=0;chip<4;chip++){
    matrix.setPosition(chip, 0, 0);
  for(int x = 0;x<8;x++){//row
      for(int y = 0; y<8;y++){//col
        
        if(arr[(chip*8)+x][y]){ //if array element is 1=true. 
        matrix.drawPixel(x, y, 1);
        }
        else {
        matrix.drawPixel(x, y, 0);
        }
        matrix.write();
      } //end for y
  }  //end for x
  }//end for chip
  
}//end init_screen

//Randomly fill the board
void random_screen(){
  
  matrix.fillScreen(0); //clear screen
  for(int chip=0;chip<4;chip++){
    matrix.setPosition(chip, 0, 0);
  for(int x = 0;x<8;x++){//row
      for(int y = 0; y<8;y++){//col
        
        if(((float)rand()/RAND_MAX) > 0.5){ //if array element is 1=true. 
        
        arr[x][y] = 1;
        matrix.drawPixel(x, y, 1);
        } //end if
        else {
        arr[x][y] = 0;
        matrix.drawPixel(x, y, 0);
        }
        matrix.write();
      } //end for y
  }  //end for x
  }//end for chip
  
}//end init_screen

//Variables.
int sum, null_cells, time_mod=0;
float h, h_new, t, t_new;
bool arr_new[MAX_ROWS][MAX_COLS];

void setup() {
  Serial.begin(9600);
  
  matrix.setIntensity(1);
  matrix.setRotation(0, 1);
  matrix.setRotation(1, 1);
  matrix.setRotation(2, 1);
  matrix.setRotation(3, 1);
  init_screen(arr);
 
  dht.begin();
  t = dht.readTemperature(); //make initial reading
  h = dht.readHumidity();
  delay(2*MAX_WAIT);
}


void loop() {//game loop.

//Read temp&humidity only if at least 2s has passed:
if(time_mod>2000){
//reset time_mod
time_mod = 0;
//Read humidity. 
h_new = dht.readHumidity();
// Read temperature as Celsius (the default)
t_new = dht.readTemperature();
// Check if any reads failed and exit early (to try again).
if (isnan(h_new) || isnan(t_new)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  //Adjust wait variable based on temperature
  while(wait>MIN_WAIT-1 && wait<MAX_WAIT+1){ //Define extreme allowed values
    wait += (h_new-h)*10;
 

    //Reset wait value if extremes are exceeded:
    if(wait<MIN_WAIT){wait = MIN_WAIT;}
    if(wait>MAX_WAIT){wait = MAX_WAIT;}
    
    break;
  } 

  
  
  h = h_new;
  
  
  //Print temperature and humidity.
  Serial.print(h_new/100);
  Serial.print("\t");
  Serial.print((float)(wait) /1000);
  Serial.print("\n");
}



null_cells = 0; //number of empty cells

//Find sum of neighbors for each cell
for(int x = 0;x<MAX_ROWS;x++){
  for(int y = 0;y<MAX_COLS; y++){
      //arr_new[x][y] = 0;
      sum = 0;

      //find number of null cells
      if(arr[x][y]==0){ null_cells++;}

      //if null cells == 64 (one panel empty, we are at x,y = 7,7) place pattern
      if(null_cells==64){
        arr_new[x-2][y-2] = 1;
        arr_new[x-3][y-2] = 1;
        arr_new[x-4][y-2] = 1;
        arr_new[x-5][y-2] = 1;
        null_cells = 0;
      }

      //First(bottom) row, y in mid-range.
      if(x == 0  && (y>0 && y<MAX_COLS)){
        sum = arr[x][y-1]+arr[x+1][y+1]+arr[x+1][y]+arr[x+1][y-1]+arr[x][y+1];
      }
      
      //Last(top) row, y in mid-range.
      if(x == MAX_ROWS  && (y>0 && y<MAX_COLS)){
        sum = arr[x][y+1]+arr[x-1][y+1]+arr[x-1][y]+arr[x-1][y-1]+arr[x][y-1];
        }
      
      //Left, x in mid-range
      if(y == MAX_COLS  && (x>0 && x<MAX_ROWS)){
        sum = arr[x-1][y]+arr[x-1][y-1]+arr[x][y-1]+arr[x+1][y-1]+arr[x+1][y];
      }

      //Right, x in mid-range
      if(y == 0  && (x>0 && x<MAX_ROWS)){
        sum = arr[x-1][y]+arr[x-1][y+1]+arr[x][y+1]+arr[x+1][y+1]+arr[x+1][y];
      }
      
      //Corner BL
      if(x == 0 && y == MAX_COLS){
        sum = arr[x][y-1]+arr[x+1][y-1]+arr[x+1][y];
      }
    
      //Corner BR
      if(x == 0 && y == 0){
        sum = arr[x][y+1]+arr[x+1][y+1]+arr[x+1][y];
      }

      //Corner TL
      if(x == MAX_ROWS && y == MAX_COLS){
        sum = arr[x-1][y]+arr[x-1][y-1]+arr[x][y-1];
      }

      //Corner TR
      if(x == MAX_ROWS && y == 0){
        sum = arr[x][y+1]+arr[x-1][y+1]+arr[x-1][y-1];
      }


      //Both x,y in mid-range.
      //8 neighbors.
      if((x>0 && x<MAX_ROWS) && (y>0 && y<MAX_COLS)){
          sum = arr[x-1][y-1] + arr[x-1][y] + arr[x-1][y+1] + arr[x][y-1] + arr[x][y+1] + arr[x+1][y-1] + arr[x+1][y] +arr[x+1][y+1];
      }
      
      if(sum == 3){arr_new[x][y]=1; }//birth
      if(sum != 2 && sum!=3){arr_new[x][y]=0;} //death
  }  //end for y
} //end for x

//matrix.fillScreen(0);
init_screen(arr_new);
//arr = arr_new;
for(int i = 0;i < MAX_ROWS;i++){
  for(int j = 0;j < MAX_COLS; j++){
      arr[i][j] = arr_new[i][j];
  }  //end for y
} //end for x
delay(wait); 

time_mod += wait; //value in last delay().
  
}//end loop


