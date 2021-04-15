#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 9, 8, 3, 2, A1);

//Declaring and Initializing variables
int ledRock = 7; //specifies which pin each are found in
int ledPaper = 6;
int ledScissor = 5;
int btnRock = 12;
int btnPaper = 11;
int btnScissor = 10;
int soundPin = 4;
int userScore = 0;
int compScore = 0;
int userChoice = 10;
char txt = "N";

byte leds = 0; 
//Arrays with sound frequencies
int winSound[] = {523.25, 698.46};
int loseSound[] = {196, 185, 174};
int newRoundSound[] = {349, 392, 440, 523, 440, 523};
int winGameSound[] = {262, 262, 392, 330, 262, 245, 220, 349, 349, 330, 262, 294, 262};
int loseGameSound[] = {262, 245, 233, 220, 208, 196, 185, 175, 165, 156, 147, 139, 131};
int tmp;

void setup() { //Runs once only when the program first starts
  Serial.begin(9600); //Initializes Serial
  while(!Serial); //Waits until the Serial is ready to go before continuing
  pinMode(ledRock, OUTPUT); //These lines specify what reach of the pins are used for; output or input
  pinMode(ledPaper, OUTPUT);
  pinMode(ledScissor, OUTPUT);
  pinMode(btnRock, INPUT_PULLUP);  
  pinMode(btnPaper, INPUT_PULLUP);
  pinMode(btnScissor, INPUT_PULLUP);
  lcd.begin(16, 2); //Initializes LCD: 16 columns, 2 rows
  Serial.println("======================WELCOME TO ROCK, PAPER, SCISSORS!======================");
  Serial.println("Please type 'R' - rock, 'P' - paper, or 'S' - scissors, to play.");
  lcd.setCursor(0, 0); //Starting index of where the LCD starts to display text (column number, then row)
  lcd.print("WELCOME TO ROCK");
  lcd.setCursor(0, 1);
  lcd.print("PAPER, SCISSORS!");
  delay(3500);
  newRound(); //calls the newRound() function
}

void loop() { //Continuously runs over and over again
  int tmp = random(3); //generates a random number between 0-2
  readInput(); //calls readInput() function
  if (userChoice == 0){ //0 == rock, 1  == paper, 2 == scissors
    if (tmp == 0){ //tie 
      digitalWrite(ledRock, HIGH); //turns that LED on
      Serial.println("You threw ROCK! Arduino threw ROCK!");
      tieRound(); //calls tieRound() function
    } else if (tmp == 1) { //lose
      digitalWrite(ledPaper, HIGH);
      Serial.println("You threw ROCK! Arduino threw PAPER!");
      loseRound(); //calls loseRound() function
    } else if (tmp == 2) { //win
      digitalWrite(ledScissor, HIGH);
      Serial.println("You threw ROCK! Arduino threw SCISSORS!");
      winRound(); //calls winRound() function
    } 
    newRound(); //calls newRound() function
  } else if (userChoice == 1){
    if (tmp == 0){ //win
      digitalWrite(ledRock, HIGH);
      Serial.println("You threw PAPER! Arduino threw ROCK!");
      winRound();
    } else if (tmp == 1) { //tie
      digitalWrite(ledPaper, HIGH);
      Serial.println("You threw PAPER! Arduino threw PAPER!");
      tieRound();
    } else if (tmp == 2) { //lose
      digitalWrite(ledScissor, HIGH);
      Serial.println("You threw PAPER! Arduino threw SCISSORS!");
      loseRound();
    } 
    newRound();
  } else if (userChoice == 2){
    if (tmp == 0){ //lose
      digitalWrite(ledRock, HIGH);
      Serial.println("You threw SCISSORS! Arduino threw ROCK!");
      loseRound();
    } else if (tmp == 1) { //win
      digitalWrite(ledPaper, HIGH);
      Serial.println("You threw SCISSORS! Arduino threw PAPER!");
      winRound();
    } else if (tmp == 2) { //tie
      digitalWrite(ledScissor, HIGH);
      Serial.println("You threw SCISSORS! Arduino threw SCISSORS!");
      tieRound();
    } 
    newRound();
  }
  delay(750);
  if (userScore == 5){ //If the user gets to 5 points
    clearLCD(); //calls resetLCD() function
    lcd.print("CONGRATS! U WON!");
    Serial.println("CONGRATS! YOU WON THE GAME!");
    winGameSong();
    resetGame(); //calls resetGame() function
  } else if (compScore == 5) { //If the comp gets to 5 points
    clearLCD();
    lcd.print("CONGRATS! U LOST!"); 
    Serial.println("SORRY! YOU LOST!"); 
    loseGameSong();
    resetGame();    
  }
}

void loseRound(){ //Called upon when user loses a round
  clearLCD(); //Calls clearLCD() function
  lcd.print("YOU LOST!");
  Serial.println("YOU LOST!");
  compScore++; //adds one to the original compScore value
  loseSong(); //calls loseSong() function
}
 
void winRound(){ //Called upon when user wins a round
  clearLCD(); //Calls clearLCD() function
  lcd.print("YOU WON!");
  Serial.println("YOU WON!");
  userScore++; //adds one to the original userScore value
  winSong(); //calls winSong() function
}

void tieRound(){ //Called upon when user ties with the comp that round
  clearLCD(); //Calls clearLCD() function
  lcd.print("YOU TIED!");
  Serial.println("YOU TIED!");
  delay(1500);
}

void updateSerial(){ //update display for the Serial Monitor for user purposes
  Serial.print("You Score: ");
  Serial.println(userScore); //prints this on the same line as before
  Serial.print("Arduino Score: ");
  Serial.println(compScore); //prints this on the same line as before
}

void readInput(){ //Obtains input from the user
  userChoice = 10; //sets userChoice value to something unused
  txt = 'x'; //sets txt value to something unused
  while (userScore <= 5){ //continuously loops until a value is received
    txt = Serial.read();
    if (digitalRead(btnRock) == LOW){ // if this button is pressed down, then:
      userChoice = 0; break; //breaks the loop and exits as soon as a value from user is received
    } else if (digitalRead(btnPaper) == LOW){
      userChoice = 1; break;
    } else if (digitalRead(btnScissor) == LOW){
      userChoice = 2; break;
    }
    if (txt == 'R'){  //is user typed something into Serial Monitor, it is processed here.
      userChoice = 0; break; //breaks the loop and exits as soon as a value from user is received
    } else if (txt == 'P'){
      userChoice = 1; break;
    } else if (txt == 'S'){
      userChoice = 2; break;
    }
  }
}

void resetGame(){ //resets the Arduino for a new game
  Serial.println("================================WELCOME TO ROCK, PAPER, SCISSORS!================================");
  Serial.println("Please type 'R' - rock, 'P' - paper, or 'S' - scissors, to play.");
  userScore = 0; //resets scores
  compScore = 0;
  clearLCD();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO ROCK");
  lcd.setCursor(0, 1);
  lcd.print("PAPER, SCISSORS!");
  delay(3500);
  newRound(); //calls newRound() function
}

void updateLCDScore(){ //Updates and changes the score on LCD
  lcd.setCursor(11, 1); //sets the cursor to appropriate location (bottom left corner)
  lcd.print("M");
  lcd.print(userScore);
  lcd.print(":A");
  lcd.print(compScore);
}

void newRound(){ //resets the round
  if (userScore == 5 || compScore == 5){ //if someone already won the game, skip this function
    return;
  }
  clearLCD(); //calls clearLCD() function
  updateLCDScore(); //calls updateLCDScore() function
  updateSerial(); //calls updateSerial() function
  digitalWrite(ledRock, LOW);
  digitalWrite(ledPaper, LOW);
  digitalWrite(ledScissor, LOW);
  delay(750);
  lcd.print("NEW ROUND");
  Serial.println("----------------------------------NEW ROUND----------------------------------");
  for (int i = 0; i < 3; i++) { //flashes the LEDs to signal a new round
    digitalWrite(ledRock, HIGH);
    digitalWrite(ledPaper, HIGH);
    digitalWrite(ledScissor, HIGH);
    delay(300);
    digitalWrite(ledRock, LOW);
    digitalWrite(ledPaper, LOW);
    digitalWrite(ledScissor, LOW);
    delay(300);
  }
  newRoundSong(); //calls newRoundSong() function
  delay(500);
  clearLCD(); //calls clearLCD() function
  lcd.setCursor(0, 0);
  lcd.print("THROW DOWN IN 3"); //counts down from 3 to signal when the user to throw down 
  Serial.println("THROW DOWN IN 3");
  digitalWrite(ledRock, HIGH); //turns on one LED at a time, following the countdown
  tone(soundPin, 65.41); 
  delay(500);
  noTone(soundPin);
  delay(500);
  clearLCD();
  lcd.print("THROW DOWN IN 2");
  Serial.println("THROW DOWN IN 2");
  digitalWrite(ledPaper, HIGH);
  tone(soundPin, 65.41);
  delay(500);
  noTone(soundPin);
  delay(500);
  clearLCD();
  lcd.print("THROW DOWN IN 1");
  Serial.println("THROW DOWN IN 1");
  digitalWrite(ledScissor, HIGH);
  tone(soundPin, 65.41);
  delay(500);
  noTone(soundPin);
  digitalWrite(ledRock, LOW); //turns all the LEDs off to signal that the user should throw down now, and prepares for the next round
  digitalWrite(ledPaper, LOW);
  digitalWrite(ledScissor, LOW);
  clearLCD(); //calls clearLCD() function
  lcd.print("THROW DOWN NOW!"); 
  Serial.println("THROW DOWN NOW!");
}

void winSong(){ //Plays a tune for when the user wins a round
  tone(soundPin, 349); //plays this frequency
  delay(300); //holds that frequency for 0.3 seconds
  tone(soundPin, 466); //plays this frequency
  delay(1000); //hold that frequency for 1 second
  noTone(soundPin); //silence
}

void clearLCD(){ //Clears the LCD for a new display, but keeps the score at the bottom left corner.
  lcd.clear();
  updateLCDScore(); //calls updateLCDScore() function
  lcd.setCursor(0, 0); //resets the cursor to most commonly used location
}

void loseSong(){ //For loop for when the user loses a round.
  for (int i = 0; i < 3; i++) { //three different frequencies in the losing tune
    tone(soundPin, loseSound[i]); //plays that frequency
    if (i == 0 || i == 1){ //when i equals any of these values, hold that tone for 1 second
      delay(1000);
    } else { //if i doesn't equal any of the above values, hold that tone for 1.75 seconds
      delay(1750);
    }
  }
  noTone(soundPin); //silence
}

void newRoundSong(){ //For loop for new round song.
  for (int i = 0; i < 6; i++) { //six different frequencies in the new round tune
    tone(soundPin, newRoundSound[i]); //plays that frequency
    if (i == 0 || i == 1 || i == 2 || i == 4) { //when i equals any of these values, hold tone for 0.2 seconds
      delay(200);
    } else { //when i doesn't equal any of those above values, hold tone for 0.4 seconds
      delay(400);
    }
  }
  noTone(soundPin); //silence
}

void winGameSong(){
  for (int i = 0; i < 13; i++){
    tone(soundPin, winGameSound[i]);
    if (i == 1 || i == 8){
      delay(150);
    } else if (i == 6 || i == 12){
      delay(1000);
    } else {
      delay(400);
    }
    noTone(soundPin);
    delay(50);
  }
  noTone(soundPin);
}

void loseGameSong(){
  for(int i = 0; i < 13; i++){
    tone(soundPin, loseGameSound[i]);
    delay(150);
  }
  noTone(soundPin);
}

