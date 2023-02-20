//---------------------------------------------------------------------------| Ip_CALC von Dietmar SCHRAUSSER 2009               

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Ip_calc.h"


void main()
{
	/*double P_n2[1000][2], n=800; 
	float s_ij;
	//int logcode;
	int iLauf;
	
	//FILE *outStream, *batStream; 

	FILE *inStream;

	inStream = fopen( "Ip_daten.asc", "r" );

	for (iLauf = 1; iLauf <= n; iLauf++)//daten einlesen
	{
		
			fscanf(inStream,"%f", &s_ij );

			 P_n2[iLauf][1] = s_ij;

			 fscanf(inStream,"%f", &s_ij );
			
			 P_n2[iLauf][2] = s_ij;	

	}
	
	//explizite datendefinition 
	P_n2[1][1]	= 12;			P_n2[1][2]	= 1;
	P_n2[2][1]	= 23.5;			P_n2[2][2]	= 2;
	P_n2[3][1]	= 14;			P_n2[3][2]	= 1;
	P_n2[4][1]	= 11.65;		P_n2[4][2]	= 2;
	P_n2[5][1]	= 11.78;		P_n2[5][2]	= 1;
	P_n2[6][1]	= 6.34;			P_n2[6][2]	= 2;
	P_n2[7][1]	= 13.54;		P_n2[7][2]	= 1;
	P_n2[8][1]	= 22.43;		P_n2[8][2]	= 2;
	P_n2[9][1]	= 23.76;		P_n2[9][2]	= 2;
	P_n2[10][1] = 10.521;		P_n2[10][2] = 2;
	P_n2[11][1] = 18.76;		P_n2[11][2] = 2;
	P_n2[12][1]	= 14.8;			P_n2[12][2]	= 1;
	P_n2[13][1]	= 5.32;			P_n2[13][2]	= 2;
	P_n2[14][1]	= 21.54;		P_n2[14][2]	= 2;
	P_n2[15][1] = 12.3;			P_n2[15][2] = 2;
	P_n2[16][1] = 9.541;		P_n2[16][2] = 1;
	P_n2[17][1]	= 15.71;		P_n2[17][2]	= 2;
	P_n2[18][1]	= 16.43;		P_n2[18][2]	= 2;
	P_n2[19][1] = 21;			P_n2[19][2] = 2;
	P_n2[20][1] = 12.54;		P_n2[20][2] = 2;

	
	fn_lin_reg(P_n2,n);

	printf ("\n_Ip_CALC_\nfn_lin_reg:\nf(x=y)= %5.3fx + %5.3f\nf(y=x)= %5.3fy + %5.3f\n", B1x_, B0x_, B1y_, B0y_);
	printf ("r= %.3f det= %3.2f%%\nEND\n\n", reg_, 100*(reg_*reg_));
	
	/*
	outStream = fopen( "Ip_CALC.asc", "a" );
	
	fprintf (outStream, "Ip_CALC:theta_zg_uv(P_n2,n)\n");
	fprintf (outStream, "\n");
	fprintf (outStream, "\tG1\ts1\tG2\tdiff\tp\n");
	fprintf (outStream, "-----------------------------------------\n");
	fprintf (outStream, "n\t%2.0f\t%2.0f\t%2.0f\t%2.0f\n",		n_1, n, n_2, n_diff);
	fprintf (outStream, "Min\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	MIN_1, MIN_, MIN_2, MIN_diff);
	fprintf (outStream, "Max\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	MAX_1, MAX_, MAX_2, MAX_diff);
	fprintf (outStream, "Rng\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	RNG_1, RNG_, RNG_2, RNG_diff);	
	fprintf (outStream, "SUM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	SUM_1, SUM_, SUM_2, SUM_diff);
	fprintf (outStream, "\n");
	fprintf (outStream, "Md\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		MD_1, MD_, MD_2, MD_diff);
	fprintf (outStream, "AM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		AM_1, AM_, AM_2, AM_diff);
	if (MIN_ > 0)
	{fprintf (outStream, "GM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	GM_1, GM_, GM_2, GM_diff);}
	fprintf (outStream, "HM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		HM_1, HM_, HM_2, HM_diff);
	fprintf (outStream, "\n");
	fprintf (outStream, "V\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		V_1, V_, V_2, V_diff);
	fprintf (outStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	V_1df1, V_df1, V_2df1, V_df1_diff);
	fprintf (outStream, "SVdf1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	S_1V_1df1, S_V_df1, S_2V_2df1, S_V_df1_diff);
	fprintf (outStream, "\n");
	fprintf (outStream, "S\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		S_1, S_, S_2, S_diff);
	fprintf (outStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	S_1df1, S_df1, S_2df1, S_df1_diff);
	fprintf (outStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	S_1df2, S_df2, S_2df2, S_df2_diff);
	fprintf (outStream, "\n");
	fprintf (outStream, "D\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",		D_1, D_, D_2, D_diff);
	fprintf (outStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	D_1df1, D_df1, D_2df1, D_df1_diff);
	fprintf (outStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n",	D_1df2, D_df2, D_2df2, D_df2_diff);
	fprintf (outStream, "DvarO\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n\n",DV_1, DV_, DV_2, DV_diff);
		
	fclose( outStream  );

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "md Ip_log_%i\n",logcode );
	fprintf (batStream, "move Ip_CALC.asc Ip_log_%i\n", logcode);
	
	fclose( batStream  );

	system("Ip_CALC.asc");
	system ("verschub.bat");
	system ("del verschub.bat");
	

	fclose( inStream  );*/
	//////////////////////////////////////////////////////////////////////////
	// systematische permutation


	
}
