#define CLOCK 2
#define READ_WRITE 3
int last_clock_high = 0;
int last_clock_low = 0;
bool clock_high = false;
char buffer[256];
const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
int clock_rate = 500;
int clock_mode = 1;
int term = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  pinMode(CLOCK, OUTPUT);
  for (int n = 0; n < 16; n++) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n++) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(READ_WRITE, INPUT);
  last_clock_high = 0;
  last_clock_low = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (clock_mode == 1) {
    clock();
  }
  term = Serial.read();
  if (term > 0) { 
    clock_mode = !clock_mode;
    if (clock_mode) Serial.println("Clock starting.");
    else Serial.println("Clock stopped.");
    Serial.print("> ");
    Serial.println(term);
  }
  //delay(50);
}

void clock() {
  int now = millis();
  if (!clock_high && (now > last_clock_low + clock_rate)) {             // Clock transitioning to HIGH
    digitalWrite(CLOCK, !clock_high);
    last_clock_high = now;
    clock_high = !clock_high;
  }
  else if (clock_high && (now > last_clock_high + clock_rate)) {        // Clock transitioning to LOW
    cpu_monitor();
    digitalWrite(CLOCK, !clock_high);
    last_clock_low = now;
    clock_high = !clock_high;
  }
  else {
    //sprintf(buffer, "clock_high = %d, last_clock_high = %d, last_clock_low = %d, now = %d, NOOP", clock_high, last_clock_high, last_clock_low, now);
    //Serial.println(buffer);
  }
}

void cpu_monitor() {
  int bit = 0;
  unsigned int address = 0;
  unsigned int data = 0;
   for (int n = 0; n < 16; n++) {
      bit = digitalRead(ADDR[n]);
      address = (address << 1) + bit;
      Serial.print(bit);
    }
    Serial.print("   ");
    for (int n = 0; n < 8; n++) {
      bit = digitalRead(DATA[n]);
      data = (data << 1) + bit;
      Serial.print(bit);
    }
    bit = digitalRead(READ_WRITE) ? 'r' : 'W';
    sprintf(buffer, "    %04x(%c):  %02x", address, bit, data);
    Serial.println(buffer);
}
