#include <dht.h>
#include <IRremote.h>
#include <LiquidCrystal.h>

// =================== //
// INITIALIZE HARDWARE //
// =================== //

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

// ================= //
// RUNTIME FUNCTIONS //
// ================= //

// function: calc f, display temp/humid
int T=0;
int H=0;
int F=0;
int lastTemp = millis();
// define degree symbol
const char deg = (char)223;  
// 
void showTemp(){
  LCD.setCursor(0,2);
  if( (( T==0)&&(H==0 )) || ( (abs(millis()-lastTemp))>7000) ) 
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
  LCD.print((char)37); // printing % this way because red box on github is annoying
  LCD.print("H");
}

// function: display seconds since boot 
unsigned long int t;
void showSecs(){
  // get time (sec) since boot
  t = millis();
  t = t/1000;
  // print seconds since boot
  LCD.setCursor(0,2);
  LCD.print(t);
}

// function: execute function designated to cycle by cFunc
unsigned short int cFunc = 1; // global, manipulated in loop(), default is temp
void function_selector(){
  switch(cFunc)
  {
    case 1: showTemp(); break;
    case 2: showSecs(); break;
    default:
      if(cFunc>2) cFunc=1; // wrap high to low
      if(cFunc<1) cFunc=2; // wrap low to high
      break; 
  }
}

// pointless loading function
void pLoad(LiquidCrystal &obj, short t){
  short tt = t/16;
  obj.setCursor(0,0);
  for(int i=0;i<16;i++)
  {
    obj.print(".");
    delay(tt);
  }
  obj.clear();
}

// ============== //
// MAIN FUNCTIONS //
// ============== //
 
void setup() {
  delay(250); 
  LCD.begin(16, 2);
  pLoad(LCD, 1440);
  LCD.setCursor(0,0); LCD.print("IR: (waiting)"); 
  IR.enableIRIn(); 
  delay(350); 
}

void loop() {
  // handle IR signal
  if (IR.decode(&results)) 
  {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("IR: ");
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
      case HND1: LCD.print("100+"); break; 
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
  function_selector(); // run "functions[cFunc]"
  delay(500); // wait 500ms before new cycle
}
