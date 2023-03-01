# Grow-active
Web server &amp; esp32 program for the data physicalization "Grow Active"
## Short description: 
Grow Active helps you build a healthy, active lifestyle routine. Inspired by indoor plants, which only grow when taken care of, the mechanical plant leaf slowly opens when the user engages in physical activity.
## Description:

Physical activity is essential to our health. However, most people working from home struggle to be active enough for various reasons. Since there is little direct daily feedback, staying motivated and continuously active is hard. Moreover, there is a general lack of knowledge about the right amount of activity. And sometimes, one simply forgets to engage in some physical activity. As a result, physical inactivity increases the risk of suffering from heart disease in the long term or of developing other promoting risk factors, such as high blood pressure or obesity.

Grow Active helps you build a healthy, active lifestyle routine. Inspired by indoor plants, which only grow when taken care of, the mechanical plant leaf slowly opens when the user engages in physical activity. The user's smartwatch tracks their time in an activity-indicating heart rate zone and sends this data to the plant. It takes 30 min of activity to open the leaf completely, following the Harvard School of Public Health’s recommendation for an optimal amount of daily activity. Furthermore, Grow Active helps you to establish your new routine of daily activity through frequent repetition. With an AR application, you can view more detailed information and engage in a 30-day challenge to stay on track. Your performance in the challenge is visualized through a colored heatmap on top of the leaf within the AR.

Grow Active is a constant reminder in your home, allowing you to read your daily activity status intuitively. In addition, the flourishing plant serves as a physical representation of your self-care and engagement in an active lifestyle to promote your health.
### Leaf operation:

When you see the paper leaf unfolding, this movement is driven by a stepper motor, which can target 30 positions for 
the measure of recommendation of 30 minutes of physical activity per day. The motor itself is controlled by an ESP32 which is a cost-efficient, 
low-power system on a chip micro controller, which readily comes with integrated on-board Wi-Fi, which makes it the perfect choice for an easy-to-setup and place-where-you-want product. When it is connect with a network and has internet access it can poll the GET endpoint of the REST API of a web server for the current sum of daily minutes in a heart rate zone above 100 BPM. When the fetched value differs from the currently hold one, the controller instructs the motor via a stepper driver to move to the new position and upon its arrival the controller stores the current position in persistent memory, so that in case there is an intermediate power loss, the absolute position is still known and the stepper can move relatively to it, which minimizes the harm of damaging the physicalization due to out-of-bounds movements.

For programming the logic of and uploading it to the ESP32, we chose PlatformIO, a fully integrated embedded developing tool, which comes as a VSCode plugin, hence offering top-notch editing/linting/debugging features (i.e. great DX) {{(as opposed to the Arduino IDE) but it is still allowing the usage of the Arduino Wiring-based Framework, which offers good abstractions over the nitty-gritty technical details and more importantly a huge community of users creating libraries for all sorts of useful things.}}(vermutlich besser rauskürzen).
To interface with stepper motor, we chose the *AccelStepper* library, because of has support for smooth acceleration/deceleration and good abstractions which made it easy to use.

We also set up the web server ourselves. Here we made the conservative choices of Python and the Django web framework. It has several tasks to perform, which consist of:
    - fetching heart rate data of the user from an exposed endpoint of the Fitbit servers,
    - extracting the relevant minutes in the custom heart rate of 100-220 BPM from the heart rate data JSON response,
    - and serving the result via an own GET REST endpoint to the ESP32.
All of these tasks are performed in a lazy manner, only on receiving a request from the ESP32.

And now we arrived at the final part of our data pipeline: To get a non-empty response from the Fitbit server, the user has to wear a Fitbit smartwatch/fitness tracker recording their heart rate data, and sending those via the Fitbit smartphone app end-to-end encrypted to the servers, where they are made available. This makes it unfortunately impossible to directly retrieve the data from the watch, but at the same time allows for heightened security/privacy as well as stability, and properly even energy-saving guarantees.

As soon as the motor arrives at its upper-bound 30 minute position, the poll intervals of the ESP32 are considerably increased, since the next move operation only has to be performed at/after 12 am, when the daily values are reset and the returned minute value is 0 again. And the stepper automatically returns to its starting position.



### AR app:

The AR app was built to give the user more interactivity options with the leaf and to provide 
a more concrete overview of the daily and the 30-day challenge progress.

We chose Unity to develop the app for Android and *Vuforia Engine* to add AR capabilities to our App, 
since it is one of the easiest tools for developing a mobile or tablet app, and one of us had already previous experience using Unity.

We designed the user interface to be simplistic, but encompass all information we wanted to convey: 
a progress bar and percentage for the daily progress and the 30-day challenge overview

We used the *Vuforia Engine* for the target detection. An AR camera was set up and parametrized to detect either an image 
or a 3D render model of the leaf. 
A 3D model of a heatmap (gameObject) is attached to the AR space on detection,
with its position and size set relatively to position and size of the detected object,making it seem as if the heatmap was steadily hovering on top of the leaf.

For implementing the progress bar's and daily activity counter's reaction and animation upon target d` etection 
and loss, we used the C# based Unity scripting API. 

For fetching the current heart rate minutes from the REST endpoint of our Django web server,we used UnityGetWebRequest 
and deserialized the response JSON, and extracted the contained data to use it with the scripts implementing the behavior of the UI.

On target detection, the progress bar fills up from 0 to the current percentage of the daily 30 minutes in a smooth transition animation. 
And on target loss the bar depletes back to 0 with the same kind of transition animation.  

The Unity scripting API offers a variety of functions to animate Unity's "gameObjects", which in our case are the daily activity counter and the progressbar. 
We used the function "Update" in the script attached to the corresponding gameObject.It gets called on every frame the app is running, 
which we employed to compare the current value of progressbar and counter to the target value and accordingly increment or decrement the progress by a certain 
amount every frame to create smooth progress animations.  
  
Another useful function we took advantage of was Coroutines, which allows spreading of tasks across several frames, unlike the Update functions. 
This was helpful since fetching data from the server takes longer than a single frame. So we programmed it to wait 1 second, 
which is enough time for the fetch request, before setting the target progress for our gameObjects to be then updated every frame.



## Installation:
AR: Unity can be installed using the following link: https://unity.com/download

Unity Ar foldershould be simply copied locally. Use the following guide to import Unity Ar to your local unity hub: https://support.unity.com/hc/en-us/articles/4402520287124-How-do-I-add-a-project-saved-on-my-computer-into-the-Unity-Hub-

Vuforia package should be installed preferably version 10.12: https://library.vuforia.com/getting-started/getting-started-vuforia-engine-unity

The project should then run as desired locally.

Note: the image target and model targets might need to be changed as they are in a private database.

## Usage: 
To use the Ar_app a very basic unity knowledge is needed, in the following is a 17 minute guide of how to use Unity and Vuforia
https://www.youtube.com/watch?v=E6A4WvsDeLE&ab_channel=CodeMonkey
https://www.youtube.com/watch?v=Z4bBMpa4xWo&ab_channel=Vuforia%2CaPTCTechnology


## Acknowledgements:
Johannes Schmidt
Firas Drass 
Tobias Sandmann 

## License: 

Copyright (c) [2023] [Grow_Active]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
