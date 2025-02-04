
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Define GPS module connections
#define RXPin 4
#define TXPin 3
#define GPSBaud 9600

// Target coordinates (modify as needed)
const double TARGET_LAT = 26.19264;  // Example Latitude
const double TARGET_LON = 91.69740;  // Example Longitude
const double RADIUS_THRESHOLD = 20.0; 

// GPS and Servo objects
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);
Servo payloadServo;
bool payloadDropped = false;  // To prevent multiple activations

// Haversine formula to calculate distance between two coordinates
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371000; // Earth radius in meters
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) + 
               cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) * 
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(GPSBaud);

    // Attach servo to pin 9
    payloadServo.attach(9);
    payloadServo.write(0); // Initial position (locked)
  
    Serial.println("GPS Payload Drop System Initialized");
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
        
        if (gps.location.isUpdated()) {
            double currentLat = gps.location.lat();
            double currentLon = gps.location.lng();

            Serial.print("Current Location: ");
            Serial.print(currentLat, 6);
            Serial.print(", ");
            Serial.println(currentLon, 6);

            // Calculate distance to target
            double distance = haversine(currentLat, currentLon, TARGET_LAT, TARGET_LON);
            Serial.print("Distance to target: ");
            Serial.print(distance);
            Serial.println(" meters");

            // If within threshold, release payload
            if (distance <= RADIUS_THRESHOLD && !payloadDropped) {
                Serial.println("Target reached! Dropping payload...");

                payloadServo.write(90);  // Rotate servo to 90 degrees
                delay(10000);            // Wait for 10 seconds (10,000 ms)

                Serial.println("Resetting servo position...");
                payloadServo.write(0);   // Reset servo back to 0 degrees
                payloadDropped = true;   // Mark as dropped to prevent multiple activations

                delay(5000); // Extra delay to avoid immediate reactivation
            }
        }
    }
}
