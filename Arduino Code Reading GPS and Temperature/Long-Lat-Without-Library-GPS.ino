#include <SoftwareSerial.h>
#include <math.h>
#define NTC_PIN A0
int RXPin = 2;
int TXPin = 3;
bool F1, F2, F3, F4, F5, F6, F7;
int GPSBaud = 9600;
char string[60];
int counter = 0;
double temperature = 0;
SoftwareSerial gpsSerial(RXPin, TXPin);
SoftwareSerial espSerial(6,5);
char timeString[10];
char latitudeString[11];
char longitudeString[11];
#if 0
void parseAndPrintValues(const char *time, const char *latitude, const char *longitude) {
  // Add 3 hours to the time
  int hours = (time[0] - '0') * 10 + (time[1] - '0') + 3;
  if (hours >= 24) {
    hours -= 24;
  }

//  Serial.print("Time: ");
//  Serial.print(hours / 10);
//  Serial.print(hours % 10);
//  Serial.print(":");
//  Serial.print(time[2]);
//  Serial.print(time[3]);
//  Serial.print(":");
//  Serial.print(time[4]);
//  Serial.print(time[5]);
//  Serial.println();

  float latitudeValue = atof(latitude);
  int degreesLat = int(latitudeValue / 100);
  float minutesLat = fmod(latitudeValue, 100);

//  Serial.print("Latitude: ");
  Serial.print(degreesLat);
  Serial.print("-");
  Serial.print(minutesLat, 5); // Print with 5 decimal places for minutes
  Serial.print("#");

//  espSerial.print("Latitude: ");
//  espSerial.print(degreesLat);
//  espSerial.print("° ");
//  espSerial.print(minutesLat, 5); // Print with 5 decimal places for minutes
//  espSerial.println();

  float longitudeValue = atof(longitude);
  int degreesLon = int(longitudeValue / 100);
  float minutesLon = fmod(longitudeValue, 100);

  //Serial.print("Longitude: ");
  Serial.print(degreesLon);
  Serial.print("-");
  Serial.print(minutesLon, 5); // Print with 5 decimal places for minutes
  Serial.print("#");
  //Serial.println();

//  espSerial.print("Longitude: ");
//  espSerial.print(degreesLon);
//  espSerial.print(" ");
//  espSerial.print(minutesLon, 5); // Print with 5 decimal places for minutes
//  espSerial.println();
//  espSerial.println();

  //Serial.print("Temperature:: ");
  Serial.print(temperature);
  Serial.print("-");
  Serial.println("#");

//  espSerial.print("Temperature:: ");
//  espSerial.print(temperature);
//  espSerial.print("°C ");
//  espSerial.println();
  
}

#endif
void parseAndPrintValues(const char *time, const char *latitude, const char *longitude) {
  float latitudeValue = atof(latitude);
  int degreesLat = int(latitudeValue / 100);
  float minutesLat = fmod(latitudeValue, 100);

  float longitudeValue = atof(longitude);
  int degreesLon = int(longitudeValue / 100);
  float minutesLon = fmod(longitudeValue, 100);

  // Concatenate latitude and longitude values into a single string
  String combinedData = String(degreesLat) + "°" + String(minutesLat, 5) + "#" +
                        String(degreesLon) + "°" + String(minutesLon, 5) + "#" + String (temperature)+"*";

  // Print the combined data using Serial.print
  Serial.println(combinedData);


  // Uncomment the following line if you want to add a newline character at the end
  // Serial.println();
}






void setup() {
  F1 = false; F2 = false; F3 = false; F4 = false; F5 = false; F6 = false; F7 = false;
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  //espSerial.begin(9600);
}

void loop() {

  //espSerial.println("Hello World");
  //Serial.println("deneme");
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    int deger = analogRead(NTC_PIN);
    temperature = Termistor(deger);
    switch (c) {
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
        if (F1 && F2 && F3 && F4 && F5 && F6 && F7) {
          string[counter] = '\0'; // Null-terminate the string
          parseAndPrintValues(timeString, latitudeString, longitudeString);
          // Reset flags
          F1 = false; F2 = false; F3 = false; F4 = false; F5 = false; F6 = false; F7 = false;
        }
        break;
      default:
        if (counter < 59) // Avoid buffer overflow
        {
          string[counter++] = c;
          if (F7) {
            // Depending on the position of the comma, assign characters to latitude or longitude
            if (counter >= 11 && counter <= 20) {
              latitudeString[counter - 11] = c;
            } else if (counter >= 23 && counter <= 32) {
              longitudeString[counter - 23] = c;
            } else if (counter <= 6) {
              timeString[counter - 1] = c;
            }
          }
        }
        break;
    }
  }
}

double Termistor(int analogOkuma)
{
  double sicaklik;
  sicaklik = log(((10240000 / analogOkuma) - 10000));
  sicaklik = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * sicaklik * sicaklik )) * sicaklik );
  sicaklik = sicaklik - 273.15;
  return sicaklik;
}
