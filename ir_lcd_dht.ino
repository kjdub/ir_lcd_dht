#include <dht.h>
#include <IRremote.h>
#include <LiquidCrystal.h>

// INITIALIZE HARDWARE
// ===================

// define pin and object for DHT11
#define dht_pin 2 
dht DHT;

// define IR receiver pin and object
int RECV_PIN = 3;
IRrecv IR(RECV_PIN);
decode_results results;
// define remote buttons :(
#define CH 16736925
#define CH_U 16769565
#define CH_D 16753245
#define NEXT 16712445
#define PREV 16720605
#define PLAY 16761405
#define VOL_U 16754775
#define VOL_D 16769055
#define EQ 16748655
#define ZERO 16738455
#define HND1 16750695
#define HND2 16756815
#define ONE 16724175
#define TWO 16718055
#define THREE 16743045
#define FOUR 16716015
#define FIVE 16726215
#define SIX 16734885
#define SEVEN 16728765
#define EIGHT 16730805
#define NINE 16732845
#define HOLD 4294967295 // received when any button is held on this remote

// initialize display (RS,E,D4,D5,D6,D7)
LiquidCrystal LCD(7,8,9,10,11,12);

// RUNTIME FUNCTIONS
// =================

// temperature/humidity display
int T=0;
int H=0;
int F=0;
int lastTemp = millis();
//
const char deg = (char)223; // define degree symbol
//
void showTemp()
{
  LCD.setCursor(0,2);
  if( (( T==0)&&(H==0 )) || ( (abs(millis()-lastTemp))>2000) ) 
  { 
    DHT.read11(dht_pin); 
    T = (int)DHT.temperature;
    H = (int)DHT.humidity;
    F = (DHT.temperature*1.8)+32; 
  }

  LCD.print(T);
  LCD.print(deg);
  LCD.print("C (");
  LCD.print(F);
  LCD.print(deg);
  LCD.print("F) ");
  LCD.print(H);
  LCD.print("%H");
}

// display seconds since boot function
unsigned long int t;
void showSecs()
{
  // get time (sec) since boot
  t = millis();
  t = t/1000;
  
  // print seconds since boot
  LCD.setCursor(0,2);
  LCD.print(t);
}

// chose which function to run in current cycle based on remote input
unsigned short int cFunc = 1; // global, manipulated in loop(), used in function_selector
void function_selector(){
  switch(cFunc)
  {
    case 1: showTemp(); break;
    case 2: showSecs(); break;
    default:
      if(cFunc>2) cFunc=1; // wrap to lowest beyond highest
      if(cFunc<1) cFunc=2; // wrap to highest beyond lowest
      break; 
  }
}

// pointless loading function
void pLoad(LiquidCrystal &obj) {
  obj.setCursor(0,0);
  for(int i=0;i<16;i++)
  {
    obj.print(".");
    delay(100);
  }
  obj.clear();
}

// MAIN FUNCTIONS
// ==============
 
void setup() {
  delay(250); 
  Serial.begin(9600);
  LCD.begin(16, 2);
  pLoad(LCD);
  IR.enableIRIn(); 
  LCD.setCursor(0,0);
  LCD.print("IR: (waiting)"); 
  delay(250); 
}

void loop() {
  // handle IR signal
  if (IR.decode(&results)) 
  {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("IR: ");
    //LCD.print(results.value);
    switch(results.value)
    {
      case CH: LCD.print("CH"); break;
      case CH_U: LCD.print("CH+"); cFunc++; break; // next function
      case CH_D: LCD.print("CH-"); cFunc--; break; // previous function
      case NEXT: LCD.print(">>|"); break;
      case PREV: LCD.print("|<<"); break;
      case PLAY: LCD.print("Play/Pause"); break;
      case VOL_U: LCD.print("VOL+"); break;
      case VOL_D: LCD.print("VOL-"); break;
      case EQ: LCD.print("EQ"); break;
      case ZERO: LCD.print("0"); break;
      case HND1: LCD.print("100+"); break; // run temp/humid
      case HND2: LCD.print("200+"); break;
      case ONE: LCD.print("1"); break;
      case TWO: LCD.print("2"); break;
      case THREE: LCD.print("3"); break;
      case FOUR: LCD.print("4"); break;
      case FIVE: LCD.print("5"); break;
      case SIX: LCD.print("6"); break;
      case SEVEN: LCD.print("7"); break;
      case EIGHT: LCD.print("8"); break;
      case NINE: LCD.print("9"); break;
      case HOLD: LCD.print("(btn held)"); break;
      default: LCD.print(results.value);
    }
    IR.resume();
  }

  function_selector(); // runs cFunc

  // wait 1/2 second
  delay(500); 
}
