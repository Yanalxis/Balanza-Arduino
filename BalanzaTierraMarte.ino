//Integrantes:
//George Elder Beltran Alcocer
//Brayan Alexis Arenas Forero

#include "HX711.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

const int DOUT=A1; //Pin DIgital HX711
const int CLK=A0; // /Pin DIgital HX711

HX711 balanza; // Creación del objeto balanza de tipo HX711

static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };
void setup() {
    Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
 
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningun  objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  ScrollUPB();
  balanza.set_scale(439430.25); // Establecemos la escala
  balanza.tare(20);  //El peso actual es considerado Tara.
  Serial.println("Listo para pesar"); 
  saludar(); 
  
}

void loop() {

  float pesoTierra = ConvertirKgG(balanza.get_units_earth(20));  // Convierte las unidades de kilogramos a gramos 
  float pesoMarte = ConvertirKgG(balanza.get_units_mars(20)); //Convierte las unidades de kilogramos a gramos 
 
  PesoTierra(pesoTierra);//llama al metodo pesoTierra para mostrar en el display el peso del objeto en la Tierra
  PesoMarte(pesoMarte);//Llama al metodo pesoMarte para mostrar en el display el peso del objeto en el planeta Marte
}

//Muestra en pantallael mensaje UPB como Scroll
void ScrollUPB(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("UPB"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

//Muestra en pantalla el mensaje "Hola!"
void saludar(void)
{
  display.clearDisplay();//Limpia la pantalla LCD OLED 
  display.setTextSize(2); // Escala del teto
  display.setTextColor(SSD1306_WHITE); //Definir el color del texto
  display.setCursor(10, 0);//Ubicacion del texto en el OLED LCD
  display.println(F("Hola!")); //Imprime en el display el mensaje
  display.display();      // Show initial text
  delay(500);//Retardo
}

// Peso Gramo
// Descripción:  Recibe como parametro de entrada el peso en kilogramos de tipo float, Retorna el parametro peso en gramos de tipo floar .
float ConvertirKgG(float peso)
{
  float pesoGramo = peso*1000; //conversion de kilogramos a gramos
  return pesoGramo;
}

// Peso Tierra
// Descripción:  Recibe como parametro de entrada el peso del objeto en la Tierra, no retorna nada.
//               El metodo PesoTierra calcula el peso del objeto en la Tierra, en Kilogramos. 
//               Imprime en el OLED 128x32 y en el puerto serial el peso registrado 
void PesoTierra(float pesoTierra)
{
  display.clearDisplay();//Limpia la pantalla LCD OLED 
  display.setTextSize(1); // Escala del teto
  display.setTextColor(SSD1306_WHITE); //Definir el color del texto
  display.setCursor(10, 0);//Ubicacion del texto en el OLED LCD
  
  Serial.print("Peso Tierra: ");//Imprime mensaje en el puerto serial  
  Serial.print(pesoTierra,3);//Imprime el peso en el puerto serial
  Serial.print(" g");  
  delay(800);//Retardo
  
  display.println(F("Peso Tierra: ")); //Imprime en el display el mensaje
  display.print(pesoTierra);//Imprime en el display el mensaje
  display.print(" g");//Imprime en el display el mensaje
  display.display();      // Show initial text
  delay(800);//Retardo
}

// Peso Marte
// Descripción:  Recibe como parametro de el peso en la Tierra, no retorna nada.
//               El metodo PesoMarte calcula el peso del objeto en Marte, en Kilogramos
void PesoMarte(float pesoMarte)
{
  display.clearDisplay();//Limpia la pantalla LCD OLED 
  display.setTextSize(1); // Escala del teto
  display.setTextColor(SSD1306_WHITE); //Definir el color del texto
  display.setCursor(10, 0);//Ubicacion del texto en el OLED LCD
  Serial.println("Peso Marte: ");//Imprime en el puerto serial
  Serial.print(pesoMarte,3);//Imprime en el puerto serial
  Serial.print(" g");//Imprime en el puerto serial
  delay(500);//Retardo
  
  display.println(F("Peso Marte: "));//Imprime en el display el mensaje
  display.print(pesoMarte);//Imprime en el display el mensaje
  display.print(" g");//Imprime en el display el mensaje
  display.display();      // Show initial text
  delay(500);//Retardo
}
