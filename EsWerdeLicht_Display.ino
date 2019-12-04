/*
    hejo1037  
    Informatik Labor --- Miniprojekt --- Hochschule Karlsruhe
    Eine Abblendlicht ein/aus-schalt Automatik in Abhängigkeit der Helligkeit mit Info-Display
*/

#include <SPI.h>                                                            /* Einbinden der Bibliotheken*/
#include <Wire.h>                               
#include <Adafruit_SSD1306.h>                                               /*Bibliotheken für den Display*/
#include <Adafruit_GFX.h>


/*************************************/
#define OLED_RESET 4                                                        /* OLED Display initialisieren*/
#define SCREEN_WIDTH 128                                                    /*128 x 32 Pixel*/
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




/*************************************/                                                 
                         
                                                                            /*Bei ESP 32:  SCL= PIN 22 ... SDA= PIN 21*/

int Relais = 19;                                                            /*GPIO19 = G19 = Pin19*/
int Licht;                                                                  /*Licht ist der Messpin wo die Mitte des Spannungsteilers auf Pin 26 (GPIO 26) liegt*/
int Licht2;                                                                 /* Licht2 -> zweiter LDR wird an GPIO 27 ausgelesen*/
int z;

void Startbildschirm();
void AN();                                                                  /*Funktionen für Bildschirm*/
void AUS();     

/*************************************/
void setup()                                                                /*Setup-Funktion für Hardware (Display und Sensor/Output)*/
{ 
 
  Serial.begin(115200);                                                     /* baudrate von Serielle Funktionen*/
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);                                /* Display Starten*/
  display.display();
  
  Startbildschirm();                                                        /* Funktion für Startbildschirm ausführen*/
  delay(1000);                                                              /* 1 sek Warten*/
  display.clearDisplay();                                                   /* Darstellung löschen*/

  pinMode (Relais, OUTPUT);                                                 /*Pin 26 ( bei uns Namens Relais) als Ausgang definiert*/

  
}

/*************************************/
void loop()
{ 
Licht = analogRead (26);                                                    /* Pin 26 und 27 analog auslesen*/
Licht2 = analogRead (27);
                
                                                                            /*0 - 4095 Werte (12bit) Aufloesung bei max 3.3V...  wegen 1k Ohm Spannungsteiler*/
if (Licht > 1000 || Licht2 > 1000)                                          /*Solange der Wert von "Licht" größer als 1000 ist ...*/
{                                                                           
 
  AN(); 
 

  Serial.print ("Licht2 an:"); Serial.println(analogRead(27));               /*analog.Read gibt den Messwert im SeriellenMonitor aus (4096) Serial.print sagt uns*/
  Serial.print ("Licht an:");  Serial.println(analogRead(26));               /*was gerade gemessen und geschalten wirdim Seriellen Monitor*/
  digitalWrite (Relais,HIGH);                                               /* Pin 26 auf HIGH (3,3 Volt) */                                                                                                       
  delay(10000);                                                             /*20sekunden verzögerung um flackern(z.B. im Tunnel oder Allee) zu überbrücken*/
  
}
                                            
                                               
else 
{   
  
  Serial.print ("Licht2 aus:"); Serial.println(analogRead(27)); 
  Serial.print ("Licht aus:");  Serial.println(analogRead(26));
  digitalWrite (Relais,LOW);
  AUS();                                                                    /* Funktion für Bildschirmausgabe*/
}
}
/*************************************/
void AN()                                                                   /* "Licht an" auf Display schreiben*/
{ 
 
  display.clearDisplay();                                                   /*Sicherstellen dass nicht auf eine alte Darstellung geschrieben wird*/
  display.setTextColor(SSD1306_WHITE);                                      /* Text Farbe ( in diesem Fall weiss)*/           
  display.setTextSize(2);                                                   /*Textgröße (doppelte Textgröße)*/
  display.setCursor(5,0);                                                   /*Position wo der Text beginnen soll (x,y)*/
  display.println("Licht an");                                              /*Text*/

      z = (-analogRead(27)/34)+120;
      display.drawRect(2,20,z,4, SSD1306_WHITE);          
    
      z = (-analogRead(26)/34)+120;                                         /* geteilt durch 34 um 120 einheiten zu erhalten ( 124 Pixel Bildschirm breite)*/
     display.drawRect(2,28,z,4, SSD1306_WHITE);                             /* leere Rechtecke zeichnen (als Lichtintensitäts anzeige/Balken)(x,y,breite,höhe,farbe)*/

    display.display();                                                      /* Darstellen */

}
/*************************************/
void AUS()                                                                   /* "Licht aus" auf display schreiben*/
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5,0);
  display.setTextSize(2);
  display.println("Licht aus");
  
      z = (-analogRead(27)/34)+120;
      display.drawRect(2,20,z,4, SSD1306_WHITE);
   
      z = (-analogRead(26)/34)+120;
      display.drawRect(2,28,z,4, SSD1306_WHITE);   
      display.display();  

}

/**************************************/
  void Startbildschirm()                                                     
  {
  display.clearDisplay();

  display.setTextSize(2);                                                    
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 0);
  display.println("AutoLicht");
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30,20);
  display.println("hejo1037");
  
  display.display();                                 

  display.startscrollleft(0x00, 0x0F);                                       /* Text nach links Scrollen*/
  delay(500);                                                                /* Scrolldauer*/
  display.stopscroll();                                                      /* Scrollen beenden*/
  
}
  
