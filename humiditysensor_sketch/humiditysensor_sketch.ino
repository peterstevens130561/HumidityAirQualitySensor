//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
// uses Liquid Crystel I2c by Frank de Brabander
//https://github.com/johnrickman/LiquidCrystal_I2C

// DFRobot ENS160
// https://github.com/DFRobot/DFRobot_ENS160
#include <DFRobot_ENS160.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!123");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!123");
   lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");

   Serial.begin(115200);


  // Init the sensor
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Found sensor");

    /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  delay(5000);

  /**
   * Users write ambient temperature and relative humidity into ENS160 for calibration and compensation of the measured gas data.
   * ambientTemp Compensate the current ambient temperature, float type, unit: C
   * relativeHumidity Compensate the current ambient humidity, float type, unit: %rH
   */
  ENS160.setTempAndHum(/*temperature=*/18, /*humidity=*/55);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("AQI");
  lcd.setCursor(0,1);
  lcd.print("TVOC");
  lcd.setCursor(0,2);
  lcd.print("Co2");

}



void loop()
{
  Serial.print("New Sample\n");
  

  printMeasurement(0,String(ENS160.getAQI()));
  printMeasurement(1,String(ENS160.getTVOC()));
  printMeasurement(2,String(ENS160.getECO2()));

  measure();
  delay(5000);
}

void printMeasurement(uint8_t row, String value) {
  lcd.setCursor(6,row);
  lcd.print(value);
}

uint8_t AirQualityIndex() {
    uint8_t AQI = ENS160.getAQI();
  Serial.print("Air quality index : ");
  Serial.println(AQI);

}

    /**
   * Get TVOC concentration
   * Return value range: 0–65000, unit: ppb
   */
uint8_t ConcentrationOrganicParticles() {

  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");
}
void measure()
{
  /**
   * Get the sensor operating status
   * Return value: 0-Normal operation, 
   *         1-Warm-Up phase, first 3 minutes after power-on.
   *         2-Initial Start-Up phase, first full hour of operation after initial power-on. Only once in the sensor’s lifetime.
   * note: Note that the status will only be stored in the non-volatile memory after an initial 24h of continuous
   *       operation. If unpowered before conclusion of said period, the ENS160 will resume "Initial Start-up" mode
   *       after re-powering.
   */
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  /**
   * Get the air quality index
   * Return value: 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
   */
  uint16_t AQI = ENS160.getAQI();
  Serial.print("Air quality index : ");
  Serial.println(AQI);



  /**
   * Get TVOC concentration
   * Return value range: 0–65000, unit: ppb
   */
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  /**
   * Get CO2 equivalent concentration calculated according to the detected data of VOCs and hydrogen (eCO2 – Equivalent CO2)
   * Return value range: 400–65000, unit: ppm
   * Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800 - 1000), 
   *               Poor(1000 - 1500), Unhealthy(> 1500)
   */
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  Serial.println();
  delay(1000);
}