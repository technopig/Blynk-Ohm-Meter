//This setup uses a simple voltage divider:
//R_known goes from GND to pin A5
//R_unknown (resistVal) goes from pin A5 to V_in
//The two resistances should be as similar as possible
//  for best accuracy.

//The blink app should have a slider on V1
//  and a value display on V0
//  The slider simply writes a value to the virtual pin

//Note that analogRead returns a value from 0 to 4094
//  mapped linearly from 0V to 3.3V on the pin (ref to GND)

#include <blynk.h>


char auth[] = "5487a1b9f1f34f2ab1d5872ec64e35f6";
double resistVal = 0; //for real resistance
int raw = 0; //for analogRead
int R_known = 10; //should be on order of measurement target
int blynk_sVal = 0; //value from blynk slider
double rollingSum = 0;
double V_in = 3.3;
double V_out = 0;

BlynkTimer timer;



BLYNK_WRITE(V1)
{
  blynk_sVal = param.asInt(); //get the value from the slider on V1
}

void sendResistVal()
{
  Blynk.virtualWrite(V0, resistVal); //send the value to the display on V0
}

void setup()
{
  delay(500); // Allow board to settle

  Particle.variable("resistVal", &resistVal, DOUBLE); //register real resistor measured value

  Blynk.begin(auth); //begin Blynk
  timer.setInterval(1000L, sendResistVal); //every 1000ms, execute sendResistVal(){}

  pinMode(A5, INPUT); // Analog resistor input
}

void loop()
{
  Blynk.run();
  timer.run();

  raw = blynk_sVal; //uncomment this line for Blynk slider resistances
  //raw = analogRead(A5); //uncomment this line for measured resistances
  V_out = V_in*raw/4094; //can possibly equal exactly 3.3
  for (int i = 0; i<10; i++)
  {
    rollingSum = rollingSum + (V_in-V_out)*R_known/V_out;
    delay(90);
  }
  resistVal=rollingSum/10;

  rollingSum=0;

}
// end
