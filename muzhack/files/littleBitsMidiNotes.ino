#define NOTE_PIN 5
#define NOTE_TRIGGER_PIN 4
#define CC_PIN 6
#define CLOCK_PIN 3
#define SPEEDFACTOR 64
#define MIN_NOTE 24
#define MAX_NOTE MIN_NOTE+61
#define NOTES_BUFFER 4

unsigned long count = 0;

const byte START = 250;
const byte STOP = 252;
const byte TICK = 248;
const byte CONTINUE = 251;

const byte NOTEON = 0x09;
const byte NOTEOFF = 0x08;
const byte CC = 0x0B;
const byte CLOCK = 0xF;

unsigned short notePointer = 0;
byte notes[NOTES_BUFFER];

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial || millis() > 3000) {
    ; // wait for serial port to connect, or 3 seconds pass
  }

  // put your setup code here, to run once:
  pinMode(NOTE_PIN, OUTPUT);
  digitalWrite(NOTE_PIN, LOW);
  pinMode(NOTE_TRIGGER_PIN, OUTPUT);
  digitalWrite(NOTE_TRIGGER_PIN, LOW);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(CLOCK_PIN, HIGH);
  
  for(int i = 0; i > NOTES_BUFFER; i++)
    notes[i] = 0;
}

void loop() {
  while(MIDIUSB.available() > 0) {
    MIDIEvent e = MIDIUSB.read();

    if(e.type == NOTEON){
      //Note to analog value
      //min note = 24
      if(e.m2 >= MIN_NOTE && e.m2 <= MAX_NOTE){
        // remember which note we're playing
        addNote(e.m2);
        outputNote();
        //send a trigger pulse
        digitalWrite(NOTE_TRIGGER_PIN, HIGH);
      }
    }
    if(e.type == NOTEOFF){
      removeNote(e.m2);
      outputNote();
      digitalWrite(NOTE_TRIGGER_PIN, LOW);  
    }
    if(e.type == CC && e.m2 == 1){ 
      // cc 1
      analogWrite(CC_PIN, e.m3 * 2);
    }
    // Clock pulses
    if(e.type == CLOCK){
      switch(e.m1){
      case TICK:
        {
          if((count % 24) == 0){
            digitalWrite(CLOCK_PIN, LOW);
            delay(1);
            digitalWrite(CLOCK_PIN, HIGH);
          }
          count++;
          break;
        }
      case START:
        {
          count = 0;
        }
      case STOP:
        {
          //ssssh
          digitalWrite(CLOCK_PIN, LOW);
        }
      }
    }
  }//end while midi available
}//end loop

void outputNote(){
  //stretch the note number across the correct range
  byte note = (notePointer - 1) % NOTES_BUFFER;
  byte analogVal;
  if( note == 0 ){
    analogVal = 0;
  } 
  else {
    analogVal = map(notes[note], MIN_NOTE, MAX_NOTE, 0, 255);
  }
  Serial.print(note);
  Serial.print(" : ");
  Serial.println(analogVal);
  //output as a voltage
  analogWrite(NOTE_PIN, analogVal);
}

void addNote(byte note){
  // just in case
  removeNote(note);
  notes[notePointer] = note;
  notePointer = (notePointer + 1) % NOTES_BUFFER;
}

void removeNote(byte note){
  boolean found = false;
  for(int i = 0; i < NOTES_BUFFER; i++){
    int index = (i + notePointer) % NOTES_BUFFER;
    if(notes[index] == note){
      found = true;
    }
    if(found){
      notes[index] = notes[(index + 1) % NOTES_BUFFER];
    }
  }
  if(found){
    notePointer = (notePointer - 1) % NOTES_BUFFER;
    notes[notePointer] = 0;
  }
}


void printNotes(){
  Serial.print("Notes[\t");
  for(int i = 0; i < NOTES_BUFFER; i++){
    Serial.print(notes[i]);
    if(i == notePointer){
      Serial.print("*");
    }
    Serial.print("\t");
  }
  Serial.println("]");
  Serial.println(notePointer);
}





