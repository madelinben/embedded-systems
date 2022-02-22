# Embedded Systems
***Snippet resource regarding "baremetal" embedded software engineering using ESP32 and PlatformIO.***

## ESP32 Pin Layout
* esp32doit-devkit-v1

## Included (IoT Device) Modules
* ***LED***, RBG values configured using Pulse Width Modulation.

* ***LDR***, including voltage divider.

* ***DHT11*** (Adafruit Unified Sensor).

* ***RotaryEncoder***, using pin interrupt.

* ***TFTDisplay*** (GFX Library and ST7735 Driver).

* ***SPIDisplay*** (TFT_eSPI) with double frame buffer implemented.

* ***SD*** (BusIO).

## Snippet Template Explained

## Methodologies Implemented
* [x] General Purpose Input/Output pin configuration.
* [x] Non-blocking task scheduling with millis and FreeRTOS.

## Coding Standards and Organisation
***Annotation:***

Suitable and descriptive comments must explain method functionality and any assumptions, following the 
*JavaDocs notation format:*
```java
/**
* [METHOD_DESCRIPTION]
* @param    [PARAMETER_NAME]    [PARAMETER_DESCRIPTION]
* @return                       [RETURN_VALUE_DESCRIPTION]
* @see      [OBJECT_RETURN_VALUE]
**/
```

***Branches:***

Branch names must follow convention of `[TYPE]/[SPRINT-NAME]`.

*Branch Types:*
- Bugfix - Issue found during testing and development.
- Hotfix - Client has found an issue.
- Feature - New feature being added to system.

## Useful Links
