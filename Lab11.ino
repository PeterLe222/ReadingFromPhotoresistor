 /*
  * 
  * light on 60 62 
  * light off 122 124
  * 
  */
  
#define LIGHTSENSOR A5  // light sensor connects to pin A5
#define LED 2 // LED connects to pin 2

const byte ledPin[] = {7,8,9,10,11,12,13};  // pin 7-13 connect to 7-segment
const byte number[] = { // number display on 7-segment
                       0b1000000, // 0
                       0b1111001, // 1
                       0b0100100, // 2
                       0b0110000, // 3
                       0b0011001, // 4
                       0b0010010, // 5
                       0b0000010, // 6
                       0b1111000, // 7
                       0b0000000, // 8
                       0b0010000, // 9
                       0b1111111  // off
                      };
int MORSEUNIT;  // variable contains unit
int STARTTIME = 0;  // variable contain time of start blinking 
int ENDTIME = 0;  // variable contain time of end blinking
int NUMBERARRAY[4][5] = {{3,3,3,3,3},  // number 0
                       {1,3,3,3,3},  // number 1
                       {1,1,3,3,3},  // number 2
                       {1,1,1,3,3}};  // number 3
int RANDOMNUMBER; // variable contains random number
int SENSORSTATUS; // variable contains the reading of light sensor
int ENDBLINK[5]; //  array contains 1 for dot and 3 for dash 
int CHECKARRAY = 0;  // variable contains number of same element in array 
int AFTERCHECKARRAY;  // variable contains the print number

int blinkLED(int MORSEUNIT);  // function blink the LED and return
                              // the result of light sensor
int getBLINK(int *ARRAY); // function add the morse code to array

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));  // create random seed
  pinMode(LIGHTSENSOR,INPUT); // light sensor
  pinMode(LED,OUTPUT);  // LED
  /* pin 7-13 7-segment */
  pinMode(7,OUTPUT);  // A
  pinMode(8,OUTPUT);  // B
  pinMode(9,OUTPUT);  // C
  pinMode(10,OUTPUT); // D
  pinMode(11,OUTPUT); // E
  pinMode(12,OUTPUT); // F
  pinMode(13,OUTPUT); // G
  /* turn off the 7-segment indicator */
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(ledPin[i],bitRead(number[10],i));
  }
}

void loop() {
  getBlink(ENDBLINK);
  for (int i = 0; i < 4; i++)
  {
     for (int j = 0; j < 5; j++)
     {
       /* compare the result from light sensor and our morse array */
       if (NUMBERARRAY[i][j] == ENDBLINK[j])
       {
         CHECKARRAY++;
         // if 5 numbers in array all are same
         if (CHECKARRAY == 5)
         {
           AFTERCHECKARRAY = i;
         }
       }
       else
       {
         CHECKARRAY = 0;
       }
     }
  }
  /* display number on 7-segment */
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(ledPin[i],bitRead(number[AFTERCHECKARRAY],i));
  }
  delay(2000);
  /* turn off the 7-segment indicator */
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(ledPin[i],bitRead(number[10],i));
  }
}
int blinkLED(int BLINKUNIT){
  if (BLINKUNIT == 1)
  {
    STARTTIME = millis(); // store executive time at the begining 
    digitalWrite(LED,HIGH); // LED on
    SENSORSTATUS = analogRead(LIGHTSENSOR); // value sensor read ~ 60-62
    delay(1000);  // one unit
    digitalWrite(LED,LOW);  // LED off
    delay(1000);  // value sensor read ~ 122-124
    ENDTIME = millis(); // store executive time at the end
  }
  else
  {
    STARTTIME = millis(); 
    digitalWrite(LED,HIGH);
    SENSORSTATUS = analogRead(LIGHTSENSOR);
    delay(3000);
    digitalWrite(LED,LOW);
    delay(1000);
    ENDTIME = millis();
  }
   /* After delay 2 seconds(2 units) the time add 2000
    * Wo if end - start ~ 2000 for dot and ~ 4000 for dash     
    * When LED on, the photoresistor gives number ~ 60-62 
    */
  if (ENDTIME - STARTTIME < 3990 && ENDTIME - STARTTIME >= 1990 
  &&  SENSORSTATUS < 63 &&  SENSORSTATUS >= 60 )
  {
    return 1; // return 1 if dot
  }
  else
  {
    return 3; // return 3 if dash
  }      
}

int getBlink(int *ARRAY){
  RANDOMNUMBER = random(0,4); // create random number from 0 to 4
  Serial.print("random number= ");Serial.println(RANDOMNUMBER);
  for (int i = 0; i < 5; i++)
  {
    MORSEUNIT = NUMBERARRAY[RANDOMNUMBER][i]; // take the morse code from morse array
    Serial.print("morse= ");Serial.println(MORSEUNIT);
    // if dot
    if (MORSEUNIT == 1)
    {
      *(ARRAY+i) = blinkLED(MORSEUNIT); // add reading from light sensor to blank array 
    }
    // if dash
    else
    {
      *(ARRAY+i) = blinkLED(MORSEUNIT);
    }
  }
  return *ARRAY;
}

