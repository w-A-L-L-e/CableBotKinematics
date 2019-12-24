Was wondering how to do the coordinate transformation for having a CNC or drawbot suspended with 2 cables.

There are some kits out there that seem promising to even use it on a router like:
https://www.maslowcnc.com/

Or just draw large things plotter style:
https://www.instructables.com/id/Drawbot/

Turns out it's pretty easily to do the transformation from x,y coordinate to the wanted stepper movements.

Here we test out the algo with some x,y coordinates and get back the lengteA1 and lengteA2.
The last 2 are the movement the stepper motors need to take in order to move tool attached to the
wanted x1,y1 coordinates.

This all depends on where your motors are located and this is now just hardcoded as (Katrol) K coordinates.
We have K1x, K1y which is the top left motor coordinate. And K2x, K2y which is the top right motor coordonate.

So after making your frame all you need to do is for instance fill in the millmimeters measured from bottom to top
and left to right for the K1 and K2 motors. The origin is in the bottom left of your frame and the maximum height
should be a little lower than the motors (same for left and right, best to keep away a few cm from the edges to allow
cables to reach).

Here's an example demo run:

```
➜  CableBotKinematics git:(master) ✗ make
g++ -O2 -Wall cable_bot_kinematics.cpp -o cable_bot_demo
➜  CableBotKinematics git:(master) ✗ ./cable_bot_demo

x1 = 9
y1 = 12
lengteA1 = 10.8167
lengteA2 = 12.53
--------------------------

x1 = 10
y1 = 12
lengteA1 = 11.6619
lengteA2 = 11.6619
--------------------------

x1 = 11
y1 = 12
lengteA1 = 12.53
lengteA2 = 10.8167
--------------------------

x1 = 3
y1 = 16
lengteA1 = 3.60555
lengteA2 = 17.1172
--------------------------

x1 = 20
y1 = 1
lengteA1 = 26.2488
lengteA2 = 17
--------------------------

```

And here's a quick and dirty page of math's that made the above code possible:


