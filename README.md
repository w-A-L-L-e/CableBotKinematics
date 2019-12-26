# Cable Bot Kinematics.
Was wondering how to do the coordinate transformation for having a CNC or drawbot suspended with 2 cables.

Many examples have been done in the past:
 - https://www.instructables.com/id/Drawbot/
 - https://hackaday.com/2018/10/08/another-drawbot-uses-a-pi-and-web-sockets/
 - https://hackaday.com/2010/06/06/drawbot-produces-portraits-very-slowly/

And yes even routers for cutting wood can be hooked up this way:
 - https://www.maslowcnc.com/

Then I saw an episode of team scheire and they hinted the actual maths involved might be complicated to run on an arduino. 
So therefore I took my pen and paper and just started scribbling ;)
Turns out it's pretty easily to do the transformation from x,y coordinate to the wanted stepper movements
using some basic maths.

Here's a quick and dirty page of math's that made the demo code possible:
![cable_bot_maths](https://user-images.githubusercontent.com/710803/71426082-9f5cae80-2672-11ea-8815-129444c5dd8a.JPG)


The demo code tests out the algo with some x,y coordinates and get back the lengteA1 and lengteA2.
The last 2 are the movement the stepper motors need to take in order to move tool attached to the
wanted x1,y1 coordinates.

This all depends on where your motors are located and this is now just hardcoded as (Katrol) K coordinates.
We have K1x, K1y which is the top left motor coordinate. And K2x, K2y which is the top right motor coordonate.

So after making your frame all you need to do is for instance fill in the distance measured from bottom to top
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

The main meat of the entire transformation is actually only this :
```
   // lengte A1 = stepper 1 beweging
  lengteA1 = sqrt( delta1*delta1 + delta2 * delta2 );
  
  // lengte A2 = stepper 2 beweging
  lengteA2 = sqrt( gamma1*gamma1 + gamma2 * gamma2 );
```

So yes it should be perfectly possible to incorporate this into an arduino firmware like marlin or klipper etc. You
only need a square root function which arduino has available in the standard libs https://www.arduino.cc/reference/en/language/functions/math/sqrt/.

It's certainly a lot simpler than the delta kinematics needed for a delta 3d printer. And is about the same as the one
needed for a corexy (in computational complexity). The hangprinter from tobben (also one that uses cables but here for full 3d motion) even has an example marlin change to the path planner: https://gitlab.com/tobben/hangprinter/blob/Openscad_version_3/firmware/Marlin_subtree/Marlin/Marlin.h



# Level2 : Going faster, adding an additional tensioning cable.

Above kinematics has one fatal flaw: it uses gravity and weights to acchieve movement.
This means you can only move the toolhead slowly and also it only works on a vertical plane as you need gravity to pull down on the toolhead.
If you try to go faster you'll just introduce a lot of wobble etc. There is a solution however. By removing the weights and
adding a third cable you can constrain the toolhead without using gravity. We only need to compute the length of that third cable
and it's attached to the bottom of the toolhead using a pully. It's fixed in the origin and on the bottom right we use a third stepper
to make this cable the exact length lengteA3.

Because we did the above maths, computing lengteA3 is actually pretty similar and not that difficult/complex either:
```
  double omega = K3x - x1;
  lengteA3 = sqrt( x1*x1 + y1*y1 ) + sqrt( omega*omega + y1*y1 );
```
In the actual code we've added some parameters omega_offset, delta_offset, gamma_offset. these are to be set to the distance of the
toolhead to the pulley. The toolhead is dead center but the pulleys are on a disk of a certain diameter to be chosen by the machine at hand and can be configured
in the code.


Now this new improved version I also computed 9 points (3 on top, 3 in middle and 3 near the bottom). And we see our formulae are doing their
work and keeping everything nice and tight:

```
nine equally spaced points to verify my quick and dirty christmas maths ;)
top 3 points:

x1 = 1
y1 = 16
lengteA1 = 2.23607
lengteA2 = 19.105
lengteA3 = 39.2407
--------------------------

x1 = 9
y1 = 16
lengteA1 = 9.21954
lengteA2 = 11.1803
lengteA3 = 36.0939
--------------------------

x1 = 18
y1 = 16
lengteA1 = 18.1108
lengteA2 = 2.82843
lengteA3 = 38.5635
--------------------------

middle 3 points:

x1 = 1
y1 = 8
lengteA1 = 10.0499
lengteA2 = 21.4709
lengteA3 = 27.3195
--------------------------

x1 = 9
y1 = 8
lengteA1 = 13.4536
lengteA2 = 14.8661
lengteA3 = 24.4402
--------------------------

x1 = 18
y1 = 8
lengteA1 = 20.5913
lengteA2 = 10.198
lengteA3 = 26.5933
--------------------------

bottom 3 points:

x1 = 1
y1 = 3
lengteA1 = 15.0333
lengteA2 = 24.2074
lengteA3 = 21.341
--------------------------

x1 = 9
y1 = 3
lengteA1 = 17.4929
lengteA2 = 18.6011
lengteA3 = 20.3999
--------------------------

x1 = 18
y1 = 3
lengteA1 = 23.4307
lengteA2 = 15.1327
lengteA3 = 20.9392
--------------------------

```

Basically what we see happening is that when the toolhead goes down lengteA3 becomes shorter and when it goes higher it's a bit longer. 

computeDrawbotImproved is a promising idea and would allow the construction of cnc and 3d printers in a new and original way. 
Currently there is standard cartesion, delta, core-xy  the etch-a-sketch or ultimaker gantry and the 'scarab arm'. 

This above version will be yet another way of moving a toolhead around quickly using 3 cables/steppers (a fourth for the z-plane if you want 3d movement). 
It's similar to the hangprinter but that one has the issue it only prints in a large pyramid: the higher you go the smaller the x-y plane gets. 
This variant only targets the x-y plane using the cable setup. And the z is done like the core-xy or ultimaker variants it improves on the original
drawbot gantry and eliminates the above issue with the hangprinter.

I'll try to see if I find some spare time when I'm back in Belgium and then incorporate this into a marlin variant like the hangprinter has done.
https://gitlab.com/tobben/hangprinter/blob/Openscad_version_3/firmware/Marlin_subtree/Marlin/Marlin.h
2020 is however already pretty busy and I'm currently on holiday in Peru until februari so it's probably not verry soon. We've also got a kickstarter
running for AnyKey (anykey.shop). And that gets priority for now...

Best wishes for 2020 and Merry Christmas for everyone!

