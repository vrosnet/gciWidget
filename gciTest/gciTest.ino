#include <TFT.h>
#include <SD.h>
#include <gciWidget.h>

PICadillo35t tft;
AnalogTouch ts(LCD_XL, LCD_XR, LCD_YU, LCD_YD, 320, 480);

gciWidgetSet widgets(tft, ts, "/TEST~DTN");

gciWidget *meter;
gciWidget *angular;
gciWidget *sw;
gciWidget *led;
gciWidget *knob;
gciWidget *gauge0;
gciWidget *gauge1;
gciWidget *button;

boolean switchState = false;
int knobValue = 0;

void setup() {
	
	analogWrite(PIN_BACKLIGHT, 255);
	tft.initializeDevice();

	ts.initializeDevice();
    ts.scaleX(4.3);
    ts.scaleY(3.3);
    ts.offsetY(5);

	
	tft.fillScreen(Color::Black);
	tft.setTextColor(Color::White);
	tft.setFont(Fonts::Default);
	if (!SD.begin(PIN_SD_SS)) {
		tft.print("Error opening SD card");
	}
	widgets.init();
	meter = widgets.getWidgetByName("Meter0");
	angular = widgets.getWidgetByName("Angularmeter0");
	sw = widgets.getWidgetByName("4Dbutton0");
	led = widgets.getWidgetByName("Led0");
	knob = widgets.getWidgetByName("Knob0");
	gauge0 = widgets.getWidgetByName("Gauge0");
	gauge1 = widgets.getWidgetByName("Gauge1");
	button = widgets.getWidgetByName("Winbutton0");

	sw->attachEvent(TAP, switchPress);
	knob->attachEvent(DRAG, turnKnob);

	button->attachEvent(PRESS, btnPress);
	button->attachEvent(RELEASE, btnRelease);
	button->attachEvent(TAP, btnTap);
}

void btnPress(gciWidget *w) {
	w->setValue(1);
}

void btnRelease(gciWidget *w) {
	w->setValue(0);
}

void btnTap(gciWidget *w) {
	knobValue = 0;
	switchState = false;
	sw->setValue(0);
	led->setValue(0);
	knob->setValue(0);
	meter->setValue(0);
	angular->setValue(0);
	gauge0->setValue(0);
	gauge1->setValue(0);
}


void switchPress(gciWidget *w) {
	switchState = !switchState;
	w->setValue(switchState ? 1 : 0);
	led->setValue(switchState ? 1 : 0);
}

void turnKnob(gciWidget *w) {

	float px = w->getEventX() - (w->getWidth() / 2);
	float py = w->getEventY() - (w->getHeight() / 2);
	float angle = atan2(py, px);
	int deg = 180 + (angle * 57.2957795);
	
	knobValue = (float)((deg + 30) % 360) / 2.3;
	
	if (knobValue > 100) knobValue = 100;
	if (knobValue < 0) knobValue = 0;
	
	knob->setValue(knobValue);
	if (switchState) {
		meter->setValue(knobValue);
		gauge0->setValue(knobValue);
	} else {
		angular->setValue(knobValue);
		gauge1->setValue(knobValue);
	}
}

void loop() {
	ts.sample();
	widgets.render();
}

