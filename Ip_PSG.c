//-----------------------------------------------------------------------------| Ip_PSG von Dietmar SCHRAUSSER 2009              
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include "Ip_CALC.h"

#define _nP_ 5000 //----------------------------------------------------------> n Thetamatrix PVt
#define _nR_ 1600 //----------------------------------------------------------> mittlerer thetabereich zur regression

double th[_nP_];//------------------------------------------------------------> Thetavektor th(nP) 
double RT[_nR_][2];//---------------------------------------------------------> Regressionsmatrix RT(_nR_,2) der mittleren _nR_ thetaelemente zur regressionsanalyse
double xR, lin_max, lin_min, min_th[25], max_th[25], psg_th_1[25], psg_th_2[25];
float t_ij;//-----------------------------------------------------------------> Thetaelement tij
double th_1[25], th_2[25],th_3[25],th_4[25], end_th_1[25],end_th_2[25],
	   sum_th_1[25], sum_th_2[25], sum_th_3[25], sum_th_4[25], 
	   p_th_1[25], p_th_2[25], p2_th_1[25], p2_th_2[25], ps_th_1[25], ps_th_2[25]; //-------------------> parameterspiegel (n_inpt, th_k), p zähler

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

int  iLauf, jLauf, kLauf, kiLauf, kjLauf, inLauf, hub, n_inpt, th_j, th_j1, th_k, logcode;

char dbuffer [9],tbuffer [9], Ip_PSG_in[50][50];

main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv     
{
	FILE  *inptStream, *inStream, *logStream, *outStream, *batStream;
	
	if (argc == 1 ) //--------------------------------------------------------> Ip_ZGP profil
	{
   		 _profil();
	}
	if (argc != 3 ) //--------------------------------------------------------> fehlerroutine argumente
	{
   		printf("\n[ ] Argumente.0\n"); _profil();
	}
	
	if ( atoi(argv[2]) > 18 || atoi(argv[2]) < 0 ) //-------------------------> fehlerroutine thetavektor th
	{
   			printf("\n[2] Thetavektor.0\n");_profil();
	}

	
	if ( atoi(argv[2]) == 0 && stricmp(argv[2], "0") != 0) //-----------------> j th keine zahl
	{
   			printf("\n[2] Format.0\n");_profil();
	}
	
	
	//------------------------------------------------------------------------> logdatei
	logStream = fopen( "Ip_PSG_log.txt", "w" );

	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_PSG_ %s %s: \nfn_lin_reg(RT,_nR_)\n\n", dbuffer, tbuffer);

	//------------------------------------------------------------------------> einlesen der inputdateinamen aus Inputdefinitionsdatei


	if (fopen (argv[1], "r") == NULL) //------------------------------> fehlt inputdatei,wird eine Vorlage erstellt.
	{
		inptStream = fopen (argv[1], "w");
		
		fprintf( inptStream,"2\nIp_ZGP_PmVtlng_01.asc\nIp_ZGP_PmVtlng_02.asc\n"); 
				
		fclose( inptStream );
		system( argv[1]);
	}

	inptStream = fopen( argv[1], "r" );
	
	fscanf(inptStream,"%i\n", &n_inpt ); 
	
	for (iLauf = 1; iLauf <= n_inpt; iLauf++)
	{
		fscanf(inptStream,"%s\n", &Ip_PSG_in[iLauf] ); 
	}
	fclose( inptStream  );



	//------------------------------------------------------------------------> einlesen der thetavektoren in theta matrix T(5000,2), sortierung, übergabe der mittleren _nR_ elemente an fn_lin_reg
	_kopfzeile();

	for (inLauf = 1; inLauf <= n_inpt; inLauf++)//----------------------------> über inputdateien
	{	
		if(atoi(argv[2]) == 0){th_k = 18;}
		if(atoi(argv[2]) != 0){th_k = 1;}
	
		for (jLauf = 1; jLauf <= th_k; jLauf++)//-----------------------------> über kT (Spaltenvektoren der Thetamatrix)
		{
			inStream = fopen( Ip_PSG_in[inLauf], "r" );
			
			th[1] = 9999999999999;//------------------------------------------> maximum
			hub=0;
		
			printf("           \r");//----------------------------------------> bildschirmzähler cls
		
			for (iLauf = 1; iLauf <= _nP_; iLauf++)//-------------------------> über nT (Zeilenvektoren der Thetamatrix)
			{
		
				for (kLauf = 1; kLauf <= 18; kLauf++)//-----------------------> j verschiebung
				{
					printf("%i %i.%i\r", n_inpt, inLauf, jLauf);//------------> bildschirmzähler
					
					fscanf(inStream,"%f", &t_ij );//--------------------------> einlesen eines elementes t aus T
			
					if(atoi(argv[2]) == 0){th_j = jLauf;th_j1 = jLauf;}
					if(atoi(argv[2]) != 0){th_j = atoi(argv[2]);th_j1 = 1;} // zu beachten:th_j(jth,jLauf) vs th_j1(1,jLauf)

					if(kLauf == th_j) //--------------------------------------> aufsteigend sortieren der thetaelemente tij in thetavektor th 
					{
						
						if(t_ij <= min_th[th_j1]) {min_th[th_j1] =  t_ij;}//---> identifizierung von maximum und minimum pro theta
						if(t_ij >= max_th[th_j1]) {max_th[th_j1] =  t_ij;}

						for (kjLauf = 0; kjLauf < iLauf; kjLauf++)
						{
							if (t_ij < th[iLauf-kjLauf])
							{
								th[(iLauf-kjLauf)+1] = th[iLauf-kjLauf]; hub++;	
							}	
						}
					
						th[(iLauf+1)-hub] = t_ij; hub=0;

					}//if(kLauf) end
				}//kLauf end
			}//iLauf end

			fclose( inStream );

			//mittlere _nR_ an reg funktion (iA _nR_ = 800 start:((_nP_ - _nR_)/2)=2100, end = _nP_ - ((_nP_ - _nR_)/2)=2900)
			xR=((_nP_ - _nR_)/2);

			for (kiLauf = 1; kiLauf <= _nP_ - ((_nP_ - _nR_)/2); kiLauf++)//--> übergabe an RT
			{	
				if(kiLauf >= ((_nP_ - _nR_)/2))
				{	
					RT[(kiLauf-((_nP_ - _nR_)/2))+1][2] = th[kiLauf];
					RT[(kiLauf-((_nP_ - _nR_)/2))+1][1] = xR;

					xR++;
				} 
			}

			//----------------------------------------------------------------> regressionsfunktion
			fn_lin_reg(RT,_nR_);

			//----------------------------------------------------------------> grenzwerte aus linearfunktion
			lin_max = B1x_ * _nP_ + B0x_; lin_min = B1x_ * 1 + B0x_;

			fprintf(logStream, "%s.%i ", Ip_PSG_in[inLauf], th_j);//----------> logausgabe 
			fprintf(logStream, "  f(x)= %.5fx + %.3f, r=%.3f,\t%5.3f\t%5.3f\n",  B1x_, B0x_, reg_, lin_min, lin_max);//logausgabe der regressionsparameter

			//----------------------------------------------------------------> parameter spiegelung
			th_1[jLauf] = B1x_;
			th_2[jLauf] = B0x_;
			th_3[jLauf] = lin_min;
			th_4[jLauf] = lin_max;

		
		}//end jLauf

		for (iLauf = 1; iLauf <= th_k ; iLauf++)//----------------------------> summierung der 4  kennwerte th_k funktioen über n_inpt
		{		
			sum_th_1[iLauf] += th_1[iLauf];
			sum_th_2[iLauf] += th_2[iLauf];
			sum_th_3[iLauf] += th_3[iLauf];
			sum_th_4[iLauf] += th_4[iLauf];
		}
	
	}//end inLauf
	
	fprintf(logStream, "-----------------------------------------------------------------------------\n");//logausgabe 

	for (iLauf = 1; iLauf <= th_k ; iLauf++)//--------------------------------> mittelung der 4  kennwerte th_k funktioen über n_inpt
	{		
		sum_th_1[iLauf] = sum_th_1[iLauf] /n_inpt;
		sum_th_2[iLauf] = sum_th_2[iLauf] /n_inpt;
		sum_th_3[iLauf] = sum_th_3[iLauf] /n_inpt;
		sum_th_4[iLauf] = sum_th_4[iLauf] /n_inpt;

		end_th_1[iLauf] = (sum_th_3[iLauf] + min_th[iLauf]) /2;
		end_th_2[iLauf] = (sum_th_4[iLauf] + max_th[iLauf]) /2;

		psg_th_1[iLauf] = (end_th_1[iLauf] + sum_th_3[iLauf]) /2;
		psg_th_2[iLauf] = (end_th_2[iLauf] + sum_th_4[iLauf]) /2;
	
		if(atoi(argv[2]) == 0){th_j = iLauf;}
		if(atoi(argv[2]) != 0){th_j = atoi(argv[2]);}
		
		fprintf(logStream, "Theta (%i)", th_j);//-----------------------------> logausgabe 
		fprintf(logStream, "\t\t  f(x)= %.5fx + %.3f,\t\t%5.3f\t%5.3f\n",  sum_th_1[iLauf], sum_th_2[iLauf], sum_th_3[iLauf], sum_th_4[iLauf]);//logausgabe der über n_inpt gemittelten regressionsparameter
		fprintf(logStream, "\t\t               \t\t\t\t\t%5.3f\t%5.3f\n",  psg_th_1[iLauf], psg_th_2[iLauf]);//logausgabe der psg werte
		fprintf(logStream, "\t\t               \t\t\t\t\t%5.3f\t%5.3f\n",  end_th_1[iLauf], end_th_2[iLauf]);//logausgabe der infinit werte
	
	
	}
	//------------------------------------------------------------------------> dateiausgabe der linearregressionsvektoren
	outStream = fopen( "Ip_PSG_linVekt.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", sum_th_1[jLauf] * iLauf + sum_th_2[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );

	//------------------------------------------------------------------------> dateiausgabe der linear minimal grenzwerte
	
	outStream = fopen( "Ip_PSG_linMin.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", sum_th_3[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );

	//------------------------------------------------------------------------> dateiausgabe der linear maximal grenzwerte
	
	outStream = fopen( "Ip_PSG_linMax.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", sum_th_4[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );

	//------------------------------------------------------------------------> dateiausgabe der minimal infinit
	
	outStream = fopen( "Ip_PSG_InfMin.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", end_th_1[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );

	//------------------------------------------------------------------------> dateiausgabe der maximal infinit
	
	outStream = fopen( "Ip_PSG_InfMax.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", end_th_2[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );
	//------------------------------------------------------------------------> dateiausgabe der minimal psg werte
	
	outStream = fopen( "Ip_PSG_psgMin.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", psg_th_1[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );

	//------------------------------------------------------------------------> dateiausgabe der maximal psg werte
	
	outStream = fopen( "Ip_PSG_psgMax.asc", "w" );
	
	for (iLauf = 1; iLauf <= _nP_ ; iLauf++)
	{
		for (jLauf = 1; jLauf <= th_k ; jLauf++)
		{		
			fprintf(outStream, "%f\t", psg_th_2[jLauf]);
		}
		fprintf(outStream, "\n");
	}
	fclose( outStream  );





	//-----------------------------------------------------------------------> prozentrangbestimmung der linearen grenzwerte
	//-----------------------------------------------------------------------> nullmatrix
	for (jLauf = 1; jLauf <= 25; jLauf++)//----------------------------------> über kT (Spaltenvektoren der Thetamatrix)
	{
		p_th_1[jLauf] = 0;
		p_th_2[jLauf] = 0;
		p2_th_1[jLauf] = 0;
		p2_th_2[jLauf] = 0;
		ps_th_1[jLauf] = 0;
		ps_th_2[jLauf] = 0;
	}
	printf("           \r");//-----------------------------------------------> bildschirmzähler cls

	for (inLauf = 1; inLauf <= n_inpt; inLauf++)//---------------------------> über inputdateien
	{
		inStream = fopen( Ip_PSG_in[inLauf], "r" );
			
		for (iLauf = 1; iLauf <= _nP_; iLauf++)//----------------------------> über nT (Zeilenvektoren der Thetamatrix)
		{
			for (kLauf = 1; kLauf <= 18; kLauf++)//--------------------------> über kT (Spaltenvektoren der Thetamatrix)
			{
				if(atoi(argv[2]) == 0){th_j = kLauf; th_j1 = kLauf;}
				if(atoi(argv[2]) != 0){th_j = atoi(argv[2]); th_j1 = 1;}// zu beachten:th_j(jth,jLauf) vs th_j1(1,jLauf)
					
				fscanf(inStream,"%f", &t_ij );//-----------------------------> einlesen eines elementes t aus T
					
				if(kLauf == th_j) 
				{
					if(t_ij >= sum_th_4[th_j1]) //---------------------------> vergleich liner endpunkt
					{ 
						p_th_2[th_j]++;//------------------------------------> element vergleich, p>=_zähler
						printf("%1.0f\r", p_th_2[th_j]);//-------------------> bildschirmzähler
					}
					if(t_ij <= sum_th_3[th_j1]) 
					{ 
						p_th_1[th_j]++;//------------------------------------> element vergleich, p<=_zähler
					}
					
					if(t_ij >= end_th_2[th_j1]) //---------------------------> vergleich infinit anfangspunkt
					{ 
						p2_th_2[th_j]++;//-----------------------------------> element vergleich, p>=_zähler
					}
					if(t_ij <= end_th_1[th_j1]) 
					{ 
						p2_th_1[th_j]++;//-----------------------------------> element vergleich, p<=_zähler
					}

					if(t_ij >= psg_th_2[th_j1]) //---------------------------> vergleich psg
					{ 
						ps_th_2[th_j]++;//-----------------------------------> element vergleich, p>=_zähler
					}
					if(t_ij <= psg_th_1[th_j1]) 
					{ 
						ps_th_1[th_j]++;//-----------------------------------> element vergleich, p<=_zähler
					}
				
				}


	
			}//kLauf end
				
		}//iLauf end
		fclose( inStream );
		printf("              \r");//----------------------------------------> bildschirmzähler cls

	}//inLauf end

	fprintf(logStream, "-----------------------------------------------------------------------------\n");//logausgabe 
	
	//-----------------------------------------------------------------------> prozentrangbestimmung p>=, p<=
	
	for (jLauf = 1; jLauf <= 18; jLauf++)//----------------------------------> über kT (Spaltenvektoren der Thetamatrix)
	{
		p_th_1[jLauf] = p_th_1[jLauf] / (n_inpt*_nP_);
		p_th_2[jLauf] = p_th_2[jLauf] / (n_inpt*_nP_);
		p2_th_1[jLauf] = p2_th_1[jLauf] / (n_inpt*_nP_);
		p2_th_2[jLauf] = p2_th_2[jLauf] / (n_inpt*_nP_);
		ps_th_1[jLauf] = ps_th_1[jLauf] / (n_inpt*_nP_);
		ps_th_2[jLauf] = ps_th_2[jLauf] / (n_inpt*_nP_);


		
		if(atoi(argv[2]) == 0){th_k = jLauf;}
		if(atoi(argv[2]) != 0){th_k = atoi(argv[2]);}
		
		if(jLauf == th_k)
		{
			fprintf(logStream, "Theta (%i)", th_k);//------------------------> logausgabe 
			fprintf(logStream, " lin_min p<= %.3f, lin_max p>= %.3f\n", p_th_1[jLauf], p_th_2[jLauf]);//logausgabe der grenzwert prozentränge
			fprintf(logStream, "          psg_min p<= %.3f, psg_max p>= %.3f\n", ps_th_1[jLauf], ps_th_2[jLauf]);//logausgabe der psg prozentränge
			fprintf(logStream, "          inf_min p<= %.3f, inf_max p>= %.3f\n", p2_th_1[jLauf], p2_th_2[jLauf]);//logausgabe der grenzwert prozentränge
			fprintf(logStream, "                                 psg p<> %.3f\n", ps_th_1[jLauf] + ps_th_2[jLauf]);//logausgabe des 2 seitigen punktsignifikanz prozentranges

		}
	}
	
	fclose( logStream  );

	system("Ip_PSG_log.txt");//-----------------------------------------------> editieren der logdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_PSG_%i\n",logcode );
	fprintf (batStream, "move Ip_PSG_log.txt Ip_PSG_%i\n", logcode);
	fprintf (batStream, "move Ip_PSG_linVekt.asc Ip_PSG_%i\n", logcode);
	
	fprintf (batStream, "move Ip_PSG_linMin.asc Ip_PSG_%i\n", logcode);
	fprintf (batStream, "move Ip_PSG_linMax.asc Ip_PSG_%i\n", logcode);
	
	fprintf (batStream, "move Ip_PSG_InfMin.asc Ip_PSG_%i\n", logcode);
	fprintf (batStream, "move Ip_PSG_InfMax.asc Ip_PSG_%i\n", logcode);
	
	fprintf (batStream, "move Ip_PSG_psgMin.asc Ip_PSG_%i\n", logcode);
	fprintf (batStream, "move Ip_PSG_psgMax.asc Ip_PSG_%i\n", logcode);

	fprintf (batStream, "xcopy Ip_PSG_in.asc Ip_PSG_%i /q > null.txt \n", logcode);
	fclose( batStream  );
	
	system ("ip_format.bat");
	system ("verschub.bat");
	system ("del null.txt");
	system ("del nil");
	system ("del verschub.bat");

	printf("%i      \n",logcode);
		
	_fusszeile();

	return 0;

}//end main

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_PSG [1]_[2]\n");
	printf("-------------------------------------------\n");
	printf("[1] Inputdefinitionsdatei\n");
	printf("[2] Index j zu Thetavektor th (0)|Gesamt T\n");
	printf("-------------------------------------------\n");
	printf("Ip_PSG von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_PSG_\n");
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END                     \n%s\n", tbuffer ); 
	printf("+\n");
}

//-----------------------------------------------------------------------------| ENDE 



	
		
