///////////////////////////////////////////////////////////////////////////////////////
/*Terms of use
///////////////////////////////////////////////////////////////////////////////////////
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.


///////////////////////////////////////////////////////////////////////////////////////
//Support
///////////////////////////////////////////////////////////////////////////////////////
Website: http://www.brokking.net/imu.html
Youtube: https://youtu.be/4BoIE8YQwM8
Version: 1.0 (May 5, 2016)

///////////////////////////////////////////////////////////////////////////////////////
//Connections
///////////////////////////////////////////////////////////////////////////////////////
Power (5V) is provided to the Arduino pro mini by the FTDI programmer

Gyro - Arduino pro mini
VCC  -  5V
GND  -  GND
SDA  -  A4
SCL  -  A5

LCD  - Arduino pro mini
VCC  -  5V
GND  -  GND
SDA  -  A4
SCL  -  A5
*//////////////////////////////////////////////////////////////////////////////////////

//Include LCD and I2C library
//#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Declaring some global variables
int gyro_x_1, gyro_y_1, gyro_z_1;
int gyro_x_2, gyro_y_2, gyro_z_2;
//int gyro_x_3, gyro_y_3, gyro_z_3;
//int gyro_x_4, gyro_y_4, gyro_z_4;
float acc_x_f_1,acc_y_f_1,acc_z_f_1;
float acc_x_f_2,acc_y_f_2,acc_z_f_2;
//float acc_x_f_3,acc_y_f_3,acc_z_f_3;
//float acc_x_f_4,acc_y_f_4,acc_z_f_4;
long acc_x_1, acc_y_1, acc_z_1, acc_total_vector;
long acc_x_2, acc_y_2, acc_z_2;
//long acc_x_3, acc_y_3, acc_z_3;
//long acc_x_4, acc_y_4, acc_z_4;
int temperature;
long gyro_x_cal_1, gyro_y_cal_1, gyro_z_cal_1;
long gyro_x_cal_2, gyro_y_cal_2, gyro_z_cal_2;
//long gyro_x_cal_3, gyro_y_cal_3, gyro_z_cal_3;
//long gyro_x_cal_4, gyro_y_cal_4, gyro_z_cal_4;
long loop_timer;
int lcd_loop_counter;
float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
boolean set_gyro_angles;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;
int counter = 0;

void setup() {
  Wire.begin();                                                        //Start I2C as master
  Serial.begin(57600);                                               //Use only for debugging
  
  
  
  pinMode(5, OUTPUT);          // sets the digital pin 4 as output
  pinMode(6, OUTPUT);          // sets the digital pin 5 as output 

  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);

  setup_mpu_6050_registers();                                 //Setup first MPU-6050 (500dfs / +/-8g) and start the gyro
  for (int cal_int = 0; cal_int < 100 ; cal_int ++){
    read_mpu_6050_data(0); 
    gyro_x_cal_1 += gyro_x_1;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal_1 += gyro_y_1;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal_1 += gyro_z_1;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);                                                          //Delay 3us to simulate the 250Hz program loop
  }
  
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  
  setup_mpu_6050_registers();                                 //Setup second MPU-6050 (500dfs / +/-8g) and start the gyro  
  for (int cal_int = 0; cal_int < 100 ; cal_int ++){
    read_mpu_6050_data(1); 
    gyro_x_cal_2 += gyro_x_2;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal_2 += gyro_y_2;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal_2 += gyro_z_2;                                              //Add the gyro z-axis offset to the gyro_z_cal variable
    delay(3);                                                          //Delay 3us to simulate the 250Hz program loop
  }
  
  gyro_x_cal_1 /= 100;                                                  //Divide the gyro_x_cal_1 variable by 100 to get the avarage offset
  gyro_y_cal_1 /= 100;                                                  //Divide the gyro_y_cal_1 variable by 100 to get the avarage offset
  gyro_z_cal_1 /= 100;                                                  //Divide the gyro_z_cal_1 variable by 100 to get the avarage offset

  gyro_x_cal_2 /= 100;                                                  //Divide the gyro_x_cal_2 variable by 100 to get the avarage offset
  gyro_y_cal_2 /= 100;                                                  //Divide the gyro_y_cal_2 variable by 100 to get the avarage offset
  gyro_z_cal_2 /= 100;                                                  //Divide the gyro_y_cal_2 variable by 100 to get the avarage offset
  
  Serial.print("Calibration Done");
  loop_timer = micros();                                               //Reset the loop timer
  
}

void loop(){

  if (counter == 0) {
    
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  
  read_mpu_6050_data(counter);
  
  gyro_x_1 -= gyro_x_cal_1;                                                //Subtract the offset calibration value from the raw gyro_x_1 value                                                
  gyro_y_1 -= gyro_y_cal_1;                                                //Subtract the offset calibration value from the raw gyro_y_1 value                                                
  gyro_z_1 -= gyro_z_cal_1;                                                //Subtract the offset calibration value from the raw gyro_z_1 value
                                                                        
  gyro_x_1 = gyro_x_1 / 65.5;
  gyro_y_1 = gyro_y_1 / 65.5;
  gyro_z_1 = gyro_z_1 / 65.5;
  
  acc_x_f_1 = acc_x_1 / 2048.0;
  acc_y_f_1 = acc_y_1 / 2048.0;
  acc_z_f_1 = acc_z_1 / 2048.0;

  acc_x_f_1 = acc_x_f_1 * 9.8;
  acc_y_f_1 = acc_y_f_1 * 9.8;
  acc_z_f_1 = acc_z_f_1 * 9.8;

  Serial.print("x1_a= ");
  Serial.print(acc_x_f_1);
  Serial.print(",");
  Serial.print("y1_a= ");
  Serial.print(acc_y_f_1);
  Serial.print(",");
  Serial.print("z1_a= ");
  Serial.print(acc_z_f_1);
  Serial.print(",");
  Serial.print("x1_g= ");
  Serial.print(gyro_x_1);
  Serial.print(",");
  Serial.print("y1_g= ");
  Serial.print(gyro_y_1);
  Serial.print(",");
  Serial.print("z1_g= ");
  Serial.print(gyro_z_1);
  //read_mpu_6050_data();
  //Serial.print(who_am_i);
  Serial.println();
  
  counter++;
  }

  if (counter == 1) {
    
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  
  read_mpu_6050_data(counter);
  
  gyro_x_2 -= gyro_x_cal_2;                                                //Subtract the offset calibration value from the raw gyro_x_2 value
  gyro_y_2 -= gyro_y_cal_2;                                                //Subtract the offset calibration value from the raw gyro_y_2 value
  gyro_z_2 -= gyro_z_cal_2;                                                //Subtract the offset calibration value from the raw gyro_z_2 value
                                                                        
  gyro_x_2 = gyro_x_2 / 65.5;
  gyro_y_2 = gyro_y_2 / 65.5;
  gyro_z_2 = gyro_z_2 / 65.5;
  
  acc_x_f_2 = acc_x_2 / 2048.0;
  acc_y_f_2 = acc_y_2 / 2048.0;
  acc_z_f_2 = acc_z_2 / 2048.0;

  acc_x_f_2 = acc_x_f_2 * 9.8;
  acc_y_f_2 = acc_y_f_2 * 9.8;
  acc_z_f_2 = acc_z_f_2 * 9.8;

  Serial.print("x2_a= ");
  Serial.print(acc_x_f_2);
  Serial.print(",");
  Serial.print("y2_a= ");
  Serial.print(acc_y_f_2);
  Serial.print(",");
  Serial.print("z2_a= ");
  Serial.print(acc_z_f_2);
  Serial.print(",");
  Serial.print("x2_g= ");
  Serial.print(gyro_x_2);
  Serial.print(",");
  Serial.print("y2_g= ");
  Serial.print(gyro_y_2);
  Serial.print(",");
  Serial.print("z2_g= ");
  Serial.print(gyro_z_2);
  //read_mpu_6050_data();
  //Serial.print(who_am_i);
  Serial.println();
  
  counter = 0;
  }

   delay(5);
}

void read_mpu_6050_data(int counter){
  //Serial.print("1");//Subroutine for reading the raw gyro and accelerometer data
  
  if (counter == 0) {
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,14);//Request 14 bytes from the MPU-6050
  while(Wire.available()<14);
  acc_x_1 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  acc_y_1 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z_1 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                              //Add the low and high byte to the temperature variable
  gyro_x_1 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y_1 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z_1 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable
  }          
  
  if (counter == 1) {
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,14);//Request 14 bytes from the MPU-6050
  while(Wire.available()<14);
  acc_x_2 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  acc_y_2 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z_2 = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  temperature = Wire.read()<<8|Wire.read();                              //Add the low and high byte to the temperature variable
  gyro_x_2 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  gyro_y_2 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  gyro_z_2 = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable
  }       

void setup_mpu_6050_registers(){
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050  (Slave Address)
  Wire.write(0x6B);                                                    //Send the requested starting register   (PWR_MGMT_1)  
  Wire.write(0x01);                                                    //Set the requested starting register    (Write all zeros + clock select x axis as gyro reference) 
  Wire.endTransmission();                                              //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050    
  Wire.write(0x1C);                                                    //Send the requested starting register   (ACCEL_CONFIG)
  Wire.write(0x18);                                                    //Set the requested starting register    (VALUE)
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register   (GYRO_CONFIG)
  Wire.write(0x08);                                                    //Set the requested starting register    (VALUE)
  Wire.endTransmission();
 // Serial.print("Stuck");
  //End the transmission
  //Set gyro axis
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1A);                                                    //Send the requested starting register   (GYRO_CONFIG)
  Wire.write(0x06);                                                    //Set the requested starting register    (VALUE)
  Wire.endTransmission();
}
