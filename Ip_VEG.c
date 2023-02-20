//-----------------------------------------------------------------------------| Ip_VEG von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include <conio.h>

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

char dbuffer [9],tbuffer [9], datenprm_dateiname[50]; //----------------------> buffer fuer time und date
int  logcode, zeichen_bis_punkt, iLauf;
double AV_eg=0, AV_eg_1[15005]; 
double Nil, S_eg[30010], S_eg_1[30010]; 
char Sinv_eg[30010][20], tmp[20];
				
main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE *inStream, *inprmStream, *outStream, *logStream, *batStream; //------> streams

	int jLauf, iLauf, kLauf, lLauf,  
		nA,//-----------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kA,//-----------------------------------------------------------------> Datenmatrix Spaltenvektoren k
		k_b_zlr, n_zlr, hub;
	double k_breite, k_b_zlr_f, n_zlr_f; 
	
	if (argc == 1 ) _profil();//----------------------------------------------> Ip_Vrn profil
	if (argc != 7) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (atoi(argv[4]) < 4 ){ printf("\n[4] n'<4 .0\n");_profil();}//----------> format
	if (atoi(argv[5]) > 1 ){ printf("\n[5] Format.0\n");_profil();}//---------> format	
	if (atoi(argv[6]) > 1 ){ printf("\n[6] Format.0\n");_profil();}//---------> format
	//if (atoi(argv[3]) == 0 || atoi(argv[3]) > 2) {printf("\n[3] Format.0\n");_profil();}
	if ((atof(argv[4]))/2 != floor(atof(argv[4])/2)) 
	{ printf("\n[4] 2\xC5n'.0\n");_profil();}//-------------------------------> einpassungsumfang n' nicht geradzahlig
	
	if (fopen (argv[1], "r") == NULL  ) //------------------------------------> fehlerroutine inputdatei
	{	
		printf("\n[1] Datenmatrixdatei.0\n");_profil();	
	}

	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.

	if ( fopen (datenprm_dateiname, "r") == NULL  ) //------------------------> fehlerroutine datenparameterdatei 
	{
		printf("\n[1] Datenmatrix-Parameterdatei.0\n");_profil();
	}

	//-------------------------------------------------------------------------| einlesen der datenmatrix parameter n, k 
	inprmStream = fopen( datenprm_dateiname, "r" );
	fscanf(inprmStream,"%i\n%i", &nA, &kA ); 
	fclose( inprmStream  );	

	if ( atoi(argv[2]) > kA ) {printf("\n[2] k<j .0\n");_profil();}//--------> fehlerroutine j>k 
	if ( atoi(argv[4]) > nA ) {printf("\n[4] n<n'.0\n");_profil();}//--------> fehlerroutine n<n'
	//if ( atoi(argv[4]) < nA && atoi(argv[4]) > nA/2 ) {printf("\n[4] n'>n/2 .0\n");_profil();}//fehlerroutine n<n'>n/2
	
	
	k_breite = nA/atof(argv[4]);//klassenbreite

	k_b_zlr =0; k_b_zlr_f=0; n_zlr=1; n_zlr_f=0;
	
	_kopfzeile();

	//-------------------------------------------------------------------------| Vektoreinpassung
	//                                                                          einlesen und von aij nach AV_exp 
	inStream = fopen( argv[1], "r" );//---------------------------------------> definition von datenmatrixdatei A(n,k)
	outStream = fopen( argv[3], "a" );
	
	for (iLauf = 1; iLauf <= nA; iLauf++)
	{
		for (jLauf = 1; jLauf <= kA; jLauf++)
		{	
			if (jLauf != atoi(argv[2]))	fscanf(inStream,"%lf", &Nil);
			if (jLauf == atoi(argv[2]))	fscanf(inStream,"%lf", &S_eg[iLauf] );// daten einlesen
		
			
			if (jLauf == atoi(argv[2]) && iLauf == nA && atoi(argv[4]) == nA )//n'= n
			{	
				if(atoi(argv[5])==1 )//---------------------------------------> Format sortiert 
				{	
					S_eg_1[1] = 9999999; //-----------------------------------> maxwert f sortierung wird S_eg_1(n+1)
					hub = 0;
					for (kLauf = 1; kLauf <= nA; kLauf++)//-------------------> sortierung aufsteigend
					{
						for (lLauf = 0; lLauf < kLauf; lLauf++)
						{
							if (S_eg[kLauf] < S_eg_1[kLauf-lLauf])
							{
								S_eg_1[(kLauf-lLauf)+1] = S_eg_1[kLauf-lLauf]; hub++;	
							}	
						}	
						S_eg_1[(kLauf+1)-hub] = S_eg[kLauf]; hub=0;	
					}
					for (kLauf = 1; kLauf <= atoi(argv[4]) ; kLauf++) fprintf(outStream,"%.3f\n", S_eg_1[kLauf] );//dateiausgabe
				}	
				if(atoi(argv[5])==0 )for (kLauf = 1; kLauf <= atoi(argv[4]) ; kLauf++) fprintf(outStream,"%.3f\n", S_eg[kLauf] );//dateiausgabe	
			}
			
			
			
			if (jLauf == atoi(argv[2]) && atoi(argv[4]) < nA )//--------------> n'< n
			{	
				if(atoi(argv[5])==1 && iLauf == nA)//-------------------------> Format sortiert dateisplittung
				{	
					S_eg_1[1] = 9999999; //-----------------------------------> maxwert f sortierung wird S_eg_1(n+1)
					hub = 0;
					for (kLauf = 1; kLauf <= nA; kLauf++)//-------------------> sortierung aufsteigend
					{
						for (lLauf = 0; lLauf < kLauf; lLauf++)
						{
							if (S_eg[kLauf] < S_eg_1[kLauf-lLauf])
							{
								S_eg_1[(kLauf-lLauf)+1] = S_eg_1[kLauf-lLauf]; hub++;	
							}	
						}	
						S_eg_1[(kLauf+1)-hub] = S_eg[kLauf]; hub=0;	
					}
				
					fprintf(outStream,"%.3f\n", S_eg_1[1]);//-----------------> dateiausgabe minimalwert
					//--------------------------------------------------------> 1te hälfte einpassen
					if(floor(k_breite) > 1)for (kLauf = 1; kLauf <= nA/2; kLauf++)//  n'<= n/2   //
					{

						if(n_zlr < atoi(argv[4])/2)//-------------------------> vor letzter n' kategorie
						{	
							if(k_b_zlr < floor(k_breite)) {AV_eg += S_eg_1[kLauf]; k_b_zlr++;}
				
							if(k_b_zlr == floor(k_breite)  )
							{
								AV_eg = AV_eg/k_b_zlr; //---------------------> mittelung
								if(n_zlr > 1) fprintf(outStream,"%.3f\n", AV_eg );//dateiausgabe
								AV_eg = 0; k_b_zlr=0; n_zlr++;
							}
						}
						if(n_zlr == atoi(argv[4])/2)//------------------------> letzte n' kategorie
						{
							AV_eg += S_eg_1[kLauf]; k_b_zlr++;
						
							if(kLauf == nA/2  )
							{
								AV_eg = AV_eg/k_b_zlr; //---------------------> mittelung
								fprintf(outStream,"%.3f\n", AV_eg );//--------> dateiausgabe
							}
						}
					}	
					
					if(floor(k_breite) <= 1)for (kLauf = 1; kLauf <= atoi(argv[4])/2; kLauf++)//  n'> n/2   //
					{
						n_zlr_f = n_zlr_f + k_breite;
						sprintf(tmp,"%.0f", n_zlr_f); n_zlr = atoi(tmp);
						
						fprintf(outStream,"%.3f\n", S_eg_1[n_zlr] );//--------> dateiausgabe
								
					}

					

					//--------------------------------------------------------> 2te hälfte einpassen	
					k_b_zlr=0;	k_b_zlr_f=0; n_zlr=1; AV_eg=0; n_zlr_f=(k_breite*(atof(argv[4])));
						
					if(floor(k_breite) > 1)for (kLauf = nA; kLauf >= nA/2; kLauf--)//  n'<= n/2   //
					{
						if(n_zlr < atoi(argv[4])/2)//-------------------------> vor letzter n' kategorie
						{	
							if(k_b_zlr < floor(k_breite)) {AV_eg += S_eg_1[kLauf]; k_b_zlr++;}
				
							if(k_b_zlr == floor(k_breite)  )
							{
								AV_eg = AV_eg/k_b_zlr; //---------------------> mittelung
								AV_eg_1[n_zlr] =  AV_eg;//--------------------> Vektor eingabe
								AV_eg = 0; k_b_zlr=0; n_zlr++;
							}
						}
						if(n_zlr == atoi(argv[4])/2)//------------------------> letzte n' kategorie
						{
							AV_eg += S_eg_1[kLauf]; k_b_zlr++;
						
							if(kLauf == nA/2  )
							{
								AV_eg = AV_eg/k_b_zlr; //---------------------> mittelung
								AV_eg_1[n_zlr] =  AV_eg;//--------------------> Vektor eingabe
							}
						}
					}

					if(floor(k_breite) <= 1)//-------------------------------->  n'> n/2   //
					{
						for (kLauf = 2; kLauf <= (atoi(argv[4])/2)-1; kLauf++)
						{
							n_zlr_f = n_zlr_f - k_breite;
							sprintf(tmp,"%.0f", n_zlr_f); n_zlr = atoi(tmp);
							
							AV_eg_1[kLauf] = S_eg_1[n_zlr] ;//----------------->vektor eingabe
						}
						n_zlr=kLauf-1;
					}
					
					for (kLauf = n_zlr; kLauf >= 2 ; kLauf--) fprintf(outStream,"%.3f\n", AV_eg_1[kLauf] );//dateiausgabe

					fprintf(outStream,"%.3f\n", S_eg_1[nA]);//----------------> dateiausgabe maximalwert
				}
	
				if(atoi(argv[5])==0)//----------------------------------------> Format roh kategorisierung in klassenbreite
				{
					
					if(n_zlr < atoi(argv[4]))//-------------------------------> vor letzter n' kategorie
					{	
						if(k_b_zlr < k_breite) {AV_eg += S_eg[iLauf]; k_b_zlr++;}
				
						if(k_b_zlr == k_breite  )
						{
							AV_eg = AV_eg/k_b_zlr; //-------------------------> mittelung
							fprintf(outStream,"%.3f\n", AV_eg );//------------> dateiausgabe
							AV_eg = 0; k_b_zlr=0; n_zlr++;
						}
					}
					if(n_zlr == atoi(argv[4]))//------------------------------> letzte n' kategorie
					{
						AV_eg += S_eg[iLauf]; k_b_zlr++;
						
						if(iLauf == nA  )
						{
							AV_eg = AV_eg/k_b_zlr; //-------------------------> mittelung
							fprintf(outStream,"%.3f\n", AV_eg );//------------> dateiausgabe
						}
					}
				}
			}
		}
	}

	fclose( inStream  );
	fclose( outStream  );
    
	if(atoi(argv[6])==1)//----------------------------------------------------> invertierung der reihenfolge
	{
		inStream = fopen( argv[3], "r" );
		for (iLauf = 1; iLauf <= atoi(argv[4]); iLauf++)
		{	
			fscanf(inStream,"%s", Sinv_eg[iLauf] );//-------------------------> daten einlesen
		}
		fclose( inStream  );
		
		outStream = fopen( argv[3], "w" );
		
		for (iLauf =  atoi(argv[4]); iLauf >=1; iLauf--)
		{	
			fprintf(outStream,"%s\n", Sinv_eg[iLauf] );//---------------------> invertierte dateiausgabe
		}
		fclose( outStream  );
	}
	
	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_VEG_log.txt", "w" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_VEG_ %s %s:\n--------------------------\n", dbuffer, tbuffer);
	if(atoi(argv[5])==1)fprintf (logStream,"n=%i, n'=%i, ranggereiht", nA, atoi(argv[4]));
	if(atoi(argv[5])==0)fprintf (logStream,"n=%i, n'=%i, ungereiht", nA, atoi(argv[4]));
	if(atoi(argv[6])==1)fprintf (logStream,", invertiert");


	fclose( logStream  );

	//system("Ip_VEG_log.txt");//---------------------------------------------> editieren der logdatei
	
	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_VEG_log_%i\n",logcode );
	fprintf (batStream, "move Ip_VEG_log.txt Ip_VEG_log_%i\n", logcode);
	fprintf (batStream, "move Ip_VEG.asc Ip_VEG_log_%i\n", logcode);

	fclose( batStream  );
	
	system ("verschub.bat");
	system ("del verschub.bat");

	printf("%i      \n",logcode);
	
	_fusszeile();

	return 0;
}

//-----------------------------------------------------------------------------| allgemeine fehlerroutine, profil                       
void _profil()
{
	printf("\n");
	printf("Ip_VEG [1]_[2]_[3]_[4]_[5]_[6]\n");
	for(iLauf=1;iLauf<=41;iLauf++) cprintf("\xC4"); printf("\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a\n");
	printf("[3] Dateiname der Datenvektordatei S(n')\n");
	printf("[4] Einpassungsumfang 4 <= n'<= n/2\n");
	printf("[5] Format (0)|Roh (1)|Rang\n");
	printf("[6] Reihenfolge (0)|urspr. (1)|invers\n");
	for(iLauf=1;iLauf<=41;iLauf++) cprintf("\xC4"); printf("\n");
	printf("Ip_VEG von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(0);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_VEG_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
