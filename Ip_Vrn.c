//-----------------------------------------------------------------------------| Ip_Vrn von Dietmar SCHRAUSSER 2009             
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <math.h>

void _profil(); //------------------------------------------------------------> prozedur profil   
void _kopfzeile(); //---------------------------------------------------------> prozedur für kopfzeilen 
void _fusszeile(); //---------------------------------------------------------> prozedur für fusszeilen 

char dbuffer [9],tbuffer [9], datenprm_dateiname[50]; //----------------------> buffer fuer time und date
int  logcode, zeichen_bis_punkt;

float s_ij;

double 	AV_exp[40],//---------------------------------------------------------> Urdatenvektor
		V_AV_exp[30];//-------------------------------------------------------> Variationssdatenvektor v
		
main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE *inStream, *inprmStream, *outStream, *logStream, *batStream; //------> streams

	int jLauf, iLauf,  
		nA,//-----------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kkA,//----------------------------------------------------------------> Datenmatrix Spaltenvektoren k
		index[21],//----------------------------------------------------------> Schablonenindex Vektor
		zlr=1;//--------------------------------------------------------------> Durchgangszähler 
	
	double V_;

	if (argc == 1 ) _profil();//----------------------------------------------> Ip_Vrn profil
	if (argc != 4) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (atoi(argv[2]) == 0 ){ printf("\n[2] Format.0\n");_profil();}//--------> format					 
	if (atoi(argv[3]) == 0 || atoi(argv[3]) > 2) {printf("\n[3] Format.0\n");_profil();}

	if (fopen (argv[1], "r") == NULL  ) //------------------------------------> fehlerroutine inputdatei
	{
   		inStream = fopen( argv[1], "w" );
		
		fprintf( inStream,"1\n"); 
		fprintf( inStream,"2\n"); 
		fprintf( inStream,"3\n"); 

		fclose( inStream  );
		system (argv[1]);
		printf("%s\n", argv[1] );
		
	}

	//-------------------------------------------------------------------------| generierung des daten param dateinamens (datendateiname +_prm.txt)
	zeichen_bis_punkt = strcspn(argv[1],"."); //------------------------------> Bestimmt die Anzahl der Zeichen bis zu "."
	strncpy (datenprm_dateiname, argv[1], zeichen_bis_punkt); //--------------> Kopiert die Zeichen bis zu "." in dateiname.
	strcat(datenprm_dateiname, "_prm.txt");//---------------------------------> anfügen von _prm.txt an dateiname.

	if ( fopen (datenprm_dateiname, "r") == NULL  ) //------------------------> fehlerroutine datenparameterdatei 
	{
   		inprmStream = fopen( datenprm_dateiname, "w" );

		fprintf( inprmStream,"3\n"); 
		fprintf( inprmStream,"1\n");

		fclose( inprmStream  );
		system (datenprm_dateiname);
		printf("%s\n", datenprm_dateiname);
	}

	//-------------------------------------------------------------------------| einlesen der datenmatrix parameter n, k 
	inprmStream = fopen( datenprm_dateiname, "r" );
	fscanf(inprmStream,"%i\n%i", &nA, &kkA ); 
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

			if (jLauf == atoi(argv[2]))	AV_exp[iLauf+1] = s_ij;//-------------> Urdatenvektor Initialisierung	
		}
	}

	fclose( inStream  );
	
	for(iLauf=1; iLauf<=nA; iLauf++) index[iLauf] = 1;//-------------------> Indexinitialisierung

	outStream = fopen( "Ip_Vrn.asc", "w" );

	//------------------------------------------------------------------------> Berechnung V=n^n
	V_= pow(nA,nA);

	_kopfzeile();

	//-------------------------------------------------------------------------| vollständige Variation V=n^n 
	while (zlr <= pow(nA,nA))
	{
		for(iLauf=1; iLauf<=nA; iLauf++)
		{
			V_AV_exp[iLauf] = AV_exp[index[iLauf]];
			if(nA < 7)//------------------------------------------------------> V<100000
			{
				if (atoi(argv[3]) == 1) fprintf(outStream, "%.0f ", V_AV_exp[iLauf]);// Datenausgabe v i
				if (atoi(argv[3]) == 2) fprintf(outStream, "%.3f ", V_AV_exp[iLauf]);// Datenausgabe v f
			}
			printf("%i", index[iLauf]);

		}
		
		for (jLauf=1; jLauf<=nA; jLauf++)
		{				
			if(jLauf == 1) index[1]++;
		
			if(index[jLauf] == nA+1) 
			{ 
				index[jLauf] = 1;
				index[jLauf+1] += 1;	
			}
		}
		if(nA < 7) fprintf(outStream,"\t%i\n", zlr);
		printf ("\r");
		zlr++;
	}

	if (nA >= 7) fprintf(outStream, "V>100000");//----------------------------> V>100000

	fclose( outStream  );
    
	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_Vrn_log.txt", "w" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_Vrn_ %s %s:\n--------------------------\n", dbuffer, tbuffer);
	fprintf (logStream, "V= n^n, n= %i, V= %.0f", nA, V_);

	fclose( logStream  );

	system("Ip_Vrn_log.txt");//-----------------------------------------------> editieren der logdatei
	if(nA<5) system("Ip_Vrn.asc");//------------------------------------------> editieren der outputdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_Vrn_log_%i\n",logcode );
	fprintf (batStream, "move Ip_Vrn_log.txt Ip_Vrn_log_%i\n", logcode);
	fprintf (batStream, "move Ip_Vrn.asc Ip_Vrn_log_%i\n", logcode);

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
	printf("Ip_Vrn [1]_[2]_[3]\n");
	printf("-----------------------------------------\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a\n");
	printf("[3] Datenformat 1|10 2|10.000\n");
	printf("-----------------------------------------\n");
	printf("Ip_Vrn von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_Vrn_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
