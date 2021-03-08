# ESP32 NANO Point-Of-Sale Prototype

## What is it?
This project is a simple [demonstration](https://twitter.com/nanosaurus_flex/status/1368875990106177536) of the capabilities of the Nano Cryptocurrency.
It is in no way complete and is a work in progress. The Project aims to create a simple PoS (Point of Sale) demonstration for everyday transactions (e.g. coffee-shop, laundromat, public toilet). It is small, only needs a powerbank and wifi and it is quite fast. This project is mostly based on [this](https://medium.com/the-nano-center/nano-esp8266-trigger-build-guide-f17f7a6517b2) medium article by James Coxon.

## Hardware
The prototype uses a ESP32-based arduino-board with integrated display. The ESP32 was chosen for its wifi and bluetooth capabilities. It was also what i had laying around.
You can order this board for about 8$ from [AliExpress](https://de.aliexpress.com/item/33048962331.html?spm=a2g0o.productlist.0.0.73857da9FYA0gE&algo_pvid=6c7d094b-4b3c-466a-af10-f220698d6140&algo_expid=6c7d094b-4b3c-466a-af10-f220698d6140-0&btsid=0bb0622916152333620317459e5499&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_), which makes it accessible for a wide audience.

## Software
The language used is [Arduino](www.arduino.cc), which is useful for quick prototypes and easily accessible to beginners thanks to a variety of freely available tutorials and resources.

### Libraries
This project uses the following libraries:
#### TFT_eSPI
to drive the display
#### SPI
required for display
#### QRCode
to generate the qr-code
#### Wifi.h
for wifi reasons
#### ArduinoJson
to parse websocket data from server
#### WebSocketsClient
for the websocket implementation

### Structure
While most of it is a hot mess, the general structure is as follows:
-  initialization of display, wifi, websockets, etc.
-  booting sequence (status information)
- generate and display qr-code, listen for and display transactions

## Get it running
### Software
Get the [Arduino IDE](https://www.arduino.cc/en/software).
Add ESP32 boards to the [boards manager](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).
Download and open the project
Adjust the wifi and adress parameters
Install the necessary libraries
Connect the TTGO T-Display board
Select ESP32 Dev Module under `Tools > Board`
Upload the code to the board.
Done!

### Display
To select the right display, you have to comment the line `#include <User_Setup.h>           // Default setup is root library folder` and uncomment the line `#include <User_Setups/Setup25_TTGO_T_Display.h>`in the file `libraries/TFT_eSPI/User_Setup_Select.h`
### Wallet
Use [Nault](www.nault.cc) or your favorite wallet for testing.

## What to watch out for
Right now, only amounts smaller than 1 Nano are displayed correctly. I might fix this in the future when i get around to it.
This readme is only a quick and dirty hack, please notify me if you find any errors in the readme or the code!


## Further Resources
[Excellent article explaining the whole process](https://medium.com/the-nano-center/nano-esp8266-trigger-build-guide-f17f7a6517b2)
[Nanos Integration Guides](https://docs.nano.org/integration-guides/the-basics/)
[TFT_eSPI Documentation](https://github.com/Bodmer/TFT_eSPI)

