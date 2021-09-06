/*
 *  Arduino Uno HW setup
 *  V+ to A2
 *  V- to A0
 *  
 *  [V3]
 *  - Assign IN_PIN as output, and write to GND before each while loop
 *  - Selector to display Moisture %
 */
bool SHOW_MOISTURE_PERC = false; // Selector to display moisture in %
const int SENSOR_READ_INTERVAL_SEC = 6; // How often sensor is read, in sec
 
const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8;  
const int MAX_ADC_VALUE = 1023;

const float CAP_100_PERCENT = 500; // pF
const float CAP_0_PERCENT = 35; // pF


void setup(){
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(9600);
}



void loop(){
    pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    // IF CAP VALUE IS <1000 pF
    if (val < 1000){
      pinMode(IN_PIN, OUTPUT);
      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
      Serial.print(F("Capacitance Value = "));
      Serial.print(capacitance, 3);
      Serial.print(F(" pF ("));
      Serial.print(val);
      Serial.println(F(") "));

      if ( SHOW_MOISTURE_PERC ){
        int moist_percent = 0; // placeholder
        if (capacitance <= 35) {
          moist_percent = 0;
        } else if (capacitance <= 500) {
          moist_percent = round(99.0/(CAP_100_PERCENT-CAP_0_PERCENT)*(capacitance-CAP_0_PERCENT));
        } else {
          moist_percent = 99;
        }
        Serial.print(F("Moisture % = "));
        Serial.println(moist_percent, 1); 
        Serial.println(""); 
      }
         
    }
    // ELSE IF CAP VALUE IS >1000 pF
    else{
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;
      do{
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));
      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);
      float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE);
      Serial.print(F("Capacitance Value = "));
      if (capacitance > 1000.0){
        Serial.print(capacitance / 1000.0, 2);
        Serial.print(F(" uF"));
      }
      else{
        Serial.print(capacitance, 2);
        Serial.print(F(" nF"));
      }
      Serial.print(F(" ("));
      Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
      Serial.print(F(", t= "));
      Serial.print(t);
      Serial.print(F(" us, ADC= "));
      Serial.print(val);
      Serial.println(F(")"));
    }

    pinMode(IN_PIN, OUTPUT);
    digitalWrite(IN_PIN, LOW);
    
    while (millis() % (SENSOR_READ_INTERVAL_SEC*1000) != 0);   
}
