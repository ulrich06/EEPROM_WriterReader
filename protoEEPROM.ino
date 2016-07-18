#include <EEPROM.h>
#define EEPROM_SIZE 512
#define TEMP 0
#define VCC 1
#define MAX_BUFFER 10

typedef struct Smartcampus
{
  int n;
  float v;
  long t;
} Smartcampus;

const int ADD_BUFFER = 0;
const int ADD_SYNC = ADD_BUFFER + sizeof(long);
const int ADD_SAMPLING = ADD_SYNC + sizeof(long);
const int ADD_BEGIN_DATA = ADD_SAMPLING + sizeof(int);
const int SIZE_UNIT_DATA = sizeof(int) + sizeof(float) + sizeof(long);

void reset();

void setup() {
  EEPROM.begin(EEPROM_SIZE);
  Serial.begin(115200);
  reset();
  Serial.println("Done");
  
  
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

void flushBuffer() {
  for (int i = ADD_BEGIN_DATA; i < EEPROM_SIZE; i ++){
    EEPROM.write(i, 0xFF);
  }
  setBufferSize(0);
}

void print_eeprom() {
  for (int i = 0; i < EEPROM_SIZE; i++){
    Serial.print(EEPROM.read(i), HEX);
    Serial.print(" ");
  }
  Serial.println();
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

void setSync(long time){
  EEPROM.put(ADD_SYNC, time);
  EEPROM.commit();
}

void setSampling(long time){
  EEPROM.put(ADD_SAMPLING, time);
  EEPROM.commit();
}

int getBufferSize(){
  int i = 0;
  EEPROM.get(ADD_BUFFER, i);
  if (i == -1) return 0; else return i;
}

long getSync(){
  long i = 0L;
  EEPROM.get(ADD_SYNC, i);
  if (i == -1) return 0L; else return i;
}

long getSampling(){
  long i = 0L;
  EEPROM.get(ADD_SAMPLING, i);
  if (i == -1) return 0L; else return i;
}

int isBufferFull(){
 return getBufferSize() >= MAX_BUFFER;
}

int isBufferEmpty(){
  return getBufferSize() == 0;
}

Smartcampus * getBuffer(){
  const int size = getBufferSize();
  Smartcampus dataArray[size];
  memset(dataArray, 0, sizeof(dataArray));
  for (int i = ADD_BEGIN_DATA; i < pointerData(); i = i + SIZE_UNIT_DATA){
     int id = 0; float v = 0.0; long t = 0L;
     EEPROM.get(i,  id);
     EEPROM.get(i + sizeof(int), v);
     EEPROM.get(i + sizeof(int) + sizeof(float), t);
     Serial.print("R- id"); Serial.println(id);
     Serial.print("R- v"); Serial.println(v);
     Serial.print("R- t"); Serial.println(t);
     Serial.print("Registering in: "); Serial.print(i/SIZE_UNIT_DATA - 1); Serial.print(" i="); Serial.println(i);
     
     dataArray[i/SIZE_UNIT_DATA - 1].n = id;
     dataArray[i/SIZE_UNIT_DATA - 1].v = v;
     dataArray[i/SIZE_UNIT_DATA - 1].t = t;
  }
  return dataArray;
}


