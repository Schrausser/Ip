//-----------------------------------------------------------------------------| Ip_Kmn von Dietmar SCHRAUSSER 2009             
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
		K_AV_exp[30],//-------------------------------------------------------> Kombinationsdatenvektor kL
		SK_AV_exp[30];//------------------------------------------------------> Kombinationsdatenvektor n-kl
		
main(int argc, char *argv[]) //-----------------------------------------------> main übernimmt n argumente im vektorzeiger argv 
{
	FILE *inStream, *inprmStream, *outStream, *logStream, *batStream; //------> streams

	int jLauf, iLauf,  
		nA,//-----------------------------------------------------------------> Datenmatrix Zeilenvektoren n 
		kkA,//----------------------------------------------------------------> Datenmatrix Spaltenvektoren k
		kA=0,//---------------------------------------------------------------> Kombinationsvektorlänge Klasse kL 
		jp2,//----------------------------------------------------------------> Index j zu SK_AV_exp Subvektorelement 
		index[21],//----------------------------------------------------------> Schablonenindex Vektor
		sb_=0,//--------------------------------------------------------------> Schablonenverschubindex
		wtr=0,
		zlr=1;//--------------------------------------------------------------> Durchgangszähler 
	
	int ft_n=1, ft_k=1, ft_nk=1, K_;

	if (argc == 1 ) _profil();//----------------------------------------------> Ip_Kmn profil
	if (argc != 5) { printf("\n[ ] Argumente.0\n"); _profil();}//-------------> argumente 
	if (atoi(argv[2]) == 0 ){ printf("\n[2] Format.0\n");_profil();}//--------> format
	if (atoi(argv[3]) == 0 ){ printf("\n[3] Format.0\n");_profil();}//--------> format

	if (atoi(argv[4]) == 0 || atoi(argv[4]) > 2) {printf("\n[3] Format.0\n");_profil();}

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

	if ( atoi(argv[3]) >= nA ) //---------------------------------------------> fehlerroutine kL>=n 
	{
   			printf("\n[3] kL>=n .0\n");_profil();
	}

	if (nA > 20) {printf("\n[1] Rechenumfang.0\n");_profil();}//--------------> Rechenumfang

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

	kA = atoi(argv[3]);

	if (kA > nA-kA) kA=nA-kA;//-----------------------------------------------> kL<=n-kL
	
	for(iLauf=1; iLauf<=nA; iLauf++) index[iLauf] = 0;//----------------------> Indexinitialisierung (zu beachten: Vrn=1/Pmn,Kmn=0 )

	outStream = fopen( "Ip_Kmn.asc", "w" );

	//------------------------------------------------------------------------> Berechnung K=n!/(k!*(n-k)!)
	for (iLauf=1; iLauf<=nA; iLauf++)		ft_n *=iLauf;//-------------------> n!
	for (iLauf=1; iLauf<=kA; iLauf++)		ft_k *=iLauf;//-------------------> k!
	for (iLauf=1; iLauf<=nA-kA; iLauf++)	ft_nk *=iLauf;//------------------> (n-k)!

	K_= ft_n/(ft_k*ft_nk);

	
	_kopfzeile();

	//-------------------------------------------------------------------------| Vollständige Kombination K zur Klasse k (kL), ohne Wiederholung, K=n!/(k!*(n-k)!), 
	//                                                                           entsprechend der Permutation (P) bei np=2, P=n!/(p1!*p2!), wobei k=p1.
	do 
	{	
		//--------------------------------------------------------------------> Schablonenindexgenerierung
		spr:
		for (jLauf=1; jLauf<=nA-kA+1; jLauf++)
		{				
			if(jLauf == 1) index[1]++;
			if(index[jLauf] == nA-kA+2) 
			{ 
				index[jLauf]    = 1;
				index[jLauf+1] += 1;	
			}
			if( index[jLauf-1] < index[jLauf]) goto spr;//--------------------> Rangindikator
		}
		//--------------------------------------------------------------------> Schablonenverschub
		while( index[1]-1 + sb_ <= nA-kA )
		{
			//----------------------------------------------------------------> Datenzuordnung kL
			for (iLauf=1; iLauf<=kA; iLauf++)
			{
				if( index[iLauf]==0) index[iLauf]=1;
				K_AV_exp[iLauf] = AV_exp[(kA+(index[iLauf]-1+sb_))-iLauf+1];
				
				if(atoi(argv[3]) <= nA - atoi(argv[3]))
				{
					if (atoi(argv[4]) == 1) fprintf(outStream, "%.0f ", K_AV_exp[iLauf]);// Datenausgabe kL i
					if (atoi(argv[4]) == 2) fprintf(outStream, "%.3f ", K_AV_exp[iLauf]);// Datenausgabe kL f
				}
			}

			//----------------------------------------------------------------> Vektorspiegelung bei k>n-k, 
			if(atoi(argv[3]) > nA - atoi(argv[3]))
			{
				jp2=1;
				for (iLauf=1; iLauf<= nA; iLauf++)
				{
					for (jLauf=1; jLauf<= kA; jLauf++)
					{
						if(K_AV_exp[jLauf] == AV_exp[iLauf]) wtr = 1;//-----------> Vektorelementvergleich
					}
					if(wtr == 0) { SK_AV_exp[jp2] = AV_exp[iLauf]; jp2++; }

					wtr=0;
				}
				if (atoi(argv[4]) == 1)//-----------------------------------------> Datenausgabe kL (n-kL) i
				{
					for (iLauf=1; iLauf<= nA-kA; iLauf++) fprintf(outStream, "%.0f ", SK_AV_exp[iLauf]);
				}
				if (atoi(argv[4]) == 2)//-----------------------------------------> Datenausgabe kL (n-kL) f
				{
					for (iLauf=1; iLauf<= nA-kA; iLauf++) fprintf(outStream, "%.3f ", SK_AV_exp[iLauf]);
				}
			}
			
			fprintf (outStream,"\t%i\n",zlr); //------------------------------> Dateiausgabe Durchgangszähler
			printf ("%i\r",zlr); //-------------------------------------------> Bildschirmausgabe Durchgangszähler

			zlr++; sb_++;	
		}
		sb_ = 0;
		
	} while (index[kA-1] <= nA-kA);//do end 
	
	fclose( outStream  );
    
	//------------------------------------------------------------------------> log
	logStream = fopen( "Ip_Kmn_log.txt", "w" );
	
	_strdate( dbuffer ); _strtime( tbuffer );
	
	fprintf (logStream,"Ip_Kmn_ %s %s:\n--------------------------\n", dbuffer, tbuffer);

	fprintf (logStream, "K= n!/(k!(n-k)!), n= %i, k= %i, n-k= %i\n\n", nA, atoi(argv[3]), nA-atoi(argv[3]));
	fprintf (logStream, "K= %i", K_);

	fclose( logStream  );

	system("Ip_Kmn_log.txt");//-----------------------------------------------> editieren der logdatei
	system("Ip_Kmn.asc");//---------------------------------------------------> editieren der outputdatei

	batStream = fopen( "verschub.bat", "w" );

	logcode= time(0)-1234567890;
	
	fprintf (batStream, "@echo off\nmd Ip_Kmn_log_%i\n",logcode );
	fprintf (batStream, "move Ip_Kmn_log.txt Ip_Kmn_log_%i\n", logcode);
	fprintf (batStream, "move Ip_Kmn.asc Ip_Kmn_log_%i\n", logcode);

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
	printf("Ip_Kmn [1]_[2]_[3]_[4]\n");
	printf("-----------------------------------------\n");
	printf("[1] Dateiname der Datenmatrixdatei A(n,k)\n");
	printf("[2] Index j zu Datenvektor a\n");
	printf("[3] Umfang m zu Kombinationsvektor kL\n");
	printf("[4] Datenformat 1|10 2|10.000\n");
	printf("-----------------------------------------\n");
	printf("Ip_Kmn von Dietmar Schrausser\n");
    printf("<\\> %s @ %s\n", __DATE__, __TIME__);
	exit(EXIT_FAILURE);
}	

//-----------------------------------------------------------------------------| titelzeile bildschirmausgabe                   
void _kopfzeile() 
{	
	_strtime( tbuffer );
	printf("\n%s\n",tbuffer ); 
	printf("_Ip_Kmn_\n");	
}

//-----------------------------------------------------------------------------| fusszeile bildschirmausgabe  
void _fusszeile() 
{	
	_strtime( tbuffer );
	printf("END\n%s\n", tbuffer ); 
	printf("+\n");	
}

//-----------------------------------------------------------------------------| ENDE               
