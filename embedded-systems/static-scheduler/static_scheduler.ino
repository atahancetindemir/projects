// https://wokwi.com/projects/429671919866211329

uint8_t state = 0; // 0: stopped, 1: running
uint32_t start_time = 0;
uint32_t stop_time = 0;
uint32_t next_display_update = 0;

const uint8_t BUTTON_START = 2; // red
const uint8_t BUTTON_STOP = 3; // green
const uint8_t BUTTON_CLEAR = 4; // blue

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_START, INPUT_PULLUP);
  pinMode(BUTTON_STOP, INPUT_PULLUP);
  pinMode(BUTTON_CLEAR, INPUT_PULLUP);
}

void loop() {
  uint32_t current = millis();
  
  // button control with debounce
  static uint32_t last_debounce = 0;
  if (current - last_debounce > 50) {
    last_debounce = current;
    
    // start
    if (!digitalRead(BUTTON_START)) {
      if (state == 0) {
        state = 1;
        start_time = current;
      }
    }
  
    // stop
    if (!digitalRead(BUTTON_STOP)) {
      if (state == 1) {
        state = 0;
        stop_time += current - start_time;
      }
    }
    
    // clear
    if (!digitalRead(BUTTON_CLEAR)) {
      if (state == 0) {
        stop_time = 0;
      }
    }
  }

  // reflesh display
  if (current >= next_display_update) {
    uint32_t total = stop_time;
    if (state == 1) {
      total += current - start_time;
    }
    
    Serial.print("Time: ");
    Serial.print(total / 1000);
    Serial.print(".");
    Serial.println(total % 1000);
    
    next_display_update = current + 100;
  }
}