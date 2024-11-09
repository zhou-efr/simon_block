#include <Arduino.h>

// Game variables
const int starting_sequence_length = 1;
int sequence_length = starting_sequence_length, *sequence = NULL, *sequence_user = NULL;
// Serial communication variables
const byte numChars = 32;
char receivedChars[numChars];
boolean newData = false;
int dataNumber = 0;   

// Function prototypes
void logArray(int *array, int length);
void updateSequence();
void recvWithEndMarker();
void showNewNumber();
int getUserInput();
void endTurn();

// Setup and loop
void setup() {
  Serial.begin(9600);
  Serial.println("Starting up...");	
  Serial.println("");
  Serial.println("      ___                       ___           ___           ___     ");
  Serial.println("     /\\__\\                     /\\  \\         /\\  \\         /\\  \\    ");
  Serial.println("    /:/ _/_       ___         |::\\  \\       /::\\  \\        \\:\\  \\   ");
  Serial.println("   /:/ /\\  \\     /\\__\\        |:|:\\  \\     /:/\\:\\  \\        \\:\\  \\  ");
  Serial.println("  /:/ /::\\  \\   /:/__/      __|:|\\:\\  \\   /:/  \\:\\  \\   _____\\:\\  \\ ");
  Serial.println(" /:/_/:/\\:\\__\\ /::\\  \\     /::::|_\\:\\__\\ /:/__/ \\:\\__\\ /::::::::\\__\\");
  Serial.println(" \\:\\/:/ /:/  / \\/\\:\\  \\__  \\:\\~~\\  \\/__/ \\:\\  \\ /:/  / \\:\\~~\\~~\\/__/");
  Serial.println("  \\::/ /:/  /   ~~\\:\\/\\__\\  \\:\\  \\        \\:\\  /:/  /   \\:\\  \\      ");
  Serial.println("   \\/_/:/  /       \\::/  /   \\:\\  \\        \\:\\/:/  /     \\:\\  \\     ");
  Serial.println("     /:/  /        /:/  /     \\:\\__\\        \\::/  /       \\:\\__\\    ");
  Serial.println("     \\/__/         \\/__/       \\/__/         \\/__/         \\/__/       ");
  Serial.println("");
}

void loop() {
  updateSequence();
  logArray(sequence, sequence_length);
  logArray(sequence_user, sequence_length);

  sequence_user[sequence_length-1] = getUserInput();
  logArray(sequence_user, sequence_length);

  endTurn();
}

// Function definitions
void logArray(int *array, int length){
  Serial.print("Array: ");
  for (int i = 0; i < length; i++){
    Serial.print(array[i]);
    Serial.print(" ");
  }
  Serial.println("");
}
void updateSequence(){
  if (sequence == NULL){
    sequence = (int*) malloc(sequence_length * sizeof(int));
    sequence_user = (int*) malloc(sequence_length * sizeof(int));
    for (int i = 0; i < sequence_length; i++){
      sequence[i] = random(4);
      sequence_user[i] = -1;
    }

    return;
  }

  sequence_length++;

  int *temp_sequence = sequence;
  int *temp_sequence_user = sequence_user;

  sequence = (int*) malloc(sequence_length * sizeof(int));
  sequence_user = (int*) malloc(sequence_length * sizeof(int));

  for (int i = 0; i < sequence_length-1; i++){
    sequence[i] = temp_sequence[i];
    sequence_user[i] = temp_sequence_user[i];
  }

  sequence[sequence_length-1] = random(4);
  sequence_user[sequence_length-1] = -1;
  
  free(temp_sequence);
  free(temp_sequence_user);
}
int getUserInput(){
  while(!newData){
    recvWithEndMarker();
  }
  showNewNumber();

  int input = dataNumber-1;

  if (input < 0 || input > 3){
    Serial.println("Invalid input. Please enter a number between 1 and 4.");
    return getUserInput();
  }
  return input;
}
void recvWithEndMarker() {
    // Serial.println("User input: ");
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (Serial.available() == 0);
    // Serial.print("Serial.available() = ");
    // Serial.println(Serial.available());
    if (Serial.available() > 0) {
        rc = Serial.read();
        // Serial.print("rc = ");
        // Serial.println(rc);
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}
void showNewNumber() {
    if (newData == true) {
        dataNumber = 0;             // new for this version
        dataNumber = atoi(receivedChars);   // new for this version
        Serial.print("This just in ... ");
        Serial.println(receivedChars);
        Serial.print("Data as Number ... ");    // new for this version
        Serial.println(dataNumber);     // new for this version
        newData = false;
    }
}
void endTurn(){
  if (sequence[sequence_length-1] != sequence_user[sequence_length-1]){
    Serial.println("You lose!");
    Serial.print("The correct sequence was: ");
    logArray(sequence, sequence_length);
    Serial.print("Your sequence was: ");
    logArray(sequence_user, sequence_length);
    Serial.println("Game over!");
    sequence_length = starting_sequence_length;
    free(sequence);
    free(sequence_user);
    sequence = NULL;
    sequence_user = NULL;
  }
  // else if (sequence_length == 10){
  //   Serial.println("You win!");
  //   Serial.print("The correct sequence was: ");
  //   logArray(sequence, sequence_length);
  //   Serial.print("Your sequence was: ");
  //   logArray(sequence_user, sequence_length);
  //   Serial.println("Game over!");
  //   sequence_length = starting_sequence_length;
  //   free(sequence);
  //   free(sequence_user);
  //   sequence = NULL;
  //   sequence_user = NULL;
  // }
  else{
    Serial.println("Correct! Next round.");
  }
}
