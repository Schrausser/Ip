//---------------------------------------------------------------------------| Ip_CALC.h von Dietmar SCHRAUSSER 2009

#include <math.h>

//#define _nP_ 5000 hier als globale setzen
//#define _nR_ 1600
#define _n_ 1600 //nA maximal Dimensionierung

double SIGMA = 34.0/45, quasizufallszahl,

	  //theta --------------------------------------------------------------------		   
			   n_1,   n_2,   n_diff, MIN_,    MIN_1,     MIN_2,     MIN_diff,       
						             MAX_,    MAX_1,     MAX_2,     MAX_diff,
	   RNG_, RNG_1, RNG_2, RNG_diff, SUM_,    SUM_1,     SUM_2,     SUM_diff,  
	    
	    MD_,  MD_1,  MD_2,  MD_diff,  AM_,     AM_1,      AM_2,      AM_diff,
	    GM_,  GM_1,  GM_2,  GM_diff,  HM_,     HM_1,      HM_2,      HM_diff, 
		 
		 V_,   V_1,   V_2,   V_diff,   V_df1,   V_1df1,    V_2df1,    V_df1_diff, 
									   S_V_df1, S_1V_1df1, S_2V_2df1, S_V_df1_diff, 
	     S_,   S_1,   S_2,   S_diff,   S_df1,   S_1df1,    S_2df1,    S_df1_diff,
									   S_df2,   S_1df2,    S_2df2,    S_df2_diff,
	     D_,   D_1,   D_2,   D_diff,   D_df1,   D_1df1,    D_2df1,    D_df1_diff,
									   D_df2,   D_1df2,    D_2df2,    D_df2_diff,         
	    DV_,  DV_1,  DV_2,  DV_diff,

		//lineare regression ------------------------------------------------------

		B0x_, B1x_, B0y_, B1y_, reg_;
	    

//Zufallszahlenfunktion, n(0,1)
double qzufall(double seed)
{
	quasizufallszahl =           10*( pow(seed,SIGMA) - floor( pow(seed,SIGMA) ) )
						- floor( 10*( pow(seed,SIGMA) - floor( pow(seed,SIGMA) ) ) );
	
	return quasizufallszahl;
};

//Kennwertfunktion für 2 unverbundene Gruppen
double theta_zg_uv(double P_n2[_n_][2]/*Planmatrix S*/, double n/*nS*/)
{
	int iLauf, jLauf, x = 0, x1 = 0, x2 = 0, hub = 0, index;
	double P_n2_1[100]/*sortierungsvektor*/, P_n21[50]/*gruppenvektor*/;
	
	         n_1 =   n_2  = 
	SUM_ = SUM_1 = SUM_2  = AM_ = AM_1 = AM_2 = 
	  D_ =   D_1 =   D_2  =  V_ =  V_1 =  V_2 = 
	 HM_ =  HM_1 =  HM_2                      =  0;
   
	 GM_ =  GM_1 =  GM_2  = 1;

	MIN_ = MIN_1 =  MIN_2 = 
	MAX_ = MAX_1 =  MAX_2 = P_n2[1][1];//9999999 im zsh mit sortierung beachten
	
	//n_g, min, max, summe, range, md, am, gm, hm
	//---------------------------------------------------------------------------------------
	for (iLauf = 1; iLauf <= n; iLauf++)
	{
		//gesamt s1
		SUM_ += P_n2[iLauf][1];
		 GM_ *= P_n2[iLauf][1];
		 HM_ += 1/P_n2[iLauf][1];
		
		if(MIN_ > P_n2[iLauf][1]) { MIN_ = P_n2[iLauf][1]; }
		if(MAX_ < P_n2[iLauf][1]) { MAX_ = P_n2[iLauf][1]; }
		
		//g1
		if (P_n2[iLauf][2] == 1)	
		{ 
			SUM_1 += P_n2[iLauf][1];
			 GM_1 *= P_n2[iLauf][1];  
			 HM_1 += 1/P_n2[iLauf][1];  n_1++; 
			
			if(MIN_1 > P_n2[iLauf][1]) { MIN_1 = P_n2[iLauf][1]; }
			if(MAX_1 < P_n2[iLauf][1]) { MAX_1 = P_n2[iLauf][1]; }
		}
		
		//g2
		if (P_n2[iLauf][2] == 2)	
		{ 
			SUM_2 += P_n2[iLauf][1];
			 GM_2 *= P_n2[iLauf][1];
			 HM_2 += 1/P_n2[iLauf][1];  n_2++; 
		
			if(MIN_2 > P_n2[iLauf][1]) { MIN_2 = P_n2[iLauf][1]; }
			if(MAX_2 < P_n2[iLauf][1]) { MAX_2 = P_n2[iLauf][1]; }
		}
	}

	//sortierung md bestimmung gesamt s1
	P_n2_1[1] = MAX_;

	for (iLauf = 1; iLauf <= n; iLauf++)
	{
		for (jLauf = 0; jLauf < iLauf; jLauf++)
		{
			if (P_n2[iLauf][1] < P_n2_1[iLauf-jLauf])
			{
				P_n2_1[(iLauf-jLauf)+1] = P_n2_1[iLauf-jLauf]; hub++;	
			}	
		}

		P_n2_1[(iLauf+1)-hub] = P_n2[iLauf][1]; hub=0;

	}
	
	//md gesamt s1
	if (floor(n/2) == n/2)//n geradzahlig
	{
		for (iLauf = 1; iLauf <= n/2; iLauf++)
		{
			MD_ = ( P_n2_1[iLauf]+P_n2_1[iLauf+1] ) /2;
		}
	}
	else
	{
		for (iLauf = 1; iLauf <= (floor((n-1)/2))+1; iLauf++)
		{
			MD_ = P_n2_1[iLauf];
		}
	}
	
	//sortierung md bestimmung g1
	//übertragen des gruppenvectors s11 nach P_n21
	
	index=1;

	for (iLauf = 1; iLauf <= n; iLauf++)
	{
		if (P_n2[iLauf][2] == 1)	
		{ 
			P_n21[index] = P_n2[iLauf][1]; 
			index++;	
		}
	}
	
	P_n2_1[1] = MAX_1; hub = 0;

	for (iLauf = 1; iLauf <= n_1; iLauf++)
	{
		for (jLauf = 0; jLauf < iLauf; jLauf++)
		{
			
			if (P_n21[iLauf] < P_n2_1[iLauf-jLauf])
			{
				P_n2_1[(iLauf-jLauf)+1] = P_n2_1[iLauf-jLauf]; hub++;	
			}	
		}
	
		P_n2_1[(iLauf+1)-hub] = P_n21[iLauf]; hub=0;
	}

	/*//test bildschirmausgabe
	for (iLauf = 1; iLauf <= n_1; iLauf++)
	{
		printf("%5.3f\n", P_n2_1[iLauf]);
	}*/

	//md g1
	if (floor(n_1/2) == n_1/2)//n_1 geradzahlig
	{
		for (iLauf = 1; iLauf <= n_1/2; iLauf++)
		{
			MD_1 = ( P_n2_1[iLauf]+P_n2_1[iLauf+1] ) /2;
		}
	}
	else
	{
		for (iLauf = 1; iLauf <= (floor((n_1-1)/2))+1; iLauf++)
		{
			MD_1 = P_n2_1[iLauf];
		}
	}
	
	//sortierung md bestimmung g2
	//übertragen des gruppenvectors s12 nach P_n21
	
	index=1;
	
	for (iLauf = 1; iLauf <= n; iLauf++)
	{
		if (P_n2[iLauf][2] == 2)	
		{ 
			P_n21[index] = P_n2[iLauf][1];
			index++;
		}
	}

	/*//test bildschirmausgabe
	for (iLauf = 1; iLauf <= n_1; iLauf++)
	{
		printf("%5.3f\n", P_n21[iLauf]);
	}*/

	P_n2_1[1] = MAX_2; hub = 0;

	for (iLauf = 1; iLauf <= n_2; iLauf++)
	{
		for (jLauf = 0; jLauf < iLauf; jLauf++)
		{
			
			if (P_n21[iLauf] < P_n2_1[iLauf-jLauf])
			{
				P_n2_1[(iLauf-jLauf)+1] = P_n2_1[iLauf-jLauf]; hub++;
			}
		}
	
		P_n2_1[(iLauf+1)-hub] = P_n21[iLauf]; hub=0;
	}

	//md g2
	if (floor(n_2/2) == n_2/2)//n_2 geradzahlig
	{
		for (iLauf = 1; iLauf <= n_2/2; iLauf++)
		{
			MD_2 = ( P_n2_1[iLauf]+P_n2_1[iLauf+1] ) /2;
		}
	}
	else
	{
		for (iLauf = 1; iLauf <= (floor((n_2-1)/2))+1; iLauf++)
		{
			MD_2 = P_n2_1[iLauf];
		}
	}
	
	//theta berechnung
	
	RNG_ =  MAX_ -  MIN_;    AM_  = SUM_  / n;	
	RNG_1 = MAX_1 - MIN_1;   AM_1 = SUM_1 / n_1;	
	RNG_2 = MAX_2 - MIN_2;   AM_2 = SUM_2 / n_2;

	if (MIN_ > 0)// geometrisches mittel gm bei min > 0
	{
		GM_ =  pow(GM_, (1/n)); 
		GM_1 = pow(GM_1,(1/n_1)); 
		GM_2 = pow(GM_2,(1/n_2));
	}

	HM_  = n   / HM_; 
	HM_1 = n_1 / HM_1; 
	HM_2 = n_2 / HM_2; 
	
	//differenz g2-g1
	  n_diff =   n_2 -   n_1;
	MIN_diff = MIN_2 - MIN_1;	MD_diff = MD_2 - MD_1; 
	MAX_diff = MAX_2 - MAX_1;	AM_diff = AM_2 - AM_1;
	RNG_diff = RNG_2 - RNG_1;	GM_diff = GM_2 - GM_1;
	SUM_diff = SUM_2 - SUM_1;	HM_diff = HM_2 - HM_1;

	//varianz, standardabweichung, Schrausser's d, df1 df2 korrektur, dvaro, 
	//---------------------------------------------------------------------------------------
	for (iLauf = 1; iLauf <= n; iLauf++)
	{
		//gesamtsummierung s1
		 V_ += pow((AM_ - P_n2[iLauf][1]),2);
		 D_ += sqrt(pow((AM_ - P_n2[iLauf][1]),2));

		if (AM_ == P_n2[iLauf][1]) {  x++; }//x zur n korrektur bei 0 differenz 

		//summierung g1
		if (P_n2[iLauf][2] == 1)
		{
			 V_1 += pow((AM_1 - P_n2[iLauf][1]),2);
			 D_1 += sqrt(pow((AM_1 - P_n2[iLauf][1]),2));
		
			if (AM_1 == P_n2[iLauf][1]) {  x1++; }//x1 zur n_1 korrektur bei 0 diff 
		}

		//summierung g2
		if (P_n2[iLauf][2] == 2)
		{
			 V_2 += pow((AM_2 - P_n2[iLauf][1]),2);
			 D_2 += sqrt(pow((AM_2 - P_n2[iLauf][1]),2));
		
			if (AM_2 == P_n2[iLauf][1])	{  x2++; }//x2 zur n_2 korrektur bei 0 diff 
		}	
	}
	
	//theta berechnung
									//df korrektur
	V_  =  V_ / n;   S_  = sqrt(V_);  S_df1  = S_  * (n   /(n   - 1));  S_df2 = S_  * sqrt(n   /(n   - 1));	
	V_1 = V_1 / n_1; S_1 = sqrt(V_1); S_1df1 = S_1 * (n_1 /(n_1 - 1)); S_1df2 = S_1 * sqrt(n_1 /(n_1 - 1));
	V_2 = V_2 / n_2; S_2 = sqrt(V_2); S_2df1 = S_2 * (n_2 /(n_2 - 1)); S_2df2 = S_2 * sqrt(n_2 /(n_2 - 1));	
	 
									   V_df1 = V_  * (n   /(n   - 1));    
									  V_1df1 = V_1 * (n_1 /(n_1 - 1));   	
									  V_2df1 = V_2 * (n_2 /(n_2 - 1));	
	
								   S_V_df1   = sqrt(V_df1);						       /*S_V_df1 == S_df2*/
								   S_1V_1df1 = sqrt(V_1df1);					     /*S_1V_1df1 == S_1df2*/
								   S_2V_2df1 = sqrt(V_2df1);                         /*S_2V_2df1 == S_2df2*/
									  
	//Schrausser's d (n korrektur, division durch null = 0)
	D_  = D_  / (n   - x );	 if(n==x) D_=0;     D_df1 = D_  * (n   /(n   - 1));  D_df2 = D_  * sqrt(n   /(n   - 1));
	D_1 = D_1 / (n_1 - x1);  if(n_1==x1) D_1=0; D_1df1 = D_1 * (n_1 /(n_1 - 1)); D_1df2 = D_1 * sqrt(n_1 /(n_1 - 1));
	D_2 = D_2 / (n_1 - x2);  if(n_2==x2) D_2=0; D_2df1 = D_2 * (n_2 /(n_2 - 1)); D_2df2 = D_2 * sqrt(n_2 /(n_2 - 1));

	//dvaro
	DV_ = sqrt(D_  /((1/25) + SIGMA)); 
	DV_1= sqrt(D_1 /((1/25) + SIGMA)); 
	DV_2= sqrt(D_2 /((1/25) + SIGMA));
	
	//differenz g2-g1
	V_diff  =  V_2 -  V_1;		  V_df1_diff =    V_2df1 -    V_1df1;
								S_V_df1_diff = S_2V_2df1 - S_1V_1df1;

	S_diff  =  S_2 -  S_1;        S_df1_diff =    S_2df1 -    S_1df1;	S_df2_diff =  S_2df2 - S_1df2;
	D_diff  =  D_2 -  D_1;        D_df1_diff =  D_2df1 - D_1df1;	    D_df2_diff =  D_2df2 - D_1df2;
	DV_diff = DV_2 - DV_1; 
 
	return 0;
};

//Lineare Regressionsfunktion (f(x=y)=bx+a, b=B1y_, a=B0y_; f(y=x)=by+a, b=B1x_, a=B0x_; r=reg_)
double fn_lin_reg(double R[_n_][2]/*Regressionsmatrix R(x,y)*/, double n/*nR*/)
{
	int iLauf; 
	double sum_x=0 ,sum_y=0, sum_y2=0, sum_x2=0, sum_xy=0, am_x, am_y, s2_x=0, s2_y=0, s_x, s_y, r_=0;
	
	for (iLauf = 1; iLauf <= n; iLauf++)// summierng x, y, y2, xy über n
	{
		sum_x  +=     R[iLauf][1]; 
		sum_y  +=     R[iLauf][2]; 
		sum_x2 += pow(R[iLauf][1],2);
		sum_y2 += pow(R[iLauf][2],2); 
		sum_xy +=     R[iLauf][1]*R[iLauf][2];
	}
	
	//mittelung
	am_x = sum_x / n;
	am_y = sum_y / n;

	for (iLauf = 1; iLauf <= n; iLauf++)//standardabweichungsberechnung (nb s unkorrigiert nach df)
	{
		s2_x  +=    pow(R[iLauf][1]-am_x,2);
		s2_y  +=    pow(R[iLauf][2]-am_y,2); 
	}

	s_x = sqrt(s2_x/n);
	s_y = sqrt(s2_y/n);

	//beta gewichte
	B1y_ = ((n * sum_xy) - (sum_x * sum_y)) / ((n * sum_x2) - pow(sum_x,2)); // byx
	B1x_ = ((n * sum_xy) - (sum_x * sum_y)) / ((n * sum_y2) - pow(sum_y,2)); // bxy

	B0y_ = am_y - (B1y_ * am_x);// ayx
	B0x_ = am_x - (B1x_ * am_y);// axy

	
	for (iLauf = 1; iLauf <= n; iLauf++)//korrelationskoeffizient
	{
		r_ += ((R[iLauf][1]-am_x) /s_x) * ((R[iLauf][2]-am_y) /s_y);	
	}
	
	reg_ = r_ /n;

	return 0;
};