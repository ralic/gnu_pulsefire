# PulseFire TODO

## Current ERRATA

* On heavy serial use chip crashes sometimes.(use slower pull time)
* GUI works only on current version of firmware. 
* MAL editor does not auto renumber goto statements.

## Chip code

* qmap++ with multiple columns.
* Second variable in pulse_fire/reset/hold/resum_map has no function. (after qmap++)
* Add menu support for map variables.
* Added more commands/tests to MAL
* Added nested if support in mal
* i2c and MCP4725 I2C DAC support

## Chip PF_VARS

* Find precompiler way to find variable idx to 'replace' Vars_getIndexFromName for push data.
* Make almost all code work via Vars_setValue for auto push.
* Make more easy to use current steps to add one variable are;
	1) Add var in vars.h struct;					add;		volatile uint8_t	lcd_input;
	2) Increase defined size in vars.h;		change; PF_VARS_LCD_SIZE	10
	3) Add string in strings.c;						add;		const char pmDataLcdInput[]	CHIP_PROGMEM = "lcd_input";
	4) Add string export in strings.h;		add;		extern const char pmDataLcdInput[];
	5) Add var meta data line in vars.c;	add;		{PFVT_8BIT,  (CHIP_PTR_TYPE)&pf_data.lcd_input.....
	6) optional add non zero defined default in vars.h and use in vars.c
	7) optional add code for change listener or minimal value check in vars.c

## Device GUI

* Operation manual
* docbook to help file support with ids.
* Remove all config data from CommandName enum to support multiple connections in one classloader.
* (~20%)id + i18n all ui components.
* Custom graph panel(s)
* Nice inner bottom fire border panel for inputs panel.
* Pretty abstract dialog and impl on all dialogs.
* MAL load/save dialog.
* Chip resets/etc wait dialogs. 
* headless support and traybar option.
* move all rxtx code out of init + UI thread.

## Pulsefire 2.0

* storage build on mongo or pg
* server version
* Convert to c++ with modules
* Message interface
* Add ethernet support.
* Port to arm7/9 aka BeagleBoard-Bone / PandaBoard

