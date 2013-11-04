/*
  This sketch reads button pushes from a keypad to akt as a calculator, using a lcd screen
  Created by Don van der Drift, last updated: November 3, 2013.
  Released into the public domain.
*/

#include <LiquidCrystal.h>
#include "calculator_stack.h"

byte RS = 8;
byte EN = 9;
byte D4 = 10;
byte D5 = 11;
byte D6 = 12;
byte D7 = 13;

byte Ain = 0;

int count = 0;
int old_count = 0;
int array_values[] = {
	196, 248, 388, 504,
	224, 272, 404, 514,
	321, 360, 463, 554,
	420, 449, 527, 600
};
char array_text[] = {
	'1', '2', '3', '+',
	'4', '5', '6', '-',
	'7', '8', '9', '*',
	'C', '0', '=', '/'
};
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
Stack stack;

void setup() {
	// 16x2 display
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("Booting...");

	Serial.begin(9600);

	stack.push('0');
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Pressed: ");
	lcd.setCursor(0, 1);
	lcd.print("Value: ");
	lcd.print(stack.top_value);
}

void loop() {
	count = analogRead(Ain);
	if ((count < old_count - 6) | (count > old_count + 6)) {
		delay(50); // debounce
		count = analogRead(Ain);
		old_count = count;
		if (count < 900) {
			int d = 1023;
			int d_test = 0;
			byte k = 0;
			for (int i=0; i < 16; i++) {
				d_test = abs(count - array_values[i]);
				if (d_test <= d) {
					d = d_test;
					k = i;
				}
			}

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Pressed: ");
			lcd.print(array_text[k]);

			stack.push(array_text[k]);
			print_stack(stack);

			lcd.setCursor(0, 1);
			lcd.print("Value: ");
			lcd.print(stack.top_value);
			delay(1500);
		}
	}
}

void print_stack(Stack stack) {
	Serial.print("\nValues: ");
	for (byte i=0; i < 5; i++) {
		Serial.print(stack.values[i]);
		Serial.print(", ");
	}
	Serial.print("\nTypes: ");
	for (byte i=0; i < 5; i++) {
		Serial.print(stack.types[i]);
		Serial.print(", ");
	}
	Serial.print("\n\n");
}