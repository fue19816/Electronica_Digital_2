
// include the SD library:
#include <SPI.h>
#include <SD.h>

File gato;
File voidh;
File tri;

Sd2Card card;
SdVolume volume;
SdFile root;

int opcion;
int bandera = 0;
const int chipSelect = PA_3; //cs PIN

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.setModule(0);

  pinMode(PA_3, OUTPUT);
  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  Serial.print("\nInitializing SD card...");
  
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

    // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  
}


void loop(void) {
  if(bandera == 0){
    bandera = 1;
    Serial.println("\nArchivos encontrados en la tarjeta: ");
    root.openRoot(volume);
    root.ls(LS_R | LS_DATE | LS_SIZE);                  //Se listan los archivos contenidos en la tarjeta SD
    Serial.println("Presione 1 para abrir el primer archivo, 2 para el segundo archivo y 3 para el tercer archivo");
  }

  while(Serial.available()){
    int opcion = (int)Serial.read();
    Serial.println("I recieved:");
    Serial.print(opcion);
    
    if(opcion >= 49 && opcion <= 51){
      switch(opcion){
        case 49:
        delay(1000);
        gato = SD.open("gato.txt");
        if (gato) {
          delay(1000);
          Serial.println("gato.txt:");

          while(gato.available()){
            Serial.write(gato.read());
          }

          gato.close();
        }
        else{
          Serial.println("\nNo se pudo abrir el archivo");
        }
        delay(250);
        break;

        case 50:
        delay(1000);
        tri = SD.open("tri.txt");
        if (tri) {
          delay(1000);
          Serial.println("tri.txt:");

          while(tri.available()){
            Serial.write(tri.read());
          }

          tri.close();
        }
        else{
          Serial.println("\nNo se pudo abrir el archivo");
        }
        delay(250);
        break;

        case 51:
        delay(1000);
        voidh = SD.open("void.txt");
        if(voidh){
          Serial.println("void.txt");


          while(voidh.available()){
            Serial.write(voidh.read());
          }

          voidh.close();
        }
        else{
          Serial.println("\nNo se pudo abrir el archivo");
        }
        delay(250);
        break;
      }
    }
  }
}
