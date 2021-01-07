#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

BLEService AccelerationService("1111");
BLEUnsignedCharCharacteristic AccServiceChar("2100", BLERead | BLENotify); // Acclerometer
//BLEUnsignedCharCharacteristic AccServiceChar1("2101", BLERead | BLENotify);
//BLEUnsignedCharCharacteristic AccServiceChar2("2102", BLERead | BLENotify);

float x, y, z;
float acc;
int acc1;
int btconnected = 0;

void getIMUvalues()
{
  if (IMU.accelerationAvailable()) 
  {
    IMU.readAcceleration(x, y, z);


    acc=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    acc1=acc*100;
  }

  /*Serial.print("AY value: ");
  Serial.println(ay);
  Serial.print("AZ value: ");
  Serial.println(az);*/
}



void initBLE()
{
  while (!Serial);
  Serial.println("Started");

  if(!BLE.begin())
  {
    Serial.println("BLE start failed!");
    while(1);
  }

  if (!IMU.begin()) 
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Init serial communication, set local name for BLE device
  BLE.setLocalName("Accelerationservice");
  BLE.setAdvertisedService(AccelerationService);
  AccelerationService.addCharacteristic(AccServiceChar);
 
  BLE.addService(AccelerationService);

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void advertiseBLE() // Send BLE data
{
  // Start advertising BLE 
  BLEDevice central = BLE.central();
  if(central)
  {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    btconnected = 1;

    while(central.connected()) // Continiously send data to connected device
    {
      getIMUvalues();

      Serial.print("acc value: ");
      Serial.println(acc1);
      
      AccServiceChar.writeValue(acc1); 
      delay(20);
    }
  }
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
  btconnected = 0;
}

void setup() {

  initBLE();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  advertiseBLE();
}
