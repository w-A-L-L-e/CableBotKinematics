/*=============================================================================
author            : Walter Schreppers
filename          : cable_bot_kinematics.cpp
description       : computeDrawbotTransform: Makes moving in X-Y plane possible using 2 cables.
                      This is pretty standard and has been done before in various drawing bots.
                      The reason is you only need 2 steppers to move a toolhead in a large plane.
                  : computeDrawbotImproved: Use third stepper to pull down the toolhead.
                    This allows faster movements, it also allows to have a horizontal x-y plane (for instance
                    on a 3d printer. And it solves many mechanical issues. You need a third stepper
                    but still it allows to construct large drawing machines and you have the benifit of being
                    able to move fast. The dead weight is now also eliminated and therefore you can move less mass
                    around and not worry about wobbling, chatter etc.
First version     : 24/12/2019
Improved version  : 25/12/2019
copyright         : Walter Schreppers
=============================================================================*/

#include <cmath>
#include <iostream>

using namespace std;

// square function, using -O2 it's optimised to inline and
// makes later formulae shorter
double sq( double n ){
  return n*n;
}

// Here we use 2 cables to move a toolhead in x-y plane. This relies however on gravity
// pulling down on the toolhead. The issue is that you will always need to go slow and the x-y plane
// needs to be vertical. Also in space you would be out of luck using these mechanics :)
void computeDrawbotTransform(double x1, double y1, double &lengteA1, double &lengteA2)
{
    // K1 x,y is katrol 1 x+y coordinate
    double K1x = 0.0;
    double K1y = 18.0;

    // K2 x,y is katrol 1 x+y coordinate
    double K2x = 20.0;
    double K2y = 18.0;

    double gamma1 = K2x - x1;
    double gamma2 = K2y - y1;

    double delta1 = x1 - K1x;
    double delta2 = K1y - y1;

    // lengte A1 = stepper 1 beweging
    lengteA1 = sqrt( sq(delta1) + sq(delta2) ); 

    // lengte A2 = stepper 2 beweging
    lengteA2 = sqrt( sq(gamma1) + sq(gamma2) );
}

// Improved version, we have 3rd cable pulling down here, no need for bricks/dead weight
// as the third cable constrains the toolhead by pulling down it's attached to the origin
// and a third motor is in the bottom right. The cable is attached using a pulley at the bottom
// of the toolhead by making it shorter/longer the right amount you get a fully constrained toolhead in the x-y plane.
// this function computes the length of A1, A2 and the new A3 length.
//
// It's actually similar to the hangprinter kinematics but does not attempt 3d motion but proper 2d motion. The
// third access can be done easily like traditional printers using a moving bed in the z-plane.
//
// I've looked around and although the idea is pretty simple it seems like a Never Been Done beforei (NBD's are just like in skateboarding
// pretty rare in IT nowadays ;) ) 
void computeDrawbotImproved(double x1, double y1, double &lengteA1, double &lengteA2, double &lengteA3)
{

    // K1 x,y is katrol 1 x+y coordinate (set this to match your machine)
    double K1x = 0.0;
    double K1y = 18.0;

    // K2 x,y is katrol 2 x+y coordinate
    double K2x = 20.0;
    double K2y = 18.0;

    // K3 x,y is katrol 3 x+y coordinate
    double K3x = 20.0;
    // double K3y = 0.0; //not used, we assume it's always zero in further calculations

    // delta_offset and gamma_offset are added because the pulleys are most likely a bit off-center (they ride on a ring with certain
    // diameter which determines distance to center) for a simple drawbot it probably doesn't make much difference but if you want an exact
    // cnc this needs to be added.
    double gamma_offset = 0.0; // set this to be the distance to the center
    double delta_offset = 0.0; // set this to be distance to the center

    double gamma1 = K2x - x1;
    double gamma2 = K2y - y1;

    double delta1 = x1 - K1x;
    double delta2 = K1y - y1;

    // lengte A1 = stepper 1 needs to move so that lengteA1 is acchieved. Basically moving distance will be defined by: previous lengteA1 -
    // lengteA1
    lengteA1 = sqrt( sq(delta1) + sq(delta2) ) - delta_offset;

    // lengte A2 = stepper 2 needs to make cable 2 lengteA2 long
    lengteA2 = sqrt( sq(gamma1) + sq(gamma2) ) - gamma_offset;

    // lengte A3 = stepper 3 needs to make the bottom (additional tensioning cable) lengteA3 long so that everything stays tight without the
    // need of having bricks or weights on the toolhead.
    double omega_offset=1.0; // most likely the bottom pulley is offset to be lower by a few cm from center of toolhead. this allows to adjust that length
    double omega1 = K3x - x1;
    double omega2 = y1 - omega_offset;
    lengteA3 = sqrt( sq(x1) + sq(omega2) ) + sqrt( sq(omega1) + sq(omega2) );
}

void debugDrawbot(double x1, double y1, double &l1, double &l2)
{
    computeDrawbotTransform(x1, y1, l1, l2);

    cout << endl;
    cout << "x1 = " << x1 << endl;
    cout << "y1 = " << y1 << endl;
    cout << "lengteA1 = " << l1 << endl;
    cout << "lengteA2 = " << l2 << endl;
    cout << "--------------------------" << endl;
}

void debugDrawbotImproved(double x1, double y1, double &l1, double &l2, double &l3)
{
    computeDrawbotImproved(x1, y1, l1, l2, l3);

    cout << endl;
    cout << "x1 = " << x1 << endl;
    cout << "y1 = " << y1 << endl;
    cout << "lengteA1 = " << l1 << endl;
    cout << "lengteA2 = " << l2 << endl;
    cout << "lengteA3 = " << l3 << endl;
    cout << "--------------------------" << endl;
}

int main()
{

    double l1 = 0;
    double l2 = 0;

    debugDrawbot(9.0, 12.0, l1, l2);
    debugDrawbot(10.0, 12.0, l1, l2);
    debugDrawbot(11.0, 12.0, l1, l2);
    debugDrawbot(3.0, 16.0, l1, l2);
    debugDrawbot(20.0, 1.0, l1, l2);

    // same points but now also compute length of third bottom cable l3 which pulls down our toolhead
    double l3 = 0;
    debugDrawbotImproved(9.0, 12.0, l1, l2, l3);
    debugDrawbotImproved(10.0, 12.0, l1, l2, l3);
    debugDrawbotImproved(11.0, 12.0, l1, l2, l3);
    debugDrawbotImproved(3.0, 16.0, l1, l2, l3);
    debugDrawbotImproved(20.0, 1.0, l1, l2, l3);

    cout << "nine equally spaced points to verify my quick and dirty christmas maths ;) " << endl;

    cout << "top 3 points:" << endl;
    debugDrawbotImproved(1.0, 16.0, l1, l2, l3);  // top left
    debugDrawbotImproved(9.0, 16.0, l1, l2, l3);  // top center (approximately)
    debugDrawbotImproved(18.0, 16.0, l1, l2, l3); // top right
    cout << endl;

    cout << "middle 3 points:" << endl;
    debugDrawbotImproved(1.0, 8.0, l1, l2, l3);  // middle left
    debugDrawbotImproved(9.0, 8.0, l1, l2, l3);  // middle center (approx.)
    debugDrawbotImproved(18.0, 8.0, l1, l2, l3); // middle right
    cout << endl;

    cout << "bottom 3 points:" << endl;          // stay away from going to much into corners as cables won't be able to reach/tension
    debugDrawbotImproved(1.0, 3.0, l1, l2, l3);  // bottom left
    debugDrawbotImproved(9.0, 3.0, l1, l2, l3);  // bottom center (approx)
    debugDrawbotImproved(18.0, 3.0, l1, l2, l3); // bottom right
    cout << endl;

    return 0;
}


