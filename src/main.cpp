#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

ThreeWire wires(3, 2, 4);
RtcDS1302<ThreeWire> rtc(wires);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0 ,0);
  lcd.print("Real Clock Module");

  lcd.setCursor(0,2);
  lcd.print("Date: ");

  lcd.setCursor(0,3);
  lcd.print("Time: ");

  rtc.Begin();

  RtcDateTime initialDate = RtcDateTime(__DATE__, __TIME__);

  if (!rtc.IsDateTimeValid()) {  
    if (rtc.GetIsWriteProtected()){
      rtc.SetIsWriteProtected(false);
    }

    rtc.SetDateTime(initialDate);

    if (!rtc.GetIsRunning()){
      rtc.SetIsRunning(true);
    }

    rtc.SetIsWriteProtected(true);
  }
}

void loop() {
  RtcDateTime date = rtc.GetDateTime();

  char dateString[11];
  char timeString[9];
  String dayOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  snprintf_P(dateString,
          sizeof(dateString),
          PSTR("%02u/%02u/%04u"),
          date.Month(),
          date.Day(),
          date.Year());

  snprintf_P(timeString, 
          sizeof(timeString),
          PSTR("%02u:%02u:%02u"),
          date.Hour(),
          date.Minute(),
          date.Second());  

  lcd.setCursor(0, 1);
  lcd.print(dayOfTheWeek[date.DayOfWeek()]);

  lcd.setCursor(6, 2);
  lcd.print(dateString);

  lcd.setCursor(6, 3);
  lcd.print(timeString);

  delay(1000);
}
