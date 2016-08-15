
#include <SimpleModbusMaster.h>
#include <LiquidCrystal.h>  // use LiquidCrystal library
/*
   The example will use packet1 to read a register from address 0 (the adc ch0 value)
   from the arduino slave (id=1). It will then use this value to adjust the brightness
   of an led on pin 9 using PWM.
   It will then use packet2 to write a register (its own adc ch0 value) to address 1 
   on the arduino slave (id=1) adjusting the brightness of an led on pin 9 using PWM.
*/
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//int pin for peizo buzzer
int piezoPin = 12;

//int the variable for the display scrol value
int displayedln = 0;

//General varibales
float var1 = 0.0;   // stores the calculated temperature
float var2 = 0.0;
float var3 = 3.0;
float var4 = 4.0;
float var5 = 5.0;
float var6 = 6.0;
float var7 = 7.0;
float var8 = 8.0;
float var9 = 9.0;


//inisitalize the Device ID value.
int ID = 16;

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()

{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

//////////////////// Port information ///////////////////
#define baud 9600
#define timeout 1000
#define polling 200 // the scan rate
#define retry_count 10

// used to toggle the receive/transmit pin on the driver
#define TxEnablePin 2 

#define LED 9

// The total amount of available memory on the master to store data
#define TOTAL_NO_OF_REGISTERS 15

// This is the easiest way to create new packets
// Add as many as you want. TOTAL_NO_OF_PACKETS
// is automatically updated.
enum
{
  PACKET1,
  PACKET2,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

// Create an array of Packets to be configured
Packet packets[TOTAL_NO_OF_PACKETS];

// Masters register array
unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup()
{
  // Initialize each packet 
//modbus_construct(packet, id, function, address, data, register array)
  modbus_construct(&packets[PACKET1], ID, READ_HOLDING_REGISTERS, 1, 15, 0);
//modbus_construct(&packets[PACKET2], 1, PRESET_MULTIPLE_REGISTERS, 1, 1, 0);
  
  // Initialize the Modbus Finite State Machine
  modbus_configure(&Serial1, baud, SERIAL_8N1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);

  //play start up sound 
  tone( 46, 500, 500);
  delay(215);
  tone( 46, 1000, 500);
  delay(150);
  tone( 46, 2000, 500);
  delay(150);
  
  Serial.begin(9600);            // start the serial port data dump (debug)
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(5,0);
  lcd.print("SUBCOE"); // print te staty up message
  lcd.setCursor(1,1);
  lcd.print("RS-485 Display");
 

  delay(2000);
  lcd.clear();
//  pinMode(LED, OUTPUT);\

// display the port information
  lcd.setCursor(0,0);
  lcd.print("Device ID:");
  lcd.print(ID);
  lcd.setCursor(0,1);
  lcd.print("Baud:");
  lcd.print(baud);
  lcd.print(" 8-N-1");
  delay(2500);
  lcd.clear();
  
}

void loop()
{
  modbus_update();
 lcd_key = read_LCD_buttons();  // read the buttons
 
  switch (lcd_key) {             // depending on which button was pushed, preform the scrol or correct function (only up and down function)
 
   case btnRIGHT:
     tone( 46, 2000, 150);
     lcd.clear();
     lcd.setCursor(4,0);
     lcd.print("INVALID");
     delay (1250);
     lcd.clear();
   break;
    
   case btnLEFT:
     tone( 46, 2000, 150);
     lcd.clear();
     lcd.setCursor(4,0);
     lcd.print("INVALID");
     delay (1250);
     lcd.clear();
   break;
     
   case btnUP:
     --displayedln;
     tone( 46, 2000, 150);
     lcd.clear();
   break;
     
   case btnDOWN:
     ++displayedln;
     tone( 46, 2000, 150);
     lcd.clear();
   break;
     
   case btnSELECT:
     tone( 46, 2000, 150);
     lcd.clear();
     lcd.setCursor(4,0);
     lcd.print("INVALID");
     delay (1250);
     lcd.clear();
   break;
     
   case btnNONE:
   break;
   }


// baiased on the scrol variable display the correct scrol display.
switch (displayedln){
  case 0:
    lcd.setCursor(0,0);
    lcd.print("1. ");
    lcd.print(regs[9]);
    lcd.print(" Volts A");
    lcd.setCursor(0,1);
    lcd.print("2. ");
    lcd.print(regs[10]);
    lcd.print(" Volts B");
  break;

  case 1:
    lcd.setCursor(0,0);
    lcd.print("2. ");
    lcd.print(regs[10]);
    lcd.print(" Volts B");
    lcd.setCursor(0,1);
    lcd.print("3. ");
    lcd.print(regs[11]);
    lcd.print(" Volts C");
  break;

  case 2:
    lcd.setCursor(0,0);
    lcd.print("3. ");
    lcd.print(regs[11]);
    lcd.print(" Volts C");
    lcd.setCursor(0,1);
    lcd.print("4. ");
    lcd.print(regs[0]);
    lcd.print(" Device ID");
  break;

  case 3:
    lcd.setCursor(0,0);
    lcd.print("4. ");
    lcd.print(regs[0]);
    lcd.print(" Device ID");
    lcd.setCursor(0,1);
    lcd.print("5. ");
    lcd.print(regs[8]);
    lcd.print(" Read Hz");
  break;

  case 4:
    lcd.setCursor(0,0);
    lcd.print("5. ");
    lcd.print(regs[8]);
    lcd.print(" Read Hz");
    lcd.setCursor(0,1);
    lcd.print("6. ");
    lcd.print(regs[9]);
    lcd.print("% Ground Fault");
  break;

 // case 5:
    lcd.setCursor(0,0);
    lcd.print("6. ");
    lcd.print(var6);
    lcd.print(" Deg C.");
    lcd.setCursor(0,1);
    lcd.print("7. ");
    lcd.print(var7);
    lcd.print(" Deg C.");
  break;

 // case 6:
    lcd.setCursor(0,0);
    lcd.print("7. ");
    lcd.print(var7);
    lcd.print(" Deg C.");
    lcd.setCursor(0,1);
    lcd.print("8. ");
    lcd.print(var8);
    lcd.print(" Deg C.");
  break;

//  case 7:
    lcd.setCursor(0,0);
    lcd.print("8. ");
    lcd.print(var8);
    lcd.print(" Deg C.");
    lcd.setCursor(0,1);
    lcd.print("9. ");
    lcd.print(var9);
    lcd.print(" Deg C.");
  break;

  default:
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("INVALID");
    delay(1250);
    displayedln = 0;
    lcd.clear();
  break;
}
// print debug infoirmation
  Serial.print("Device ID: ");
  Serial.println(regs[0]);  
  Serial.print("Baud Rate: ");
  Serial.println(regs[1]);
  
  Serial.print("Volts A: ");
  Serial.println(regs[9]);
  Serial.print("Volts B: ");
  Serial.println(regs[10]);
  Serial.print("Volts C: ");
  Serial.println(regs[11]);
  
  delay(250);
  
    
  
 // regs[0] = analogRead(0); // update data to be written to arduino slave
  
 // analogWrite(LED, regs[0]>>2); // constrain adc value from the arduino slave to 255
}
