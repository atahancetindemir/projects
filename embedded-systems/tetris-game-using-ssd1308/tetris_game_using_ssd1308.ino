// https://wokwi.com/projects/433857724363311105

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET      4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_ROTATE   2
#define BUTTON_RIGHT    3
#define BUTTON_SPEED    4
#define BUTTON_LEFT     5

// Shape definitions
#define SHAPE_DOT       0
#define SHAPE_LINE      1
#define SHAPE_T         2

// Grid dimensions (blocks, not pixels)
#define GRID_WIDTH     20
#define GRID_HEIGHT    15
#define BLOCK_SIZE      3
#define BLOCK_SPACING   4 // 3px block + 1px spacing

// Game area dimensions (pixels)
#define GAME_X_START   46
#define GAME_Y_START    0
#define GAME_X_END    126
#define GAME_Y_END     60

// Starting position (grid coordinates)
#define START_GRID_X   10  // Center of 20-block width
#define START_GRID_Y    0


// Game state variables
int8_t grid_x = START_GRID_X;
int8_t grid_y = START_GRID_Y;
uint8_t current_shape = random(0, 3);
uint8_t next_shape = random(0, 3);
uint16_t score = 0;
uint8_t rotation_state = 0;
bool game_active = true;


// Grid for placed blocks (15 rows x 20 columns)
bool placed_blocks[GRID_HEIGHT][GRID_WIDTH] = {0};


// Function prototypes
void draw_next_shape_info();
void clear_completed_rows();
void spawn_new_shape();
bool check_collision(int8_t test_x, int8_t test_y, uint8_t test_rotation);
void fix_shape_to_grid();
void draw_all_placed_blocks();
void rotate_shape();
bool move_shape(int8_t dx, int8_t dy);
void draw_block_at_grid(uint8_t grid_x, uint8_t grid_y);
bool is_valid_position(int8_t grid_x, int8_t grid_y, uint8_t rotation);


// Convert grid position to pixel position
uint8_t grid_to_pixel_x(uint8_t grid_x) {
  return GAME_X_START + grid_x * BLOCK_SPACING;
}


uint8_t grid_to_pixel_y(uint8_t grid_y) {
  return GAME_Y_START + grid_y * BLOCK_SPACING;
}


// Draw a block at grid coordinates
void draw_block_at_grid(uint8_t grid_x, uint8_t grid_y) {
  uint8_t pixel_x = grid_to_pixel_x(grid_x);
  uint8_t pixel_y = grid_to_pixel_y(grid_y);
  
  for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
    for(uint8_t j = 0; j < BLOCK_SIZE; j++) {
      display.drawPixel(pixel_x + j, pixel_y + i, SSD1306_WHITE);
    }
  }
}


// Draw shape using grid coordinates
void draw_shape(uint8_t shape, int8_t grid_x, int8_t grid_y, uint8_t rotation) {
  switch(shape) {
    case SHAPE_DOT:
      draw_block_at_grid(grid_x, grid_y);
      break;
      
    case SHAPE_LINE:
      if(rotation % 2 == 0) { // Vertical
        draw_block_at_grid(grid_x, grid_y);
        draw_block_at_grid(grid_x, grid_y + 1);
        draw_block_at_grid(grid_x, grid_y + 2);
      } else { // Horizontal
        draw_block_at_grid(grid_x, grid_y);
        draw_block_at_grid(grid_x + 1, grid_y);
        draw_block_at_grid(grid_x + 2, grid_y);
      }
      break;
      
    case SHAPE_T:
      switch(rotation % 4) {
        case 0: // T pointing down
          draw_block_at_grid(grid_x, grid_y);
          draw_block_at_grid(grid_x - 1, grid_y + 1);
          draw_block_at_grid(grid_x, grid_y + 1);
          draw_block_at_grid(grid_x + 1, grid_y + 1);
          break;
        case 1: // T pointing right
          draw_block_at_grid(grid_x, grid_y);
          draw_block_at_grid(grid_x, grid_y + 1);
          draw_block_at_grid(grid_x + 1, grid_y + 1);
          draw_block_at_grid(grid_x, grid_y + 2);
          break;
        case 2: // T pointing up
          draw_block_at_grid(grid_x, grid_y + 1);
          draw_block_at_grid(grid_x - 1, grid_y);
          draw_block_at_grid(grid_x, grid_y);
          draw_block_at_grid(grid_x + 1, grid_y);
          break;
        case 3: // T pointing left
          draw_block_at_grid(grid_x, grid_y);
          draw_block_at_grid(grid_x - 1, grid_y + 1);
          draw_block_at_grid(grid_x, grid_y + 1);
          draw_block_at_grid(grid_x, grid_y + 2);
          break;
      }
      break;
  }
}


// Check if a position is valid for the current shape
bool is_valid_position(int8_t test_x, int8_t test_y, uint8_t test_rotation) {
  // Check boundaries and collisions for each block in the shape
  switch(current_shape) {
    case SHAPE_DOT:
      if(test_x < 0 || test_x >= GRID_WIDTH || test_y >= GRID_HEIGHT) 
        return false;
      if(test_y >= 0 && placed_blocks[test_y][test_x]) 
        return false;
      break;
      
    case SHAPE_LINE:
      if(test_rotation % 2 == 0) { // Vertical
        for(int i = 0; i < 3; i++) {
          int8_t y_pos = test_y + i;
          if(test_x < 0 || test_x >= GRID_WIDTH || y_pos >= GRID_HEIGHT) 
            return false;
          if(y_pos >= 0 && placed_blocks[y_pos][test_x]) 
            return false;
        }
      } else { // Horizontal
        for(int i = 0; i < 3; i++) {
          int8_t x_pos = test_x + i;
          int8_t y_pos = test_y;
          if(x_pos < 0 || x_pos >= GRID_WIDTH || y_pos >= GRID_HEIGHT) 
            return false;
          if(y_pos >= 0 && placed_blocks[y_pos][x_pos]) 
            return false;
        }
      }
      break;
      
    case SHAPE_T:
      switch(test_rotation % 4) {
        case 0: // T down
          // Center top
          if(test_x < 0 || test_x >= GRID_WIDTH || test_y >= GRID_HEIGHT) return false;
          if(test_y >= 0 && placed_blocks[test_y][test_x]) return false;
          
          // Left middle
          if(test_x - 1 < 0 || test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x - 1]) return false;
          
          // Center middle
          if(test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x]) return false;
          
          // Right middle
          if(test_x + 1 >= GRID_WIDTH || test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x + 1]) return false;
          break;
          
        case 1: // T right
          // Top center
          if(test_x < 0 || test_x >= GRID_WIDTH || test_y >= GRID_HEIGHT) return false;
          if(test_y >= 0 && placed_blocks[test_y][test_x]) return false;
          
          // Middle center
          if(test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x]) return false;
          
          // Middle right
          if(test_x + 1 >= GRID_WIDTH || test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x + 1]) return false;
          
          // Bottom center
          if(test_y + 2 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 2][test_x]) return false;
          break;
          
        case 2: // T up
          // Center bottom
          if(test_x < 0 || test_x >= GRID_WIDTH || test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x]) return false;
          
          // Left top
          if(test_x - 1 < 0 || test_y < 0) return false;
          if(test_y >= 0 && placed_blocks[test_y][test_x - 1]) return false;
          
          // Center top
          if(test_y < 0) return false;
          if(placed_blocks[test_y][test_x]) return false;
          
          // Right top
          if(test_x + 1 >= GRID_WIDTH || test_y < 0) return false;
          if(placed_blocks[test_y][test_x + 1]) return false;
          break;
          
        case 3: // T left
          // Top center
          if(test_x < 0 || test_x >= GRID_WIDTH || test_y >= GRID_HEIGHT) return false;
          if(test_y >= 0 && placed_blocks[test_y][test_x]) return false;
          
          // Middle left
          if(test_x - 1 < 0 || test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x - 1]) return false;
          
          // Middle center
          if(test_y + 1 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 1][test_x]) return false;
          
          // Bottom center
          if(test_y + 2 >= GRID_HEIGHT) return false;
          if(placed_blocks[test_y + 2][test_x]) return false;
          break;
      }
      break;
  }
  
  return true;
}


// Add all blocks of current shape to grid
void fix_shape_to_grid() {
  switch(current_shape) {
    case SHAPE_DOT:
      if(grid_y >= 0) {
        placed_blocks[grid_y][grid_x] = true;
      }
      break;
      
    case SHAPE_LINE:
      if(rotation_state % 2 == 0) { // Vertical
        for(int i = 0; i < 3; i++) {
          if(grid_y + i >= 0) {
            placed_blocks[grid_y + i][grid_x] = true;
          }
        }
      } else { // Horizontal
        for(int i = 0; i < 3; i++) {
          if(grid_y >= 0) {
            placed_blocks[grid_y][grid_x + i] = true;
          }
        }
      }
      break;
      
    case SHAPE_T:
      switch(rotation_state % 4) {
        case 0: // T down
          if(grid_y >= 0) placed_blocks[grid_y][grid_x] = true;
          if(grid_y + 1 >= 0) {
            placed_blocks[grid_y + 1][grid_x - 1] = true;
            placed_blocks[grid_y + 1][grid_x] = true;
            placed_blocks[grid_y + 1][grid_x + 1] = true;
          }
          break;
          
        case 1: // T right
          if(grid_y >= 0) placed_blocks[grid_y][grid_x] = true;
          if(grid_y + 1 >= 0) {
            placed_blocks[grid_y + 1][grid_x] = true;
            placed_blocks[grid_y + 1][grid_x + 1] = true;
          }
          if(grid_y + 2 >= 0) placed_blocks[grid_y + 2][grid_x] = true;
          break;
          
        case 2: // T up
          if(grid_y >= 0) {
            placed_blocks[grid_y][grid_x - 1] = true;
            placed_blocks[grid_y][grid_x] = true;
            placed_blocks[grid_y][grid_x + 1] = true;
          }
          if(grid_y + 1 >= 0) placed_blocks[grid_y + 1][grid_x] = true;
          break;
          
        case 3: // T left
          if(grid_y >= 0) placed_blocks[grid_y][grid_x] = true;
          if(grid_y + 1 >= 0) {
            placed_blocks[grid_y + 1][grid_x - 1] = true;
            placed_blocks[grid_y + 1][grid_x] = true;
          }
          if(grid_y + 2 >= 0) placed_blocks[grid_y + 2][grid_x] = true;
          break;
      }
      break;
  }
}


void draw_score() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2,2);
  display.print(F("Score:"));
  display.setCursor(2,11);
  display.println(score);
}


void draw_next_shape_info() {
  if(!game_active) return;  // Don't show next shape when game is over
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 25);
  display.print(F("Next:"));
  
  // Draw next shape preview at fixed grid position
  draw_shape(next_shape, -10, 9, 0);
}


void draw_map() {
  // Draw game area border
  display.drawRect(GAME_X_START-1, GAME_Y_START, 
                  GAME_X_END+1 - GAME_X_START, 
                  GAME_Y_END - GAME_Y_START, 
                  SSD1306_WHITE);
}


void draw_all_placed_blocks() {
  for(uint8_t row = 0; row < GRID_HEIGHT; row++) {
    for(uint8_t col = 0; col < GRID_WIDTH; col++) {
      if(placed_blocks[row][col]) {
        draw_block_at_grid(col, row);
      }
    }
  }
}


void clear_completed_rows() {
  for(uint8_t row = 0; row < GRID_HEIGHT; row++) {
    bool row_complete = true;
    for(uint8_t col = 0; col < GRID_WIDTH; col++) {
      if(!placed_blocks[row][col]) {
        row_complete = false;
        break;
      }
    }
    
    if(row_complete) {
      // Remove the completed row
      for(uint8_t r = row; r > 0; r--) {
        for(uint8_t c = 0; c < GRID_WIDTH; c++) {
          placed_blocks[r][c] = placed_blocks[r-1][c];
        }
      }
      
      // Clear top row
      for(uint8_t c = 0; c < GRID_WIDTH; c++) {
        placed_blocks[0][c] = false;
      }
      
      score += 10;
    }
  }
}


void spawn_new_shape() {
  current_shape = next_shape;
  next_shape = random(0, 3);
  grid_x = START_GRID_X;
  grid_y = START_GRID_Y;
  rotation_state = 0;
  
  // Check if game over (new shape collides immediately)
  if(!is_valid_position(grid_x, grid_y, rotation_state)) {
    game_active = false;
  }
}


void rotate_shape() {
  uint8_t old_rotation = rotation_state;
  rotation_state = (rotation_state + 1) % 4;
  
  // If rotation causes collision, revert it
  if(!is_valid_position(grid_x, grid_y, rotation_state)) {
    rotation_state = old_rotation;
  }
}


bool move_shape(int8_t dx, int8_t dy) {
  int8_t new_x = grid_x + dx;
  int8_t new_y = grid_y + dy;
  
  // Check if new position is valid
  if(is_valid_position(new_x, new_y, rotation_state)) {
    grid_x = new_x;
    grid_y = new_y;
    return true;
  }
  return false;
}


void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  pinMode(BUTTON_ROTATE, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_SPEED, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);

  randomSeed(analogRead(A0));
}


void loop() {
  static unsigned long last_move_time = 0;
  bool move_down = false;

  // Handle button inputs
  if(digitalRead(BUTTON_ROTATE) == LOW) {
    rotate_shape();
    delay(200); // Simple debounce
  }
  
  if(digitalRead(BUTTON_RIGHT) == LOW) {
    move_shape(01, 0);
    delay(10);
  }
  
  if(digitalRead(BUTTON_LEFT) == LOW) {
    move_shape(-1, 0);
    delay(10);
  }
  
  if(digitalRead(BUTTON_SPEED) == LOW) {
    move_down = true;
  }

  // Handle automatic downward movement
  if(millis() - last_move_time > (move_down ? 100 : 1000)) {
    last_move_time = millis();
    
    if(!move_shape(0, 1)) {
      // If movement failed, shape has landed
      fix_shape_to_grid();
      clear_completed_rows();
      spawn_new_shape();
    }
  }

  // Drawing
  display.clearDisplay();
  
  draw_map();
  draw_score();
  draw_next_shape_info();
  
  if(game_active) {
    draw_all_placed_blocks();
    draw_shape(current_shape, grid_x, grid_y, rotation_state);
  } else {
    // Game over screen
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 40);
    display.print(F("Game"));
    display.setCursor(2, 50);
    display.print(F("Over!"));
  }
  
  display.display();
}

