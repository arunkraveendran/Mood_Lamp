//Edwin Lee & Pingda Li 
//Arduino program that will control a bi-directional motor, 
//a multicolour LED, and Piezo pins by taking in user input. 
//Nov 28, 2013

//introducing variables
#include <Servo.h>
Servo doorMan; 
int doorPos=0,systemState=0; //systemState is the currently selected mode
int advancePin=4, powerPin=7, prevPin=2; //input pins
int redPin=6,greenPin=5,bluePin=3,piezo1Pin=9,servoPin=10,piezo2Pin=12,bidirectionalPin=11; //output pins
double powerMod=0; //variable for the potentiometer  
boolean runSystem=true; //toggles system on/off  
int numIncrements=10; //number of steps to break each mode into

//Setting up variables
void setup()
{
  doorMan.attach(10); 
  
  //Assign pins for input buttons- _PULLUP removes need for pullup resistor
  pinMode (advancePin,INPUT_PULLUP);
  pinMode (powerPin,INPUT_PULLUP);
  pinMode (prevPin,INPUT_PULLUP);
  
  //Assign output pins
  pinMode (redPin,OUTPUT);   
  pinMode (greenPin,OUTPUT);
  pinMode (bluePin,OUTPUT);
  pinMode (piezo1Pin,OUTPUT);
  pinMode (servoPin,OUTPUT);
  pinMode (piezo2Pin,OUTPUT);  
  
  //Turn off all lights 
  analogWrite(redPin,255); 
  analogWrite(greenPin,255); 
  analogWrite(bluePin,255); 
  
  Serial.begin(115200); //for debugging, use Serial.println("TEXT") to check values 
}

//This program will loop and constantly see if the user has changed the variable resistor or
//has clicked a button. The program will cycle between 4 modes: action, sad, happy, and random.
//When the user clicks, the mode will change. It will go forward a mode or back a mode depending
//on which button is pressed. There is another button to turn the moodLamp on or off.
void loop(){
 
   int increment=0; //counter for later loop 
   powerMod=analogRead(0)/1024.0; //express potentiometer reading as a fraction  
   Serial.println(powerMod); //debugging 
   
   //Advance mode button
   if (digitalRead(advancePin)==LOW){
    
    //Loop will run until user releases button
    while(digitalRead(advancePin)==LOW){ }
    //Advances state as prescribed 
     systemState++; 
     //Deals with state counter going above number of states
     if (systemState>3)
       systemState=0; 
   }
   
    //Previous mode button
   if (digitalRead(prevPin)==LOW){
    //Loop runs until user releases button
     while(digitalRead(prevPin)==LOW){ }
     
     //Reduces state as prescribed
     systemState--;
     //Deals with state counter going below zero
      if (systemState<0)
        systemState=3; 
   }
    Serial.println(systemState); //debugging  
   
   //Toggle power button
   if (digitalRead(powerPin)==LOW){  
     
     //Wait until user releases button
     while(digitalRead(powerPin)==LOW){ }
    
    //Switches state from on to off, or off to on
    //Opens box if turning light on, closes box if turning light off
    
     if (runSystem==false){
       //moveDoor is called, the 2 variables are starting degrees and speed at which to move at
      moveDoor(doorPos,30,15); 
       runSystem=true;  
     }
     
     else{ 
       //moveDoor is called, the 2 variables are starting degrees and speed at which to move at
       runSystem=false; 
       moveDoor(doorPos,60,15); 
     }  
   }
    
   //Turns off all lights and motor if system is turned off
   if (runSystem==false){
   //everything off
   analogWrite(bidirectionalPin,0); 
   analogWrite(redPin,255); 
   analogWrite(greenPin,255); 
   analogWrite(bluePin,255); 
   
   delay (10);    
   }
  
  //Operations for when system is turned on
  else {
   //Like a for loop using increment as counter, but exit loop as soon as one of the buttons is pressed
   while (increment<numIncrements&&digitalRead(advancePin)==HIGH&&digitalRead(prevPin)==HIGH&&digitalRead(powerPin)==HIGH){
    
    analogWrite(bidirectionalPin,30) ;  
     
    //Calls various modes depending on which is currently selected
    //Modes are broken into increments so that they can be interruped part way through if need be (ie button pushed)
    
    if (systemState==0){
      mode0(increment,powerMod); 
    }
    
    else if (systemState==1){
      mode1(increment,powerMod);
    }
    
    else if (systemState==2){
      mode2(increment,powerMod);
    }
    
    else if (systemState==3){
      mode3(increment,powerMod);
    }
    
    increment++; //Increases counter, this will also be sent into the song and will play the individual notes
  }
 }
}


//These funcitons are the various moods offered by the moodlamp 
//action mode
void mode0(int increment,double pwrPercent){
  red (increment);
}

//random mode
void mode1(int increment,double pwrPercent){
      randomSounds();
}

//happy mode
void mode2(int increment,double pwrPercent){
      green(increment);
}

//sad mode
void mode3(int increment,double pwrPercent){
      blue (increment);
}

//The following programs will have their own set of notes and will play these notes in order
//They will have their own tempo and will have changing lights depending on the note played
//Inside the blue, green, and red code, there will will notes where the light will change
void randomSounds ()
{
      char notes[] = {'p','o','i','u','y','t','c','r','d','e','f','w','g','a','z','b','C','q','D','E','F','l','G','A','k','B'};
      int tempo = 200;
      int i = random (0,25);
      
      analogWrite(redPin,random(0,255));
      analogWrite(greenPin,random(0,255)); 
      analogWrite(bluePin,random(0,255));
      playNote(notes[i], tempo);
}

//sad music Still Alive - Valve
void blue (int i)
{
numIncrements = 29; // the number of notes
char notes[] = "GlEEl aGlEElDEa aElGEqDEaal "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 2, 4, 1, 1, 1, 1, 2, 3, 2, 1, 2, 1, 1, 3, 1, 3, 1, 2, 3, 2, 1, 2, 2, 4 };//the beats of each note above
int tempo = 200;//speed of the song

//powermod is inputted from the variable resistor
powerMod=analogRead(0)/1024.0;

    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
      analogWrite(redPin,255);
      analogWrite(greenPin,255); 
      analogWrite(bluePin,255-255*powerMod);
    } 
    else if (notes[i] == 'l')
    {
      analogWrite(redPin,255-255*powerMod);
      analogWrite(greenPin,255); 
      analogWrite(bluePin,255-255*powerMod);
      playNote(notes[i], beats[i] * tempo);
    }
    
    else {
      analogWrite(redPin,255);
      analogWrite(greenPin,255); 
      analogWrite(bluePin,255-255*powerMod);
      playNote(notes[i], beats[i] * tempo);
    }
   
    // pause between notes
    delay(tempo / 2); 
}

//happy music - Rudoplh the Red Nosed Reindeer
void green (int i)
{
numIncrements = 33; // the number of notes
char notes[] = "gageCag gagagCb fgfdbag gagagae "; // a space represents a rest
int beats[] = { 1, 2, 1, 2, 1, 1, 3, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 2, 3};
int tempo = 200;

powerMod=analogRead(0)/1024.0;
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
      analogWrite(redPin,255-255*powerMod);
      analogWrite(greenPin,255); 
      analogWrite(bluePin,255);
    } 
    else if (notes[i] == 'g')
    {
      analogWrite(redPin,255);
      analogWrite(greenPin,255-255*powerMod); 
      analogWrite(bluePin,255);
      playNote(notes[i], beats[i] * tempo);
    }
    else {
      analogWrite(redPin,255-255*powerMod);
      analogWrite(greenPin,255); 
      analogWrite(bluePin,255);
      playNote(notes[i], beats[i] * tempo);
    }
    
    // pause between notes
    delay(tempo / 2); 
  }

//Action music - Mission Impossible Theme Song
void red (int i)
{
numIncrements = 33; // the number of notes
char notes[] = "i iyci ipoi iyci ipozgd zgr zgc tc "; // a space represents a rest
int beats[] = {2,1,3,2,2,2,1,3,2,2,2,1,3,2,2,2,1,3,2,2,1,1,5,2,1,1,5,2,1,1,5,2,1,1,2};
int tempo = 150;

powerMod=analogRead(0)/1024.0;

    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
      analogWrite(redPin,255);
      analogWrite(greenPin,255-255*powerMod); 
      analogWrite(bluePin,255);  
    } 
    else if (notes[i] == 'i' || notes[i] == 'z'){
      playNote(notes[i], beats[i] * tempo);
      analogWrite(redPin,255-255*powerMod);
      analogWrite(greenPin,255-255*powerMod); 
      analogWrite(bluePin,255-255*powerMod);  
    }
    
    else {
      playNote(notes[i], beats[i] * tempo);
      analogWrite(redPin,255);
      analogWrite(greenPin,255-255*powerMod); 
      analogWrite(bluePin,255);  
    }
    
    // pause between notes
    delay(tempo / 2); 
  

}

//This method will output the tone on the piezo
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(piezo1Pin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(piezo1Pin, LOW);
    delayMicroseconds(tone);
  }
}

//This method will play the note
void playNote(char note, int duration) {
  
  char names[] = {'p','o','i','u','y','t','c','r','d','e','f','w','g','a','z','b','C','q','D','E','F','l','G','A','k','B'};
  int numnotes = 26;
  int tones[] = {2864,2702,2551,2273,2146,2024,1915,1805,1700,1519,1432,1351,1275,1136,1073,1014,956,919,850,760,716,676,638,568,536,507};

  
  // play the tone corresponding to the note name
  for (int i = 0; i < numIncrements; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

//Method controls servo motor (opens and closes box door)
void moveDoor(int &doorPos,int target,int delayTime){

  //If motor is moving to a greater position
 if (target>doorPos){
  //Errorhandling, motor only goes up to 180
  if (target>180)
    target=180;
  
  //Loop gradually brings motor to target position 
  for(doorPos=doorPos; doorPos < target; doorPos += 1){ 
    doorMan.write(doorPos); // tell servo to go to position in variable 'doorPos'
    delay(delayTime); // waits selected time for the servo to reach the position
  }
 }
 
 //If motor is moving to a smaller position 
 else if (target<doorPos){
  //Errorhandling, motor has minimum position of 0
  if (target<0)
   target=0;
  
  //Loop gradually brings motor to target position 
  for (doorPos=doorPos;doorPos>target;doorPos-=1){
   doorMan.write(doorPos); //tells servo to go to position in variable 'doorPos'
   delay(delayTime); //waits prescribd time for the servo to reach the position 
  } 
 }
}




