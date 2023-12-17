#include <SoftwareSerial.h>

int RXPin = 2;
int TXPin = 3;
bool F1, F2, F3, F4, F5, F6,F7;
int GPSBaud = 9600;
char string[60];
int counter = 0;
SoftwareSerial gpsSerial(RXPin, TXPin);

char timeString[10];
float latitude, longitude;

void setup()
{
  F1 = false; F2 = false; F3 = false; F4 = false; F5 = false; F6 = false; F7 = false;
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
}

void parseAndPrintValues(const char *data) {
   sscanf(data, "%9[^,],%f,%*c,%f", timeString, &latitude, &longitude);

  if (0) {
    Serial.print("Time: ");
    Serial.println(timeString);
    Serial.print("Latitude: ");
    Serial.println(latitude, 6); // Print with 6 decimal places
    Serial.print("Longitude: ");
    Serial.println(longitude, 6); // Print with 6 decimal places
    Serial.println();
  //} else {
  //  Serial.println("Parsing error. Not all values were extracted.");
  }
}

void loop()
{
  while (gpsSerial.available() > 0)
  {
    char c = gpsSerial.read();

    switch (c)
    {
      case '$':
        F1 = true;
        counter = 0; // Reset counter when a new sentence starts
        break;
      case 'G':
        F2 = true;
        break;
      case 'P':
        F3 = true;
        break;
      case 'R':
        F4 = true;
        break;
      case 'M':
        F5 = true;
        break;
      case 'C':
        F6 = true;
        break;

      case ',':
        F7 = true;
        break;        
      case '\n':
        // End of sentence, process the data
        if (F1 && F2 && F3 && F4 && F5 && F6&&F7)
        {
          string[counter] = '\0'; // Null-terminate the string
          parseAndPrintValues(string);
          // Reset flags
          Serial.println(string);
          F1 = false; F2 = false; F3 = false; F4 = false; F5 = false; F6 = false;F7 = false;
        }
        break;
      default:
        if (counter < 59) // Avoid buffer overflow
        {
          string[counter++] = c;
        }
        break;
    }
  }
}
