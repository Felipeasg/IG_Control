/*
 * PLL_1P.c
 *
 *  Created on: 16-Aug-2015
 *      Author: Jaspreet Singh
 */

#include "PLL_1P.h"
#include <math.h>

void osg_compute(OSG *v)
{
	v->yn_1 = (v->B0 * (v->In - v->xn_2)) +
			v->A1*v->yn_2 +
			v->A2*v->yn_3;

	v->yn_3 = v->yn_2;
	v->yn_2 = v->yn_1;


	v->yn_q1 = (v->qB0 * v->xn_1 + v->qB1*v->xn_1 + v->qB2*v->xn_3) +
			v->A1*v->yn_q2 +
			v->A2*v->yn_q3;

	v->yn_q3 = v->yn_q2;
	v->yn_q2 = v->yn_q1;

	v->xn_1 = v->In;
	v->xn_2 = v->xn_1;
	v->xn_3 = v->xn_2;

	v->out = v->yn_1;
	v->qout = v->yn_q1;
}

void pll_1p_compute(PLL_1P *v)
{
#if 0
	float temp = Ts;
	float OMEGA_0 = (6.2831852 * v->setFreq);
#endif


	if(v->Notch_EN == true) {

		v->Notch1.In   =   (v->In * v->Mycos_1);

#if 1
		v->Notch1.compute(&v->Notch1);
#else
		float temp1 = 0.0;

	    temp1   =  (v->Notch1.B0 * v->Notch1.In);
	    temp1  +=  (v->Notch1.B1 * v->Notch1.xn_1);
	    temp1  +=  (v->Notch1.B2 * v->Notch1.xn_2);
	    temp1  -=  (v->Notch1.A1 * v->Notch1.yn_1);
	    temp1 -=  (v->Notch1.A2 * v->Notch1.yn_2);

	    v->Notch1.xn_2 = v->Notch1.xn_1;
	    v->Notch1.xn_1 = v->Notch1.In;

	    v->Notch1.yn_2 = v->Notch1.yn_1;
	    v->Notch1.yn_1 = temp1;
	    v->Notch1.Out  = temp1;
#endif

	    //Update pireg
		v->pi1.Ref 	   =   v->Notch1.Out;
	}

	else {
		v->pi1.Ref     =   (v->In * v->Mycos_1);
	}

	v->pi1.Fdb 	   =   0;
	v->pi1.Kp  	   =   v->Kp;
	v->pi1.Ki_ts   =   v->Ki_ts;
	v->pi1.OutMax  =   v->OutMax;
	v->pi1.OutMin  =   v->OutMin;

#if 1
	v->pi1.compute(&v->pi1);
#else
	v->pi1.Err =  v->pi1.Ref - v->pi1.Fdb;

	v->pi1.integral =  v->pi1.integral + (v->Ki_ts * v->pi1.Err);

    if ( v->pi1.integral > v->pi1.OutMax){
    	v->pi1.integral = v->pi1.OutMax;
    } else if (v->pi1.integral < v->pi1.OutMin){
    	v->pi1.integral =  v->pi1.OutMin;
    }

    v->pi1.Out = (v->Kp * v->pi1.Err) + v->pi1.integral;

    if ( v->pi1.Out > v->pi1.OutMax)
    {
    	v->pi1.Out = v->pi1.OutMax;
	} else if ( v->pi1.Out < v->pi1.OutMin){
		v->pi1.Out =  v->pi1.OutMin;
	}
#endif

#if 0
    //NEw omega
	OMEGA_0   = OMEGA_0 + v->pi1.Out;

	v->wo = OMEGA_0;


	temp = (temp * OMEGA_0);

	v->Mysin_1 = v->Mysin_2 + (temp * v->Mycos_2);
	v->Mycos_1 = v->Mycos_2 - (temp * v->Mysin_2);

	if(v->Mysin_1 > 1.0) {
		v->Mysin_1 = 1.0;
	}
	else if(v->Mysin_1 < -1.0) {
		v->Mysin_1 = -1.0;
	}

	if(v->Mycos_1 > 1.0) {
		v->Mycos_1 = 1.0;
	}
	else if(v->Mycos_1 < -1.0) {
		v->Mycos_1 = -1.0;
	}

	v->Mysin_2   =  v->Mysin_1;
	v->Mycos_2   =  v->Mycos_1;
	v->out_sine  =  v->Mysin_1;

	v->out_theta =  asin(v->Mysin_1);  // Inverse SINE
#endif
}

void filt_2p2z_compute(FILT_2P2Z *v){

#if 0
	float temp = 0.0;

    temp   =  (v->B0 * v->In);
    temp  +=  (v->B1 * v->xn_1);
    temp  +=  (v->B2 * v->xn_2);
    temp  -=  (v->A1 * v->yn_1);
    temp  -=  (v->A2 * v->yn_2);

    v->xn_2 = v->xn_1;
    v->xn_1 = v->In;

    v->yn_2 = v->yn_1;
    v->yn_1 = temp;
    v->Out  = temp;
#endif

    v->Out = -(v->A1 * v->yn_1) -
    		(v->A2 * v->yn_2) +
			(v->B0 * v->In) +
			(v->B1 * v->xn_1) +
			(v->B2 * v->xn_2);

    v->xn_2 = v->xn_1;
    v->xn_1 = v->In;

    v->yn_2 = v->yn_1;
    v->yn_1 = v->Out;
}

void pireg_compute(PIREG *v){
    v->Err =  v->Ref - v->Fdb;

    v->integral =  v->integral + (v->Ki_ts * v->Err);

    if ( v->integral > v->OutMax){
    	 v->integral = v->OutMax;
    } else if (v->integral < v->OutMin){
    	 v->integral =  v->OutMin;
    }

    v->Out = (v->Kp * v->Err) + v->integral;

    if ( v->Out > v->OutMax){
		 v->Out = v->OutMax;
	} else if ( v->Out < v->OutMin){
		 v->Out =  v->OutMin;
	}
}
