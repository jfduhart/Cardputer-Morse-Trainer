  #include "M5Cardputer.h"
#include <map>
#include <vector>
#include <SD.h>

File dataFile;
// Path to the data file on the SD card
const char* folderPath = "/morsetrainer";
const char *dataFilePath = "/morsetrainer/data.txt";

// Morse code dictionary
std::map<char, String> morse_dict = {
  {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."},
  {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
  {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},
  {'S', "..."}, {'T', "-"}, {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
  {'Y', "-.--"}, {'Z', "--.."}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"},
  {'4', "....-"}, {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."},
  {'9', "----."}, {'0', "-----"},
  {',', "--..--"}, {'.', ".-.-.-"}, {'?', "..--.."}, {'=', "-...-"}, {'/', "-..-."},
};

// Max score dictionary (level,score)
std::map<int, int> max_score = {
  {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0},
  {11, 0}, {12, 0}, {13, 0}, {14, 0}, {15, 0}, {16, 0}, {17, 0}, {18, 0}, {19, 0}, {20, 0},
  {21, 0}, {22, 0}, {23, 0}, {24, 0}, {25, 0}, {26, 0}, {27, 0}, {28, 0}, {29, 0}, {30, 0},
  {31, 0}, {32, 0}, {33, 0}, {34, 0}, {35, 0}, {36, 0}, {37, 0}, {38, 0}, {39, 0}, {40, 0}
};

// Character pools for each level
std::vector<char> level_pools[40] = {
  {'K', 'M'},                        // Level 1
  {'K', 'M', 'U'},                   // Level 2
  {'K', 'M', 'U', 'R'},              // Level 3
  {'K', 'M', 'U', 'R', 'E'},         // Level 4
  {'K', 'M', 'U', 'R', 'E', 'S'},    // Level 5
  {'K', 'M', 'U', 'R', 'E', 'S', 'N'}, // Level 6
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A'}, // Level 7
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P'}, // Level 8
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T'}, // Level 9
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L'}, // Level 10
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W'}, // Level 11
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I'}, // Level 12
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.'}, // Level 13
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J'}, // Level 14
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z'}, // Level 15
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '='}, // Level 16
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F'}, // Level 17
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O'}, // Level 18
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y'}, // Level 19
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ','}, // Level 20
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V'}, // Level 21
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G'}, // Level 22
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5'}, // Level 23
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/'}, // Level 24
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q'}, // Level 25
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9'}, // Level 26
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2'}, // Level 27
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H'}, // Level 28
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3'}, // Level 29
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8'}, // Level 30
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B'}, // Level 31
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?'}, // Level 32
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4'}, // Level 33
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7'}, // Level 34
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C'}, // Level 35
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C', '1'}, // Level 36
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C', '1', 'D'}, // Level 37
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C', '1', 'D', '6'}, // Level 38
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C', '1', 'D', '6', '0'}, // Level 39
  {'K', 'M', 'U', 'R', 'E', 'S', 'N', 'A', 'P', 'T', 'L', 'W', 'I', '.', 'J', 'Z', '=', 'F', 'O', 'Y', ',', 'V', 'G', '5', '/', 'Q', '9', '2', 'H', '3', '8', 'B', '?', '4', '7', 'C', '1', 'D', '6', '0', 'X'}  // Level 40
};


// Current pool based on selected level
std::vector<char> current_pool;

// Game variables
char current_chars[5] = {' ', ' ', ' ', ' ', ' '};  // Five characters
char current_level_test_chars[59] = {' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' '};  // 50 characters and 9 spaces, 10 groups of 5 characters separated by a space
String user_input = "";
String user_level_test_input = "";
bool waiting_for_input = false;
bool waiting_for_level_test_input = false;
int wpm = 20;    // Default WPM
int level = 1;    // Default Level
int spk_volume = 45;//0-255
int score = 0;
int morsetone = 600;//Hz

// Menu navigation variables
int selected_option = 0;
bool menu_open = false;
bool after_round_opts = false;
bool after_test_opts = false;
const char* menu_items[] = {"Level Test","Level Game","Level Study", "Level", "WPM", "Volume"};
const int menu_items_count = 6;

// Morse timings variables
float wpm_factor = 1200.0 / wpm;  // WPM to milliseconds for a dot
int dot_duration = wpm_factor;               // Duration of a dot
int dash_duration = 3 * dot_duration;        // Duration of a dash
int inter_element_pause = dot_duration;      // Pause between elements (dot and dash)
int inter_character_pause = 3 * dot_duration; // Pause between characters
int inter_word_pause = 7 * dot_duration;     // Pause between words

void createFolderIfNotExists(const char* folderPath) {
  if (!SD.exists(folderPath)) {
    if (SD.mkdir(folderPath)) {
      Serial.println("Folder created successfully.");
    } else {
      Serial.println("Failed to create folder.");
    }
  }
}

void readDataFromFile() {
  createFolderIfNotExists(folderPath);  // Ensure the folder exists
  if (SD.exists(dataFilePath)) {
    dataFile = SD.open(dataFilePath, FILE_READ);
    if (dataFile) {
      while (dataFile.available()) {
        String line = dataFile.readStringUntil('\n');
        line.trim(); // Remove leading/trailing whitespaces
        
        // Parse the line and update variables
        if (line.startsWith("volume=")) {
          spk_volume = line.substring(7).toInt();
        } else if (line.startsWith("level=")) {
          level = line.substring(6).toInt();
        } else if (line.startsWith("wpm=")) {
          wpm = line.substring(4).toInt();
        } else if (line.startsWith("tone=")) {
          morsetone = line.substring(5).toInt();
        } else if (line.startsWith("max_score_1=")) {
            max_score[1] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_2=")) {
            max_score[2] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_3=")) {
            max_score[3] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_4=")) {
            max_score[4] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_5=")) {
            max_score[5] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_6=")) {
            max_score[6] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_7=")) {
            max_score[7] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_8=")) {
            max_score[8] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_9=")) {
            max_score[9] = line.substring(12).toInt();
        } else if (line.startsWith("max_score_10=")) {
            max_score[10] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_11=")) {
            max_score[11] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_12=")) {
            max_score[12] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_13=")) {
            max_score[13] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_14=")) {
            max_score[14] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_15=")) {
            max_score[15] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_16=")) {
            max_score[16] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_17=")) {
            max_score[17] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_18=")) {
            max_score[18] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_19=")) {
            max_score[19] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_20=")) {
            max_score[20] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_21=")) {
            max_score[21] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_22=")) {
            max_score[22] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_23=")) {
            max_score[23] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_24=")) {
            max_score[24] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_25=")) {
            max_score[25] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_26=")) {
            max_score[26] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_27=")) {
            max_score[27] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_28=")) {
            max_score[28] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_29=")) {
            max_score[29] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_30=")) {
            max_score[30] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_31=")) {
            max_score[31] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_32=")) {
            max_score[32] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_33=")) {
            max_score[33] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_34=")) {
            max_score[34] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_35=")) {
            max_score[35] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_36=")) {
            max_score[36] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_37=")) {
            max_score[37] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_38=")) {
            max_score[38] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_39=")) {
            max_score[39] = line.substring(13).toInt();
        } else if (line.startsWith("max_score_40=")) {
            max_score[40] = line.substring(13).toInt();
        }

      }
      dataFile.close();
    } else {
      Serial.println("Failed to open file for reading");
    }
  } else {
    Serial.println("File not found, creating file using default values.");
    writeDataToFile();
  }
}

void writeDataToFile() {
  createFolderIfNotExists(folderPath);  // Ensure the folder exists
  dataFile = SD.open(dataFilePath, FILE_WRITE);
  if (dataFile) {
    dataFile.println("volume=" + String(spk_volume));
    dataFile.println("level=" + String(level));
    dataFile.println("wpm=" + String(wpm));
    dataFile.println("tone=" + String(morsetone));
    dataFile.println("max_score_1=" + String(max_score[1]));
    dataFile.println("max_score_2=" + String(max_score[2]));
    dataFile.println("max_score_3=" + String(max_score[3]));
    dataFile.println("max_score_4=" + String(max_score[4]));
    dataFile.println("max_score_5=" + String(max_score[5]));
    dataFile.println("max_score_6=" + String(max_score[6]));
    dataFile.println("max_score_7=" + String(max_score[7]));
    dataFile.println("max_score_8=" + String(max_score[8]));
    dataFile.println("max_score_9=" + String(max_score[9]));
    dataFile.println("max_score_10=" + String(max_score[10]));
    dataFile.println("max_score_11=" + String(max_score[11]));
    dataFile.println("max_score_12=" + String(max_score[12]));
    dataFile.println("max_score_13=" + String(max_score[13]));
    dataFile.println("max_score_14=" + String(max_score[14]));
    dataFile.println("max_score_15=" + String(max_score[15]));
    dataFile.println("max_score_16=" + String(max_score[16]));
    dataFile.println("max_score_17=" + String(max_score[17]));
    dataFile.println("max_score_18=" + String(max_score[18]));
    dataFile.println("max_score_19=" + String(max_score[19]));
    dataFile.println("max_score_20=" + String(max_score[20]));
    dataFile.println("max_score_21=" + String(max_score[21]));
    dataFile.println("max_score_22=" + String(max_score[22]));
    dataFile.println("max_score_23=" + String(max_score[23]));
    dataFile.println("max_score_24=" + String(max_score[24]));
    dataFile.println("max_score_25=" + String(max_score[25]));
    dataFile.println("max_score_26=" + String(max_score[26]));
    dataFile.println("max_score_27=" + String(max_score[27]));
    dataFile.println("max_score_28=" + String(max_score[28]));
    dataFile.println("max_score_29=" + String(max_score[29]));
    dataFile.println("max_score_30=" + String(max_score[30]));
    dataFile.println("max_score_31=" + String(max_score[31]));
    dataFile.println("max_score_32=" + String(max_score[32]));
    dataFile.println("max_score_33=" + String(max_score[33]));
    dataFile.println("max_score_34=" + String(max_score[34]));
    dataFile.println("max_score_35=" + String(max_score[35]));
    dataFile.println("max_score_36=" + String(max_score[36]));
    dataFile.println("max_score_37=" + String(max_score[37]));
    dataFile.println("max_score_38=" + String(max_score[38]));
    dataFile.println("max_score_39=" + String(max_score[39]));
    dataFile.println("max_score_40=" + String(max_score[40]));

    dataFile.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}

// Draw a bordered rectangle for GUI elements
void draw_bordered_rect(int x, int y, int width, int height, uint16_t color, uint16_t bg_color, bool filled=true) {
  if(filled)
  {
    M5Cardputer.Display.fillRect(x, y, width, height, bg_color);
  }
  M5Cardputer.Display.drawRect(x, y, width, height, color);
}

// Draw a centered text
void draw_centered_text(String text, int y, uint16_t color, int txt_size) {
  M5Cardputer.Display.setTextSize(txt_size);
  int text_width = M5Cardputer.Display.textWidth(text);
  int x = (M5Cardputer.Display.width() - text_width) / 2;
  M5Cardputer.Display.setTextColor(color);  
  M5Cardputer.Display.drawString(text, x, y);
}

void draw_left_text(String text, int y, uint16_t color, int txt_size) {
  M5Cardputer.Display.setTextSize(txt_size); 
  int x = 10;
  M5Cardputer.Display.setTextColor(color);  
  M5Cardputer.Display.drawString(text, x, y);
}

void draw_right_text(String text, int y, uint16_t color, int txt_size) {
  M5Cardputer.Display.setTextSize(txt_size);
  int text_width = M5Cardputer.Display.textWidth(text);
  int x = (M5Cardputer.Display.width() - text_width);
  M5Cardputer.Display.setTextColor(color);  
  M5Cardputer.Display.drawString(text, x, y);
}

void draw_multiline_text(String text, int x, int y,uint16_t color, int txt_size, int box_width, int lineHeight=0) {
   M5Cardputer.Display.setTextColor(color); 
   M5Cardputer.Display.setTextSize(txt_size);
  int screenWidth = box_width;  // Get screen width
  int charWidth = M5Cardputer.Display.textWidth(" "); // Width of a single space (average char width)
  if(lineHeight==0){
  lineHeight = M5Cardputer.Display.fontHeight(txt_size)/2 +5;  // Get font height for line spacing
  }
  
  int currentX = x; // Start position
  int currentY = y;

  for (int i = 0; i < text.length(); i++) {
    // Get the width of the current character
    String currentChar = String(text[i]);
    charWidth = M5Cardputer.Display.textWidth(currentChar);

    // If adding this character exceeds screen width, move to next line
    if (currentX + charWidth > screenWidth) {
      currentX = x; // Reset X to the start
      currentY += lineHeight; // Move to the next line
    }

    // Draw the character at the current position
    M5Cardputer.Display.drawString(currentChar, currentX, currentY);

    // Move the X position for the next character
    currentX += charWidth;
  }
}


void recalculate_timings(){
   wpm_factor = 1200.0 / wpm;  // WPM to milliseconds for a dot
   dot_duration = wpm_factor;               // Duration of a dot
   dash_duration = 3 * dot_duration;        // Duration of a dash
   inter_element_pause = dot_duration;      // Pause between elements (dot and dash)
   inter_character_pause = 3 * dot_duration; // Pause between characters
   inter_word_pause = 7 * dot_duration;     // Pause between words
}

// Play the Morse code for a given character (beeps)
void play_morse_code(char c) {
  String code = morse_dict[c];
  for (int i = 0; i < code.length(); i++) {
    if (code[i] == '.') {
      M5.Speaker.tone(morsetone, dot_duration);  // Beep for dot
      delay(dot_duration);                  // Delay for the duration of the dot
    } else if (code[i] == '-') {
      M5.Speaker.tone(morsetone, dash_duration); // Beep for dash
      delay(dash_duration);                 // Delay for the duration of the dash
    }
    delay(inter_element_pause);             // Pause between dots and dashes
  }
  delay(inter_character_pause);             // Pause before the next character
}

// Play Morse code for five characters with correct timing between them
void play_five_morse_codes() {
  for (int i = 0; i < 5; i++) {
    play_morse_code(current_chars[i]);
    if (i < 4) { // Add inter-character pause between characters, except after the last one
      delay(inter_character_pause);  // Inter-character pause
    }
  }
}

void play_level_test_morse_codes() {
  for (int i = 0; i < 60; i++) {
    if(current_level_test_chars[i]==' ')
    {
      delay(inter_word_pause);
    }
    else
    {
      play_morse_code(current_level_test_chars[i]);
      delay(inter_character_pause);  // Inter-character pause      
    }
  }
}

// Check if the user's input is correct
void check_input() {
  String correct_answer = "";
  for (int i = 0; i < 5; i++) {
    correct_answer += String(current_chars[i]);
  }
    
  if (user_input.equalsIgnoreCase(correct_answer)) {   
    draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, GREEN, BLACK, false);
    draw_centered_text(correct_answer, 70, TFT_GREEN,2);
    draw_centered_text("Correct", 90, TFT_GREEN,2);  
    score += 1;
    if(score>max_score[level]){
      max_score[level]=score;   
      writeDataToFile();   
    }
  } else {    
    draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, RED, BLACK, false);
    draw_centered_text(correct_answer, 70, TFT_RED,2);
    draw_centered_text("Incorrect", 90, TFT_RED,2); 
    score=0;   
  }
  delay(1000);
  user_input="";
  after_round_options();
  
}

void check_level_test_input() {

  bool areEqual = true;
  float total_errors = 0.0;
  String wrong_chars = "";
  if (user_level_test_input.length() != sizeof(current_level_test_chars)) {
  areEqual = false;
  } else {
    // Loop through each character and compare
    for (int i = 0; i < user_level_test_input.length(); i++) {
      if (user_level_test_input[i] != current_level_test_chars[i]) {
        areEqual = false;
        total_errors +=1.0;
        wrong_chars = wrong_chars+user_level_test_input[i];
      }
      else
      {
        wrong_chars = wrong_chars+" ";
      }
    }
  }
  float test_score = 100.0 -(total_errors*100.0/59.0);
  String test_chars = String(current_level_test_chars);
  String test_chars_fix = test_chars.substring(0,59);
  draw_bordered_rect(5, 10, M5Cardputer.Display.width()-5, 27, BLACK, BLACK);
  if (test_score>90.0) {   
    draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, GREEN, BLACK, false);
    draw_centered_text("Pass - "+String(test_score)+"%", 15, TFT_GREEN,2); 
  } else {    
    draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, RED, BLACK, false);
    draw_centered_text("Fail - "+String(test_score)+"%", 15, TFT_RED,2); 
  }
  draw_multiline_text(test_chars_fix, 33,59, GREEN,1, M5Cardputer.Display.width()-25, 25);
  draw_multiline_text(wrong_chars, 33,50, RED,1, M5Cardputer.Display.width()-25, 25);
  
  delay(1000);
  user_level_test_input="";
  after_level_test_options();
  
}

void printscores(){
  draw_right_text("Score:"+String(score), 8, YELLOW,1);
  draw_right_text("Max Score:"+String(max_score[level]), 18, YELLOW,1);
}

// Wait for user input about what to do next
void after_round_options(){
  printscores();
  
  M5Cardputer.Display.fillRect(5, M5Cardputer.Display.height()-20, M5Cardputer.Display.width()-5, 20, WHITE);
  draw_centered_text("Next Round : Enter  |    Menu : Esc", 120, TFT_BLACK,1);
  after_round_opts = true;
  menu_open = false; 
  waiting_for_input = false;
}

void after_level_test_options(){
  
  M5Cardputer.Display.fillRect(5, M5Cardputer.Display.height()-20, M5Cardputer.Display.width()-5, 20, WHITE);
  draw_centered_text("Menu : Esc", 120, TFT_BLACK,1);
  after_test_opts = true;
  menu_open = false; 
  waiting_for_input = false;
}

// Start a new round by displaying new Morse code sequences for five characters
void new_round() {
  for (int i = 0; i < 5; i++) {
    current_chars[i] = get_random_char();
  }
  user_input = "";  // Clear previous input
  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, ORANGE, BLACK);
  draw_left_text("Listen...", 10, TFT_WHITE,2);
  play_five_morse_codes();
  waiting_for_input = true;

  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, WHITE, BLACK);
  draw_centered_text("Answer", 20, TFT_WHITE,2);
  draw_bordered_rect((M5Cardputer.Display.width()/2)-37, 42, 74, 22, TFT_WHITE, BLACK);
}

void new_level_test() {
  M5Cardputer.Display.clear();
  draw_multiline_text("You will listen 10 groups of 5 random characters based on the selected      level. Write them down somewhere and  then type them here when audio ends.  Use a space character between groups. Press  enter to begin...", 5,15, TFT_WHITE,1,M5Cardputer.Display.width()-5);
   while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      // Handle Enter key (finalize WPM input)
      if (status.enter) {
        M5Cardputer.Display.clear();
        draw_centered_text("3", M5Cardputer.Display.height()/2-10, TFT_WHITE,4);
        delay(1000);
        M5Cardputer.Display.clear();
        draw_centered_text("2", M5Cardputer.Display.height()/2-10, TFT_WHITE,4);
        delay(1000);
        M5Cardputer.Display.clear();
        draw_centered_text("1", M5Cardputer.Display.height()/2-10, TFT_WHITE,4);
        delay(1000);
       break;
      }
    }
   }
    
  int z=0;
  for(int x = 0; x < 10; x++){
    for (int i = 0; i < 5; i++) {
      current_level_test_chars[z] = get_random_char();
      z+=1;
    }
    current_level_test_chars[z] = ' ';
    z+=1;
  }
  user_level_test_input = "";  // Clear previous input
  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, ORANGE, BLACK);
  draw_left_text("Listen...", 10, TFT_WHITE,2);
  play_level_test_morse_codes();
  waiting_for_level_test_input = true;
  waiting_for_input=false;
  after_round_opts=false;
  after_test_opts=false;

  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, WHITE, BLACK);
  draw_centered_text("Type answer:", 20, TFT_WHITE,2);
  draw_bordered_rect(25,42, M5Cardputer.Display.width()-50, 58, TFT_WHITE, BLACK);
  draw_multiline_text("_", 33,50, TFT_WHITE,1, M5Cardputer.Display.width()-25, 25);
}

// Get a random character from the current pool
char get_random_char() {
  int index = random(current_pool.size());
  return current_pool[index];
}

// Display the main menu
void show_menu() {
  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, TFT_WHITE, BLACK);
  String menu_val="";
  int color = TFT_GREEN;
  for (int i = 0; i < menu_items_count; i++) {
    if (i == selected_option) {
      color = TFT_GREEN; // Highlight selected option
    } else {
      color = TFT_WHITE;
    }
    
    switch (i) {
    case 0:  
      break;
    case 1:  
      break;
    case 2:  
      break;
    case 3:  
      menu_val=":"+String(level);
      break;
    case 4: 
      menu_val=":"+String(wpm);
      break;
    case 5:  
      menu_val=":"+String(spk_volume);
      break;
  }
    draw_left_text(menu_items[i]+menu_val,10 + (i * 20), color,2);
    menu_val="";
  }
}

// Handle user input for WPM and Level in the menu
void handle_menu_selection() {
  switch (selected_option) {
    case 0:  // Start Game
      new_level_test();
      break;
    case 1:  // Start Game
      new_round();
      break;
    case 2:  // Set Level
      study_level();
      break;
    case 3:  // Set Level
      set_level();
      break;
    case 4:  // Set WPM
      set_wpm();
      break;
    case 5:  // Set Volume
      set_volume();
      break;
  }
}

//Shows info and plays example for the currently selected level
void study_level(){
  M5Cardputer.Display.clear();
  draw_centered_text("Current Level:"+String(level), 10, TFT_WHITE,2);

  M5Cardputer.Display.fillRect(5, M5Cardputer.Display.height()-20, M5Cardputer.Display.width()-5, 20, WHITE);
  draw_centered_text("Play : Enter  |    Menu : Esc", 120, TFT_BLACK,1);
  
  if(level==1)
  {
    M5Cardputer.Display.setTextSize(3);
    M5Cardputer.Display.setTextColor(WHITE); 
    M5Cardputer.Display.drawString(String(current_pool[0])+":"+String(morse_dict[current_pool[0]]), (M5Cardputer.Display.width()/4)-40, 55); 
    M5Cardputer.Display.drawString(String(current_pool[1])+":"+String(morse_dict[current_pool[1]]), ((M5Cardputer.Display.width()/4)*3)-40, 55);

    M5Cardputer.Display.setTextColor(RED); 
    M5Cardputer.Display.drawString(String(current_pool[0])+":"+String(morse_dict[current_pool[0]]), (M5Cardputer.Display.width()/4)-40, 55); 
    play_morse_code(current_pool[0]);
    M5Cardputer.Display.setTextColor(WHITE); 
    M5Cardputer.Display.drawString(String(current_pool[0])+":"+String(morse_dict[current_pool[0]]), (M5Cardputer.Display.width()/4)-40, 55); 
    delay(1000);
    M5Cardputer.Display.setTextColor(RED); 
    M5Cardputer.Display.drawString(String(current_pool[1])+":"+String(morse_dict[current_pool[1]]), ((M5Cardputer.Display.width()/4)*3)-40, 55); 
    play_morse_code(current_pool[1]);
    M5Cardputer.Display.setTextColor(WHITE); 
    M5Cardputer.Display.drawString(String(current_pool[1])+":"+String(morse_dict[current_pool[1]]), ((M5Cardputer.Display.width()/4)*3)-40, 55); 
  }
  else
  {
    draw_centered_text(String(current_pool[current_pool.size()-1])+":"+String(morse_dict[current_pool[current_pool.size()-1]]), 50, RED ,5);
    play_morse_code(current_pool[current_pool.size()-1]);
    draw_centered_text(String(current_pool[current_pool.size()-1])+":"+String(morse_dict[current_pool[current_pool.size()-1]]), 50, WHITE ,5);
  }
  
 

  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      
      if (status.enter ) {       
        //play character
        if(level==1)
        {
          M5Cardputer.Display.setTextSize(3);
          M5Cardputer.Display.setTextColor(RED); 
          M5Cardputer.Display.drawString(String(current_pool[0])+":"+String(morse_dict[current_pool[0]]), (M5Cardputer.Display.width()/4)-40, 55); 
          play_morse_code(current_pool[0]);
          M5Cardputer.Display.setTextColor(WHITE); 
          M5Cardputer.Display.drawString(String(current_pool[0])+":"+String(morse_dict[current_pool[0]]), (M5Cardputer.Display.width()/4)-40, 55); 
          delay(1000);
          M5Cardputer.Display.setTextColor(RED); 
          M5Cardputer.Display.drawString(String(current_pool[1])+":"+String(morse_dict[current_pool[1]]), ((M5Cardputer.Display.width()/4)*3)-40, 55); 
          play_morse_code(current_pool[1]);
          M5Cardputer.Display.setTextColor(WHITE); 
          M5Cardputer.Display.drawString(String(current_pool[1])+":"+String(morse_dict[current_pool[1]]), ((M5Cardputer.Display.width()/4)*3)-40, 55);  
        }
        else
        {
          draw_centered_text(String(current_pool[current_pool.size()-1])+":"+String(morse_dict[current_pool[current_pool.size()-1]]), 50, RED ,5);
          play_morse_code(current_pool[current_pool.size()-1]);
          draw_centered_text(String(current_pool[current_pool.size()-1])+":"+String(morse_dict[current_pool[current_pool.size()-1]]), 50, WHITE ,5);
        }
      }
      else if (M5Cardputer.Keyboard.isKeyPressed('`')) 
      {
       
        break;  // Exit the loop
      }
    }
  }
   menu_open = true; 
   show_menu();
}

// Menu for adjusting WPM
void set_wpm() {
  String wpm_input = "";
  M5Cardputer.Display.clear();
  draw_centered_text("Enter WPM", 30, TFT_WHITE,2);
  draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 50, 50, 22, TFT_WHITE, BLACK);
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      // Handle number input
      for (auto key : status.word) {
        if (isdigit(key) && wpm_input.length() < 3) {
          wpm_input += key;
          draw_centered_text("Invalid WPM", 70, BLACK,2);
        }
      }

      // Handle delete key (backspace)
      if (status.del && wpm_input.length() > 0) {
        wpm_input.remove(wpm_input.length() - 1);
        draw_centered_text("Invalid WPM", 70, BLACK,2);
      }

      // Handle Enter key (finalize WPM input)
      if (status.enter && wpm_input.length() > 0) {
        wpm = wpm_input.toInt();
        if (wpm <= 0) { // Validate WPM
          M5Cardputer.Display.setTextColor(TFT_RED);
          draw_centered_text("Invalid WPM!", 70, TFT_RED,2);
          //delay(2000);
          //wpm_input = "";
          //M5Cardputer.Display.clear();//fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), BLACK);
          continue;
        }
        recalculate_timings();
        break;  // Exit the loop when a valid WPM is entered
      }

      // Update the display with the user's input
      draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 50, 50, 22, TFT_WHITE, BLACK);
      draw_centered_text(wpm_input, 53, TFT_WHITE,2);
    }
  }

  // Confirmation message
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, GREEN, BLACK);
  draw_centered_text("WPM Set to: " + String(wpm), 50, TFT_WHITE,2);
  writeDataToFile();   
  delay(1000);
  menu_open = true;  // Return to menu after setting WPM
  show_menu();
}

// Menu for adjusting Level
void set_level() {
  String level_input = "";
  M5Cardputer.Display.clear();
  draw_centered_text("Select Level", 30, TFT_WHITE,2);
  draw_centered_text("(1-40)", 50, TFT_WHITE,2);
  draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 70, 50, 22, TFT_WHITE, BLACK);
  
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      // Handle number input
      for (auto key : status.word) {
        if (isdigit(key) && level_input.length() < 3) {
          level_input += key;
          draw_centered_text("Invalid Level", 97, BLACK,2);//"erase" warning
        }
      }

      // Handle delete key (backspace)
      if (status.del && level_input.length() > 0) {
        level_input.remove(level_input.length() - 1);
        draw_centered_text("Invalid Level", 97, BLACK,2);//"erase" warning
      }

      // Handle Enter key (finalize Level input)
      if (status.enter && level_input.length() > 0) {
        level = level_input.toInt();
        if (level < 1 || level > 40) {  // Validate Level
          M5Cardputer.Display.setTextColor(TFT_RED);
          draw_centered_text("Invalid Level", 97, TFT_RED,2);
          //delay(2000);
          //level_input = "";
          //M5Cardputer.Display.clear();//fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), BLACK);
          continue;
        }
        current_pool = level_pools[level - 1]; // Set character pool for the selected level        
        break;  // Exit the loop when a valid Level is entered
      }

      // Update the display with the user's input
      draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 70, 50, 22, TFT_WHITE, BLACK);
      draw_centered_text(level_input, 73, TFT_WHITE,2);
    }
  }

  // Confirmation message
  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, GREEN, BLACK);
  draw_centered_text("Level Set to: " + String(level), 50, TFT_WHITE,2);
  score=0;
  writeDataToFile(); 
  delay(1000);
  menu_open = true;  // Return to menu after setting Level
  show_menu();
}

// Menu for adjusting volume
void set_volume() {
  String volume_input = "";
  M5Cardputer.Display.clear();
  draw_centered_text("Enter Volume", 30, TFT_WHITE,2);
  draw_centered_text("(1-255)", 50, TFT_WHITE,2);
  draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 70, 50, 22, TFT_WHITE, BLACK);
   
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      // Handle number input
      for (auto key : status.word) {
        if (isdigit(key) && volume_input.length() < 3) {
          volume_input += key;
          draw_centered_text("Invalid Volume", 97, BLACK,2);
        }
      }

      // Handle delete key (backspace)
      if (status.del && volume_input.length() > 0) {
        volume_input.remove(volume_input.length() - 1);
        draw_centered_text("Invalid Volume", 97, BLACK,2);
      }

      // Handle Enter key (finalize Level input)
      if (status.enter && volume_input.length() > 0) {
        spk_volume = volume_input.toInt();
        if (spk_volume < 1 || spk_volume > 255) {  // Validate Level
          M5Cardputer.Display.setTextColor(TFT_RED);
          draw_centered_text("Invalid Volume", 97, TFT_RED,2);
          //delay(2000);
          //volume_input = "";
          //M5Cardputer.Display.clear();//fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), BLACK);
          continue;
        } 
        M5.Speaker.setVolume(spk_volume);            
        break;  // Exit the loop when a valid Level is entered
      }

      // Update the display with the user's input
      draw_bordered_rect((M5Cardputer.Display.width()/2)-25, 70, 50, 22, TFT_WHITE, BLACK);
      draw_centered_text(volume_input, 73, TFT_WHITE,2);
    }
  }

  // Confirmation message
  M5Cardputer.Display.clear();
  draw_bordered_rect(5, 5, M5Cardputer.Display.width()-5, M5Cardputer.Display.height()-5, GREEN, BLACK);
  draw_centered_text("Volume Set to: " + String(volume_input), 50, TFT_WHITE,2);
  writeDataToFile();   
  delay(1000);
  menu_open = true;  // Return to menu after setting Level
  show_menu();
}

// Main setup
void setup() {
  M5Cardputer.begin();

  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
    return;
  }

  readDataFromFile();
  
  M5.Speaker.setVolume(spk_volume);
  randomSeed(millis());

  // Set default character pool for level 1
  current_pool = level_pools[level - 1];
  recalculate_timings();
  // Show the initial menu
  menu_open = true;
  show_menu();
}

// Main loop
void loop() {
  M5Cardputer.update();

  if(after_round_opts)
  {     
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      if (status.enter) {
        after_round_opts = false;
        new_round();
      }
      else if (M5Cardputer.Keyboard.isKeyPressed('`')) 
      {
        menu_open = true; 
        after_round_opts = false;
        show_menu();
      }
    }
    
  }
  else if(after_test_opts)
  {     
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      if (M5Cardputer.Keyboard.isKeyPressed('`')) 
      {
        menu_open = true; 
        after_test_opts = false;
        show_menu();
      }
    }
    
  }
  else if (menu_open) {
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      if (M5Cardputer.Keyboard.isKeyPressed('.')) {
        selected_option = (selected_option + 1) % menu_items_count;  // Navigate down
        show_menu();
      } else if (M5Cardputer.Keyboard.isKeyPressed(';')) {
        selected_option = (selected_option - 1 + menu_items_count) % menu_items_count;  // Navigate up
        show_menu();
      } else if (status.enter) {
        menu_open = false;
        handle_menu_selection();  // Execute the selected menu option
      }
    }
  } 
  else if (waiting_for_input) {
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      for (auto key : status.word) {
        if ((isalnum(key) or key=='.' or key=='/' or key=='?' or key==',' or key=='=') && user_input.length() < 5) {
          user_input += key;
        }
      }
      user_input.toUpperCase();
      if (status.del && user_input.length() > 0) {
        user_input.remove(user_input.length() - 1);
      }

     

      // Update the display with the user's current input
       //M5Cardputer.Display.clear();//fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), BLACK);
       draw_bordered_rect((M5Cardputer.Display.width()/2)-37, 42, 74, 22, TFT_WHITE, BLACK);
       draw_centered_text(user_input, 45, TFT_WHITE,2);

       if (status.enter && user_input.length() == 5) {
        waiting_for_input = false;
        //M5Cardputer.Display.clear();
        //M5Cardputer.Display.fillRect(0, 0, M5Cardputer.Display.width(), M5Cardputer.Display.height(), BLACK);
        check_input();
      }
    }
  }
  else if (waiting_for_level_test_input) {
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

      for (auto key : status.word) {
        if ((isalnum(key) or key=='.' or key=='/' or key=='?' or key==',' or key=='=' or key==' ') && user_level_test_input.length() < 59) {
          user_level_test_input += key;
        }
      }
      user_level_test_input.toUpperCase();
      if (status.del && user_level_test_input.length() > 0) {
        user_level_test_input.remove(user_level_test_input.length() - 1);
      }

     

      // Update the display with the user's current input
      draw_bordered_rect(25,42, M5Cardputer.Display.width()-50, 58, TFT_WHITE, BLACK);
      if(user_level_test_input.length() == 59){
        draw_multiline_text(user_level_test_input, 33,50, TFT_WHITE,1, M5Cardputer.Display.width()-25, 25);
      }
      else{
        draw_multiline_text(user_level_test_input+"_", 33,50, TFT_WHITE,1, M5Cardputer.Display.width()-25, 25);
      }
      
      if (status.enter && user_level_test_input.length() == 59) {
        draw_multiline_text(user_level_test_input+" ", 33,50, TFT_WHITE,1, M5Cardputer.Display.width()-25, 25);
        waiting_for_level_test_input = false;
        check_level_test_input();
      }
    }
  }
}
