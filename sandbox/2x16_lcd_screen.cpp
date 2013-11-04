#include <LiquidCrystal.h>

byte RS = 8;
byte EN = 9;
byte D4 = 4;
byte D5 = 5;
byte D6 = 6;
byte D7 = 7;

byte width = 16;
byte height = 2;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

byte smiley[8] = {
	B00000,
	B10001,
	B00000,
	B00000,
	B10001,
	B01110,
	B00000,
};

byte glyph = 2;
byte row = 0;

byte block[8] = {31, 31, 31, 31, 31, 31, 31, 31};
byte clear[8] = {0, 0, 0, 14, 14, 0, 0, 0};

void setup() {
	// load custom glyphs
	lcd.createChar(0, smiley);
	lcd.createChar(1, block);
	lcd.createChar(2, clear);

	// 16x2 display
	lcd.begin(width, height);

	lcd.setCursor(0, 0);

	lcd.write(byte(0));

	delay(1000);
}

void loop() {
	row = 0;
	for (byte col = 0; col < width; col++) {
		lcd.setCursor(col, row);
		lcd.command(byte(1));
		lcd.setCursor(col, row);
		lcd.write(byte(glyph));
		delay(250);
	}
	row = height - 1;
	for (byte col = width - 1; col >= 0; col--) {
		lcd.setCursor(col, row);
		lcd.write(byte(glyph));
		delay(250);
	}
	if (glyph == 2) {
		glyph = 0;
	} else {
		glyph = 2;
	}
}