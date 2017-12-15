#define UP_MOTOR 6
#define DOWN_MOTOR 7

#define LED 2

#define DELAYTIME 280
#define DELAYTIME2 40
#define OFFTIME 9680

float dustVal=0;
float dustDensity = 0;

int send_data;

boolean autoMode;

String myString=""; //받는 문자열

void setup () {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(LED,OUTPUT);
  pinMode(UP_MOTOR, OUTPUT);
  pinMode(DOWN_MOTOR, OUTPUT);
  
  analogWrite(UP_MOTOR, 0);
  analogWrite(DOWN_MOTOR, 0);

  autoMode = false;
}
 
void loop () {
  while(Serial1.available())  //mySerial에 전송된 값이 있으면
  {
    char myChar = (char)Serial1.read();  //mySerial int 값을 char 형식으로 변환
    myString+=myChar;   //수신되는 문자를 myString에 모두 붙임 (1바이트씩 전송되는 것을 연결)
    delay(5);           //수신 문자열 끊김 방지
  }

  if(myString.equals("1")) {
    Serial.println("Level 1 mode...");
    digitalWrite(DOWN_MOTOR, HIGH);
  } else if(myString.equals("2")) {
    Serial.println("Level 2 mode...");
    digitalWrite(DOWN_MOTOR, HIGH);
    digitalWrite(UP_MOTOR, HIGH);
  } else if(myString.equals("3")) {
    Serial.println("Turn off all fan...");
    digitalWrite(DOWN_MOTOR, LOW);
    digitalWrite(UP_MOTOR, LOW);
  } else if(myString.equals("a")) {
    Serial.println("Auto mode...");
    autoMode = true;
  } else if(myString.equals("b")) {
    Serial.println("Passive mode...");
    autoMode = false;
  }
  
  if(autoMode) {
    digitalWrite(LED,LOW);
    delayMicroseconds(DELAYTIME);

    dustVal=analogRead(A0);
    delayMicroseconds(DELAYTIME2);

    digitalWrite(LED,HIGH);
    delayMicroseconds(OFFTIME);

    dustDensity = 0.17 * (dustVal * 0.0049) - 0.01;
    Serial.print("Dust density(mg/m3) = ");
    Serial.println(dustDensity);

    if(dustDensity >= 0.50) { digitalWrite(UP_MOTOR, 1); digitalWrite(DOWN_MOTOR, 1); delay(1000); analogWrite(UP_MOTOR, 255); analogWrite(DOWN_MOTOR, 255); }
    else if(dustDensity >= 0.40) { digitalWrite(UP_MOTOR, 1); digitalWrite(DOWN_MOTOR, 1); delay(1000); analogWrite(UP_MOTOR, 127); analogWrite(DOWN_MOTOR, 255); }
    else if(dustDensity >= 0.30) { digitalWrite(DOWN_MOTOR, 1); delay(1000); analogWrite(UP_MOTOR, 0); analogWrite(DOWN_MOTOR, 255); }
    else if(dustDensity >= 0.20) { digitalWrite(UP_MOTOR, 1); digitalWrite(DOWN_MOTOR, 1); delay(1000); analogWrite(UP_MOTOR, 127); analogWrite(DOWN_MOTOR, 127); }
    else if(dustDensity >= 0.10) { digitalWrite(DOWN_MOTOR, 1); delay(1000); analogWrite(UP_MOTOR, 0); analogWrite(DOWN_MOTOR, 127); }
    else { analogWrite(UP_MOTOR, 0); analogWrite(DOWN_MOTOR, 0); }

    send_data = (int)(dustDensity * 100);

    Serial1.write(send_data);
  }

  myString = "";  //버퍼 초기화
  delay(100);
}
