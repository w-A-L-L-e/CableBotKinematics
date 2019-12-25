/*=============================================================================
author            : Walter Schreppers
filename          : cable_bot_kinematics.cpp
description       : computeDrawbotTransform: Makes moving in X-Y plane possible using 2 cables.
                      This is pretty standard and has been done before in various drawing bots.
                      The reason is you only need 2 steppers to move a toolhead in a large plane.
                  : computeDrawbotZeroG: Use third stepper to pull down the toolhead.
                    This allows faster movements, it also allows to have a horizontal x-y plane (for instance
                    on a 3d printer. And it solves many mechanical issues. You need a third stepper
                    but still it allows to construct large drawing machines and you have the benifit of being
                    able to move fast. The dead weight is now also eliminated and therefore you can move less mass
                    around and not worry about wobbling, chatter etc.
First version     : 24/12/2019
Improved version  : 25/12/2019
copyright         : Walter Schreppers
=============================================================================*/

#include <iostream>
#include <cmath>

using namespace std;

// Here we use 2 cables to move a toolhead in x-y plane. This relies however on gravity
// pulling down on the toolhead. The issue is that you will always need to go slow and the x-y plane
// needs to be vertical. Also in space you would be out of luck using these mechanics :)
void computeDrawbotTransform( double x1, double y1, double& lengteA1, double& lengteA2 ){
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
  lengteA1 = sqrt( delta1*delta1 + delta2 * delta2 );
  
  // lengte A2 = stepper 2 beweging
  lengteA2 = sqrt( gamma1*gamma1 + gamma2 * gamma2 );
}

//improved version, we have 3rd cable pulling down here, no need for bricks/dead weight
//as the third cable constrains the toolhead by pulling down it's attached to the origin
//and a third motor is in the bottom right. The cable is attached using a pulley at the bottom
//of the toolhead by making it shorter/longer the right amount you get a fully constrained toolhead in the x-y plane.
//this function computes the length of A1, A2 and the new A3 length.
//It's actually similar to the hangprinter kinematics but does not attempt 3d motion but proper 2d motion. The
//third access can be done easily like traditional printers using a moving bed in the z-plane.
//
//I've looked around and although the idea is pretty simple it seems like a Never Been Done before ;)
//Its a variation on the hangprinter and also a bit like a corexy machine combined. Will test this in practice
//when I'm back in belgium.
void computeDrawbotZeroG( double x1, double y1, double& lengteA1, double& lengteA2, double& lengteA3 ){
  // K1 x,y is katrol 1 x+y coordinate
  double K1x = 0.0;
  double K1y = 18.0;

  // K2 x,y is katrol 2 x+y coordinate
  double K2x = 20.0;
  double K2y = 18.0;

  // K3 x,y is katrol 3 x+y coordinate
  double K3x = 20.0;
  // double K3y = 0.0; //not used, we assume it's always zero in further calculations

  double gamma1 = K2x - x1;
  double gamma2 = K2y - y1;

  double delta1 = x1 - K1x;
  double delta2 = K1y - y1;

  // lengte A1 = stepper 1 movement 
  lengteA1 = sqrt( delta1*delta1 + delta2 * delta2 );
  
  // lengte A2 = stepper 2 movement
  lengteA2 = sqrt( gamma1*gamma1 + gamma2 * gamma2 );

  // lengte A3 = stepper 3 movement needed (this cable pulls down on toolhead) we again use some trigenometry to calculate the exact length needed.
  double omega = K3x - x1;
  lengteA3 = sqrt( x1*x1 + y1*y1 ) + sqrt( omega*omega + y1*y1 );
}


void debugDrawbot( double x1, double y1, double& l1, double& l2 ){
  computeDrawbotTransform( x1, y1, l1, l2 );
  
  cout << endl;
  cout << "x1 = " << x1 << endl;
  cout << "y1 = " << y1 << endl;
  cout << "lengteA1 = " << l1 << endl;
  cout << "lengteA2 = " << l2 << endl;
  cout << "--------------------------" <<endl;
}


void debugDrawbotZeroG( double x1, double y1, double& l1, double& l2, double& l3 ){
  computeDrawbotZeroG( x1, y1, l1, l2, l3 );
  
  cout << endl;
  cout << "x1 = " << x1 << endl;
  cout << "y1 = " << y1 << endl;
  cout << "lengteA1 = " << l1 << endl;
  cout << "lengteA2 = " << l2 << endl;
  cout << "lengteA3 = " << l3 << endl;
  cout << "--------------------------" <<endl;
}

int main(){

  double l1 = 0;
  double l2 = 0;

  debugDrawbot( 9.0,  12.0, l1, l2 );
  debugDrawbot( 10.0, 12.0, l1, l2 );
  debugDrawbot( 11.0, 12.0, l1, l2 );
  debugDrawbot( 3.0,  16.0, l1, l2 );
  debugDrawbot( 20.0, 1.0, l1, l2 );


  // same points but now also compute length of third bottom cable l3 which pulls down our toolhead
  double l3 = 0;
  debugDrawbotZeroG( 9.0,  12.0, l1, l2, l3 );
  debugDrawbotZeroG( 10.0, 12.0, l1, l2, l3 );
  debugDrawbotZeroG( 11.0, 12.0, l1, l2, l3 );
  debugDrawbotZeroG( 3.0,  16.0, l1, l2, l3 );
  debugDrawbotZeroG( 20.0, 1.0, l1, l2, l3 );

  return 0;
}




