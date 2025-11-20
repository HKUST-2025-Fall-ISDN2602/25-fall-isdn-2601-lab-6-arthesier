/*Change all the ? in the code, add code in ???, modify getPosition() */

//L298N Driver Pin 

int A_data;
int B_data;

#define MOTOR_ENA  14 // Replace the ? with the GPIO pin you selected to connect ENA
#define MOTOR_IN1 32  // Replace the ? with the GPIO pin you selected to connect IN2
#define MOTOR_IN2 33  // Replace the ? with the GPIO pin you selected to connect IN2

//Encoder Pin 
#define ENCODER_PINA 13 // Replace the ? with the GPIO pin you selected to connect encoder A
#define ENCODER_PINB 27 // Replace the ? with the GPIO pin you selected to connect encoder B

//Encoder Counter
volatile long encoderCount = 0; 
volatile double position=0; 

// Serial Monitor command for rotation direction
String command;


// interruppt
void IRAM_ATTR encoderInterrupt() {
  if (digitalRead(ENCODER_PINA) != digitalRead(ENCODER_PINB)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

//Serial Display Function 
void getState(){
  Serial.print("Count: ");
  Serial.println(encoderCount);
  Serial.print("Position: ");
  Serial.println(getPosition());
  delay(50);
}

// To get the current position
/*Modify this function*/ 
double getPosition() {
  // Calculate the current position based on encoder count
  position = float(encoderCount)*360.0/920; // Replace 1000.0 with the actual counts per revolution

  if (position<0)
  {position = position + 360; // Ensure position is positive
  }
  
  return position;
}

void setup() {
  
/* pin mode for pins connected with L298N driver  */
  pinMode(MOTOR_ENA, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);


// encoder A pin mode for interrupt
  pinMode(ENCODER_PINA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINA), encoderInterrupt, CHANGE);

/*encoder B pin mode */   
  pinMode(ENCODER_PINB, INPUT_PULLUP);

/* set up baud rate  */
  Serial.begin(115200); //Define baud


}

void loop() {
 if (Serial.available() > 0) {
        command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any leading or trailing whitespace
        if (command == "F") {
             /*Forward rotation direction*/
             digitalWrite(MOTOR_IN1, 1);
             digitalWrite(MOTOR_IN2, 0); 
             /*Set a speed for your motor*/
            analogWrite(MOTOR_ENA, 128);

        } 
        else if (command == "B") {
             /*Backward rotation direction*/
             digitalWrite(MOTOR_IN1, 0);
             digitalWrite(MOTOR_IN2, 1); 
             /*Set a speed for your motor*/
            analogWrite(MOTOR_ENA, 128);

        }
        } 

  //print the encoderCount and Position  
  getState(); 

   //Reset encoder count
  if (position > 360 || position < 0) {
    encoderCount = 0;
  }



}
