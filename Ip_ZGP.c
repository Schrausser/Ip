//-----------------------------------------------------------------------------| Ip_ZGP von Dietmar SCHRAUSSER 2009              
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include "Ip_CALC.h"

#define _nP_ 5000 //----------------------------------------------------------> n Thetamatrix PVt
#define _ini_ 21 //-----------------------------------------------------------> n ini schalter +1

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

void _log(); //---------------------------------------------------------------> prozedur für logdatei                      

double S_n2[_n_][2];//--------------------------------------------------------> planmatrix S(n,2), 
float s_ij, t_ij;//-----------------------------------------------------------> planvektorelement sij aus datenmatrix A(n,k), thetamatrixemente tij
double P_2n2[_n_][3];//-------------------------------------------------------> permutationsmatrix P(n,3)
double PS_n2[_n_][2];//-------------------------------------------------------> permutierte Planmatrix PS(n,2)
double n_n[_n_];//------------------------------------------------------------> zufallsvektor n(n)
double seed, Oseed, nA_,  /*--------------------------------------------------> nA zur übergabe an funktion*/
	  //----------------------------------------------------------------------> theta spiegel
	  
											 __MIN_,     __MIN_1,     __MIN_2,        __MIN_diff,       
			     		                     __MAX_,     __MAX_1,     __MAX_2,        __MAX_diff,
	   __RNG_, __RNG_1, __RNG_2, __RNG_diff, __SUM_,     __SUM_1,     __SUM_2,        __SUM_diff,  
	      
	    __MD_,  __MD_1,  __MD_2,  __MD_diff,  __AM_,      __AM_1,      __AM_2,         __AM_diff,
	    __GM_,  __GM_1,  __GM_2,  __GM_diff,  __HM_,      __HM_1,      __HM_2,         __HM_diff, 
		 
		 __V_,   __V_1,   __V_2,   __V_diff,   __V_df1,    __V_1df1,    __V_2df1,   __V_df1_diff, 
									         __S_V_df1, __S_1V_1df1, __S_2V_2df1, __S_V_df1_diff, 
	     __S_,   __S_1,   __S_2,   __S_diff,   __S_df1,    __S_1df1,    __S_2df1,   __S_df1_diff,
									           __S_df2,    __S_1df2,    __S_2df2,   __S_df2_diff,
	     __D_,   __D_1,   __D_2,   __D_diff,   __D_df1,    __D_1df1,    __D_2df1,   __D_df1_diff,
									           __D_df2,    __D_1df2,    __D_2df2,   __D_df2_diff,         
	    __DV_,  __DV_1,  __DV_2,  __DV_diff,
		
			     q_n_1,   q_n_2,   q_n_diff=0, q_MIN_,     q_MIN_1,     q_MIN_2,        q_MIN_diff=0,       
			     		                       q_MAX_,     q_MAX_1,     q_MAX_2,        q_MAX_diff=0,
	   q_RNG_, q_RNG_1, q_RNG_2, q_RNG_diff=0, q_SUM_,     q_SUM_1,     q_SUM_2,        q_SUM_diff=0,  
	      
	    q_MD_,  q_MD_1,  q_MD_2,  q_MD_diff=0,  q_AM_,      q_AM_1,      q_AM_2,         q_AM_diff=0,
	    q_GM_,  q_GM_1,  q_GM_2,  q_GM_diff=0,  q_HM_,      q_HM_1,      q_HM_2,         q_HM_diff=0, 
		 
		 q_V_,   q_V_1,   q_V_2,   q_V_diff=0,   q_V_df1,    q_V_1df1,    q_V_2df1,   q_V_df1_diff=0, 
									           q_S_V_df1, q_S_1V_1df1, q_S_2V_2df1, q_S_V_df1_diff=0, 
	     q_S_,   q_S_1,   q_S_2,   q_S_diff=0,   q_S_df1,    q_S_1df1,    q_S_2df1,   q_S_df1_diff=0,
									             q_S_df2,    q_S_1df2,    q_S_2df2,   q_S_df2_diff=0,
	     q_D_,   q_D_1,   q_D_2,   q_D_diff=0,   q_D_df1,    q_D_1df1,    q_D_2df1,   q_D_df1_diff=0,
									             q_D_df2,    q_D_1df2,    q_D_2df2,   q_D_df2_diff=0,         
	    q_DV_,  q_DV_1,  q_DV_2,  q_DV_diff=0,
		
		//--------------------------------------------------------------------> ereigniszähler, p indikatoren
		p_MIN_diff_zaehler = 0, p2_MIN_diff_zaehler = 0, pkt_p_MIN_diff_zaehler = 0,  p_MIN_diff,  p2_MIN_diff, pkt_p_MIN_diff,
		p_MAX_diff_zaehler = 0, p2_MAX_diff_zaehler = 0, pkt_p_MAX_diff_zaehler = 0,  p_MAX_diff,  p2_MAX_diff, pkt_p_MAX_diff,
		p_RNG_diff_zaehler = 0, p2_RNG_diff_zaehler = 0, pkt_p_RNG_diff_zaehler = 0,  p_RNG_diff,  p2_RNG_diff, pkt_p_RNG_diff,
		p_SUM_diff_zaehler = 0, p2_SUM_diff_zaehler = 0, pkt_p_SUM_diff_zaehler = 0,  p_SUM_diff,  p2_SUM_diff, pkt_p_SUM_diff,
		
		p_MD_diff_zaehler = 0, p2_MD_diff_zaehler = 0,pkt_p_MD_diff_zaehler = 0, p_MD_diff, p2_MD_diff, pkt_p_MD_diff,
		p_AM_diff_zaehler = 0, p2_AM_diff_zaehler = 0,pkt_p_AM_diff_zaehler = 0, p_AM_diff, p2_AM_diff, pkt_p_AM_diff,
		p_GM_diff_zaehler = 0, p2_GM_diff_zaehler = 0,pkt_p_GM_diff_zaehler = 0, p_GM_diff, p2_GM_diff, pkt_p_GM_diff,
		p_HM_diff_zaehler = 0, p2_HM_diff_zaehler = 0,pkt_p_HM_diff_zaehler = 0, p_HM_diff, p2_HM_diff, pkt_p_HM_diff,
		
		p_V_diff_zaehler = 0,  p2_V_diff_zaehler = 0,pkt_p_V_diff_zaehler = 0,  p_V_diff,  p2_V_diff, pkt_p_V_diff,
		p_Vdf1_diff_zaehler = 0,  p2_Vdf1_diff_zaehler = 0,pkt_p_Vdf1_diff_zaehler = 0,  p_Vdf1_diff, p2_Vdf1_diff, pkt_p_Vdf1_diff,
		p_SVdf1_diff_zaehler = 0,  p2_SVdf1_diff_zaehler = 0,pkt_p_SVdf1_diff_zaehler = 0,  p_SVdf1_diff, p2_SVdf1_diff, pkt_p_SVdf1_diff,

		p_S_diff_zaehler = 0,  p2_S_diff_zaehler = 0,pkt_p_S_diff_zaehler = 0,  p_S_diff,  p2_S_diff,pkt_p_S_diff,
		p_Sdf1_diff_zaehler = 0, p2_Sdf1_diff_zaehler = 0, pkt_p_Sdf1_diff_zaehler = 0, p_Sdf1_diff, p2_Sdf1_diff, pkt_p_Sdf1_diff,
		p_Sdf2_diff_zaehler = 0, p2_Sdf2_diff_zaehler = 0, pkt_p_Sdf2_diff_zaehler = 0, p_Sdf2_diff, p2_Sdf2_diff, pkt_p_Sdf2_diff,
		p_D_diff_zaehler = 0, p2_D_diff_zaehler = 0,  pkt_p_D_diff_zaehler = 0,  p_D_diff,  p2_D_diff, pkt_p_D_diff,
		p_Ddf1_diff_zaehler = 0, p2_Ddf1_diff_zaehler = 0, pkt_p_Ddf1_diff_zaehler = 0, p_Ddf1_diff, p2_Ddf1_diff, pkt_p_Ddf1_diff,
		p_Ddf2_diff_zaehler = 0, p2_Ddf2_diff_zaehler = 0, pkt_p_Ddf2_diff_zaehler = 0, p_Ddf2_diff, p2_Ddf2_diff, pkt_p_Ddf2_diff,
		p_DV_diff_zaehler = 0, p2_DV_diff_zaehler = 0,  pkt_p_DV_diff_zaehler = 0,  p_DV_diff,  p2_DV_diff, pkt_p_DV_diff,
		
		pkt_p2_MIN_diff_zaehler = 0, pkt_p2_MIN_diff, 
		pkt_p2_MAX_diff_zaehler = 0, pkt_p2_MAX_diff, 
		pkt_p2_RNG_diff_zaehler = 0, pkt_p2_RNG_diff, 
		pkt_p2_SUM_diff_zaehler = 0, pkt_p2_SUM_diff, 
		
		pkt_p2_MD_diff_zaehler = 0,pkt_p2_MD_diff, 
		pkt_p2_AM_diff_zaehler = 0,pkt_p2_AM_diff, 
		pkt_p2_GM_diff_zaehler = 0,pkt_p2_GM_diff, 
		pkt_p2_HM_diff_zaehler = 0,pkt_p2_HM_diff, 
		
		pkt_p2_V_diff_zaehler = 0,pkt_p2_V_diff, 
		pkt_p2_Vdf1_diff_zaehler = 0,pkt_p2_Vdf1_diff, 
		pkt_p2_SVdf1_diff_zaehler = 0,pkt_p2_SVdf1_diff, 

		pkt_p2_S_diff_zaehler = 0,pkt_p2_S_diff,
		pkt_p2_Sdf1_diff_zaehler = 0, pkt_p2_Sdf1_diff,
		pkt_p2_Sdf2_diff_zaehler = 0, pkt_p2_Sdf2_diff, 
		pkt_p2_D_diff_zaehler = 0,  pkt_p2_D_diff, 
		pkt_p2_Ddf1_diff_zaehler = 0, pkt_p2_Ddf1_diff, 
		pkt_p2_Ddf2_diff_zaehler = 0, pkt_p2_Ddf2_diff, 
		pkt_p2_DV_diff_zaehler = 0,  pkt_p2_DV_diff; 

		

int zeichen_bis_punkt, iLauf, jLauf, kLauf, pLauf, dLauf, nA, kA; //----------> laufindices i, j, p, datenmatrix A parameter n, k
int hub=0, logcode, mP, artP;
int INI[_ini_][2];//----------------------------------------------------------> voreinstellunges matrix INI(n ini,2) übernimmt die elemente auss Ip_ZPG.ini

char dbuffer [9],tbuffer [9], datenprm_dateiname[50], null[50]; //----------------------> buffer fuer time und date


main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv     
{   
	//------------------------------------------------------------------------> datenströme
	FILE  *inStream, *inprmStream, *protStream, *Sn2Stream, *pvStream, *batStream, *iniStream; 	
	
	if (argc == 1 ) //--------------------------------------------------------> Ip_ZGP profil
	{
   		 _profil();
	}
	if (argc != 8 ) //--------------------------------------------------------> fehlerroutine argumente
	{
   		printf("\n[ ] Argumente.0\n"); _profil();
	}

	//------------------------------------------------------------------------> vorenstellungen aus Ip_ZPG.ini einlesen
	
	if (fopen ("Ip_ZPG.ini", "r") == NULL) //--------------------------> fehlt Ip_ZPG.ini wird eine Vorlage erstellt.
		{
			iniStream = fopen ("Ip_ZPG.ini", "w");
		
			fprintf( iniStream,"0\t\t1_n\n"); 
			fprintf( iniStream,"0\t\t2_min\n"); 
			fprintf( iniStream,"0\t\t3_max\n"); 
			fprintf( iniStream,"0\t\t4_rng\n"); 
			fprintf( iniStream,"0\t\t5_sum\n"); 
			fprintf( iniStream,"0\t\t6_md\n"); 
			fprintf( iniStream,"0\t\t7_am\n"); 
			fprintf( iniStream,"0\t\t8_gm\n"); 
			fprintf( iniStream,"0\t\t9_hm\n"); 
			fprintf( iniStream,"0\t\t10_v\n"); 
			fprintf( iniStream,"0\t\t11_vdf1\n"); 
			fprintf( iniStream,"0\t\t12_svdf1\n");
			fprintf( iniStream,"0\t\t13_s\n");
			fprintf( iniStream,"0\t\t14_sdf1\n");
			fprintf( iniStream,"0\t\t15_sdf2\n");
			fprintf( iniStream,"0\t\t16_d\n");
			fprintf( iniStream,"0\t\t17_ddf1\n");
			fprintf( iniStream,"0\t\t18_ddf2\n");
			fprintf( iniStream,"0\t\t19_dvaro\n");
			fprintf( iniStream,"0\t\t20_ENDE\n");
				
			fclose( iniStream );
			system("Ip_ZPG.ini");
		}

	iniStream = fopen( "Ip_ZPG.ini", "r" );
	for (iLauf = 1; iLauf <= _ini_; iLauf++)
	{
		INI[iLauf][1] = iLauf; fscanf(iniStream,"%i%s\n", &INI[iLauf][2], null ); 

		//printf("%i\n", INI[iLauf][2]);
	}
	fclose( iniStream  );


	if (fopen (argv[1], "r") == NULL  ) //------------------------------------> fehlerroutine inputdatei
	{
   		inStream = fopen( argv[1], "w" );
		
		fprintf( inStream,"0\t0\n"); 
		fprintf( inStream,"0\t0\n"); 
		fprintf( inStream,"0\t0\n"); 
		fprintf( inStream,"0\t0\n"); 
		fprintf( inStream,"0\t0\n"); 
		
		fclose( inStream  );
		system (argv[1]);
		printf("%s\n", argv[1] );
		
	}
	artP=atoi(argv[5]);//-----------------------------------------------------> art der permutationsmethode spiegelung  zur ausgabe in logdatei
	mP=atoi(argv[6]);//-------------------------------------------------------> anzahl der Permutationen spiegelung  zur ausgabe in logdatei
	if(atoi(argv[7]) != 0)	{ seed=atoi(argv[7]); Oseed=seed; }//-------------> anfangswert für zufallszahlenfunktion = argument7
	if(atoi(argv[7]) == 0)	{ seed=(time(0)-1234567890); Oseed=seed;}//-------> anfangswert über zeitfunktion

	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.

	if ( fopen (datenprm_dateiname, "r") == NULL  ) //------------------------> fehlerroutine datenparameterdatei 
	{
   		inprmStream = fopen( datenprm_dateiname, "w" );

		fprintf( inprmStream,"5\n"); 
		fprintf( inprmStream,"2\n");

		fclose( inprmStream  );
		system (datenprm_dateiname);
		printf("%s\n", datenprm_dateiname);
	}
         
	//-------------------------------------------------------------------------| einlesen der datenmatrix parameter n, k 
	inprmStream = fopen( datenprm_dateiname, "r" );
	fscanf(inprmStream,"%i\n%i", &nA, &kA ); nA_ = nA;
	fclose( inprmStream  );	

	if ( atoi(argv[2]) >> kA || atoi(argv[3]) >> kA ) //----------------------> fehlerroutine jv > kX 
	{
   			printf("\n[2][3] k < Index j\n");_profil();
	}
	
	if ( atoi(argv[2]) == 0 || atoi(argv[3]) == 0 ) //------------------------> fehlerroutine v1, v2 keine zahl
	{
   			printf("\n[2][3] Index j Format.0\n");_profil();
	}
	
	//-------------------------------------------------------------------------|
	//-------------------------------------------------------------------------| Prozedur 

    //-------------------------------------------------------------------------| einlesen von aij nach Sn2
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)

	for (iLauf = 0; iLauf < nA; iLauf++)
	{
		for (jLauf = 1; jLauf <= kA; jLauf++)
		{	
			fscanf(inStream,"%f", &s_ij );

			if (jLauf == atoi(argv[2]))	{ S_n2[iLauf+1][1] = s_ij;	}
			
			if (jLauf == atoi(argv[3]))	{ S_n2[iLauf+1][2] = s_ij;	
			
				if (s_ij < 1 || s_ij > 2)//-----------------------------------> aij nach si2 kompatibilitätsprüfung
				{										
				printf("\n[1] si2 Wertebereich.0\n"); _profil();
				}
			}
		}
	}

	fclose( inStream  );
	
	_kopfzeile();
	
	//-------------------------------------------------------------------------| ausgeben der Planmatrix Sn2 
	Sn2Stream = fopen( "Ip_ZGP_Sn2.asc", "w" );
	
	for (iLauf = 0; iLauf < nA; iLauf++)
	{	
		fprintf(Sn2Stream,"%.3f\t", S_n2[iLauf+1][1] );
		fprintf(Sn2Stream,"%.0f\t", S_n2[iLauf+1][2] );
		fprintf(Sn2Stream,"\n");
	}

	fclose( Sn2Stream  );

	//-------------------------------------------------------------------------| Kennwertberechnung
	
	theta_zg_uv(S_n2,nA_);//---------------------------------------------------> Kennwertfunktion für 2 unverbundene Gruppen

	//-------------------------------------------------------------------------> thetaspiegelung

	                                       __MIN_=MIN_; __MIN_1=MIN_1; __MIN_2=MIN_2; __MIN_diff=MIN_diff;       
			     		                   __MAX_=MAX_; __MAX_1=MAX_1; __MAX_2=MAX_2; __MAX_diff=MAX_diff;
	__RNG_=RNG_; __RNG_1=RNG_1; __RNG_2=RNG_2; __RNG_diff=RNG_diff; __SUM_=SUM_; __SUM_1=SUM_1; __SUM_2=SUM_2;__SUM_diff=SUM_diff;  
	      
	__MD_=MD_;__MD_1=MD_1;__MD_2=MD_2;__MD_diff=MD_diff;__AM_=AM_;__AM_1=AM_1;__AM_2=AM_2;__AM_diff=AM_diff;
	__GM_=GM_;__GM_1=GM_1;__GM_2=GM_2;__GM_diff=GM_diff;__HM_=HM_;__HM_1=HM_1;__HM_2=HM_2;__HM_diff=HM_diff; 
		 
	__V_=V_;__V_1=V_1;__V_2=V_2;__V_diff=V_diff;__V_df1=V_df1;__V_1df1=V_1df1;__V_2df1=V_2df1;__V_df1_diff=V_df1_diff; 
							__S_V_df1=S_V_df1; __S_1V_1df1=S_1V_1df1; __S_2V_2df1=S_2V_2df1; __S_V_df1_diff=S_V_df1_diff; 
	__S_=S_;__S_1=S_1;__S_2=S_2;__S_diff=S_diff;__S_df1=S_df1;__S_1df1=S_1df1;__S_2df1=S_2df1;__S_df1_diff=S_df1_diff;
									           __S_df2=S_df2; __S_1df2=S_1df2;__S_2df2=S_2df2;__S_df2_diff=S_df2_diff;
	__D_=D_;__D_1=D_1;__D_2=D_2;__D_diff=D_diff;__D_df1=D_df1;__D_1df1=D_1df1;__D_2df1=D_2df1;__D_df1_diff=D_df1_diff;
									           __D_df2=D_df2; __D_1df2=D_1df2;__D_2df2=D_2df2;__D_df2_diff=D_df2_diff;         
	__DV_=DV_;__DV_1=DV_1;__DV_2=DV_2;__DV_diff=DV_diff;
	

	//-------------------------------------------------------------------------|
	//-------------------------------------------------------------------------| Permutation Pr
	pvStream = fopen( "Ip_ZGP_PmVtlng.asc", "w" );

	for (dLauf = 1; dLauf <= 2; dLauf++)//------------------------------------> berechnungs phasen
	{
		//---------------------------------------------------------------------| erzeugung des zufallsvektors n
		qzufall(seed);//------------------------------------------------------> Zufallszahlenfunktion
		
		if(dLauf == 2)// zweiter permutationsdurchgang
		{
			printf("           \r");//----------------------------------------> bildschirmausgabe clear
			//----------------------------------------------------------------> AM theta_theta 
			q_MIN_diff = q_MIN_diff  / atoi(argv[6]);
			q_MAX_diff = q_MAX_diff  / atoi(argv[6]);
			q_RNG_diff = q_RNG_diff  / atoi(argv[6]);
			q_SUM_diff = q_SUM_diff  / atoi(argv[6]);
	 		 q_MD_diff =  q_MD_diff  / atoi(argv[6]);
			 q_AM_diff =  q_AM_diff  / atoi(argv[6]);
			 q_GM_diff =  q_GM_diff  / atoi(argv[6]);
			 q_HM_diff =  q_HM_diff  / atoi(argv[6]);
			  q_V_diff =   q_V_diff  / atoi(argv[6]);
			
			   q_V_df1_diff  =   q_V_df1_diff   / atoi(argv[6]);
			 q_S_V_df1_diff  = q_S_V_df1_diff   / atoi(argv[6]);
			       q_S_diff  =       q_S_diff   / atoi(argv[6]);
			   q_S_df1_diff  =   q_S_df1_diff   / atoi(argv[6]);
			   q_S_df2_diff  =   q_S_df2_diff   / atoi(argv[6]);
			       q_D_diff  =       q_D_diff   / atoi(argv[6]);
			   q_D_df1_diff  =   q_D_df1_diff   / atoi(argv[6]);
			   q_D_df2_diff  =   q_D_df2_diff   / atoi(argv[6]);
			      q_DV_diff  =      q_DV_diff   / atoi(argv[6]);

				  q_MIN_ = q_MIN_  / atoi(argv[6]);
			q_MAX_ = q_MAX_  / atoi(argv[6]);
			q_RNG_ = q_RNG_  / atoi(argv[6]);
			q_SUM_ = q_SUM_  / atoi(argv[6]);
	 		 q_MD_ =  q_MD_  / atoi(argv[6]);
			 q_AM_ =  q_AM_  / atoi(argv[6]);
			 q_GM_ =  q_GM_  / atoi(argv[6]);
			 q_HM_ =  q_HM_  / atoi(argv[6]);
			  q_V_ =   q_V_  / atoi(argv[6]);
			
			   q_V_df1  =   q_V_df1   / atoi(argv[6]);
			 q_S_V_df1  = q_S_V_df1   / atoi(argv[6]);
			       q_S_  =       q_S_   / atoi(argv[6]);
			   q_S_df1  =   q_S_df1   / atoi(argv[6]);
			   q_S_df2  =   q_S_df2   / atoi(argv[6]);
			       q_D_  =       q_D_   / atoi(argv[6]);
			   q_D_df1  =   q_D_df1   / atoi(argv[6]);
			   q_D_df2  =   q_D_df2   / atoi(argv[6]);
			      q_DV_  =      q_DV_   / atoi(argv[6]);

				   q_MIN_1= q_MIN_1 / atoi(argv[6]);
			q_MAX_1= q_MAX_1 / atoi(argv[6]);
			q_RNG_1= q_RNG_1 / atoi(argv[6]);
			q_SUM_1= q_SUM_1 / atoi(argv[6]);
	 		 q_MD_1=  q_MD_1 / atoi(argv[6]);
			 q_AM_1=  q_AM_1 / atoi(argv[6]);
			 q_GM_1=  q_GM_1 / atoi(argv[6]);
			 q_HM_1=  q_HM_1 / atoi(argv[6]);
			  q_V_1=   q_V_1 / atoi(argv[6]);
			
			   q_V_1df1  =   q_V_1df1   / atoi(argv[6]);
			 q_S_1V_1df1  = q_S_1V_1df1   / atoi(argv[6]);
			       q_S_1 =       q_S_1  / atoi(argv[6]);
			   q_S_1df1  =   q_S_1df1   / atoi(argv[6]);
			   q_S_1df2  =   q_S_1df2   / atoi(argv[6]);
			       q_D_1 =       q_D_1  / atoi(argv[6]);
			   q_D_1df1  =   q_D_1df1   / atoi(argv[6]);
			   q_D_1df2  =   q_D_1df2   / atoi(argv[6]);
			      q_DV_1 =      q_DV_1  / atoi(argv[6]);

				   q_MIN_2= q_MIN_2 / atoi(argv[6]);
			q_MAX_2= q_MAX_2 / atoi(argv[6]);
			q_RNG_2= q_RNG_2 / atoi(argv[6]);
			q_SUM_2= q_SUM_2 / atoi(argv[6]);
	 		 q_MD_2=  q_MD_2 / atoi(argv[6]);
			 q_AM_2=  q_AM_2 / atoi(argv[6]);
			 q_GM_2=  q_GM_2 / atoi(argv[6]);
			 q_HM_2=  q_HM_2 / atoi(argv[6]);
			  q_V_2=   q_V_2 / atoi(argv[6]);
			
			   q_V_2df1  =   q_V_2df1   / atoi(argv[6]);
			 q_S_2V_2df1  = q_S_2V_2df1   / atoi(argv[6]);
			       q_S_2 =       q_S_2  / atoi(argv[6]);
			   q_S_2df1  =   q_S_2df1   / atoi(argv[6]);
			   q_S_2df2  =   q_S_2df2   / atoi(argv[6]);
			       q_D_2 =       q_D_2  / atoi(argv[6]);
			   q_D_2df1  =   q_D_2df1   / atoi(argv[6]);
			   q_D_2df2  =   q_D_2df2   / atoi(argv[6]);
			      q_DV_2 =      q_DV_2  / atoi(argv[6]);
		}
	
	for (pLauf = 1; pLauf <= atoi(argv[6]); pLauf++)
	{
		printf("%i %i\r", dLauf, pLauf);//------------------------------------> bildschirmausgabe durchgang, permutationsnummer iP
	
		for (iLauf = 1; iLauf <= nA; iLauf++)
		{
			n_n[iLauf]=quasizufallszahl;//------------------------------------> zuordnung der zufallszahl zu zufallsvektorzelle ni
		
			qzufall(quasizufallszahl*10);//-----------------------------------> resultierende Zufallszahl * 10 (nachkommaverschiebung)
		}
		//seed = quasizufallszahl*10;
	
		/*//tmp
		for (iLauf = 1; iLauf <= nA*2; iLauf++)
		{
		printf("n%i=%.5f\n", iLauf, n_n[iLauf]);//bildschirmausgabe Zufallsvektor n
	
		}*/

		//--------------------------------------------------------------------> erster zeilenvektor p durch addition des 1 zeilenvektors s und des 1 elements des zufallsvektors n zu P
		P_2n2[1][1] = S_n2[1][1];
		P_2n2[1][3] = n_n[1];
	
		//---------------------------------------------------------------------| sortierung von S nach n zu P
		for (iLauf = 1; iLauf < nA; iLauf++)
		{
			for (jLauf = 0; jLauf < iLauf; jLauf++)
			{
				if (n_n[iLauf+1] < P_2n2[iLauf-jLauf][3])
				{
					P_2n2[(iLauf-jLauf)+1][1] = P_2n2[iLauf-jLauf][1];
					P_2n2[(iLauf-jLauf)+1][3] = P_2n2[iLauf-jLauf][3];

					hub++;
				}	
			}
	
			P_2n2[(iLauf+1)-hub][1] = S_n2[iLauf+1][1];
			P_2n2[(iLauf+1)-hub][3] = n_n[iLauf+1];

			hub=0;
		}
		
		//---------------------------------------------------------------------| übertragung von P zu PS
		for (iLauf = 1; iLauf <= nA; iLauf++)
		{	
			 PS_n2[iLauf][1] = P_2n2[iLauf][1] ;
			 PS_n2[iLauf][2] = S_n2[iLauf][2] ;
		}

		//tmp 
		/*for (iLauf = 0; iLauf < nA; iLauf++)
		{
			printf("p%i1=%.2f p%i2=%.2f p%i3=%.3f\n", iLauf+1, P_2n2[iLauf+1][1], iLauf+1, P_2n2[iLauf+1][2], iLauf+1, P_2n2[iLauf+1][3]);//bildschirmausgabe permutationsmatrix P
	
		}
		printf("\n");//leerzeile
		for (iLauf = 0; iLauf < nA; iLauf++)
		{
			printf("p%i1=%.2f p%i2=%.2f\n", iLauf+1, PS_n2[iLauf+1][1], iLauf+1, PS_n2[iLauf+1][2]);//bildschirmausgabe permutationsmatrix P
	
		}
		printf("\n");//leerzeile*/
		
		//---------------------------------------------------------------------| Kennwertberechnung
		theta_zg_uv(PS_n2,nA_);//---------------------------------------------> Kennwertfunktion für 2 unverbundene Gruppen
		/*//------------------------------------------------------------------> thetaspiegelung
	                                       __MIN_=MIN_; __MIN_1=MIN_1; __MIN_2=MIN_2; __MIN_diff=MIN_diff;       
			     		                   __MAX_=MAX_; __MAX_1=MAX_1; __MAX_2=MAX_2; __MAX_diff=MAX_diff;
		__RNG_=RNG_; __RNG_1=RNG_1; __RNG_2=RNG_2; __RNG_diff=RNG_diff; __SUM_=SUM_; __SUM_1=SUM_1; __SUM_2=SUM_2;__SUM_diff=SUM_diff;  
	      
		__MD_=MD_;__MD_1=MD_1;__MD_2=MD_2;__MD_diff=MD_diff;__AM_=AM_;__AM_1=AM_1;__AM_2=AM_2;__AM_diff=AM_diff;
		__GM_=GM_;__GM_1=GM_1;__GM_2=GM_2;__GM_diff=GM_diff;__HM_=HM_;__HM_1=HM_1;__HM_2=HM_2;__HM_diff=HM_diff; 
		 
		__V_=V_;__V_1=V_1;__V_2=V_2;__V_diff=V_diff;__V_df1=V_df1;__V_1df1=V_1df1;__V_2df1=V_2df1;__V_df1_diff=V_df1_diff; 
							__S_V_df1=S_V_df1; __S_1V_1df1=S_1V_1df1; __S_2V_2df1=S_2V_2df1; __S_V_df1_diff=S_V_df1_diff; 
		__S_=S_;__S_1=S_1;__S_2=S_2;__S_diff=S_diff;__S_df1=S_df1;__S_1df1=S_1df1;__S_2df1=S_2df1;__S_df1_diff=S_df1_diff;
									           __S_df2=S_df2; __S_1df2=S_1df2;__S_2df2=S_2df2;__S_df2_diff=S_df2_diff;
		__D_=D_;__D_1=D_1;__D_2=D_2;__D_diff=D_diff;__D_df1=D_df1;__D_1df1=D_1df1;__D_2df1=D_2df1;__D_df1_diff=D_df1_diff;
									           __D_df2=D_df2; __D_1df2=D_1df2;__D_2df2=D_2df2;__D_df2_diff=D_df2_diff;         
		__DV_=DV_;__DV_1=DV_1;__DV_2=DV_2;__DV_diff=DV_diff;

	*/
	if(dLauf == 1)// erster permutationsdurchgang
	{	
		if(pLauf <= _nP_)//---------------------------------------------------> dateiausgabe der ersten _nP_=5000 thetawerte der permutationsverteilung 
		{
			fprintf(pvStream,
				
			"%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%5.3f\n", 
			
			  MIN_diff,   MAX_diff, RNG_diff,   SUM_diff,	   MD_diff, AM_diff,     
			   GM_diff,    HM_diff,   V_diff, V_df1_diff, S_V_df1_diff,  S_diff, 
			S_df1_diff, S_df2_diff,   D_diff, D_df1_diff,   D_df2_diff, DV_diff);
		}

		//SUM theta_theta berechnung
		
		q_MIN_diff += MIN_diff; q_MAX_diff += MAX_diff; q_RNG_diff += RNG_diff; q_SUM_diff += SUM_diff; 
		q_MD_diff  +=  MD_diff;  q_AM_diff +=  AM_diff;  q_GM_diff +=  GM_diff;  q_HM_diff +=  HM_diff;
		
		q_V_diff  +=  V_diff;  q_V_df1_diff +=  V_df1_diff;  q_S_V_df1_diff +=  S_V_df1_diff;  
		q_S_diff +=  S_diff; q_S_df1_diff +=  S_df1_diff;  q_S_df2_diff +=  S_df2_diff;
		q_D_diff +=  D_diff; q_D_df1_diff +=  D_df1_diff;  q_D_df2_diff +=  D_df2_diff;
		q_DV_diff +=  DV_diff;
			
		q_MIN_ += MIN_; q_MAX_ += MAX_; q_RNG_ += RNG_; q_SUM_ += SUM_; 
		q_MD_  +=  MD_;  q_AM_ +=  AM_;  q_GM_ +=  GM_;  q_HM_ +=  HM_;
		
		q_V_  +=  V_;  q_V_df1 +=  V_df1;  q_S_V_df1 +=  S_V_df1;  
		q_S_ +=  S_; q_S_df1 +=  S_df1;  q_S_df2 +=  S_df2;
		q_D_ +=  D_; q_D_df1 +=  D_df1;  q_D_df2 +=  D_df2;
		q_DV_ +=  DV_;

		q_MIN_1 += MIN_1; q_MAX_1 += MAX_1; q_RNG_1 += RNG_1; q_SUM_1 += SUM_1; 
		q_MD_1 +=  MD_1;  q_AM_1 +=  AM_1;  q_GM_1 +=  GM_1;  q_HM_1 +=  HM_1;
		
		q_V_1 +=  V_1;  q_V_1df1 +=  V_1df1;  q_S_1V_1df1 +=  S_1V_1df1;  
		q_S_1 +=  S_1; q_S_1df1 +=  S_1df1;  q_S_1df2 +=  S_1df2;
		q_D_1 +=  D_1; q_D_1df1 +=  D_1df1;  q_D_1df2 +=  D_1df2;
		q_DV_1 +=  DV_1;

		q_MIN_2 += MIN_2; q_MAX_2 += MAX_2; q_RNG_2 += RNG_2; q_SUM_2 += SUM_2; 
		q_MD_2 +=  MD_2;  q_AM_2 +=  AM_2;  q_GM_2 +=  GM_2;  q_HM_2 +=  HM_2;
		
		q_V_2 +=  V_2;  q_V_2df1 +=  V_2df1;  q_S_2V_2df1 +=  S_2V_2df1;  
		q_S_2 +=  S_2; q_S_2df1 +=  S_2df1;  q_S_2df2 +=  S_2df2;
		q_D_2 +=  D_2; q_D_2df1 +=  D_2df1;  q_D_2df2 +=  D_2df2;
		q_DV_2 +=  DV_2;
	
		
	}

	if(dLauf == 2)// zweiter permutationsdurchgang
	{
		//--------------------------------------------------------------------> ereigniszähler THETA >= theta
		if ( MIN_diff >= __MIN_diff )	      { p_MIN_diff_zaehler += 1;}
		if ( MAX_diff >= __MAX_diff )         { p_MAX_diff_zaehler += 1;}
		if ( RNG_diff >= __RNG_diff )         { p_RNG_diff_zaehler += 1;}
		if ( SUM_diff >= __SUM_diff )         { p_SUM_diff_zaehler += 1;}
		if ( MD_diff >= __MD_diff )	          { p_MD_diff_zaehler += 1;}
		if ( AM_diff >= __AM_diff )           { p_AM_diff_zaehler += 1;}
		if ( GM_diff >= __GM_diff )           { p_GM_diff_zaehler += 1;}
		if ( HM_diff >= __HM_diff )           { p_HM_diff_zaehler += 1;}
		if ( V_diff >= __V_diff )		      { p_V_diff_zaehler += 1;}
		if ( V_df1_diff >= __V_df1_diff )	  { p_Vdf1_diff_zaehler += 1;}
		if ( S_V_df1_diff >= __S_V_df1_diff ) { p_SVdf1_diff_zaehler += 1;}
		if ( S_diff >= __S_diff )		      { p_S_diff_zaehler += 1;}
		if ( S_df1_diff >= __S_df1_diff )     { p_Sdf1_diff_zaehler += 1;}
		if ( S_df2_diff >= __S_df2_diff )     { p_Sdf2_diff_zaehler += 1;}
		if ( D_diff >= __D_diff )             { p_D_diff_zaehler += 1;}
		if ( D_df1_diff >= __D_df1_diff )     { p_Ddf1_diff_zaehler += 1;}
		if ( D_df2_diff >= __D_df2_diff )     { p_Ddf2_diff_zaehler += 1;}
		if ( DV_diff >= __DV_diff )           { p_DV_diff_zaehler += 1;}

		//--------------------------------------------------------------------> ereigniszähler |THETA-AMtheta| >= |theta-AMtheta|

		if ( sqrt(pow(MIN_diff-q_MIN_diff,2)) >= sqrt(pow(__MIN_diff-q_MIN_diff,2)) )	    { p2_MIN_diff_zaehler += 1;}
		if ( sqrt(pow(MAX_diff-q_MAX_diff,2)) >= sqrt(pow(__MAX_diff-q_MAX_diff,2)) )       { p2_MAX_diff_zaehler += 1;}
		if ( sqrt(pow(RNG_diff-q_RNG_diff,2)) >= sqrt(pow(__RNG_diff-q_RNG_diff,2)) )       { p2_RNG_diff_zaehler += 1;}
		if ( sqrt(pow(SUM_diff-q_SUM_diff,2)) >= sqrt(pow(__SUM_diff-q_SUM_diff,2)) )       { p2_SUM_diff_zaehler += 1;}
		if ( sqrt(pow(MD_diff-q_MD_diff,2)) >= sqrt(pow(__MD_diff-q_MD_diff,2)) )	        { p2_MD_diff_zaehler += 1;}
		if ( sqrt(pow(AM_diff-q_AM_diff,2)) >= sqrt(pow(__AM_diff-q_AM_diff,2)))            { p2_AM_diff_zaehler += 1;}
		if ( sqrt(pow(GM_diff-q_GM_diff,2)) >= sqrt(pow(__GM_diff-q_GM_diff,2)) )           { p2_GM_diff_zaehler += 1;}
		if ( sqrt(pow(HM_diff-q_HM_diff,2)) >= sqrt(pow(__HM_diff-q_HM_diff,2)) )           { p2_HM_diff_zaehler += 1;}
		if ( sqrt(pow(V_diff-q_V_diff,2)) >= sqrt(pow(__V_diff-q_V_diff,2)) )		        { p2_V_diff_zaehler += 1;}
		if ( sqrt(pow(V_df1_diff-q_V_df1_diff,2)) >= sqrt(pow(__V_df1_diff-q_V_df1_diff,2)) )	      { p2_Vdf1_diff_zaehler += 1;}
		if ( sqrt(pow(S_V_df1_diff-q_S_V_df1_diff,2)) >= sqrt(pow(__S_V_df1_diff-q_S_V_df1_diff,2)) ) { p2_SVdf1_diff_zaehler += 1;}
		if ( sqrt(pow(S_diff-q_S_diff,2)) >= sqrt(pow(__S_diff-q_S_diff,2)) )		                  { p2_S_diff_zaehler += 1;}
		if ( sqrt(pow(S_df1_diff-q_S_df1_diff,2)) >= sqrt(pow(__S_df1_diff-q_S_df1_diff,2)) )         { p2_Sdf1_diff_zaehler += 1;}
		if ( sqrt(pow(S_df2_diff-q_S_df2_diff,2)) >= sqrt(pow(__S_df2_diff-q_S_df2_diff,2)) )         { p2_Sdf2_diff_zaehler += 1;}
		if ( sqrt(pow(D_diff-q_D_diff,2)) >= sqrt(pow(__D_diff-q_D_diff,2)) )                         { p2_D_diff_zaehler += 1;}
		if ( sqrt(pow(D_df1_diff-q_D_df1_diff,2)) >= sqrt(pow(__D_df1_diff-q_D_df1_diff,2)) )         { p2_Ddf1_diff_zaehler += 1;}
		if ( sqrt(pow(D_df2_diff-q_D_df2_diff,2)) >= sqrt(pow(__D_df2_diff-q_D_df2_diff,2)) )         { p2_Ddf2_diff_zaehler += 1;}
		if ( sqrt(pow(DV_diff-q_DV_diff,2)) >= sqrt(pow(__DV_diff-q_DV_diff,2)) )                     { p2_DV_diff_zaehler += 1;}

		//--------------------------------------------------------------------> ereigniszähler THETA = theta, |THETA-AMtheta| = |theta-AMtheta| 
		if ( artP == 3 || artP == 4)
		{
			if ( MIN_diff == __MIN_diff )	      { pkt_p_MIN_diff_zaehler += 1;}
			if ( MAX_diff == __MAX_diff )         { pkt_p_MAX_diff_zaehler += 1;}
			if ( RNG_diff == __RNG_diff )         { pkt_p_RNG_diff_zaehler += 1;}
			if ( SUM_diff == __SUM_diff )         { pkt_p_SUM_diff_zaehler += 1;}
			if ( MD_diff == __MD_diff )	          { pkt_p_MD_diff_zaehler += 1;}
			if ( AM_diff == __AM_diff )           { pkt_p_AM_diff_zaehler += 1;}
			if ( GM_diff == __GM_diff )           { pkt_p_GM_diff_zaehler += 1;}
			if ( HM_diff == __HM_diff )           { pkt_p_HM_diff_zaehler += 1;}
			if ( V_diff == __V_diff )		      { pkt_p_V_diff_zaehler += 1;}
			if ( V_df1_diff == __V_df1_diff )	  { pkt_p_Vdf1_diff_zaehler += 1;}
			if ( S_V_df1_diff == __S_V_df1_diff ) { pkt_p_SVdf1_diff_zaehler += 1;}
			if ( S_diff == __S_diff )		      { pkt_p_S_diff_zaehler += 1;}
			if ( S_df1_diff == __S_df1_diff )     { pkt_p_Sdf1_diff_zaehler += 1;}
			if ( S_df2_diff == __S_df2_diff )     { pkt_p_Sdf2_diff_zaehler += 1;}
			if ( D_diff == __D_diff )             { pkt_p_D_diff_zaehler += 1;}
			if ( D_df1_diff == __D_df1_diff )     { pkt_p_Ddf1_diff_zaehler += 1;}
			if ( D_df2_diff == __D_df2_diff )     { pkt_p_Ddf2_diff_zaehler += 1;}
			if ( DV_diff == __DV_diff )           { pkt_p_DV_diff_zaehler += 1;}

			if ( sqrt(pow(MIN_diff-q_MIN_diff,2)) == sqrt(pow(__MIN_diff-q_MIN_diff,2)) )	        { pkt_p2_MIN_diff_zaehler += 1;}
			if ( sqrt(pow(MAX_diff-q_MAX_diff,2)) == sqrt(pow(__MAX_diff-q_MAX_diff,2)) )         { pkt_p2_MAX_diff_zaehler += 1;}
			if ( sqrt(pow(RNG_diff-q_RNG_diff,2)) == sqrt(pow(__RNG_diff-q_RNG_diff,2)) )         { pkt_p2_RNG_diff_zaehler += 1;}
			if ( sqrt(pow(SUM_diff-q_SUM_diff,2)) == sqrt(pow(__SUM_diff-q_SUM_diff,2)) )         { pkt_p2_SUM_diff_zaehler += 1;}
			if ( sqrt(pow(MD_diff-q_MD_diff,2)) == sqrt(pow(__MD_diff-q_MD_diff,2)) )	        { pkt_p2_MD_diff_zaehler += 1;}
			if ( sqrt(pow(AM_diff-q_AM_diff,2)) == sqrt(pow(__AM_diff-q_AM_diff,2)))            { pkt_p2_AM_diff_zaehler += 1;}
			if ( sqrt(pow(GM_diff-q_GM_diff,2)) == sqrt(pow(__GM_diff-q_GM_diff,2)) )           { pkt_p2_GM_diff_zaehler += 1;}
			if ( sqrt(pow(HM_diff-q_HM_diff,2)) == sqrt(pow(__HM_diff-q_HM_diff,2)) )           { pkt_p2_HM_diff_zaehler += 1;}
			if ( sqrt(pow(V_diff-q_V_diff,2)) == sqrt(pow(__V_diff-q_V_diff,2)) )		        { pkt_p2_V_diff_zaehler += 1;}
			if ( sqrt(pow(V_df1_diff-q_V_df1_diff,2)) == sqrt(pow(__V_df1_diff-q_V_df1_diff,2)) )	    { pkt_p2_Vdf1_diff_zaehler += 1;}
			if ( sqrt(pow(S_V_df1_diff-q_S_V_df1_diff,2)) == sqrt(pow(__S_V_df1_diff-q_S_V_df1_diff,2)) ) { pkt_p2_SVdf1_diff_zaehler += 1;}
			if ( sqrt(pow(S_diff-q_S_diff,2)) == sqrt(pow(__S_diff-q_S_diff,2)) )		        { pkt_p2_S_diff_zaehler += 1;}
			if ( sqrt(pow(S_df1_diff-q_S_df1_diff,2)) == sqrt(pow(__S_df1_diff-q_S_df1_diff,2)) )     { pkt_p2_Sdf1_diff_zaehler += 1;}
			if ( sqrt(pow(S_df2_diff-q_S_df2_diff,2)) == sqrt(pow(__S_df2_diff-q_S_df2_diff,2)) )     { pkt_p2_Sdf2_diff_zaehler += 1;}
			if ( sqrt(pow(D_diff-q_D_diff,2)) == sqrt(pow(__D_diff-q_D_diff,2)) )             { pkt_p2_D_diff_zaehler += 1;}
			if ( sqrt(pow(D_df1_diff-q_D_df1_diff,2)) == sqrt(pow(__D_df1_diff-q_D_df1_diff,2)) )     { pkt_p2_Ddf1_diff_zaehler += 1;}
			if ( sqrt(pow(D_df2_diff-q_D_df2_diff,2)) == sqrt(pow(__D_df2_diff-q_D_df2_diff,2)) )     { pkt_p2_Ddf2_diff_zaehler += 1;}
			if ( sqrt(pow(DV_diff-q_DV_diff,2)) == sqrt(pow(__DV_diff-q_DV_diff,2)) )           { pkt_p2_DV_diff_zaehler += 1;}
		}
	}
	
	}//end pLauf
	}//end dlauf
	
	
	
	//------------------------------------------------------------------------> punktwahrscheinlichkeitsberechnung p= und p>= korrektur als midp = p>= - (p= /2)
	if ( artP == 3 || artP == 4)	
	{
		pkt_p_MIN_diff = pkt_p_MIN_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_MAX_diff = pkt_p_MAX_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_RNG_diff = pkt_p_RNG_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_SUM_diff = pkt_p_SUM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_MD_diff = pkt_p_MD_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_AM_diff = pkt_p_AM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_GM_diff = pkt_p_GM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_HM_diff = pkt_p_HM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_V_diff = pkt_p_V_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_Vdf1_diff = pkt_p_Vdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_SVdf1_diff = pkt_p_SVdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_S_diff = pkt_p_S_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_Sdf1_diff = pkt_p_Sdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_Sdf2_diff = pkt_p_Sdf2_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_D_diff = pkt_p_D_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_Ddf1_diff = pkt_p_Ddf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_Ddf2_diff = pkt_p_Ddf2_diff_zaehler/(atoi(argv[6])+1);
		pkt_p_DV_diff = pkt_p_DV_diff_zaehler/(atoi(argv[6])+1);

		pkt_p2_MIN_diff = pkt_p2_MIN_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_MAX_diff = pkt_p2_MAX_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_RNG_diff = pkt_p2_RNG_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_SUM_diff = pkt_p2_SUM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_MD_diff = pkt_p2_MD_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_AM_diff = pkt_p2_AM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_GM_diff = pkt_p2_GM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_HM_diff = pkt_p2_HM_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_V_diff = pkt_p2_V_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_Vdf1_diff = pkt_p2_Vdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_SVdf1_diff = pkt_p2_SVdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_S_diff = pkt_p2_S_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_Sdf1_diff = pkt_p2_Sdf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_Sdf2_diff = pkt_p2_Sdf2_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_D_diff = pkt_p2_D_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_Ddf1_diff = pkt_p2_Ddf1_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_Ddf2_diff = pkt_p2_Ddf2_diff_zaehler/(atoi(argv[6])+1);
		pkt_p2_DV_diff = pkt_p2_DV_diff_zaehler/(atoi(argv[6])+1);
		
		p_MIN_diff_zaehler = p_MIN_diff_zaehler - (pkt_p_MIN_diff_zaehler /2);
		p_MAX_diff_zaehler = p_MAX_diff_zaehler - (pkt_p_MAX_diff_zaehler /2);
		p_RNG_diff_zaehler = p_RNG_diff_zaehler - (pkt_p_RNG_diff_zaehler /2);
		p_SUM_diff_zaehler = p_SUM_diff_zaehler - (pkt_p_SUM_diff_zaehler /2);
		p_MD_diff_zaehler = p_MD_diff_zaehler - (pkt_p_MD_diff_zaehler /2);
		p_AM_diff_zaehler = p_AM_diff_zaehler - (pkt_p_AM_diff_zaehler /2);
		p_GM_diff_zaehler = p_GM_diff_zaehler - (pkt_p_GM_diff_zaehler /2);
		p_HM_diff_zaehler = p_HM_diff_zaehler - (pkt_p_HM_diff_zaehler /2);
		p_V_diff_zaehler = p_V_diff_zaehler - (pkt_p_V_diff_zaehler /2);
		p_Vdf1_diff_zaehler = p_Vdf1_diff_zaehler - (pkt_p_Vdf1_diff_zaehler /2);
		p_SVdf1_diff_zaehler = p_SVdf1_diff_zaehler - (pkt_p_SVdf1_diff_zaehler /2);
		p_S_diff_zaehler = p_S_diff_zaehler - (pkt_p_S_diff_zaehler /2);
		p_Sdf1_diff_zaehler = p_Sdf1_diff_zaehler - (pkt_p_Sdf1_diff_zaehler /2);
		p_Sdf2_diff_zaehler = p_Sdf2_diff_zaehler - (pkt_p_Sdf2_diff_zaehler /2);
		p_D_diff_zaehler = p_D_diff_zaehler - (pkt_p_D_diff_zaehler /2);
		p_Ddf1_diff_zaehler = p_Ddf1_diff_zaehler - (pkt_p_Ddf1_diff_zaehler /2);
		p_Ddf2_diff_zaehler = p_Ddf2_diff_zaehler - (pkt_p_Ddf2_diff_zaehler /2);
		p_DV_diff_zaehler = p_DV_diff_zaehler - (pkt_p_DV_diff_zaehler /2);

		p2_MIN_diff_zaehler = p2_MIN_diff_zaehler - (pkt_p2_MIN_diff_zaehler /2);
		p2_MAX_diff_zaehler = p2_MAX_diff_zaehler - (pkt_p2_MAX_diff_zaehler /2);
		p2_RNG_diff_zaehler = p2_RNG_diff_zaehler - (pkt_p2_RNG_diff_zaehler /2);
		p2_SUM_diff_zaehler = p2_SUM_diff_zaehler - (pkt_p2_SUM_diff_zaehler /2);
		p2_MD_diff_zaehler = p2_MD_diff_zaehler - (pkt_p2_MD_diff_zaehler /2);
		p2_AM_diff_zaehler = p2_AM_diff_zaehler - (pkt_p2_AM_diff_zaehler /2);
		p2_GM_diff_zaehler = p2_GM_diff_zaehler - (pkt_p2_GM_diff_zaehler /2);
		p2_HM_diff_zaehler = p2_HM_diff_zaehler - (pkt_p2_HM_diff_zaehler /2);
		p2_V_diff_zaehler = p2_V_diff_zaehler - (pkt_p2_V_diff_zaehler /2);
		p2_Vdf1_diff_zaehler = p2_Vdf1_diff_zaehler - (pkt_p2_Vdf1_diff_zaehler /2);
		p2_SVdf1_diff_zaehler = p2_SVdf1_diff_zaehler - (pkt_p2_SVdf1_diff_zaehler /2);
		p2_S_diff_zaehler = p2_S_diff_zaehler - (pkt_p2_S_diff_zaehler /2);
		p2_Sdf1_diff_zaehler = p2_Sdf1_diff_zaehler - (pkt_p2_Sdf1_diff_zaehler /2);
		p2_Sdf2_diff_zaehler = p2_Sdf2_diff_zaehler - (pkt_p2_Sdf2_diff_zaehler /2);
		p2_D_diff_zaehler = p2_D_diff_zaehler - (pkt_p2_D_diff_zaehler /2);
		p2_Ddf1_diff_zaehler = p2_Ddf1_diff_zaehler - (pkt_p2_Ddf1_diff_zaehler /2);
		p2_Ddf2_diff_zaehler = p2_Ddf2_diff_zaehler - (pkt_p2_Ddf2_diff_zaehler /2);
		p2_DV_diff_zaehler = p2_DV_diff_zaehler - (pkt_p2_DV_diff_zaehler /2);
		
	}

	//------------------------------------------------------------------------> wahrscheinlichkeitsberechnung p>= / mp>=
	p_MIN_diff = p_MIN_diff_zaehler/(atoi(argv[6])+1);
	p_MAX_diff = p_MAX_diff_zaehler/(atoi(argv[6])+1);
	p_RNG_diff = p_RNG_diff_zaehler/(atoi(argv[6])+1);
	p_SUM_diff = p_SUM_diff_zaehler/(atoi(argv[6])+1);
	p_MD_diff = p_MD_diff_zaehler/(atoi(argv[6])+1);
	p_AM_diff = p_AM_diff_zaehler/(atoi(argv[6])+1);
	p_GM_diff = p_GM_diff_zaehler/(atoi(argv[6])+1);
	p_HM_diff = p_HM_diff_zaehler/(atoi(argv[6])+1);
	p_V_diff = p_V_diff_zaehler/(atoi(argv[6])+1);
	p_Vdf1_diff = p_Vdf1_diff_zaehler/(atoi(argv[6])+1);
	p_SVdf1_diff = p_SVdf1_diff_zaehler/(atoi(argv[6])+1);
	p_S_diff = p_S_diff_zaehler/(atoi(argv[6])+1);
	p_Sdf1_diff = p_Sdf1_diff_zaehler/(atoi(argv[6])+1);
	p_Sdf2_diff = p_Sdf2_diff_zaehler/(atoi(argv[6])+1);
	p_D_diff = p_D_diff_zaehler/(atoi(argv[6])+1);
	p_Ddf1_diff = p_Ddf1_diff_zaehler/(atoi(argv[6])+1);
	p_Ddf2_diff = p_Ddf2_diff_zaehler/(atoi(argv[6])+1);
	p_DV_diff = p_DV_diff_zaehler/(atoi(argv[6])+1);

	//------------------------------------------------------------------------> wahrscheinlichkeitsberechnung p<> /mp<>
	p2_MIN_diff = p2_MIN_diff_zaehler/(atoi(argv[6])+1);
	p2_MAX_diff = p2_MAX_diff_zaehler/(atoi(argv[6])+1);
	p2_RNG_diff = p2_RNG_diff_zaehler/(atoi(argv[6])+1);
	p2_SUM_diff = p2_SUM_diff_zaehler/(atoi(argv[6])+1);
	p2_MD_diff = p2_MD_diff_zaehler/(atoi(argv[6])+1);
	p2_AM_diff = p2_AM_diff_zaehler/(atoi(argv[6])+1);
	p2_GM_diff = p2_GM_diff_zaehler/(atoi(argv[6])+1);
	p2_HM_diff = p2_HM_diff_zaehler/(atoi(argv[6])+1);
	p2_V_diff = p2_V_diff_zaehler/(atoi(argv[6])+1);
	p2_Vdf1_diff = p2_Vdf1_diff_zaehler/(atoi(argv[6])+1);
	p2_SVdf1_diff = p2_SVdf1_diff_zaehler/(atoi(argv[6])+1);
	p2_S_diff = p2_S_diff_zaehler/(atoi(argv[6])+1);
	p2_Sdf1_diff = p2_Sdf1_diff_zaehler/(atoi(argv[6])+1);
	p2_Sdf2_diff = p2_Sdf2_diff_zaehler/(atoi(argv[6])+1);
	p2_D_diff = p2_D_diff_zaehler/(atoi(argv[6])+1);
	p2_Ddf1_diff = p2_Ddf1_diff_zaehler/(atoi(argv[6])+1);
	p2_Ddf2_diff = p2_Ddf2_diff_zaehler/(atoi(argv[6])+1);
	p2_DV_diff = p2_DV_diff_zaehler/(atoi(argv[6])+1);


	//------------------------------------------------------------------------> p>= inversion
	if(p_MIN_diff > 0.5)	{ p_MIN_diff = 1 - p_MIN_diff; }
	if(p_MAX_diff > 0.5)	{ p_MAX_diff = 1 - p_MAX_diff; }
	if(p_RNG_diff > 0.5)    { p_RNG_diff = 1 - p_RNG_diff; }
	if(p_SUM_diff > 0.5)    { p_SUM_diff = 1 - p_SUM_diff; }
	if(p_MD_diff > 0.5)	    { p_MD_diff = 1 - p_MD_diff; }
	if(p_AM_diff > 0.5)		{ p_AM_diff = 1 - p_AM_diff; }
	if(p_GM_diff > 0.5)	    { p_GM_diff = 1 - p_GM_diff; }
	if(p_HM_diff > 0.5)	    { p_HM_diff = 1 - p_HM_diff; }
	if(p_V_diff  > 0.5)		{ p_V_diff  = 1 - p_V_diff; }
	if(p_Vdf1_diff > 0.5)   { p_Vdf1_diff = 1 - p_Vdf1_diff; }
	if(p_SVdf1_diff > 0.5)  { p_SVdf1_diff = 1 - p_SVdf1_diff; }
	if(p_S_diff  > 0.5)		{ p_S_diff  = 1 - p_S_diff; }
	if(p_Sdf1_diff > 0.5)   { p_Sdf1_diff = 1 - p_Sdf1_diff; }
	if(p_Sdf2_diff > 0.5)   { p_Sdf2_diff = 1 - p_Sdf2_diff; }
	if(p_D_diff > 0.5)      { p_D_diff = 1 - p_D_diff; }
	if(p_Ddf1_diff > 0.5)   { p_Ddf1_diff = 1 - p_Ddf1_diff; }
	if(p_Ddf2_diff > 0.5)   { p_Ddf2_diff = 1 - p_Ddf2_diff; }
	if(p_DV_diff > 0.5)     { p_DV_diff = 1 - p_DV_diff; }
	

	_log();//-----------------------------------------------------------------> ausgabe der resultate in logdatei

	
	
	//tmp
	/*system ("azube Ip_ZGP_PmVtlng.asc Ip_ZGP_PmVtlng1.asc . ,");//------------> punkt zu beistrich umformatierung
	system ("del Ip_ZGP_PmVtlng.asc");
	system ("ren Ip_ZGP_PmVtlng1.asc Ip_ZGP_PmVtlng.asc");*/

   
	//-------------------------------------------------------------------------| Protokoll 
	protStream = fopen( "Ip_ZGP_prot.rtf", "a" );
	fprintf (protStream,"Protokolltext");
	fclose( protStream );
	fclose( pvStream );
	
	//system("Ip_ZGP_Sn2.asc");//-----------------------------------------------> editieren der Planmatrixdatei
	//system("Ip_ZGP_log.txt");//-----------------------------------------------> editieren der logdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_ZGP_%i\n",logcode );
	fprintf (batStream, "move Ip_ZGP_log.txt Ip_ZGP_%i\n", logcode);
	fprintf (batStream, "move Ip_ZGP_Sn2.asc Ip_ZGP_%i\n", logcode);
	fprintf (batStream, "move Ip_ZGP_prot.rtf Ip_ZGP_%i\n", logcode);
	fprintf (batStream, "move Ip_ZGP_PmVtlng.asc Ip_ZGP_%i\n", logcode);
	fclose( batStream  );
	
	system ("verschub.bat");
	system ("del verschub.bat");

	printf("%i      \n",logcode);

    _fusszeile();
	
	return 0;
}

//-----------------------------------------------------------------------------| log datei
void _log()
{
	char methode [5];
	FILE *logStream; 

	if(artP == 1) {strcpy(methode,"P");}  if(artP == 2) {strcpy(methode,"Pr");}
	if(artP == 3) {strcpy(methode,"mP");} if(artP == 4) {strcpy(methode,"mPr");}
	if(artP == 5) {strcpy(methode,"Bt");}

	logStream = fopen( "Ip_ZGP_log.txt", "a" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_ZGP_ %s %s: \ntheta_zg_uv(Sn2,n)\n", dbuffer, tbuffer);
	fprintf (logStream, "\n");
	if ( artP == 1 || artP == 2 || artP == 5)//-------------------------------> ausgabe bei p>=	
	{
		fprintf (logStream, "\tG1\ts1\tG2\tdiff\tp>\tp<>\n");
		fprintf (logStream, "-----------------------------------------------------\n");
		if (INI[1][2] == 1)
		{fprintf (logStream, "n\t%2.0f\t%2.0f\t%2.0f\t%2.0f\n",		n_1, nA_, n_2, n_diff);}
		if (INI[2][2] == 1)
		{fprintf (logStream, "Min\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__MIN_1, __MIN_, __MIN_2, __MIN_diff, p_MIN_diff, p2_MIN_diff);}
		if (INI[3][2] == 1)
		{fprintf (logStream, "Max\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__MAX_1, __MAX_, __MAX_2, __MAX_diff, p_MAX_diff, p2_MAX_diff);}
		if (INI[4][2] == 1)
		{fprintf (logStream, "Rng\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__RNG_1, __RNG_, __RNG_2, __RNG_diff, p_RNG_diff, p2_RNG_diff);}	
		if (INI[5][2] == 1)
		{fprintf (logStream, "SUM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__SUM_1, __SUM_, __SUM_2, __SUM_diff, p_SUM_diff, p2_SUM_diff);}
		//fprintf (logStream, "\n");
		if (INI[6][2] == 1)
		{fprintf (logStream, "Md\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__MD_1, __MD_, __MD_2, __MD_diff, p_MD_diff, p2_MD_diff);}
		if (INI[7][2] == 1)
		{fprintf (logStream, "AM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__AM_1, __AM_, __AM_2, __AM_diff, p_AM_diff, p2_AM_diff);}
		if (INI[8][2] == 1 && __MIN_ > 0)
		{	fprintf (logStream, "GM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__GM_1, __GM_, __GM_2, __GM_diff, p_GM_diff, p2_GM_diff);}
		if (INI[9][2] == 1)
		{fprintf (logStream, "HM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__HM_1, __HM_, __HM_2, __HM_diff, p_HM_diff, p2_HM_diff);}
		//fprintf (logStream, "\n");
		if (INI[10][2] == 1)
		{fprintf (logStream, "V\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__V_1, __V_, __V_2, __V_diff, p_V_diff, p2_V_diff);}
		if (INI[11][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__V_1df1, __V_df1, __V_2df1, __V_df1_diff, p_Vdf1_diff, p2_Vdf1_diff);}
		if (INI[12][2] == 1)
		{fprintf (logStream, "SVdf1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__S_1V_1df1, __S_V_df1, __S_2V_2df1, __S_V_df1_diff, p_SVdf1_diff, p2_SVdf1_diff);}
		//fprintf (logStream, "\n");
		if (INI[13][2] == 1)
		{fprintf (logStream, "S\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__S_1, __S_, __S_2, __S_diff, p_S_diff, p2_S_diff);}
		if (INI[14][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__S_1df1, __S_df1, __S_2df1, __S_df1_diff, p_Sdf1_diff, p2_Sdf1_diff);}
		if (INI[15][2] == 1)
		{fprintf (logStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__S_1df2, __S_df2, __S_2df2, __S_df2_diff, p_Sdf2_diff, p2_Sdf2_diff);}
		//fprintf (logStream, "\n");
		if (INI[16][2] == 1)
		{fprintf (logStream, "D\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",		__D_1, __D_, __D_2, __D_diff, p_D_diff, p2_D_diff);}
		if (INI[17][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__D_1df1, __D_df1, __D_2df1, __D_df1_diff, p_Ddf1_diff, p2_Ddf1_diff);}
		if (INI[18][2] == 1)
		{fprintf (logStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n",	__D_1df2, __D_df2, __D_2df2, __D_df2_diff, p_Ddf2_diff, p2_Ddf2_diff);}
		if (INI[19][2] == 1)
		{fprintf (logStream, "DvarO\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\n\n",__DV_1, __DV_, __DV_2, __DV_diff, p_DV_diff, p2_DV_diff);}
	}
	if ( artP == 3 || artP == 4)//--------------------------------------------> ausgabe bei mp>=		
	{
		fprintf (logStream, "\tG1\ts1\tG2\tdiff\tp=\tmp>\tmp<>\n");
		fprintf (logStream, "-------------------------------------------------------------\n");
		if (INI[1][2] == 1)
		{fprintf (logStream, "n\t%2.0f\t%2.0f\t%2.0f\t%2.0f\n",		n_1, nA_, n_2, n_diff);}
		if (INI[2][2] == 1)
		{fprintf (logStream, "Min\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__MIN_1, __MIN_, __MIN_2, __MIN_diff, pkt_p_MIN_diff, p_MIN_diff, p2_MIN_diff);}
		if (INI[3][2] == 1)
		{fprintf (logStream, "Max\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__MAX_1, __MAX_, __MAX_2, __MAX_diff, pkt_p_MAX_diff, p_MAX_diff, p2_MAX_diff);}
		if (INI[4][2] == 1)
		{fprintf (logStream, "Rng\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__RNG_1, __RNG_, __RNG_2, __RNG_diff, pkt_p_RNG_diff, p_RNG_diff, p2_RNG_diff);}	
		if (INI[5][2] == 1)
		{fprintf (logStream, "SUM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__SUM_1, __SUM_, __SUM_2, __SUM_diff, pkt_p_SUM_diff, p_SUM_diff, p2_SUM_diff);}
		//fprintf (logStream, "\n");
		if (INI[6][2] == 1)
		{fprintf (logStream, "Md\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__MD_1, __MD_, __MD_2, __MD_diff, pkt_p_MD_diff, p_MD_diff, p2_MD_diff);}
		if (INI[7][2] == 1)
		{fprintf (logStream, "AM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__AM_1, __AM_, __AM_2, __AM_diff, pkt_p_AM_diff, p_AM_diff, p2_AM_diff);}
		if (INI[8][2] == 1 && __MIN_ > 0)
		{	fprintf (logStream, "GM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__GM_1, __GM_, __GM_2, __GM_diff, pkt_p_GM_diff, p_GM_diff, p2_GM_diff);}
		if (INI[9][2] == 1)
		{fprintf (logStream, "HM\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",		__HM_1, __HM_, __HM_2, __HM_diff, pkt_p_HM_diff, p_HM_diff, p2_HM_diff);}
		//fprintf (logStream, "\n");
		if (INI[10][2] == 1)
		{fprintf (logStream, "V\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",		__V_1, __V_, __V_2, __V_diff, pkt_p_V_diff, p_V_diff, p2_V_diff);}
		if (INI[11][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__V_1df1, __V_df1, __V_2df1, __V_df1_diff, pkt_p_Vdf1_diff, p_Vdf1_diff, p2_Vdf1_diff);}
		if (INI[12][2] == 1)
		{fprintf (logStream, "SVdf1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__S_1V_1df1, __S_V_df1, __S_2V_2df1, __S_V_df1_diff, pkt_p_SVdf1_diff, p_SVdf1_diff, p2_SVdf1_diff);}
		//fprintf (logStream, "\n");
		if (INI[13][2] == 1)
		{fprintf (logStream, "S\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",		__S_1, __S_, __S_2, __S_diff, pkt_p_S_diff, p_S_diff, p2_S_diff);}
		if (INI[14][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__S_1df1, __S_df1, __S_2df1, __S_df1_diff, pkt_p_Sdf1_diff, p_Sdf1_diff, p2_Sdf1_diff);}
		if (INI[15][2] == 1)
		{fprintf (logStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__S_1df2, __S_df2, __S_2df2, __S_df2_diff, pkt_p_Sdf2_diff, p_Sdf2_diff, p2_Sdf2_diff);}
		//fprintf (logStream, "\n");
		if (INI[16][2] == 1)
		{fprintf (logStream, "D\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",		__D_1, __D_, __D_2, __D_diff, pkt_p_D_diff, p_D_diff, p2_D_diff);}
		if (INI[17][2] == 1)
		{fprintf (logStream, "df1\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__D_1df1, __D_df1, __D_2df1, __D_df1_diff, pkt_p_Ddf1_diff, p_Ddf1_diff, p2_Ddf1_diff);}
		if (INI[18][2] == 1)
		{fprintf (logStream, "df2\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n",	__D_1df2, __D_df2, __D_2df2, __D_df2_diff, pkt_p_Ddf2_diff, p_Ddf2_diff, p2_Ddf2_diff);}
		if (INI[19][2] == 1)
		{fprintf (logStream, "DvarO\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%.3f\t%.3f\t%.3f\n\n",__DV_1, __DV_, __DV_2, __DV_diff, pkt_p_DV_diff, p_DV_diff, p2_DV_diff);}
		}
	fprintf (logStream, "\nMethode: %s, seed=%.0f, M=%i\n\n", methode, Oseed, mP);
	
	fprintf (logStream, "\tG1\ts1\tG2\tdiff\n");
	fprintf (logStream, "AM   ---------------------------------\n\n");
	if (INI[2][2] == 1)
	{fprintf (logStream, "qMin\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_MIN_1,     q_MIN_,    q_MIN_2,	   q_MIN_diff);}
	if (INI[3][2] == 1)
	{fprintf (logStream, "qMax\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_MAX_1,     q_MAX_,    q_MAX_2,	   q_MAX_diff);}
	if (INI[4][2] == 1)
	{fprintf (logStream, "qRng\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_RNG_1,     q_RNG_,    q_RNG_2,	   q_RNG_diff);}
	if (INI[5][2] == 1)
	{fprintf (logStream, "qSum\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_SUM_1,     q_SUM_,    q_SUM_2,     q_SUM_diff);}
	//fprintf (logStream, "\n");
	if (INI[6][2] == 1)
	{fprintf (logStream, "qMd\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",      q_MD_1,      q_MD_,     q_MD_2,	   q_MD_diff);}
	if (INI[7][2] == 1)
	{fprintf (logStream, "qAM\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",      q_AM_1,      q_AM_,     q_AM_2,	   q_AM_diff);}
	if (INI[8][2] == 1 && __MIN_ > 0)
	{	fprintf (logStream, "qGM\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",  q_GM_1,      q_GM_,	  q_GM_2,	   q_GM_diff);}
	if (INI[9][2] == 1)
	{fprintf (logStream, "qHM\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",      q_HM_1,      q_HM_,     q_HM_2,	   q_HM_diff);}
	//fprintf (logStream, "\n");
	if (INI[10][2] == 1)
	{fprintf (logStream, "qV\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",       q_V_1,	   q_V_,      q_V_2,	   q_V_diff);}
	if (INI[11][2] == 1)
	{fprintf (logStream, "qdf1\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_V_1df1,	   q_V_df1,   q_V_2df1,	   q_V_df1_diff);}
	if (INI[12][2] == 1)
	{fprintf (logStream, "qSVdf1\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",   q_S_1V_1df1, q_S_V_df1, q_S_2V_2df1, q_S_V_df1_diff);}
	//fprintf (logStream, "\n");
	if (INI[13][2] == 1)
	{fprintf (logStream, "qS\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",       q_S_1,	   q_S_,      q_S_2,	   q_S_diff);}
	if (INI[14][2] == 1)
	{fprintf (logStream, "qdf1\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_S_1df1,    q_S_df1,   q_S_2df1,	   q_S_df1_diff);}
	if (INI[15][2] == 1)
	{fprintf (logStream, "qdf2\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_S_1df2,    q_S_df2,   q_S_2df2,	   q_S_df2_diff);}
	//fprintf (logStream, "\n");
	if (INI[16][2] == 1)
	{fprintf (logStream, "qD\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",       q_D_1,	   q_D_,	  q_D_2,	   q_D_diff);}
	if (INI[17][2] == 1)
	{fprintf (logStream, "qdf1\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_D_1df1,    q_D_df1,   q_D_2df1,	   q_D_df1_diff);}
	if (INI[18][2] == 1)
	{fprintf (logStream, "qdf2\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n",     q_D_1df2,    q_D_df2,   q_D_2df2,	   q_D_df2_diff);}
	if (INI[19][2] == 1)
	{fprintf (logStream, "qDvarO\t%5.2f\t%5.2f\t%5.2f\t%5.2f\n\n", q_DV_1,      q_DV_,     q_DV_2,	   q_DV_diff);}

	fclose( logStream );	
}

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_ZGP [1]_[2]_[3]_[4]_[5]_[6]_[7]\n");
	printf("--------------------------------------------\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a1 (AV)\n");
	printf("[3] Index j zu Datenvektor a2 (UV/AV2)\n");
	printf("[4] (1)|UV (2)|V\n");
	printf("[5] (1)|P (2)|Pr (3)|mP (4)|mPr (5)|V (6)|mV\n");
	printf("[6] Anzahl der Permutationen m\n");
	printf("[7] (*)|Anfangswert (0)|Zeitwert\n");
	printf("--------------------------------------------\n");
	printf("Ip_ZGP von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_ZGP_\n");
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");
}

//-----------------------------------------------------------------------------| ENDE                                                                 