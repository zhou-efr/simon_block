#include <Arduino.h>

// Game variables
const int starting_sequence_length = 1;
int sequence_length = starting_sequence_length, *sequence = NULL, *sequence_user = NULL;
// Serial communication variables

// Function prototypes
void logArray(int *array, int length);
void updateSequence();
char recvWithEndMarker();
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

  for (int i = 0; i < sequence_length; i++){
    Serial.print("Enter the number at position ");
    Serial.print(i);
    Serial.print(" in the sequence: ");
    sequence_user[i] = getUserInput();
  }
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

  sequence = (int*) malloc(sequence_length * sizeof(int));
  sequence_user = (int*) malloc(sequence_length * sizeof(int));

  for (int i = 0; i < sequence_length-1; i++){
    sequence[i] = temp_sequence[i];
    sequence_user[i] = -1;
  }

  sequence[sequence_length-1] = random(4);
  sequence_user[sequence_length-1] = -1;
  
  free(temp_sequence);
}
int getUserInput(){
  const byte numChars = 4;
  char receivedChars[numChars];
  boolean newData = false;
  int char_count = 0;

  while(!newData){
    char temp_char = recvWithEndMarker();

    if (temp_char != '\0'){
      receivedChars[char_count] = temp_char;
      char_count++;
    }
    if (char_count > numChars){
      char_count--;
    }
    if (temp_char == '\0'){
      newData = true;
    }
  }

  int input = atoi(receivedChars)-1;

  if (input < 0 || input > 3){
    Serial.println("Invalid input. Please enter a number between 1 and 4.");
    return getUserInput();
  }
  return input;
}
char recvWithEndMarker() {
  char endMarker = '\n';
  char rc;
  while (Serial.available() == 0);
  if (Serial.available() > 0) {
    rc = Serial.read();
    if (rc != endMarker) {
      return rc;
    }
    else {
      return '\0';  
    }
  }
  return '\0';
}
void endTurn(){
  bool correct = true;

  for (int i = 0; i < sequence_length; i++){
    if (sequence[i] != sequence_user[i]){
      correct = false;
    }
  }

  if (!correct){
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
    int *temp_sequence = sequence_user;
    sequence_user = NULL;
    free(temp_sequence);
  }
}
