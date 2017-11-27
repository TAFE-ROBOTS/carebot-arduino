/*
  This code is for the 2 motors and 16x16 LED control
 */

//Requires LinkedList library
#include <LinkedList.h>
#include <LedMatrixObject.h>

// Current Version used to inform raspberryPi so it can choose when to update Arduino
String version = "1";

// Set variables
LedMatrixObject *led = new LedMatrixObject(36, 34, 32, 30, 28, 26, 24, 22);
int currentIcon = 0; //0 = Blank screen
String ioBuffer = ""; // a string to hold incomplete incoming data over serial interface
//Consts for motor
int RIGHT_SPEED = 9;
int LEFT_SPEED = 10;
int RIGHT_DIR_A = 8;
int RIGHT_DIR_B = 11;
int LEFT_DIR_A = 12;
int LEFT_DIR_B = 13;


//EVENT QUEUE BELOW THIS LINE
//===========================
//Define the class to place on the queue
class Event {
  public:
    long timestamp;
    int param;
    void(*func)(int);
};

//Define the queue
LinkedList<Event*> eventQueue = LinkedList<Event*>();

//Define the shape of the add function
void addEvent(void (*)(int), int param, long wait);

void setup() {
  Serial.begin(9600);
  start();
}

//Sorting function
int timeIndexed(Event *&eventA, Event *&eventB) {
  return eventA->timestamp - eventB->timestamp;
}

//Add a single event
void addEvent(void (*function)(int), int param, long wait) {
  Event *event = new Event();
  event->timestamp = wait+millis();
  event->func = function;
  event->param = param;
  eventQueue.add(event);
  eventQueue.sort(timeIndexed);
}

void loop() {
  //Read the top event and fire it
  Event *topEvent = eventQueue.get(0);
  if (topEvent->timestamp < millis()) {
    Event *eventToPlay = eventQueue.shift();
    eventToPlay->func(eventToPlay->param);
    delete eventToPlay;
  } else {
    Serial.println("waiting");
  }
  //Render icons
  render();
}

//===========================
//EVENT QUEUE ABOVE THIS LINE


//Catalog of icons to render

unsigned char Cross[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 1

unsigned char Question[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
    {1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 2

unsigned char Exclamation[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 3

unsigned char Yes[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1},
    {1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1},
    {1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1},
    {1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 4

unsigned char No[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1},
    {1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
    {1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1},
    {1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 5

unsigned char Heart[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 6

unsigned char Beat[16][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
    {1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; // Index 7

void render() {
  switch currentIcon {
    case 0:
      //Do nothing
      break;
    case 1:
      led->setScene(Cross);
      led->draw();
      break;
    case 2:
      led->setScene(Question);
      led->draw();
      break;
    case 3:
      led->setScene(Exclamation);
      led->draw();
      break;
    case 4:
      led->setScene(Yes);
      led->draw();
      break;
    case 5:
      led->setScene(No);
      led->draw();
      break;
    case 6:
      led->setScene(Heart);
      led->draw();
      break;
    case 7:
      led->setScene(Beat);
      led->draw();
      break;
  }
}

void start() {
  heartbeat(0);
}

void doCommand(char command) {
  switch (command) {
    case 'v':
      Serial.println(version);
    case '0':
      analogWrite(LEFT_SPEED, 0);
      analogWrite(RIGHT_SPEED, 0);
      break;
    case '1':
      analogWrite(LEFT_SPEED, 20);
      analogWrite(RIGHT_SPEED, 20);
      break;
    case '2':
      analogWrite(LEFT_SPEED, 60);
      analogWrite(RIGHT_SPEED, 60);
      break;
    case '3':
      analogWrite(LEFT_SPEED, 100);
      analogWrite(RIGHT_SPEED, 100);
      break;
    case 's':
      digitalWrite(LEFT_DIR_A, LOW);
      digitalWrite(LEFT_DIR_B, LOW);
      digitalWrite(RIGHT_DIR_A, LOW);
      digitalWrite(RIGHT_DIR_B, LOW);
      break;
    case 'l':
      digitalWrite(LEFT_DIR_A, HIGH);
      digitalWrite(LEFT_DIR_B, LOW);
      break;
    case 'L':
      digitalWrite(LEFT_DIR_A, LOW);
      digitalWrite(LEFT_DIR_B, HIGH);
      break;
    case 'r':
      digitalWrite(RIGHT_DIR_A, HIGH);
      digitalWrite(RIGHT_DIR_B, LOW);
      break;
    case 'R':
      digitalWrite(RIGHT_DIR_A, LOW);
      digitalWrite(RIGHT_DIR_B, HIGH);
      break;
    case '-':
      currentIcon = 0;
      break;
    case '+':
      currentIcon = 1;
      break;
    case '?':
      currentIcon = 2;
      break;
    case '!':
      currentIcon = 3;
      break;
    case 'y':
      currentIcon = 4;
      break;
    case 'n':
      currentIcon = 5;
      break;
    case 'h':
      heartbeat(0);
      break;
  }
}

void heartbeat(int phase) {
  switch phase {
    case 1:
      currentIcon = 7;
      addEvent(heartbeat, 2, 200);
      break;
    case 2:
      currentIcon = 6;
      addEvent(heartbeat, 3, 200);
      break;
    case 3:
      currentIcon = 7;
      addEvent(heartbeat, 4, 200);
      break;
    case 4:
      currentIcon = 6;
      addEvent(heartbeat, 1, 600);
      break;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char in = (char)Serial.read();
    doCommand(in);
  }
}
