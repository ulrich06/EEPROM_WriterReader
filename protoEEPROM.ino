#include <EEPROM.h>
#define EEPROM_SIZE 512
#define TEMP 0
#define VCC 1
#define MAX_BUFFER 10


const int ADD_BUFFER = 0;
const int ADD_SYNC = ADD_BUFFER + sizeof(int);
const int ADD_SAMPLING = ADD_SYNC + sizeof(int);
const int ADD_BEGIN_DATA = ADD_SAMPLING + sizeof(int);
const int SIZE_UNIT_DATA = sizeof(int) + sizeof(float) + sizeof(long);

void reset();

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  delay(200);
}

void loop() {}

int pointerData() {
  return ADD_BEGIN_DATA + getBufferSize() * SIZE_UNIT_DATA;
}

void reset() {
  for (int i = 0; i < EEPROM_SIZE; i ++){
    EEPROM.write(i, 0xFF);
  }
}

void debug() {
  for (int i = 0; i < EEPROM_SIZE; i++){
    Serial.print(EEPROM.read(i), HEX);
    Serial.print(" ");
  }
}


void addData(int id, float value, long time){
  if (!isBufferFull()){
    int addr = pointerData();
    EEPROM.put(addr, id);
    EEPROM.put(addr + sizeof(int), value);
    EEPROM.put(addr + sizeof(int) + sizeof(float), time);
    EEPROM.commit();
    setBufferSize(getBufferSize() + 1);  
  }
}

void setBufferSize(int size){
  EEPROM.put(ADD_BUFFER, size);
  EEPROM.commit();
}

void setSyncPeriod(int period){
  EEPROM.put(ADD_SYNC, period);
  EEPROM.commit();
}

void setSamplingPeriod(int period){
  EEPROM.put(ADD_SAMPLING, period);
  EEPROM.commit();
}

int getBufferSize(){
  int i = 0;
  EEPROM.get(ADD_BUFFER, i);
  if (i == -1) return 0; else return i;
}

int getSyncPeriod(){
  int i = 0;
  EEPROM.get(ADD_SYNC, i);
  if (i == -1) return 0; else return i;
}

int getSamplingPeriod(){
  int i = 0;
  EEPROM.get(ADD_SAMPLING, i);
  if (i == -1) return 0; else return i;
}

int isBufferFull(){
 return getBufferSize() >= MAX_BUFFER;
}
