//-----------------------------------------------------------------------------| Ip_ESP von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include "Ip_CALC.h"

#define _ini_ 30 //-----------------------------------------------------------> n ini 

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 
void _theta_dg1();
void _theta_dg2();


char dbuffer [9],tbuffer [9], datenprm_dateiname[50], null[50]; //------------> buffer fuer time und date
int  logcode, zeichen_bis_punkt, artV, hub=0;

int INI[_ini_][2];//----------------------------------------------------------> voreinstellungs matrix INI(n ini,2) übernimmt die elemente auss Ip_ESP.ini

float s_ij; 

double 	AV_exp[40][2],//------------------------------------------------------> Urdatenmatrix
		V_AV_exp[30][2],//----------------------------------------------------> Variationssdatenmatrix v
		V_AV_zf[30][2],//-----------------------------------------------------> randomisierte Variationssdatenmatrix 
		n_n[_n_],//-----------------------------------------------------------> zufallsvektor n(n)
		nA_,
		q_AV_exp[20], q_V_AV_exp[20],//---------------------------------------> Thetaspiegel
		p_zaehler[20], pkt_p_zaehler[20], p2_zaehler[20], pkt_p2_zaehler[20],
		pfg, seed;
		
main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE	*iniStream,  *inStream, *inprmStream, *SnStream, //---------------> streams
			*outStream, *logStream,   *batStream; 

	int jLauf, iLauf, dLauf, vLauf, zLauf,
		nA,//-----------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kkA,//----------------------------------------------------------------> Datenmatrix Spaltenvektoren k
		index[21],//----------------------------------------------------------> Schablonenindex Vektor
		zlr=1;//--------------------------------------------------------------> Durchgangszähler 
	
	double V_;

	if (argc == 1 ) _profil();//----------------------------------------------> Ip_ESP profil
	if (argc != 7) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (atoi(argv[2]) == 0 ){ printf("\n[2] Format.0\n");_profil();}//--------> format					 
	if (atoi(argv[4]) == 0 || atoi(argv[4]) > 4) {printf("\n[4] Format.0\n");_profil();}

	//------------------------------------------------------------------------> vorenstellungen aus Ip_ESP.ini einlesen
	
	if (fopen ("Ip_ESP.ini", "r") == NULL) //--------------------------> fehlt Ip_ESP.ini wird eine Vorlage erstellt.
		{
			iniStream = fopen ("Ip_ESP.ini", "w");
		
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
			fprintf( iniStream,"0\t\t20_log_fortlaufend\n");
			fprintf( iniStream,"0\t\t21_ENDE\n");
				
			fclose( iniStream );
			system("Ip_ESP.ini");
		}

	iniStream = fopen( "Ip_ESP.ini", "r" );
	
	for (iLauf = 1; iLauf <= _ini_; iLauf++)
	{
		INI[iLauf][1] = iLauf; fscanf(iniStream,"%i%s\n", &INI[iLauf][2], null ); 

		//printf("%i\n", INI[iLauf][2]);
	}
	
	fclose( iniStream  );

	if (fopen (argv[1], "r") == NULL  ) //------------------------------------> fehlerroutine inputdatei
	{
   		inStream = fopen( argv[1], "w" );
		
		fprintf( inStream,"1\n"); 
		fprintf( inStream,"2\n"); 
		fprintf( inStream,"3\n"); 
		fprintf( inStream,"4\n"); 
		fprintf( inStream,"5\n");

		fclose( inStream  );
		system (argv[1]);
		printf("%s\n", argv[1] );
		
	}
	artV=atoi(argv[4]);//-----------------------------------------------------> Spiegelung der Variationsmethodemethodenart   
	pfg=atof(argv[3]);//------------------------------------------------------> Spiegelung der Prüfgrösse
	if(atoi(argv[6]) != 0)	 seed=atoi(argv[6]);  //--------------------------> anfangswert für zufallszahlenfunktion = argument7
	if(atoi(argv[6]) == 0)	 seed=(time(0)-1234567890); //--------------------> anfangswert über zeitfunktion
	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.

	if ( fopen (datenprm_dateiname, "r") == NULL  ) //------------------------> fehlerroutine datenparameterdatei 
	{
   		inprmStream = fopen( datenprm_dateiname, "w" );

		fprintf( inprmStream,"5\n"); 
		fprintf( inprmStream,"1\n");

		fclose( inprmStream  );
		system (datenprm_dateiname);
		printf("%s\n", datenprm_dateiname);
	}

	//-------------------------------------------------------------------------| einlesen der datenmatrix parameter n, k 
	inprmStream = fopen( datenprm_dateiname, "r" );
	fscanf(inprmStream,"%i\n%i", &nA, &kkA ); nA_ = nA; 
	fclose( inprmStream  );	

	if ( atoi(argv[2]) >> kkA ) //--------------------------------------------> fehlerroutine j>k 
	{
   			printf("\n[2] k<j .0\n");_profil();
	}

	if (nA > 8) {printf("\n[1] Rechenumfang.0\n");_profil();}//---------------> Rechenumfang

	//-------------------------------------------------------------------------| einlesen von aij nach AV_exp
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)

	for (iLauf = 0; iLauf < nA; iLauf++)
	{
		for (jLauf = 1; jLauf <= kkA; jLauf++)
		{	
			fscanf(inStream,"%f", &s_ij );

			if (jLauf == atoi(argv[2]))	AV_exp[iLauf+1][1] = s_ij;//----------> Urdatenvektor Initialisierung	
		}
	}

	fclose( inStream  );
	
	for(iLauf=1; iLauf<=nA; iLauf++) 
	{
		V_AV_exp[iLauf][2] = 1;	AV_exp[iLauf][2] = 1;//-----------------------> Gruppierugsvektoren zu 1
		index[iLauf] = 1;//---------------------------------------------------> Indexinitialisierung
	}

	for(iLauf=1; iLauf<=20; iLauf++) 
	{
		q_V_AV_exp[iLauf] = 0;//----------------------------------------------> Thetasumspiegelinitialisierung
		p_zaehler[iLauf] = 0; pkt_p_zaehler[iLauf] = 0;//---------------------> p Zähler Initialisierung
		p2_zaehler[iLauf] = 0; pkt_p2_zaehler[iLauf] = 0;
	}
		
	if( artV == 1 || artV == 2) V_= pow(nA,nA);//-----------------------------> Berechnung V=n^n
	if( artV == 3 || artV == 4) V_= atof(argv[5]);//--------------------------> V = nv


	outStream = fopen( "Ip_ESP_VnVtlng.asc", "w" );

	_kopfzeile();

	//-------------------------------------------------------------------------| ausgeben des Planvektors Sn 
	SnStream = fopen( "Ip_ESP_Sn.asc", "w" );
	
	for (iLauf = 1; iLauf <= nA; iLauf++) fprintf(SnStream,"%.3f\n", AV_exp[iLauf][1] );

	fclose( SnStream  );


	theta_zg_uv(AV_exp,nA_);//------------------------------------------------> Kennwertfunktion AV_exp (angewendet auf theta_1)

	//------------------------------------------------------------------------> Thetaspiegelung
	q_AV_exp[1] = MIN_1; 
	q_AV_exp[2] = MAX_1; 
	q_AV_exp[3] = RNG_1; 
	q_AV_exp[4] = SUM_1;
	q_AV_exp[5] = MD_1; 
	q_AV_exp[6] = AM_1; 
	q_AV_exp[7] = GM_1; 
	q_AV_exp[8] = HM_1;
	q_AV_exp[9] = V_1;
	q_AV_exp[10] = V_1df1; 
	q_AV_exp[11] = S_V_df1; 
	q_AV_exp[12] = S_1; 
	q_AV_exp[13] = S_1df1;
	q_AV_exp[14] = S_1df2;
	q_AV_exp[15] = D_1;
	q_AV_exp[16] = D_1df1;
	q_AV_exp[17] = D_1df2;
	q_AV_exp[18] = DV_1;

	for (dLauf = 1; dLauf <= 2; dLauf++)//------------------------------------> Berechnungs Phasen
	{
		
		
		
		
		if( artV == 1 || artV == 2)//-------------------------| Methode V
		{
		zlr=1;
		if(dLauf == 2)// Zweiter Variationsdurchgang
		{
			printf("           \r");//----------------------------------------> bildschirmausgabe clear
			//----------------------------------------------------------------> AM theta_theta 
			for(iLauf=1; iLauf<=20; iLauf++) q_V_AV_exp[iLauf] = q_V_AV_exp[iLauf]/V_; 
		}
		
		//---------------------------------------------------------------------| vollständige Variation V=n^n 
		while (zlr <= V_)
		{
			printf("%i ", dLauf);//-------------------------------------------> Bildschirmausgabe Durchgang
			
			for(iLauf=1; iLauf<=nA; iLauf++)
			{
				V_AV_exp[iLauf][1] = AV_exp[index[iLauf]][1];
			
				printf("%i", index[iLauf]);//---------------------------------> Bildschirmausgabe Variationsvektor
			}

			theta_zg_uv(V_AV_exp,nA_);//--------------------------------------> Kennwertfunktion V_AV_exp (angewendet auf theta_1)
		
			if(dLauf == 1 && nA < 7)// ---------------------------------------> Theta Dateiausgabe VnVtlng
			{
				fprintf(outStream, 
				"%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n", 
					
				MIN_1, MAX_1, RNG_1, SUM_1, MD_1, AM_1, GM_1, HM_1, V_1, V_1df1, 
				S_V_df1, S_1, S_1df1, S_1df2, D_1, D_1df1, D_1df2, DV_1);
			}

			if(dLauf == 1)_theta_dg1();//-------------------------------------> Erster Variationsdurchgang
			if(dLauf == 2)_theta_dg2();//-------------------------------------> Zweiter Variationsdurchgang
	
			for (jLauf=1; jLauf<=nA; jLauf++)
			{				
				if(jLauf == 1) index[1]++;
		
				if(index[jLauf] == nA+1) 
				{ 
					index[jLauf] = 1;
					index[jLauf+1] += 1;	
				}
			}
			printf ("\r");
			zlr++;
		}//while end
		
		if (nA >= 7) fprintf(outStream, "V>100000");//----------------------------> V>100000
		
		}//if methode V end


	
		 
		 
	//*
		if( artV == 3 || artV == 4)//-----------------------------| Methode Vr
		{
			//--------------------------------------------------------------------> erzeugung des zufallsvektors n
			qzufall(seed);//------------------------------------------------------> Zufallszahlenfunktion
			if(dLauf == 2)// Zweiter Variationsdurchgang
			{
				printf("           \r");//----------------------------------------> bildschirmausgabe clear
				//----------------------------------------------------------------> AM theta_theta 
				for(iLauf=1; iLauf<=20; iLauf++) q_V_AV_exp[iLauf] = q_V_AV_exp[iLauf]/V_; 
			}

			for (vLauf = 1; vLauf <= atoi(argv[5]); vLauf++)
			{
				printf("%i %i\r", dLauf, vLauf);//------------------------------------> bildschirmausgabe durchgang, Variationsnummer iV
				
				for (zLauf = 1; zLauf <= nA; zLauf++)
				{
				
				for (iLauf = 1; iLauf <= nA; iLauf++)
				{
					n_n[iLauf]=quasizufallszahl;//------------------------------------> zuordnung der zufallszahl zu zufallsvektorzelle ni
					qzufall(quasizufallszahl*10);//-----------------------------------> resultierende Zufallszahl * 10 (nachkommaverschiebung)
				}
			
				
					//--------------------------------------------------------------------> erster zeilenvektor v durch addition des 1 zeilenvektors s und des 1 elements des zufallsvektors n zu V
					V_AV_zf[1][1] = AV_exp[1][1];
					V_AV_zf[1][2] = n_n[1];

  					//---------------------------------------------------------------------| sortierung von S nach n zu V
					for (iLauf = 1; iLauf < nA; iLauf++)
					{
						for (jLauf = 0; jLauf < iLauf; jLauf++)
						{
							if (n_n[iLauf+1] < V_AV_zf[iLauf-jLauf][2])
							{
								V_AV_zf[(iLauf-jLauf)+1][1] = V_AV_zf[iLauf-jLauf][1];
								V_AV_zf[(iLauf-jLauf)+1][2] = V_AV_zf[iLauf-jLauf][2];

								hub++;
							}	
						}
	
						V_AV_zf[(iLauf+1)-hub][1] = AV_exp[iLauf+1][1];
						V_AV_zf[(iLauf+1)-hub][2] = n_n[iLauf+1];

						hub=0;
					}
		
				V_AV_exp[zLauf][1] = V_AV_zf[1][1] ;//---------------------------------------------------------------------| übertragung von V zu VS
		
				}//zLauf end

				theta_zg_uv(V_AV_exp,nA_);//--------------------------------------> Kennwertfunktion V_AV_exp (angewendet auf theta_1)

				if(dLauf == 1 && vLauf <= 3125)// ---------------------------------------> Theta Dateiausgabe der ersten 5000 Elemente VnVtlng
				{
					fprintf(outStream, 
					"%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n", 
					
					MIN_1, MAX_1, RNG_1, SUM_1, MD_1, AM_1, GM_1, HM_1, V_1, V_1df1, 
					S_V_df1, S_1, S_1df1, S_1df2, D_1, D_1df1, D_1df2, DV_1);
				}

				if(dLauf == 1) _theta_dg1();//-------------------------------------> Erster Variationsdurchgang
				if(dLauf == 2) _theta_dg2();//-------------------------------------> Zweiter Variationsdurchgang
		
			}//end vlauf
		
		}//if methode Vr end*/
	
		


	}//dLauf end

	

	fclose( outStream  );

	//------------------------------------------------------------------------> punktwahrscheinlichkeitsberechnung p= und p>=, p<> korrektur 
	if ( artV == 2 || artV == 4)	
	{
		for(iLauf=1; iLauf<=20; iLauf++) 
		{
			pkt_p_zaehler[iLauf] = pkt_p_zaehler[iLauf]/V_;
			
			p_zaehler[iLauf] = p_zaehler[iLauf] - (pkt_p_zaehler[iLauf] /2);
			p2_zaehler[iLauf] = p2_zaehler[iLauf] - (pkt_p_zaehler[iLauf] /2);
		}
	}

	//------------------------------------------------------------------------> Wahrscheinlichkeitsberechnung  p>=. / mp>=  p<>  / mp<>
	for(iLauf=1; iLauf<=20; iLauf++) 
	{
		p_zaehler[iLauf] = p_zaehler[iLauf]/V_;//-----------------------------> bei Vollvariation nicht V+1 (experimentalanordnung ist in allen variationen enthalten)
		p2_zaehler[iLauf] = p2_zaehler[iLauf]/V_;
	}

	//------------------------------------------------------------------------> p>= p= inversion
	for(iLauf=1; iLauf<=20; iLauf++) 
	{
		if(p_zaehler[iLauf] > 0.5)	 p_zaehler[iLauf] = 1 - p_zaehler[iLauf]; 
		if(pkt_p_zaehler[iLauf] > 0.5)	 pkt_p_zaehler[iLauf] = 1 - pkt_p_zaehler[iLauf];
	}

	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_ESP_log.txt", "a" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_ESP_ %s %s: \ntheta_zg_uv(Sn,n)\n\n", dbuffer, tbuffer);
	if ( artV == 1 || artV == 3)	
	{
		fprintf (logStream, "\ts\tp>\tp<>\n");
	    fprintf (logStream, "-----------------------------\n");
		if (INI[1][2] == 1) fprintf (logStream, "n\t%i\n", nA);
		if (INI[2][2] == 1) fprintf (logStream, "Min\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[1],    p_zaehler[1],  p2_zaehler[1]);
		if (INI[3][2] == 1) fprintf (logStream, "Max\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[2],    p_zaehler[2],  p2_zaehler[2]);
		if (INI[4][2] == 1) fprintf (logStream, "Rng\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[3],    p_zaehler[3],  p2_zaehler[3]);
		if (INI[5][2] == 1) fprintf (logStream, "Sum\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[4],    p_zaehler[4],  p2_zaehler[4]);
		if (INI[6][2] == 1) fprintf (logStream, "Md\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[5],    p_zaehler[5],  p2_zaehler[5]);
		if (INI[7][2] == 1) fprintf (logStream, "AM\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[6],    p_zaehler[6],  p2_zaehler[6]);
		if (INI[8][2] == 1) fprintf (logStream, "GM\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[7],    p_zaehler[7],  p2_zaehler[7]);
		if (INI[9][2] == 1) fprintf (logStream, "HM\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[8],    p_zaehler[8],  p2_zaehler[8]);
		if (INI[10][2] == 1) fprintf (logStream, "V\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[9],    p_zaehler[9],  p2_zaehler[9]);
		if (INI[11][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[10],   p_zaehler[10],  p2_zaehler[10]);
		if (INI[12][2] == 1) fprintf (logStream, "SVdf1\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[11],   p_zaehler[11],  p2_zaehler[11]);
		if (INI[13][2] == 1) fprintf (logStream, "S\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[12],   p_zaehler[12],  p2_zaehler[12]);
		if (INI[14][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[13],   p_zaehler[13],  p2_zaehler[13]);
		if (INI[15][2] == 1) fprintf (logStream, "df2\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[14],   p_zaehler[14],  p2_zaehler[14]);
		if (INI[16][2] == 1) fprintf (logStream, "D\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[15],   p_zaehler[15],  p2_zaehler[15]);
		if (INI[17][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[16],   p_zaehler[16],  p2_zaehler[16]);
		if (INI[18][2] == 1) fprintf (logStream, "df2\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[17],   p_zaehler[17],  p2_zaehler[17]);
		if (INI[19][2] == 1) fprintf (logStream, "DV\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[18],   p_zaehler[18],  p2_zaehler[18]);

		if ( artV == 1) fprintf (logStream, "\nPrüfgr= %.3f, Methode: V, V= %.0f\n", pfg, V_);
		if ( artV == 3) fprintf (logStream, "\nPrüfgr= %.3f, Methode: Vr, seed=%.0f, V= %.0f\n", pfg, seed, atof(argv[5]));
		
		fprintf (logStream, "\n");
	}

	if ( artV == 2 || artV == 4)	
	{
		fprintf (logStream, "\ts\tp=\tmp>\tmp<>\n");
		fprintf (logStream, "-------------------------------------\n");
		if (INI[1][2] == 1) fprintf (logStream, "n\t%i\n", nA);
		if (INI[2][2] == 1) fprintf (logStream, "Min\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[1],  pkt_p_zaehler[1],  p_zaehler[1],  p2_zaehler[1]);
		if (INI[3][2] == 1) fprintf (logStream, "Max\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[2],  pkt_p_zaehler[2],  p_zaehler[2],  p2_zaehler[2]);
		if (INI[4][2] == 1) fprintf (logStream, "Rng\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[3],  pkt_p_zaehler[3],  p_zaehler[3],  p2_zaehler[3]);
		if (INI[5][2] == 1) fprintf (logStream, "Sum\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[4],  pkt_p_zaehler[4],  p_zaehler[4],  p2_zaehler[4]);
		if (INI[6][2] == 1) fprintf (logStream, "Md\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[5],  pkt_p_zaehler[5],  p_zaehler[5],  p2_zaehler[5]);
		if (INI[7][2] == 1) fprintf (logStream, "AM\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[6],  pkt_p_zaehler[6],  p_zaehler[6],  p2_zaehler[6]);
		if (INI[8][2] == 1) fprintf (logStream, "GM\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[7],  pkt_p_zaehler[7],  p_zaehler[7],  p2_zaehler[7]);
		if (INI[9][2] == 1) fprintf (logStream, "HM\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[8],  pkt_p_zaehler[8],  p_zaehler[8],  p2_zaehler[8]);
		if (INI[10][2] == 1) fprintf (logStream, "V\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[9],  pkt_p_zaehler[9],  p_zaehler[9],  p2_zaehler[9]);
		if (INI[11][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[10],  pkt_p_zaehler[10], p_zaehler[10],  p2_zaehler[10]);
		if (INI[12][2] == 1) fprintf (logStream, "SVdf1\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[11],  pkt_p_zaehler[11], p_zaehler[11],  p2_zaehler[11]);
		if (INI[13][2] == 1) fprintf (logStream, "S\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[12],  pkt_p_zaehler[12], p_zaehler[12],  p2_zaehler[12]);
		if (INI[14][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[13],  pkt_p_zaehler[13], p_zaehler[13],  p2_zaehler[13]);
		if (INI[15][2] == 1) fprintf (logStream, "df2\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[14],  pkt_p_zaehler[14], p_zaehler[14],  p2_zaehler[14]);
		if (INI[16][2] == 1) fprintf (logStream, "D\t%.3f\t%.3f\t%.3f\t%.3f\n",		q_AV_exp[15],  pkt_p_zaehler[15], p_zaehler[15],  p2_zaehler[15]);
		if (INI[17][2] == 1) fprintf (logStream, "df1\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[16],  pkt_p_zaehler[16], p_zaehler[16],  p2_zaehler[16]);
		if (INI[18][2] == 1) fprintf (logStream, "df2\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[17],  pkt_p_zaehler[17], p_zaehler[17],  p2_zaehler[17]);
		if (INI[19][2] == 1) fprintf (logStream, "DV\t%.3f\t%.3f\t%.3f\t%.3f\n",	q_AV_exp[18],  pkt_p_zaehler[18], p_zaehler[18],  p2_zaehler[18]);

		if ( artV == 2) fprintf (logStream, "\nPrüfgr= %.3f, Methode: mV, V= %.0f\n", pfg, V_);
		if ( artV == 4) fprintf (logStream, "\nPrüfgr= %.3f, Methode: mVr, seed=%.0f, V= %.0f\n", pfg, seed, atof(argv[5]));
		fprintf (logStream, "\n");
	}

	fprintf (logStream, "\ts\n");
	fprintf (logStream, "AM   -----------\n\n");
	if (INI[2][2] == 1) fprintf (logStream, "qMin\t%.3f\n",		q_V_AV_exp[1]);
	if (INI[3][2] == 1) fprintf (logStream, "qMax\t%.3f\n",		q_V_AV_exp[2]);
	if (INI[4][2] == 1) fprintf (logStream, "qRng\t%.3f\n",		q_V_AV_exp[3]);
	if (INI[5][2] == 1) fprintf (logStream, "qSum\t%.3f\n",		q_V_AV_exp[4]);
	if (INI[6][2] == 1) fprintf (logStream, "qMd\t%.3f\n",		q_V_AV_exp[5]);
	if (INI[7][2] == 1) fprintf (logStream, "qAM\t%.3f\n",		q_V_AV_exp[6]);
	if (INI[8][2] == 1) fprintf (logStream, "qGM\t%.3f\n",		q_V_AV_exp[7]);
	if (INI[9][2] == 1) fprintf (logStream, "qHM\t%.3f\n",		q_V_AV_exp[8]);
	if (INI[10][2] == 1) fprintf (logStream, "qV\t%.3f\n",		q_V_AV_exp[9]);
	if (INI[11][2] == 1) fprintf (logStream, "qdf1\t%.3f\n",	q_V_AV_exp[10]);
	if (INI[12][2] == 1) fprintf (logStream, "qSVdf1\t%.3f\n",	q_V_AV_exp[11]);
	if (INI[13][2] == 1) fprintf (logStream, "qS\t%.3f\n",		q_V_AV_exp[12]);
	if (INI[14][2] == 1) fprintf (logStream, "qdf1\t%.3f\n",	q_V_AV_exp[13]);
	if (INI[15][2] == 1) fprintf (logStream, "qdf2\t%.3f\n",	q_V_AV_exp[14]);
	if (INI[16][2] == 1) fprintf (logStream, "qD\t%.3f\n",		q_V_AV_exp[15]);
	if (INI[17][2] == 1) fprintf (logStream, "qdf1\t%.3f\n",	q_V_AV_exp[16]);
	if (INI[18][2] == 1) fprintf (logStream, "qdf2\t%.3f\n",	q_V_AV_exp[17]);
	if (INI[19][2] == 1) fprintf (logStream, "qDV\t%.3f\n",		q_V_AV_exp[18]);


	logcode= time(0)-1234567890;

	if (INI[20][2] == 1) fprintf (logStream, "\nIp_ESP_log_%i\n\n\n", logcode);

	fclose( logStream  );

	system("Ip_ESP_log.txt");//-----------------------------------------------> editieren der logdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_ESP_log_%i\n",logcode );
	if (INI[20][2] == 0) fprintf (batStream, "move Ip_ESP_log.txt Ip_ESP_log_%i\n", logcode);
	fprintf (batStream, "move Ip_ESP_Sn.asc Ip_ESP_log_%i\n", logcode);
	fprintf (batStream, "move Ip_ESP_VnVtlng.asc Ip_ESP_log_%i\n", logcode);

	fclose( batStream  );
	
	system ("verschub.bat");
	system ("del verschub.bat");

	printf("%i      \n",logcode);
	
	_fusszeile();

	return 0;
}

void _theta_dg1()
{
	//------------------------------------------------------------------------> Thetasummierung
	q_V_AV_exp[1]  += MIN_1; 
	q_V_AV_exp[2]  += MAX_1; 
	q_V_AV_exp[3]  += RNG_1; 
	q_V_AV_exp[4]  += SUM_1;
	q_V_AV_exp[5]  += MD_1; 
	q_V_AV_exp[6]  += AM_1; 
	q_V_AV_exp[7]  += GM_1; 
	q_V_AV_exp[8]  += HM_1;
	q_V_AV_exp[9]  += V_1;
	q_V_AV_exp[10] += V_1df1; 
	q_V_AV_exp[11] += S_V_df1; 
	q_V_AV_exp[12] += S_1; 
	q_V_AV_exp[13] += S_1df1;
	q_V_AV_exp[14] += S_1df2;
	q_V_AV_exp[15] += D_1;
	q_V_AV_exp[16] += D_1df1;
	q_V_AV_exp[17] += D_1df2;
	q_V_AV_exp[18] += DV_1;
}

void _theta_dg2()
{
	//------------------------------------------------------------------------> ereigniszähler THETA >= theta
	if ( MIN_1		>= pfg )	p_zaehler[1]  += 1; 
	if ( MAX_1		>= pfg )	p_zaehler[2]  += 1; 
	if ( RNG_1		>= pfg )	p_zaehler[3]  += 1; 
	if ( SUM_1		>= pfg )	p_zaehler[4]  += 1; 
	if ( MD_1		>= pfg )	p_zaehler[5]  += 1; 
	if ( AM_1		>= pfg )	p_zaehler[6]  += 1; 
	if ( GM_1		>= pfg )	p_zaehler[7]  += 1; 
	if ( HM_1		>= pfg )	p_zaehler[8]  += 1; 
	if ( V_1		>= pfg )	p_zaehler[9]  += 1; 
	if ( V_1df1		>= pfg )	p_zaehler[10] += 1; 
	if ( S_1V_1df1	>= pfg )	p_zaehler[11] += 1; 
	if ( S_1		>= pfg )	p_zaehler[12] += 1; 
	if ( S_1df1		>= pfg )	p_zaehler[13] += 1; 
	if ( S_1df2		>= pfg )	p_zaehler[14] += 1; 
	if ( D_1		>= pfg )	p_zaehler[15] += 1; 
	if ( D_1df1		>= pfg )	p_zaehler[16] += 1; 
	if ( D_1df2		>= pfg )	p_zaehler[17] += 1; 
	if ( DV_1		>= pfg )	p_zaehler[18] += 1; 

	//------------------------------------------------------------------------> ereigniszähler |THETA-AMtheta| >= |theta-AMtheta|
	if (sqrt(pow(MIN_1-q_V_AV_exp[1],2)) >= sqrt(pow(pfg-q_V_AV_exp[1],2)))	p2_zaehler[1] += 1;
	if (sqrt(pow(MAX_1-q_V_AV_exp[2],2)) >= sqrt(pow(pfg-q_V_AV_exp[2],2)))	p2_zaehler[2] += 1;
	if (sqrt(pow(RNG_1-q_V_AV_exp[3],2)) >= sqrt(pow(pfg-q_V_AV_exp[3],2)))	p2_zaehler[3] += 1;
	if (sqrt(pow(SUM_1-q_V_AV_exp[4],2)) >= sqrt(pow(pfg-q_V_AV_exp[4],2)))	p2_zaehler[4] += 1;
	if (sqrt(pow( MD_1-q_V_AV_exp[5],2)) >= sqrt(pow(pfg-q_V_AV_exp[5],2)))	p2_zaehler[5] += 1;
	if (sqrt(pow( AM_1-q_V_AV_exp[6],2)) >= sqrt(pow(pfg-q_V_AV_exp[6],2)))	p2_zaehler[6] += 1;
	if (sqrt(pow( GM_1-q_V_AV_exp[7],2)) >= sqrt(pow(pfg-q_V_AV_exp[7],2)))	p2_zaehler[7] += 1;
	if (sqrt(pow( HM_1-q_V_AV_exp[8],2)) >= sqrt(pow(pfg-q_V_AV_exp[8],2)))	p2_zaehler[8] += 1;
	if (sqrt(pow(  V_1-q_V_AV_exp[9],2)) >= sqrt(pow(pfg-q_V_AV_exp[9],2)))	p2_zaehler[9] += 1;
	if (sqrt(pow(V_1df1-q_V_AV_exp[10],2)) >= sqrt(pow(pfg-q_V_AV_exp[10],2)))	p2_zaehler[10] += 1;
	if (sqrt(pow(S_1V_1df1-q_V_AV_exp[11],2)) >= sqrt(pow(pfg-q_V_AV_exp[11],2)))	p2_zaehler[11] += 1;
	if (sqrt(pow( S_1-q_V_AV_exp[12],2)) >= sqrt(pow(pfg-q_V_AV_exp[12],2)))	p2_zaehler[12] += 1;
	if (sqrt(pow(S_1df1-q_V_AV_exp[13],2)) >= sqrt(pow(pfg-q_V_AV_exp[13],2)))	p2_zaehler[13] += 1;
	if (sqrt(pow( S_1df2-q_V_AV_exp[14],2)) >= sqrt(pow(pfg-q_V_AV_exp[14],2)))	p2_zaehler[14] += 1;
	if (sqrt(pow(   D_1-q_V_AV_exp[15],2)) >= sqrt(pow(pfg-q_V_AV_exp[15],2)))	p2_zaehler[15] += 1;
	if (sqrt(pow(D_1df1-q_V_AV_exp[16],2)) >= sqrt(pow(pfg-q_V_AV_exp[16],2)))	p2_zaehler[16] += 1;
	if (sqrt(pow(D_1df2-q_V_AV_exp[17],2)) >= sqrt(pow(pfg-q_V_AV_exp[17],2)))	p2_zaehler[17] += 1;
	if (sqrt(pow(  DV_1-q_V_AV_exp[18],2)) >= sqrt(pow(pfg-q_V_AV_exp[18],2)))	p2_zaehler[18] += 1;
				

	if ( artV == 2 || artV == 4)	
	{
		//--------------------------------------------------------------------> ereigniszähler THETA = theta
		if ( MIN_1		== pfg )	pkt_p_zaehler[1]  += 1; 
		if ( MAX_1		== pfg )	pkt_p_zaehler[2]  += 1; 
		if ( RNG_1		== pfg )	pkt_p_zaehler[3]  += 1; 
		if ( SUM_1		== pfg )	pkt_p_zaehler[4]  += 1; 
		if ( MD_1		== pfg )	pkt_p_zaehler[5]  += 1; 
		if ( AM_1		== pfg )	pkt_p_zaehler[6]  += 1; 
		if ( GM_1		== pfg )	pkt_p_zaehler[7]  += 1; 
		if ( HM_1		== pfg )	pkt_p_zaehler[8]  += 1; 
		if ( V_1		== pfg )	pkt_p_zaehler[9]  += 1; 
		if ( V_1df1		== pfg )	pkt_p_zaehler[10] += 1; 
		if ( S_1V_1df1  == pfg )	pkt_p_zaehler[11] += 1; 
		if ( S_1		== pfg )	pkt_p_zaehler[12] += 1; 
		if ( S_1df1		== pfg )	pkt_p_zaehler[13] += 1; 
		if ( S_1df2		== pfg )	pkt_p_zaehler[14] += 1; 
		if ( D_1		== pfg )	pkt_p_zaehler[15] += 1; 
		if ( D_1df1		== pfg )	pkt_p_zaehler[16] += 1; 
		if ( D_1df2		== pfg )	pkt_p_zaehler[17] += 1; 
		if ( DV_1		== pfg )	pkt_p_zaehler[18] += 1;

		//--------------------------------------------------------------------> ereigniszähler |THETA-AMtheta| = |theta-AMtheta|			
		if (sqrt(pow(MIN_1-q_V_AV_exp[1],2)) == sqrt(pow(pfg-q_V_AV_exp[1],2)))	pkt_p2_zaehler[1] += 1;
		if (sqrt(pow(MAX_1-q_V_AV_exp[2],2)) == sqrt(pow(pfg-q_V_AV_exp[2],2)))	pkt_p2_zaehler[2] += 1;
		if (sqrt(pow(RNG_1-q_V_AV_exp[3],2)) == sqrt(pow(pfg-q_V_AV_exp[3],2)))	pkt_p2_zaehler[3] += 1;
		if (sqrt(pow(SUM_1-q_V_AV_exp[4],2)) == sqrt(pow(pfg-q_V_AV_exp[4],2)))	pkt_p2_zaehler[4] += 1;
		if (sqrt(pow( MD_1-q_V_AV_exp[5],2)) == sqrt(pow(pfg-q_V_AV_exp[5],2)))	pkt_p2_zaehler[5] += 1;
		if (sqrt(pow( AM_1-q_V_AV_exp[6],2)) == sqrt(pow(pfg-q_V_AV_exp[6],2)))	pkt_p2_zaehler[6] += 1;
		if (sqrt(pow( GM_1-q_V_AV_exp[7],2)) == sqrt(pow(pfg-q_V_AV_exp[7],2)))	pkt_p2_zaehler[7] += 1;
		if (sqrt(pow( HM_1-q_V_AV_exp[8],2)) == sqrt(pow(pfg-q_V_AV_exp[8],2)))	pkt_p2_zaehler[8] += 1;
		if (sqrt(pow( V_1-q_V_AV_exp[9],2)) == sqrt(pow(pfg-q_V_AV_exp[9],2)))	pkt_p2_zaehler[9] += 1;
		if (sqrt(pow(V_1df1-q_V_AV_exp[10],2)) == sqrt(pow(pfg-q_V_AV_exp[10],2)))	pkt_p2_zaehler[10] += 1;
		if (sqrt(pow(S_1V_1df1-q_V_AV_exp[11],2)) == sqrt(pow(pfg-q_V_AV_exp[11],2)))	pkt_p2_zaehler[11] += 1;
		if (sqrt(pow( S_1-q_V_AV_exp[12],2)) == sqrt(pow(pfg-q_V_AV_exp[12],2)))	pkt_p2_zaehler[12] += 1;
		if (sqrt(pow(S_1df1-q_V_AV_exp[13],2)) == sqrt(pow(pfg-q_V_AV_exp[13],2)))	pkt_p2_zaehler[13] += 1;
		if (sqrt(pow( S_1df2-q_V_AV_exp[14],2)) == sqrt(pow(pfg-q_V_AV_exp[14],2)))	pkt_p2_zaehler[14] += 1;
		if (sqrt(pow( D_1-q_V_AV_exp[15],2)) == sqrt(pow(pfg-q_V_AV_exp[15],2)))	pkt_p2_zaehler[15] += 1;
		if (sqrt(pow(D_1df1-q_V_AV_exp[16],2)) == sqrt(pow(pfg-q_V_AV_exp[16],2)))	pkt_p2_zaehler[16] += 1;
		if (sqrt(pow(D_1df2-q_V_AV_exp[17],2)) == sqrt(pow(pfg-q_V_AV_exp[17],2)))	pkt_p2_zaehler[17] += 1;
		if (sqrt(pow(DV_1-q_V_AV_exp[18],2)) == sqrt(pow(pfg-q_V_AV_exp[18],2)))	pkt_p2_zaehler[18] += 1;			
	}
}

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_ESP [1]_[2]_[3]_[4]_[5]_[6]\n");
	printf("-----------------------------------------\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a\n");
	printf("[3] Pruefgroesse\n");
	printf("[4] (1)|V (2)|mV (3)|Vr (4)|mVr\n");
	printf("[5] Anzahl der Variationen nv\n");
	printf("[6] (*)|Anfangswert (0)|Zeitwert\n");
	printf("-----------------------------------------\n");
	printf("Ip_ESP von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_ESP_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
