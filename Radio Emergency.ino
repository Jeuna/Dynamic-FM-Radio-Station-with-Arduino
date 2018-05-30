
#include "U8glib.h" // The library that we use in order to use Nokia 5110 LCD Display https://github.com/olikraus/u8glib/wiki/userreference
#include <FMTX.h> // The library we use in order to broadcast FM signals https://github.com/phrm/fmtx

U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8); // In this line, we informed Ardunio about which pin from LCD Display connected to which pin on Ardunio 
float channel=0;
float fm_freq = 88.0f;  // Here set the default FM frequency
float reading=0; // A variable in order to keep track of potentiometer value
float last_reading=0;
float Current_reading=0;
float mapping_reading=0; // A variable created in order to save value after mapped from potentiometer value (0-1023) into frequency limits (88-108)s


void setup(void){
  
pinMode(A0,INPUT);// Setting Analog 0 pin of Ardunio to get input from potentiometer.
Serial.begin(9600); // Starting serial monitor
fmtx_init(fm_freq, USA); // Initial FM Configuration. We gave fm_freq parameter which equals 88 initially and we selected USA as area 
u8g.setRot180(); // Setting up the rotation of screen 180 which is horizontal
u8g.setColorIndex(1); // Parameter 1 means set pixel, parameter 0 means clear pixel
//..........................................
u8g.firstPage();  // Launching the first page
  do{
    set_screen(1);
  }
  while(u8g.nextPage() );
  delay(1000);
 //....................
  u8g.firstPage();  
  do{
    set_screen(0);
  }
  while(u8g.nextPage() );
  delay(1000);
   Clear();
   //..........
   
    u8g.firstPage();  
  do{
    projectname(); // calls the function which shows project name
  }
  while(u8g.nextPage() );
  delay(2500);
   u8g.firstPage();  
  do{
    author();// calls the function which shows the author  :)
  }
  while(u8g.nextPage() );
  delay(2500);
  Clear();

//..........................................
}
// Reading Any Tunning change here.
void Analog_pin_read(){  // A function which builded in order to read the the data through Analog 0 pin. Which means from potentiometer 
   channel= mapping_reading/1.0f; // Read the input, map it and assign the channel variable
   Current_reading=channel;
   channel = (channel*10) / 10.0f;
   fmtx_set_freq(channel-2); // Set new frequency
   
}

void set_screen(int i){
 u8g.setColorIndex(i); // pixel on !
 for (int x_axis=0;x_axis<84;x_axis++){
 for (int y_axis=0;y_axis<44;y_axis++){
 u8g.drawPixel(x_axis,y_axis);
 }
 }
}

void Clear(void){   // A function which clears the screen 
  u8g .setFont(u8g_font_04b_03);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop(); 
}
void projectname(void){ // shows project name
  u8g.setColorIndex(1);
  u8g .setFont(u8g_font_osr18);
  u8g.drawStr( 7, 25, "Dynamic");
  u8g .setFont(u8g_font_unifont);
  u8g.drawStr( 0, 40, "Radio");
}
void author(void){ // shows author 
  u8g.setColorIndex(1);
  u8g.drawFrame(0,0,83,47);
  u8g .setFont(u8g_font_04b_03);
  u8g.drawStr( 2, 7, " ");
  u8g.drawStr( 15, 15, "Prepared by:");
  u8g.drawStr( 20, 26, "ILKER UNAL"); 
  
}
//change frequency value On lcd here
void number(float value){
 u8g.setColorIndex(1);
  u8g.drawFrame(0,0,83,47);
  u8g .setFont(u8g_font_04b_03);
  u8g.drawStr( 5, 15, "Frequency ");
  u8g .setFont(u8g_font_unifont);
  u8g.setPrintPos(6,38);
  u8g.print(value);
  u8g .setFont(u8g_font_04b_03);
  u8g.drawStr( 56, 38, "MHz ");
  
}

void loop(){  
  reading=analogRead(A0);   // read input
  mapping_reading=map(reading,0,1023,8800,10800); // map values between 0-1023 to 88-108
  mapping_reading = mapping_reading / 100.0f;
  if( mapping_reading - Current_reading >= 0.2f || Current_reading - mapping_reading >= 0.2f  ){ // detect changes of the frequency values
  Clear();
  Analog_pin_read();
u8g.firstPage();  
  do{
  number(channel);
  }
  while(u8g.nextPage() );
  delay(100);
  }
}

