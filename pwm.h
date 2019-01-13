#ifndef PWM_H
#define	PWM_H

#define MIN_ABS (-10000)
#define MAX_ABS (10000)

void setupPWM ();
int saturateRPM(int n);
int refreshPWMvalue(int n1, int n2);
short int refreshPWMRange(int min, int max);
int sign(int x);

#endif	/* PWM_H */

