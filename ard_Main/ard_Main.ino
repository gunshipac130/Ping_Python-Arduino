#include <ESPping.h>  // WiFi Pingscreen

#include <SPI.h>               //SSD1306 OLED screen i2c
#include <Wire.h>              //SSD1306 OLED screen i2c
#include <Adafruit_GFX.h>      //SSD1306 OLED screen i2c
#include <Adafruit_SSD1306.h>  //SSD1306 OLED screen i2c

// Ping
#define ping_array_size 106
#define ping_array_big_size 3
int ping_array[106];
int ping_array_big[3] = { 0, 0, 0 };
unsigned long big_ping_timestamp = 0;
int ping_ms = 0;

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 OLEDscreen_128x32(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initiate Seririal Communication
  Serial.begin(115200);
  Serial.setTimeout(1);
  delay(100);

  // Initiate SSD1306 OLED screen 128*32 i2c
  Job_Setup_SSD1306();
  Job_Setup_Python_Ping();
}

void loop() {
  Job_Loop_Ping();
  Job_Loop_SSD1306();
}




//
//
// !! Function Zone !!
//
//

int Work_read_serial_ping() {
  String str_rcv = ""; // string receive
  String str_buffer = ""; // string buffer
  int ping_int;
  bool data_rcv_completion = false;
  
  while (!Serial.available());
  str_rcv = Serial.readString();
  
  while (data_rcv_completion == false) {
    if (str_rcv[str_rcv.length()-1] == 'z') { // string terminator at last position; check if end with z
      str_buffer += str_rcv;
      Serial.print(str_buffer);
      data_rcv_completion = true;
    } else {
      str_buffer += str_rcv;
      str_rcv = "";
      str_rcv = Serial.readString();
    }
  }
  ping_int = str_buffer.toInt();
  return ping_int;
}

void Job_Loop_Ping() {
  ping_ms = Work_read_serial_ping();

  if (ping_ms > 0) {
    if (ping_ms <= 999) {
      Work_Ping_push_array(ping_array, ping_ms, ping_array_size);
    } else {
      Work_Ping_push_array(ping_array, 999, ping_array_size);
    }
  } else {
    Serial.println(" Error !");
    Work_Ping_push_array(ping_array, 999, ping_array_size);
  }

  Work_Ping_big_check(ping_array_big, ping_ms, ping_array_big_size);
}

void Work_Ping_push_array(int arr[], int a, int array_size) {  // array, new ping, array_size
  for (int i = array_size - 1; i >= 1; i--) {                  // i = ping array size
    arr[i] = arr[i - 1];
  }
  arr[0] = a;
}

void Work_Ping_big_check(int arr[], int big_ping, int array_size) {  // array, new ping 140+ ms, array_size
  if (big_ping >= 140) {                                       // if ping 140+ms, push array and fill new ping
    Work_Ping_push_array(arr, big_ping, array_size);
    big_ping_timestamp = millis();
  }
  if (millis() - big_ping_timestamp >= 10000) {  // if time pass more than 10sec without new ping action, push and fill 0
    Work_Ping_push_array(arr, 0, array_size);
    big_ping_timestamp = millis();
  }
}

void Job_Setup_SSD1306() {
  if (!OLEDscreen_128x32.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("SSD1306 OLED Started!!!");
  }

  //SSD1306.display();  //show Adafruit splash screen in initial display buffer
  //delay(2000); // Pause for 2 seconds
  OLEDscreen_128x32.clearDisplay();
  OLEDscreen_128x32.setCursor(0, 0);
  OLEDscreen_128x32.setTextSize(4);              // height 32 = 4 line from total 4
  OLEDscreen_128x32.setTextColor(BLACK, WHITE);  // black test on white background
  OLEDscreen_128x32.print("          ");         // fill with white
  OLEDscreen_128x32.display();
  delay(1000);
  OLEDscreen_128x32.clearDisplay();
  OLEDscreen_128x32.display();
}

void Job_Setup_Python_Ping() {
  int connect_wait_time = 0;
  int time_buffer;
  time_buffer = millis();
  while (!Serial.available()) {
    OLEDscreen_128x32.clearDisplay();
    OLEDscreen_128x32.setTextSize(1);
    OLEDscreen_128x32.setTextColor(WHITE, BLACK);
    OLEDscreen_128x32.setCursor(8, 12);
    OLEDscreen_128x32.print(F("Wait for Ping... "));
    if (millis() - time_buffer >= 1000) {
      OLEDscreen_128x32.setCursor(108, 12);
      OLEDscreen_128x32.printf("%2d\n", connect_wait_time);
      connect_wait_time = connect_wait_time + 1;
      OLEDscreen_128x32.display();
      time_buffer = millis();
    }
  }
}

void Job_Loop_SSD1306() {
  OLEDscreen_128x32.clearDisplay();
  // .drawPixel(x,y,color);  // single pixel
  // .drawLine(x0,y0,x1,y1,color);  // line
  // .drawRect(x0,y0,w,h,color);  // rectangle
  // .drawe(Circle(x0,y0,radius,color);  // circle
  // .drawRoundRect(x0,y0,w,h,radius,color); // rectangle with round edge
  // .drawTrianglx0,y0,x1,y1,x2,y2,color); // triangle
  Work_OLED_Ping_box(140, 100);
  Work_OLED_Big_Ping();
  Work_OLED_Ping_content();

  OLEDscreen_128x32.display();
}

void Work_OLED_Ping_Setup_Info() {
  OLEDscreen_128x32.clearDisplay();
  OLEDscreen_128x32.setTextSize(1);
  OLEDscreen_128x32.setTextColor(WHITE, BLACK);
  OLEDscreen_128x32.setCursor(16, 12);
  OLEDscreen_128x32.print(F("Connecting... "));
  OLEDscreen_128x32.printf("%2d\n", big_ping_timestamp);
  big_ping_timestamp = big_ping_timestamp + 1;
  OLEDscreen_128x32.display();
}

void Work_OLED_Ping_box(int Ping_top, int Ping_bottom) {
  int Ping_mid = (Ping_top + Ping_bottom) / 2;
  // ping Y axis number
  OLEDscreen_128x32.setTextSize(1);
  OLEDscreen_128x32.setTextColor(WHITE, BLACK);
  OLEDscreen_128x32.setCursor(0, 2);
  OLEDscreen_128x32.print(Ping_top);
  OLEDscreen_128x32.setCursor(0, 12);
  OLEDscreen_128x32.print(Ping_mid);
  OLEDscreen_128x32.setCursor(0, 22);
  OLEDscreen_128x32.print(Ping_bottom);
  // ping Y axis line
  OLEDscreen_128x32.drawLine(20, 0, 20, 32, WHITE);
  OLEDscreen_128x32.drawLine(18, 5, 21, 5, WHITE);
  OLEDscreen_128x32.drawLine(18, 15, 21, 15, WHITE);
  OLEDscreen_128x32.drawLine(18, 25, 21, 25, WHITE);
  // big Current ping number
  OLEDscreen_128x32.setCursor(64, 0);
  OLEDscreen_128x32.setTextSize(2);
  OLEDscreen_128x32.setTextColor(WHITE, BLACK);
  OLEDscreen_128x32.printf("  %3d", ping_array[0]);
  // big Current ping destination
  OLEDscreen_128x32.setCursor(96, 25);
  OLEDscreen_128x32.setTextSize(1);
  OLEDscreen_128x32.setTextColor(WHITE, BLACK);
  OLEDscreen_128x32.print("FFXIV");  // remote_ip, remote_host
}

void Work_OLED_Ping_content() {
  // ping graph line
  int y_axis;
  for (int x_axis = 0; x_axis < 106; x_axis++) {
    // limit y position
    y_axis = 75 - (ping_array[x_axis] / 2);
    if (y_axis < 0) { y_axis = 0; }
    if (y_axis > 32) { y_axis = 31; }
    // extra pixel for big ping
    int extra_y = 0;
    if (ping_array[x_axis] > 120)
      extra_y = (ping_array[x_axis] - 111) / 10;  // start add extra at ping 121
    // drawing pixel/line
    OLEDscreen_128x32.drawLine(22 + x_axis, y_axis, 22 + x_axis, y_axis + extra_y, WHITE);  // dot on graph
  }
}

void Work_OLED_Big_Ping() {
  OLEDscreen_128x32.setTextSize(1);
  OLEDscreen_128x32.setTextColor(WHITE, BLACK);
  OLEDscreen_128x32.setCursor(24, 25);
  for (int i = 0; i < 3; i++) {
    if (ping_array_big[i] == 0) {
      OLEDscreen_128x32.print("    ");
    } else {
      OLEDscreen_128x32.printf("%3d ", ping_array_big[i]);
    }
  }
}