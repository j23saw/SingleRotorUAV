#include "src/config.h"
#include "src/dshot.h"
#include "src/BNO080.h" // IMU Library

#include <SPI.h>
#include <Servo.h>

BNO080 IMU;

/* Prepare two DSHOT outputs */
DShot ESC(2);

void setup() {
  Serial.begin(115200);

  if(IMU.beginSPI(IMU_CS_PIN, IMU_WAK_PIN, IMU_INT_PIN, IMU_RST_PIN) == false){
    Serial.println(F("BNO080 over SPI not detected."));
    while(1);
  }

  /* Enable continous stream of data from IMU */
  IMU.enableGyro( 3 );  // 2.5ms / 400hz
  IMU.enableRotationVector(5); // 5ms / 200Hz
  
  // configESCs();
  
}


uint16_t report_ID = 0;
float gx, gy, gz = 0;
float roll, pitch, yaw = 0;
uint8_t accuracy; 

void loop() {
  
  if (report_ID = IMU.getReadings())
  {
    // Attitude estimate
    if( report_ID == SENSOR_REPORTID_ROTATION_VECTOR ){
      roll = IMU.getRoll();
      pitch = IMU.getPitch();
      yaw = IMU.getYaw();
    }

    // Raw Gyro data
    if( report_ID == SENSOR_REPORTID_GYROSCOPE ) {
      IMU.getGyro(gx, gy, gz, accuracy);
      Serial.print(gx);
      Serial.print(",");
      Serial.println(gy);
    }
  }

}

void readRCReceiver( void ) {
  uint16_t roll = pulseIn( receiver_pins[0], HIGH);
  uint16_t pitch = pulseIn( receiver_pins[2], HIGH);
  uint16_t temp = pulseIn( receiver_pins[1], HIGH);
  uint16_t throttle = map(temp, 920, 1920, 0, 200) + 47;
}

void configESCs( void ) {

  Serial1.begin(115200);
  Serial2.begin(115200);

  /* Configure the DSHOT outputs */
  ESC.setup();

  /* Config the ESCs */
  ESC.setConfig( DSHOT_PORT_1, DSHOT_CMD_SPIN_DIRECTION_2 );
  ESC.setConfig( DSHOT_PORT_1, DSHOT_CMD_3D_MODE_OFF );

  ESC.setConfig( DSHOT_PORT_2, DSHOT_CMD_SPIN_DIRECTION_1 );
  ESC.setConfig( DSHOT_PORT_2, DSHOT_CMD_3D_MODE_OFF );

  delay(1000);

  /* Arm all DSHOT ports / ESCs */
  ESC.armMotors();
}
