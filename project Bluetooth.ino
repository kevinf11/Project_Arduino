unsigned long signal_len,t1,t2;//Variables for the time a button is pressed
int inputPin = 2;//Push Button Input Pin
int ledPin = 4;//Output pin for the led
String code = "";//Variable that stores an alphabet

void setup() {
  //The serial connection and the input and output pins are configured
  Serial.begin(9600);//Frequency
  pinMode(inputPin, INPUT_PULLUP);//Receive the entry on pin 2
  pinMode(ledPin, OUTPUT);//Output pin
}

void loop()
{
NextDotDash:
  while (digitalRead(inputPin) == HIGH) {}
  t1 = millis();//Time to press the button
  digitalWrite(ledPin, HIGH);//LED on while pressing the button
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();//Time the button is not pressed
  digitalWrite(ledPin, LOW);//LED off when the button is not pressed
  signal_len = t2 - t1;//Total time the button is pressed
  if (signal_len > 50)//Take into account the anti-rebound button
  {
    code += readio();//Function to read point or script
  }
  while ((millis() - t2) < 500)//If the time between the point or script entered is greater than 0.5 seconds, the loop ends and receives the following alphabet
  {     
    if (digitalRead(inputPin) == LOW)
    {
      goto NextDotDash;
    }
  }
  convertor();//Function to decipher the code in the alphabet
}

char readio()
{
  if (signal_len < 600 && signal_len > 50)
  {
    return '.';//If the button is pressed less than 0.6 seconds, it will be a point
  }
  else if (signal_len > 600)
  {
    return '-';//If the button is pressed more than 0.6 seconds, it will be a script
  }
}

//Function to convert dots and dashes into alphabet
void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
                            }; //Letter combinations
  int i = 0;
  if (code == ".-.-.-")//If the sequence is the previous one, it will print a dot on the screen
  {
    Serial.print(".");
  }
  else if (code == "-.-.-.")//If the sequence is the previous one, it will print a space on the screen
  {
    Serial.print(" ");
  }
  else
  {
    while (letters[i] != "E") //Cycle to compare arrangement letters
    {
      if (letters[i] == code)//Check the position in variable "letters" and compare it with the alphabet called "code", while the variable "i" advances
      {
        Serial.print(char('A' + i));//Go through the alphabet that we all know from "A" to "Z" with the variable "i"
        break; //As soon as he finds the letter, he leaves the cycle
      }
      i++;
    }
    if (letters[i] == "E")//If the alphabet entered is none of the above combinations, then it will be "E"
    {
      Serial.println(" ");//In this case the "E" will be ENTER or line break
    }
  }
  code = "";//We reset the alphabet waiting for a new character to be entered
}
