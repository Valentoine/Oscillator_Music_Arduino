#include <SPI.h>

int CS = 7;
int LDAC = 8;

#define DAC_FLAGS_A 0b00110000 //Select DAC channel A (MCP 4921 or (MCP 4922)
#define DAC_FLAGS_B 0b10110000 //Select DAC channel B (only on MCP 4922)

int DO = 33;
int RE = 29;
int MI = 31;
int FA = 27;
int SOL = 37;
int LA = 2;
int SI = 3;
int DO2 = 4;

int changer_oct = 45;
int changer_forme = 12;

int taille_tab = 700;
int * tab_essai;
int i = 0;

double tab_freq [8] = { 25  , 21 , 17 , 15 , 11 , 8 , 5 , 4 };
double octave = 1;
String forme_onde = "sinu";


void setup() {

  tab_essai = (int*) malloc (sizeof(int)*taille_tab);
  changer_tab(forme_onde);

  pinMode(DO, INPUT);
  pinMode(RE, INPUT);
  pinMode(MI, INPUT);
  pinMode(FA, INPUT);
  pinMode(SOL, INPUT);
  pinMode(LA, INPUT);
  pinMode(SI, INPUT);
  pinMode(DO2, INPUT);

  pinMode(changer_oct, INPUT);
  pinMode(changer_forme, INPUT);

  pinMode(CS, OUTPUT);
  pinMode(LDAC, OUTPUT);
  digitalWrite(CS, HIGH);
  digitalWrite(LDAC, LOW);
  SPI.begin();
}

int j = 0;

void loop() {

  if (digitalRead(DO) == HIGH)
  {jouer_note(tab_freq[0], DO);}

  if (digitalRead(RE) == HIGH)
  {jouer_note(tab_freq[1], RE);}

  if (digitalRead(MI) == HIGH)
  {jouer_note(tab_freq[2], MI);}

  if (digitalRead(FA) == HIGH)
  {jouer_note(tab_freq[3], FA);}

  if (digitalRead(SOL) == HIGH)
  {jouer_note(tab_freq[4], SOL);}

  if (digitalRead(LA) == HIGH)
  {jouer_note(tab_freq[5], LA);}

  if (digitalRead(SI) == HIGH)
  {jouer_note(tab_freq[6], SI);}

  if (digitalRead(DO2) == HIGH)
  {jouer_note(tab_freq[7], DO2);}

  if(digitalRead(changer_oct) == HIGH)
  {
    if(octave<=3)
    {octave+=1;taille_tab/=2;
    changer_tab(forme_onde);}
    else 
    {octave=1;taille_tab=80;
    changer_tab(forme_onde);}
      
    delay(500);
  }

  if(digitalRead(changer_forme) == HIGH)
  {
    if(forme_onde == "sinu")
    {forme_onde = "triangle";
    changer_tab(forme_onde);}
    else if(forme_onde == "triangle")
    {forme_onde = "scie";
    changer_tab(forme_onde);}
    else if(forme_onde=="scie")
    {forme_onde = "carre";
    changer_tab(forme_onde);}
    else
    {forme_onde = "sinu";
    changer_tab(forme_onde);}
      
    delay(500);
  }
}

void changer_tab(String onde)
{
  if(onde == "sinu")
  {
    for (int j = 0; j <taille_tab; j++)
    {
        tab_essai[j] = 2000 + 1000 * cos(2 * PI  * j/taille_tab);
        
    }
  }
  else if(onde == "carre")
  {
    for (int j = 0; j <taille_tab/2; j++)
    {
        tab_essai[j] = 3000;
    }
    for (int j = taille_tab/2; j <taille_tab; j++)
    {
        tab_essai[j] = 1000;
    }
  }
  else if(onde == "triangle")
  {
    for (int j = 0; j <taille_tab/4; j++)
    {
        tab_essai[j] = 2000 + 4000/taille_tab*j;
    }
    for (int j = 0; j <taille_tab/2; j++)
    {
        tab_essai[j] = 3000 - 4000/taille_tab*j;
    }
    for (int j = 0; j <taille_tab/4; j++)
    {
        tab_essai[j] = 1000 + 4000/taille_tab*j;
    }
  }
  else if(onde == "scie")
  {
    for (int j = 0; j <taille_tab/2; j++)
    {
        tab_essai[j] = 2000 + 4000/taille_tab*j;
    }
    for (int j = 0; j <taille_tab/2; j++)
    {
        tab_essai[j] = 1000 + 4000/taille_tab*j;
    }
  }
  delay(500);
}

void jouer_note(double freq, int pin)
{
  while (digitalRead(pin) == HIGH)
  {
    for (int j = 0; j < taille_tab; j++)
      {
        sendToDAC('A',tab_essai[j]);
        delayMicroseconds(freq);
      }
  }
}


void sendToDAC(char channel, int val) {
  byte lVal = val;
  byte hVal = (val >> 8);
  noInterrupts();
  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  digitalWrite(CS, LOW);
  if (channel == 'A') {
    SPI.transfer(hVal | DAC_FLAGS_A); //Send data to DAC output 'A' (MCP4921 pin 8 or MCP4922 pin 14)
  } else {
    SPI.transfer(hVal | DAC_FLAGS_B); //Send data to DAC output 'B' (MCP4922 pin 10)
  }
  SPI.transfer(lVal);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
  interrupts();
}
