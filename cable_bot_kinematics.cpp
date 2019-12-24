#include <iostream>
#include <cmath>

using namespace std;


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

void debugDrawbot( double x1, double y1, double& l1, double& l2 ){
  computeDrawbotTransform( x1, y1, l1, l2 );
  
  cout << endl;
  cout << "x1 = " << x1 << endl;
  cout << "y1 = " << y1 << endl;
  cout << "lengteA1 = " << l1 << endl;
  cout << "lengteA2 = " << l2 << endl;
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

  return 0;
}




