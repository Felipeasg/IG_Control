/*
 * PLL_1P.h
 *
 *  Created on: 16-Aug-2015
 *      Author: Jaspreet Singh
 */

#ifndef PLL_INCLUDE_PLL_1P_H_
#define PLL_INCLUDE_PLL_1P_H_
#define GLOBAL_Q 	22


#include <stdbool.h>


#include "pll_1p.h"

//--------------------------------------------------------------//
#define  Fs	 					1000  						    // Sampling Frequency (TIMER_0_PERIOD = 1000000/(2*Fs))
//--------------------------------------------------------------//

#define  Ts					    (float) 1/Fs					// Sampling Period
#define  Ts_2					(float) 0.5/Fs					// Half Sampling Period

#define OSG_EN

typedef struct {
	float In;

	float B0;
	float B2;
	float A1;
	float A2;

	float qB0;
	float qB1;
	float qB2;

	float yn_1;
	float yn_2;
	float yn_3;

	float yn_q1;
	float yn_q2;
	float yn_q3;

	float xn_1;
	float xn_2;
	float xn_3;

	float out;
	float qout;

	void	(*compute)();
} OSG;

typedef struct {
	float	In;
	float	A0;
	float 	A1;
	float	A2;
	float	B0;
	float	B1;
	float	B2;
	float yn_1;
	float yn_2;
	float xn_1;
	float xn_2;

	float	Out;

	void	(*compute)();
} FILT_2P2Z;

typedef struct {
	float	Ref; 		// PI reference
	float	Fdb;    	// PI feedback
	float	Err;    	// (Do Not Change)
	float 	Kp;     	// Proportional Gain
	float	Ki_ts;  	// Integral Gain
	float	OutMax; 	// PI Output MAX Clamp
	float	OutMin; 	// PI Output MIN Clamp
	float	Out;    	// PI Output (Read)
	float	integral;   // (Do Not Change)

	void	(*compute)();
} PIREG;

typedef struct {
	float	In;			    // PLL Input
	float   Kp;             // Proportional Gain for PLL PI
	float   Ki_ts;          // Integral Gain for PLL PI
	float	OutMax; 	    // PI Output MAX Clamp
	float	OutMin; 	    // PI Output MIN Clamp
	float 	setFreq;	    // PLL Lock Frequency (Hz)
	float 	out_sine;		// PLL SINE Output  (Read)
	float 	out_theta;	    // PLL THETA Output (Read)

	bool Notch_EN;      // Set to "True" to enable Input 100Hz Notch

	float Mysin_1;	    // (Do Not Change)
	float Mysin_2;        // (Do Not Change)
	float Mycos_1;        // (Do Not Change) : Start Value 1.0
	float Mycos_2;        // (Do Not Change) : Start Value 1.0

	float wo;

	OSG			osg1;
	OSG			osg2;
	PIREG 		pi1;    // (Do Not Change)
	FILT_2P2Z 	Notch1; // (Do Not Change)

	void	(*compute)();
} PLL_1P;


//typedef PLL_1P *PLL_1P_handle;

#define OSG_F0_60Hz_Fs_16k	{0.0, 0.005855184, -0.005855184, 1.987737795, -0.988289633, 6.89798E-05, 0.00013796, 6.89798E-05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, osg_compute}
#define OSG_F0_60Hz_Fs_1k	{0.0, 0.083421477, -0.083421477, 1.707360421, -0.833157045, 0.015724578, 0.031449156, 0.015724578, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, osg_compute}

#define PIREG_DEFAULTS { 0, 0, 0, 0, 0, 0, 0, 0, 0, pireg_compute}


#define NOTCH_2P2Z_F0_0_1k_FS_16k_G_30_Q_12  { 0.0, 1.0, -1.995194282, 0.996733692, 0.998366846, -1.995194282, 0.998366846, 0.0, 0.0, 0.0, 0.0, 0.0, filt_2p2z_compute }
#define NOTCH_2P2Z_F0_0_1k_FS_1k  { 0.0, 1.0, -1.41750602, 0.944536385, 0.972268193, -1.41750602, 0.972268193, 0.0, 0.0, 0.0, 0.0, 0.0, filt_2p2z_compute }

//[1,-1.849203552627690,1.417692766130437]
//[1,-1.999984920355263,1.568474133858010]
#define NOTCH_2P2Z_F0_0_1k_FS_1k_MATLAB  { 0.0, 1.0, -1.849203552627690, 1.417692766130437, 1, -1.999984920355263, 1.568474133858010, 0.0, 0.0, 0.0, 0.0, 0.0, filt_2p2z_compute }

#define PLL_1P_DEFAULTS  {0.0, 50.0, 0.4, 200.0, -200.0, 50.0, 0.0, 0.0, 0.0, false, 0.0, 1.0, 1.0, 0.0, OSG_F0_60Hz_Fs_1k, OSG_F0_60Hz_Fs_1k, PIREG_DEFAULTS, NOTCH_2P2Z_F0_0_1k_FS_1k, pll_1p_compute};

void pll_1p_compute(PLL_1P *v);
void filt_2p2z_compute(FILT_2P2Z *v);
void pireg_compute(PIREG *v);
void osg_compute(OSG *v);

#endif /* PLL_INCLUDE_PLL_1P_H_ */
