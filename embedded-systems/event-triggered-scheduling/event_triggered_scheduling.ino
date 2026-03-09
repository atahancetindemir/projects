// https://wokwi.com/projects/429670629239828481

volatile uint8_t state = 0; // 0: stopped, 1: running
volatile uint32_t elapsed_time = 0;
volatile uint16_t display_delay = 100;
volatile bool display_update_needed = false;

const uint8_t BUTTON_START = 2; // red
const uint8_t BUTTON_STOP = 3; // green
const uint8_t BUTTON_CLEAR = 4; // blue

// start interrupt
void start_pressed() {
  if (state == 0) state = 1;
}

// stop interrupt
void stop_pressed() {
  state = 0;
}

// clear interrupt
void clear_pressed() {
  if (state == 0) elapsed_time = 0;
}

// Timer1 1ms interrupt
ISR(TIMER1_COMPA_vect) {
  if (state == 1) {
    elapsed_time++;
    if (--display_delay == 0) {
      display_delay = 100;
      display_update_needed = true;
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  pinMode(BUTTON_CLEAR, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(BUTTON_START), start_pressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_STOP), stop_pressed, FALLING);
  
  // Clear Pin Change Interrupt (PCINT20 - D4)
  PCICR |= (1 << PCIE2);    // PCINT16-23
  PCMSK2 |= (1 << PCINT20); // D4 -> PCINT20

  // Timer1 1ms
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC, 64 prescaler
  OCR1A = 249; // 16MHz/(64*250) = 1000Hz
  TIMSK1 |= (1 << OCIE1A);
}

// Clear PC Interrupt
ISR(PCINT2_vect) {
  if (!digitalRead(BUTTON_CLEAR)) {
    clear_pressed();
  }
}

void loop() {
  if (display_update_needed) {
    uint32_t time;
    // atomic read
    noInterrupts();
    time = elapsed_time;
    interrupts();
    
    Serial.print("Time: ");
    Serial.print(time / 1000);
    Serial.print(".");
    Serial.println(time % 1000);
    
    display_update_needed = false;
  }
}