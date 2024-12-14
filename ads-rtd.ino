#include <SPI.h>

const int ADResetPulse = 1;  // ADC reset (min. 4x244 = 976 ns, low), 244 ns = 1/4,096 MHz;
const int ADResetDelay = 1;  // ADC delay afrter reset in ms min 0.6 ms
int mux1settings;
int a = 1;

const int CS = 10;  //clock pin

const int DRDY = 9;   //data ready pin
const int START = 8;  //start pin
const int RST = 7;    //data ready pin

#define MUX0 0x00   //Multiplexer Control Register 0
#define VBIAS 0X01  //Bias Voltage Register
#define MUX1 0x02   //Multiplexer Control REgister 1
#define SYS0 0x03   //System Control Register 0

#define MUX_SP2_AIN0 B00000000  // 000 AIN0 (default)
#define MUX_SP2_AIN1 B00001000  // 001 AIN1
#define MUX_SP2_AIN2 B00010000  // 010 AIN2
#define MUX_SP2_AIN3 B00011000  // 011 AIN3

#define MUX_SN2_AIN0 B00000000  // 000 AIN0
#define MUX_SN2_AIN1 B00000001  // 001 AIN1 (default)
#define MUX_SN2_AIN2 B00000010  // 010 AIN2
#define MUX_SN2_AIN3 B00000011  // 011 AIN3

#define WAKEUP 0x00  //Exit Sleep Mode
#define SLEEP 0x01   //Enter Sleep Mode
#define SYNC 0x04    //Synchornize the A/D Conversion
#define RESET 0x06   //Reset To Power UP values
#define NOP 0xFF     //NO Operation
/*DATA READ*/
#define RDATA 0x12   //Read data once
#define RDATAC 0x14  //Read data continously
#define SDATAC 0x16  //Stop reading data continously
/*READ REGISTER */
#define RREG 0x20  //Read From Register
#define WREG 0x40  //Write To Register
/*Calibration */
#define SYSOCAL 0x60   //System Offset Calibration
#define SYSGCAL 0x61   //System Gain Calibration
#define SELFOCAL 0x62  //Self Offset Calibration

#define PGA2_0 B00000000    // 1 (default)
#define PGA2_2 B00010000    // 2
#define PGA2_4 B00100000    // 4
#define PGA2_8 B00110000    // 8
#define PGA2_8 B00110000    // 8
#define PGA2_16 B01000000   // 16
#define PGA2_32 B01010000   // 32
#define PGA2_64 B01100000   // 64
#define PGA2_128 B01110000  // 128

#define DOR3_5 B00000000    // 5SPS (default)
#define DOR3_10 B00000001   // 10SPS
#define DOR3_20 B00000010   // 20SPS
#define DOR3_40 B00000011   // 40SPS
#define DOR3_80 B00000100   // 80SPS
#define DOR3_160 B00000101  // 160SPS

#define VREFCON1_OFF B00000000  // Internal reference is always off (default)
#define VREFCON1_ON B00100000   // Internal reference is always on
#define VREFCON1_PS B01100000   // Internal reference is on when a conversion is in progress

/* REFSELT1:0 These bits select the reference input for the ADC  */
#define REFSELT1_REF0 B00000000     // REF0 input pair selected (default)
#define REFSELT1_REF1 B00001000     // REF1 input pair selected
#define REFSELT1_ON B00010000       // Onboard reference selected
#define REFSELT1_ON_REF0 B00011000  // Onboard reference selected and internally connected to REF0 input pair

#define VBIAS_3 B00001000  // AIN3
#define VBIAS_2 B00000100  // AIN2
#define VBIAS_1 B00000010  // AIN1
#define VBIAS_0 B00000001  // AIN0

#define IDAC0 0x0A  //IDAC Control Register 0
#define IDAC1 0x0B  //IDAC Control Register 1

#define I1DIR_AIN0 B00000000   // AIN0
#define I1DIR_AIN1 B00010000   // AIN1
#define I1DIR_AIN2 B00100000   // AIN2
#define I1DIR_AIN3 B00110000   // AIN3
#define I1DIR_IEXT1 B10000000  // IEXT1
#define I1DIR_IEXT2 B10010000  // IEXT2
#define I1DIR_OFF B11000000

#define I2DIR_AIN0 B00000000   // AIN0
#define I2DIR_AIN1 B00000001   // AIN1
#define I2DIR_AIN2 B00000010   // AIN2
#define I2DIR_AIN3 B00000011   // AIN3
#define I2DIR_IEXT1 B00001000  // IEXT1
#define I2DIR_IEXT2 B00001001  // IEXT2
#define I2DIR_OFF B00001100

//The IMAG bits control the magnitude of the excitation current. The IDACs require the internal reference to be on.  */
#define IMAG2_OFF B00000000   // off (default)
#define IMAG2_50 B00000001    // 50uA
#define IMAG2_100 B00000010   // 100uA
#define IMAG2_250 B00000011   // 250uA
#define IMAG2_500 B00000100   // 500uA
#define IMAG2_750 B00000101   // 750uA
#define IMAG2_1000 B00000110  // 1000uA
#define IMAG2_1500 B00000111  // 1500uA

#define SPI_SPEED 4000000

unsigned long GetRegisterValue(uint8_t regAdress) {
  uint8_t bufr;
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
  digitalWrite(CS, LOW);
  delayMicroseconds(1);
  SPI.transfer(RREG | regAdress);  // send 1st command byte, address of the register
  SPI.transfer(0x00);              // send 2nd command byte, read only one register
  delayMicroseconds(1);
  bufr = SPI.transfer(NOP);  // read data of the register
  delayMicroseconds(1);
  digitalWrite(CS, HIGH);
  //digitalWriteFast(_START, LOW);
  SPI.endTransaction();
  return bufr;
}

long GetConversion() {
  int32_t regData;
  ;
  while (digitalRead(DRDY) != 0)
    ;
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(CS, LOW);  //Pull SS Low to Enable Communications with ADS1247
  delayMicroseconds(10);  // RD: Wait 25ns for ADC12xx to get ready
  SPI.transfer(RDATA);    //Issue RDATA
  delayMicroseconds(10);
  regData |= SPI.transfer(NOP);
  //delayMicroseconds(10);
  regData <<= 8;
  regData |= SPI.transfer(NOP);
  //delayMicroseconds(10);
  regData <<= 8;
  regData |= SPI.transfer(NOP);
  delayMicroseconds(10);
  digitalWrite(CS, HIGH);
  SPI.endTransaction();
  return regData;
}

void SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
  uint8_t regValuePre = GetRegisterValue(regAdress);
  if (regValue != regValuePre) {
    //digitalWriteFast(_START, HIGH);
    delayMicroseconds(100);
    while (digitalRead(DRDY) != 0)
      ;
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
    digitalWrite(CS, LOW);
    delayMicroseconds(100);
    SPI.transfer(SDATAC);  //Issue SDATAC
    delayMicroseconds(100);
    SPI.transfer(WREG | regAdress);  // send 1st command byte, address of the register
    SPI.transfer(0x00);              // send 2nd command byte, write only one register
    SPI.transfer(regValue);          // write data (1 Byte) for the register
    delayMicroseconds(10);
    digitalWrite(CS, HIGH);
    //digitalWriteFast(_START, LOW);
    if (regValue != GetRegisterValue(regAdress)) {  //Check if write was succesfull
      Serial.print("Write to Register 0x");
      Serial.print(regAdress, HEX);
      Serial.println(" failed!");
    } else {
      Serial.print("Write to Register 0x");
      Serial.print(regAdress, HEX);
      Serial.println(" OK");
    }
    SPI.endTransaction();
  }
}

void ADreset() {
  digitalWrite(RST, LOW);
  delay(ADResetPulse);
  digitalWrite(RST, HIGH);
  delay(ADResetDelay);
}

void ADread() {
  digitalWrite(10, LOW);
  delayMicroseconds(1);  // atraso mínimo de 10 ns (tcssc).
  SPI.transfer(0x12);    // Transfere a leitura do ADC para o buffer de saida
  /* A estrutura abixo salva byte a byte do bauffer de saida do ADC, na mesma vairiavel (regData), e com o bit-shifting,
 quando estamos salvando os bytes em um vertor, perdiamos o zero à esquerda o que complicava, na hora da converção para DEC */
  int32_t regData;
  regData |= SPI.transfer(0xff);
  regData <<= 8;
  regData |= SPI.transfer(0xff);
  regData <<= 8;
  regData |= SPI.transfer(0xff);
  Serial.print(regData, HEX);
  Serial.print(" <--> ");
  Serial.print(regData, DEC);
  Serial.print(" <--> ");
  float val = regData;
  //if (regData >> 23 == 1) {
  //val = ~regData-  0xFF000000 + 1;
  //val = (2.048/(8388608))*val;

  if (regData >> 23 == 1) {
    Serial.println("NaN");

  } else {
    Serial.print(val);
    Serial.print("--");
    double result_volt = val / (8388608 * 2);
    double result_resistance = 820 * result_volt / 0.002;  //
    Serial.print("Volt--");
    Serial.print(result_volt, DEC);
    Serial.print("--Rest--");
    Serial.println(result_resistance, DEC);
  }


  //else{
  //val = val*2.048/(8388608);
  // val = (1.813728/7429030)*(val/32.0)*1000;
  // Serial.println(val,DEC);
  //}
}

#define IDAC0_ID 144

void ADSSetup() {

  SetRegisterValue(MUX0, MUX_SP2_AIN0 | MUX_SN2_AIN1);
  SetRegisterValue(MUX1, REFSELT1_REF1 | VREFCON1_ON);  //ADS Reference on REF1, Internal Reference on
  SetRegisterValue(SYS0, PGA2_2 | DOR3_20);
}
#define MUXCAL2_TEMP B00000011  // Temperature diode
void test_intTemp(void) {
  SetRegisterValue(MUX1, MUXCAL2_TEMP | VREFCON1_ON | REFSELT1_ON);
  Serial.println("Wait for Measurement:");
  delay(1000);
  unsigned long temp_val = GetConversion();
  Serial.println(temp_val, DEC);
  double temp_voltage = (2.048 / 8388608) * temp_val;  //only 23bit data here.
  Serial.println(temp_voltage, DEC);
  double temp = ((temp_voltage - 0.118) / (405 * 0.000001)) + 25;  //see Datasheet S.30
  Serial.print("Internal Temperature is ");
  Serial.print(temp);
  Serial.println(" Grad Celsius!");
}

void SPIinitialize() {
  // Arduino ADC interface (wire accordingly)
  // inverse means that when pin is low relavent function is active e.g.:
  // DRDY low means ADC has data to send
  pinMode(CS, OUTPUT);     // pin 10 as CS (inverse) - CS = Chip Select, habilita comunicação (em low) SPI com ADS1247.
  pinMode(DRDY, INPUT);    // pin 07 as DRDY (inverse) - LOW indica conversão (ADC) completa.
  pinMode(START, OUTPUT);  // pin 08 as reset (inverse) - LOW (min. 4x244 ns =976 ns; 244 ns = 1/4,096 MHz) p/ reset.
  pinMode(RST, OUTPUT);    // pin 09 as start - High (min. 3x244 = 732 ns) inicia conversão (ADC). POde ficar high sempre.

  digitalWrite(CS, HIGH);     // pins set to high, desablita a comunicação SPI.
  digitalWrite(RST, HIGH);    // operação normal, reset em LOW.
  digitalWrite(START, HIGH);  // operação normal, ADC convertendo continuamente.

  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // SPI setup SPI communication
  delay(200);                                                         // SPI delay for initialization
  SPI.begin();                                                        // SPI Module 'wake up'
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPIinitialize();  // Arduino setup SPI.
  Serial.println("Start...");
  pinMode(LED_BUILTIN, OUTPUT);
  //ADSSetup();
  //test_intTemp();
  RTDSetup();
}
//3-wire RTD 
void RTDSetup() {

  // 0--A// 2--20 3--0 A--96 B--3
	SetRegisterValue(0x00,0x0A);
	SetRegisterValue(MUX1, 0x20);
  SetRegisterValue(IDAC0, 0x96);  //	IDAC at 1,5mA current
  SetRegisterValue(IDAC1, 0x03 ); 
	SetRegisterValue(SYS0, 0x22);


  delay(100);
  Serial.println(GetRegisterValue(MUX0), HEX);
  Serial.println(GetRegisterValue(MUX1), HEX);
  Serial.println(GetRegisterValue(SYS0), HEX);
  Serial.println(GetRegisterValue(IDAC0), HEX);
  Serial.println(GetRegisterValue(IDAC1), HEX);
}

void test_inputRange(){
	SetRegisterValue(MUX0,MUX_SN2_AIN1 | MUX_SP2_AIN2);
	SetRegisterValue(MUX1, VREFCON1_ON | REFSELT1_ON);
  SetRegisterValue(IDAC0, 144 | 0x06);  //	IDAC at 1,5mA current
  SetRegisterValue(IDAC1, 0x03 ); 
	SetRegisterValue(SYS0, 0x22);

  delay(100);
  Serial.println(GetRegisterValue(MUX0), HEX);
  Serial.println(GetRegisterValue(MUX1), HEX);
  Serial.println(GetRegisterValue(SYS0), HEX);
  Serial.println(GetRegisterValue(IDAC0), HEX);
  Serial.println(GetRegisterValue(IDAC1), HEX);

	uint32_t val = GetConversion();
	
	if (val >> 23 == 1) {
		val = ~val-  0xFF000000 + 1;
		Serial.print(2.048/8388608*val );;Serial.print(" --- ");Serial.println(val);
		
	}
	else{
		Serial.print(2.048/8388608*val );Serial.print(" --- ");;Serial.println(val);
	}
	//Serial.println(val, HEX);


}

double test_RTD(void) {
  double result_temp = 0;

  unsigned long val = GetConversion();
  if (val >> 23 == 1) {
	  val = ~val-  0xFF000000 + 1;
		//Serial.print(2.048/8388608*val );;Serial.print(" --- ");Serial.println(val);
    double result_volt = (2.048 / 8388608) * (val/4);
    double result_resistance = result_volt*820;  //
    Serial.print("1.data--");
    Serial.print(val);
    Serial.print("--Volt--");
    Serial.print(result_volt, DEC);
    Serial.print("--Rest--");
    Serial.println(result_resistance);

  } else {
    double result_volt = (2.048 / 8388608) * (val/4);
    double result_resistance =  result_volt*820;  //
    Serial.print("2.data--");
    Serial.print(val);
    Serial.print("--Volt--");
    Serial.print(result_volt, DEC);
    Serial.print("--Rest--");
    Serial.println(result_resistance);
    //result_temp = (-R0*A+sqrt(pow(R0*A,2)-4*R0*B*(R0-result_resistance)))/(2*R0*B);
    //Serial << millis() << "\t" << _FLOAT(result_volt,4) << "\t" << _FLOAT(result_resistance,4) << "\t" <<  _FLOAT(result_temp,4) << "\t" << val << endl;
  }
  return result_temp;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
  //Serial.println("Reaing...");
  if (digitalRead(DRDY) == 0) {
    test_RTD();  // read data when ADC dataReady is low
  }
}
