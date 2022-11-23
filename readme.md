# ICT 1011 Project: Watchmouse

Watchmouse is a watch with built in bluetooth volume control, mouse control. It also has left/right keyboard button control which is used to simulate a powerpoint clicker.

## Libraries 

### Installation

Drag and drop the contents of the "libraries" folder into your "Arduino/Libraries" folder.

By default, your Arduino folder should be under Documents. Also, if there is no Libraries folder, just create it yourself.

We have included text files that credits all projects we referenced from, or copied from.

For BLE related, it is in Credits (BLE implementation).txt

We have cited the list of references in the project video as well.

## Usage Instructions

### Clock settings

On the starting menu page,
pressing the bottom right button will bring you to the change time settings page.

In this page, the top left/btm left buttons increase/decrease the currently selected value.

The top right button changes the currently selected value to the one on the right.

(e.g. currently selected value changes from hour to minute)

### Stopwatch

On the starting menu page, pressing the top right button will bring you to the stopwatch page.

Pressing the top left button will start/pause/resume the stopwatch.

Pressing the top right button while paused will reset the stopwatch.

Pressing the bottom left button will return to the starting menu page.

### Bluetooth remote controls

At the starting menu page, you should see either of the following at the bottom right part of the screen:

"Remote x" - this means that you have not connected the watch to a bluetooth device yet

"Remote >" - this means you can now access remote control functions.

Go to the device you wish to connect to and select "Watchmouse" on the bluetooth devices list to connect to the watch. If Watchmouse does not show up devices list, you may have to try using the nRFConnect app to pair the devices. 

(After pairing, there is no need to use the app again, unless you unpair the watch)

To access remote control functions, press the lower right button. 

In the remote control screen, you can switch between remote functions using the bottom left and right buttons.

The top left and right buttons are used to send control inputs, such as increase or decrease volume, left or right click.