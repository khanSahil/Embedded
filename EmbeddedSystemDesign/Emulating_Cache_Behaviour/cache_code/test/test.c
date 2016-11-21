#include <math.h>
#include "nrutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROTATE(a,i,j,k,l) g=a[i][j];\
						  h=a[k][l];\
						  a[i][j]=g-s*(h+g*tau);\
						  a[k][l]=h+s*(g-h*tau);

#define CACHE_ALIGN __declspec(align(64))	
#define TRUE 1
#define FALSE 0

// global variable 
unsigned int readMiss= 1;
unsigned int writeMiss = 1;

//degree of freedom
unsigned int mode;		// defualt mode
unsigned int L ;
unsigned int N ;		// N-way associativity
unsigned int B ;		// blocks Size in Bytes
						//   1 = 8 Bytes
						//   2 = 16 bytes
						//   4 = 32 bytes
						//   8 = 64 bytes

#define cacheSize 65536	// size of cache in KiloBytes
#define maxN  8		// maximum possible N-way associativity
#define maxL  8192
// maximum possible number of line in the Line array


// Write strategies 
unsigned int WB = 0;		// writeBack
unsigned int WTA = 1;		// writeThroughAllocate
unsigned int WTNA = 2;		// writeAllocate

// Cache parameters
int valid[maxL][maxN];				
int dirty[maxL][maxN];				
int tag[maxL][maxN];					
int lru[maxL][maxN];

// Counters
unsigned int writeMemCount				= 0;
unsigned int writeThroughMemCount		= 0;
unsigned int writeLineCount				= 0;
unsigned int writeLineHitCount			= 0;
unsigned int writeLineMissCount			= 0;
unsigned int writeLineReplaceCount		= 0;
unsigned int writeLineReplaceDirtyCount	= 0;
unsigned int writeMemoryFlushCount		= 0;

unsigned int readMemCount				= 0;
unsigned int readLineCount				= 0;
unsigned int readLineHitCount			= 0;
unsigned int readLineMissCount			= 0;
unsigned int readLineReplaceCount		= 0;
unsigned int readLineReplaceDirtyCount	= 0;


// Function declarations
void updateLRU(unsigned int,unsigned int);
void clearCache();
void flushCache();
void readMemory(void*,unsigned int);
void writeMemory(void*,unsigned int);
void readLine(unsigned int,int);
void writeLine(unsigned int,int);
void writeThroughMemory(unsigned int,unsigned int);
unsigned int getLine(unsigned int);
unsigned int getTag(unsigned int);
unsigned int getMsb(unsigned int);
void clearCounter();

// Function Definitions




void jacobi(double **a, int n, double d[], double **v, int *nrot)
{
	int j,iq,ip,i;
	double tresh,theta,tau,t,sm,s,h,g,c,*b,*z;

	readMemory(&n,sizeof(n));
	writeMemory(&b,sizeof(b));
	b=dvector(1,n);
	readMemory(&n,sizeof(n));
	writeMemory(&z,sizeof(z));
	z=dvector(1,n); 

	readMemory(&ip,sizeof(ip));
	readMemory(&n,sizeof(n));
	writeMemory(&ip,sizeof(ip));
	for (ip=1;ip<=n;ip++)	// Initialize to the identity matrix.
	{ 
		readMemory(&iq,sizeof(iq));
		readMemory(&n,sizeof(n));
		writeMemory(&iq,sizeof(iq));
		for(iq=1;iq<=n;iq++) 
		{
			readMemory(&ip,sizeof(ip));
			readMemory(&iq,sizeof(iq));
			readMemory(&v[ip],sizeof(v[ip]));
			writeMemory(&v[ip][iq],sizeof(v[ip][iq]));
			v[ip][iq]=0.0;

			readMemory(&iq,sizeof(iq));
			readMemory(&n,sizeof(n));
			writeMemory(&iq,sizeof(iq));
		}

		readMemory(&ip,sizeof(ip));
		readMemory(&v[ip],sizeof(v[ip]));
		writeMemory(&v[ip][ip],sizeof(v[ip][ip]));
		v[ip][ip]=1.0;

		readMemory(&iq,sizeof(ip));
		readMemory(&n,sizeof(n));
		writeMemory(&iq,sizeof(ip));
	}

	readMemory(&ip,sizeof(ip));
	readMemory(&n,sizeof(n));
	writeMemory(&ip,sizeof(ip));
	for (ip=1;ip<=n;ip++)	//Initialize b and d to the diagonal of a.
	{
		readMemory(&ip,sizeof(ip));
		readMemory(&a[ip],sizeof(a[ip]));
		readMemory(&a[ip][ip],sizeof(a[ip][ip]));
		writeMemory(&d[ip],sizeof(d[ip]));
		writeMemory(&b[ip],sizeof(b[ip]));
		b[ip]=d[ip]=a[ip][ip];		

		readMemory(&ip,sizeof(ip));
		writeMemory(&z[ip],sizeof(z[ip]));
		z[ip]=0.0;	// This vector will accumulate termsof the form tapq as in equation (11.1.14).

		readMemory(&ip,sizeof(ip));
		readMemory(&n,sizeof(n));
		writeMemory(&ip,sizeof(ip));
	}

	readMemory(&nrot,sizeof(nrot));			
	writeMemory(&nrot,sizeof(nrot));		
	*nrot=0;								

	readMemory(&i,sizeof(i));
	writeMemory(&i,sizeof(i));
	for (i=1;i<=50;i++) 
	{
		writeMemory(&sm,sizeof(sm));
		sm=0.0;

		readMemory(&ip,sizeof(ip));
		readMemory(&n,sizeof(n));
		writeMemory(&ip,sizeof(ip));
		for (ip=1;ip<=n-1;ip++) //Sum off-diagonal elements.
		{ 
			readMemory(&iq,sizeof(iq));
			readMemory(&ip,sizeof(ip));
			readMemory(&n,sizeof(n));
			writeMemory(&iq,sizeof(iq));
			for (iq=ip+1;iq<=n;iq++)
			{
				readMemory(&ip,sizeof(ip));
				readMemory(&iq,sizeof(iq));
				readMemory(&a[ip],sizeof(a[ip]));
				readMemory(&a[ip][iq],sizeof(a[ip][ip]));
				readMemory(&sm,sizeof(sm));
				writeMemory(&sm,sizeof(sm));
				sm += fabs(a[ip][iq]);

				readMemory(&iq,sizeof(ip));
				readMemory(&n,sizeof(n));
				writeMemory(&iq,sizeof(ip));
			}
			readMemory(&ip,sizeof(ip));
			readMemory(&n,sizeof(n));
			writeMemory(&ip,sizeof(ip));
		}

		readMemory(&sm,sizeof(sm));
		if (sm == 0.0) //The normal return, which relieson quadratic convergence to machine underflow.
		{ 
			readMemory(&z,sizeof(z));
			readMemory(&n,sizeof(n));			
			free_dvector(z,1,n);				
												
			readMemory(&b,sizeof(b));			
			readMemory(&n,sizeof(n));			
			free_dvector(b,1,n);
			return;
		}

		readMemory(&i,sizeof(i));
		if (i < 4)
		{
			readMemory(&n,sizeof(n));
			readMemory(&sm,sizeof(sm));
			writeMemory(&tresh,sizeof(tresh));
			tresh=0.2*sm/(n*n); //...on the first three sweeps.
		}
		else
		{
			writeMemory(&tresh,sizeof(tresh));
			tresh=0.0; //...thereafter.
		}

		readMemory(&ip,sizeof(ip));
		readMemory(&n,sizeof(n));
		writeMemory(&ip,sizeof(ip));
		for (ip=1;ip<=n-1;ip++)
		{
			readMemory(&iq,sizeof(iq));
			readMemory(&ip,sizeof(ip));
			readMemory(&n,sizeof(n));
			writeMemory(&iq,sizeof(iq));
			for (iq=ip+1;iq<=n;iq++)
			{
				readMemory(&ip,sizeof(ip));
				readMemory(&iq,sizeof(iq));
				readMemory(&a[ip],sizeof(a[ip]));
				readMemory(&a[ip][iq],sizeof(a[ip][iq]));
				writeMemory(&g,sizeof(g));
				g=100.0*fabs(a[ip][iq]); //After four sweeps, skip the rotation if the off-diagonal element is small.

				readMemory(&i,sizeof(i));
				readMemory(&ip,sizeof(ip));
				readMemory(&d[ip],sizeof(d[ip]));
				readMemory(&g,sizeof(g));
				readMemory(&iq,sizeof(iq));
				readMemory(&d[iq],sizeof(d[iq]));
				if (i > 4 && (double)(fabs(d[ip])+g) == (double)fabs(d[ip]) && (double)(fabs(d[iq])+g) == (double)fabs(d[iq]))
				{
					readMemory(&ip,sizeof(ip));
					readMemory(&iq,sizeof(iq));
					readMemory(&a[ip],sizeof(a[ip]));
					writeMemory(&a[ip][iq],sizeof(a[ip][iq]));
					a[ip][iq]=0.0;
				}
				else if (fabs(a[ip][iq]) > tresh)
				{
					readMemory(&ip,sizeof(ip));
					readMemory(&iq,sizeof(iq));
					readMemory(&a[ip],sizeof(a[ip]));
					readMemory(&a[ip][iq],sizeof(a[ip][iq]));
					readMemory(&tresh,sizeof(tresh));

					readMemory(&iq,sizeof(iq));
					readMemory(&ip,sizeof(ip));
					readMemory(&d[iq],sizeof(d[iq]));
					readMemory(&d[ip],sizeof(d[ip]));
					writeMemory(&h,sizeof(h));
					h=d[iq]-d[ip];

					readMemory(&h,sizeof(h));
					readMemory(&g,sizeof(g));
					if ((double)(fabs(h)+g) == (double)fabs(h))
					{
						readMemory(&ip,sizeof(ip));
						readMemory(&iq,sizeof(iq));
						readMemory(&a[ip],sizeof(a[ip]));
						readMemory(&a[ip][iq],sizeof(a[ip][iq]));
						readMemory(&h,sizeof(h));
						writeMemory(&t,sizeof(t));
						t=(a[ip][iq])/h;		//t = 1/(2?)
					}
					else
					{
						readMemory(&ip,sizeof(ip));
						readMemory(&iq,sizeof(iq));
						readMemory(&a[ip],sizeof(a[ip]));
						readMemory(&a[ip][iq],sizeof(a[ip][iq]));
						readMemory(&h,sizeof(h));
						writeMemory(&theta,sizeof(theta));
						theta=0.5*h/(a[ip][iq]);		//Equation (11.1.10).

						readMemory(&theta,sizeof(theta));
						writeMemory(&t,sizeof(t));
						t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));

						readMemory(&theta,sizeof(theta));
						if (theta < 0.0)
						{
							readMemory(&t,sizeof(t));
							writeMemory(&t,sizeof(t));
							t = -t;
						}
					}

					readMemory(&t,sizeof(t));
					writeMemory(&c,sizeof(c));
					c=1.0/sqrt(1+t*t);

					readMemory(&t,sizeof(t));
					readMemory(&c,sizeof(c));
					writeMemory(&s,sizeof(s));
					s=t*c;

					readMemory(&c,sizeof(c));
					readMemory(&c,sizeof(s));
					writeMemory(&tau,sizeof(tau));
					tau=s/(1.0+c);

					readMemory(&ip,sizeof(ip));
					readMemory(&iq,sizeof(iq));
					readMemory(&a[ip],sizeof(a[ip]));
					readMemory(&a[ip][iq],sizeof(a[ip][iq]));
					readMemory(&t,sizeof(t));
					writeMemory(&h,sizeof(h));
					h=t*a[ip][iq];

					readMemory(&h,sizeof(h));
					readMemory(&ip,sizeof(ip));
					readMemory(&z[ip],sizeof(z[ip]));
					writeMemory(&z[ip],sizeof(z[ip]));
					z[ip] -= h;

					readMemory(&h,sizeof(h));
					readMemory(&iq,sizeof(iq));
					readMemory(&z[iq],sizeof(z[iq]));
					writeMemory(&z[iq],sizeof(z[iq]));
					z[iq] += h;

					readMemory(&h,sizeof(h));
					readMemory(&ip,sizeof(ip));
					readMemory(&d[ip],sizeof(d[ip]));
					writeMemory(&d[ip],sizeof(d[ip]));
					d[ip] -= h;

					readMemory(&h,sizeof(h));
					readMemory(&iq,sizeof(iq));
					readMemory(&d[iq],sizeof(d[iq]));
					writeMemory(&d[iq],sizeof(d[iq]));
					d[iq] += h;

					readMemory(&ip,sizeof(ip));
					readMemory(&iq,sizeof(iq));
					readMemory(&a[ip],sizeof(a[ip]));
					writeMemory(&a[ip][iq],sizeof(a[ip][iq]));
					a[ip][iq]=0.0;

					readMemory(&j,sizeof(j));
					readMemory(&ip,sizeof(ip));
					writeMemory(&j,sizeof(j));
					for (j=1;j<=ip-1;j++)	//Case of rotations 1 ? j < p.
					{
						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(ip));
						readMemory(&a[j],sizeof(a[j]));
						readMemory(&a[j][ip],sizeof(a[j][ip]));
						writeMemory(&g,sizeof(g));
						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&a[j],sizeof(a[j]));
						readMemory(&a[j][iq],sizeof(a[j][iq]));
						writeMemory(&h,sizeof(h));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(ip));
						readMemory(&a[j],sizeof(a[j]));
						writeMemory(&a[j][ip],sizeof(a[j][ip]));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(iq));
						readMemory(&a[j],sizeof(a[j]));
						writeMemory(&a[j][iq],sizeof(a[j][iq]));	
						ROTATE(a,j,ip,j,iq)							

						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(ip));
						writeMemory(&j,sizeof(j));
					}

					readMemory(&j,sizeof(j));
					readMemory(&ip,sizeof(ip));
					readMemory(&iq,sizeof(iq));
					writeMemory(&j,sizeof(j));
					for (j=ip+1;j<=iq-1;j++)	//Case of rotations p < j < q.
					{ 
						readMemory(&ip,sizeof(ip));
						readMemory(&j,sizeof(j));
						readMemory(&a[ip],sizeof(a[ip]));
						readMemory(&a[ip][j],sizeof(a[ip][j]));
						writeMemory(&g,sizeof(g));
						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&a[j],sizeof(a[j]));
						readMemory(&a[j][iq],sizeof(a[j][iq]));
						writeMemory(&h,sizeof(h));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&ip,sizeof(ip));
						readMemory(&j,sizeof(j));
						readMemory(&a[ip],sizeof(a[ip]));
						writeMemory(&a[ip][j],sizeof(a[ip][j]));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&a[j],sizeof(a[j]));	
						writeMemory(&a[j][iq],sizeof(a[j][iq]));	
						ROTATE(a,ip,j,j,iq)

						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						writeMemory(&j,sizeof(j));
					}

					readMemory(&j,sizeof(j));
					readMemory(&iq,sizeof(iq));
					readMemory(&n,sizeof(n));
					writeMemory(&j,sizeof(j));
					for (j=iq+1;j<=n;j++)	//Case of rotations q < j ? n
					{
						readMemory(&ip,sizeof(ip));
						readMemory(&j,sizeof(j));
						readMemory(&a[ip],sizeof(a[ip]));
						readMemory(&a[ip][j],sizeof(a[ip][j]));
						writeMemory(&g,sizeof(g));
						readMemory(&iq,sizeof(iq));
						readMemory(&j,sizeof(j));
						readMemory(&a[iq],sizeof(a[iq]));
						readMemory(&a[iq][j],sizeof(a[iq][j]));
						writeMemory(&h,sizeof(h));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&ip,sizeof(ip));
						readMemory(&j,sizeof(j));
						readMemory(&a[ip],sizeof(a[ip]));
						writeMemory(&a[ip][j],sizeof(a[ip][j]));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&ip,sizeof(iq));
						readMemory(&j,sizeof(j));
						readMemory(&a[iq],sizeof(a[iq]));
						writeMemory(&a[iq][j],sizeof(a[iq][j]));	
						ROTATE(a,ip,j,iq,j)

						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&n,sizeof(n));
						writeMemory(&j,sizeof(j));
					}


					/*#define ROTATE(a,i,j,k,l)
												g=a[i][j];\
									v j ip j iq	h=a[k][l];\
												a[i][j]=g-s*(h+g*tau);\
												a[k][l]=h+s*(g-h*tau);
					*/

					readMemory(&j,sizeof(j));
					readMemory(&n,sizeof(n));
					writeMemory(&j,sizeof(j));
					for (j=1;j<=n;j++)
					{
						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(ip));
						readMemory(&v[j],sizeof(v[j]));
						readMemory(&v[j][ip],sizeof(v[j][ip]));
						writeMemory(&g,sizeof(g));
						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&v[j],sizeof(v[j]));
						readMemory(&v[j][iq],sizeof(v[j][iq]));
						writeMemory(&h,sizeof(h));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&j,sizeof(j));
						readMemory(&ip,sizeof(ip));
						readMemory(&v[j],sizeof(v[j]));
						writeMemory(&v[j][ip],sizeof(v[j][ip]));
						readMemory(&g,sizeof(g));
						readMemory(&s,sizeof(s));
						readMemory(&h,sizeof(h));
						readMemory(&tau,sizeof(tau));
						readMemory(&j,sizeof(j));
						readMemory(&iq,sizeof(iq));
						readMemory(&v[j],sizeof(v[j]));
						writeMemory(&v[j][iq],sizeof(v[j][iq]));
						ROTATE(v,j,ip,j,iq)

						readMemory(&j,sizeof(j));
						readMemory(&n,sizeof(n));
						writeMemory(&j,sizeof(j));
					}

					readMemory(&nrot,sizeof(nrot));		//----------------------------------------------//
					readMemory(&nrot,sizeof(*nrot));	// How many read and write for this operation...?
					writeMemory(&nrot,sizeof(nrot));	//----------------------------------------------//
					++(*nrot);
				}
				readMemory(&ip,sizeof(ip));		
				readMemory(&iq,sizeof(iq));	
				readMemory(&n,sizeof(n));
				writeMemory(&iq,sizeof(iq));
			}
			readMemory(&ip,sizeof(ip));
			readMemory(&n,sizeof(n));
			writeMemory(&ip,sizeof(ip));
		}

		readMemory(&ip,sizeof(ip));
		readMemory(&n,sizeof(n));
		writeMemory(&ip,sizeof(ip));
		for (ip=1;ip<=n;ip++)
		{
			readMemory(&ip,sizeof(ip));
			readMemory(&z[ip],sizeof(z[ip]));
			readMemory(&b[ip],sizeof(b[ip]));
			writeMemory(&b[ip],sizeof(b[ip]));
			b[ip] += z[ip];

			readMemory(&ip,sizeof(ip));
			readMemory(&b[ip],sizeof(b[ip]));
			writeMemory(&d[ip],sizeof(d[ip]));
			d[ip]=b[ip]; //Update d with the sum of tapq,

			readMemory(&ip,sizeof(ip));
			writeMemory(&z[ip],sizeof(z[ip]));
			z[ip]=0.0; //and reinitialize z.

			readMemory(&ip,sizeof(ip));
			readMemory(&n,sizeof(n));
			writeMemory(&ip,sizeof(ip));
		}

		readMemory(&i,sizeof(i));
		writeMemory(&i,sizeof(i));
	}
}

void clearCounter()
{
	writeMemCount				= 0;
	writeThroughMemCount		= 0;
	writeLineCount				= 0;
	writeLineHitCount			= 0;
	writeLineMissCount			= 0;
	writeLineReplaceCount		= 0;
	writeLineReplaceDirtyCount	= 0;
	writeMemoryFlushCount		= 0;

	readMemCount				= 0;
	readLineCount				= 0;
	readLineHitCount			= 0;
	readLineMissCount			= 0;
	readLineReplaceCount		= 0;
	readLineReplaceDirtyCount	= 0;
}

void updateLRU(unsigned int lineNum,unsigned int block)
{
	int i;
	if((!readMiss)||(!writeMiss))	// readMiss = 0 or writeMiss = 0  means refreshing
	{
		for(i=0;i<N;i++)
		{
			if(lru[lineNum][i]>lru[lineNum][block])
				lru[lineNum][i] -=1;
		}
	}	
	else if(readMiss||writeMiss)		// readMiss = 1 or writeMiss = 1  means replacement
	{
		for(i=0;i<N;i++)
		{
			if(i==block)
				continue;
			else
				lru[lineNum][i] -=1;
		}
	}
}

void clearCache()		// resets everything back to original
{
	int i,j;
	for(i=0;i<maxL;i++)
	{
		for(j=0;j<maxN;j++)
			valid[i][j] = 0;
	}
	for(i=0;i<maxL;i++)
	{
		for(j=0;j<maxN;j++)
			dirty[i][j] = 0;
	}
	for(i=0;i<maxL;i++)
	{
		for(j=0;j<maxN;j++)
			lru[i][j] = j;
	}
	for(i=0;i<maxL;i++)
	{
		for(j=0;j<maxN;j++)
			tag[i][j] = 0;
	}
}
void flushCache()
{
	int i,j;
	for(i=0;i<maxL;i++)
	{
		for(j=0;j<maxN;j++)
		{
			if((dirty[i][j] == 1) && (mode == WB))
			{
				writeMemoryFlushCount++;
				dirty[i][j] = 0;
			}
		}
	}
}

void writeThroughMemory(unsigned int lineNum,unsigned int tagNum)
{
	unsigned int i,j;
	for(i=0;i<N;i++)							// finding the block to write data
		if(valid[lineNum][i] == TRUE)
			if(tag[lineNum][i] == tagNum)
			{
				writeMiss = 0;			// do for valid and dirty and tag
				valid[lineNum][i] = 1;
				tag[lineNum][i] = tagNum;
				if(mode==WTA || mode == WTNA)
					dirty[lineNum][i] = 0;
				updateLRU(lineNum,i);
				break;
			}

	if(!writeMiss)		// bingo we got the entry
		writeLineHitCount++;
	else
		writeLineMissCount++;

	if(writeMiss && (mode == WTA))	 // remove WB and WTA from condition	
	{
		for(i=0;i<N;i++)
			if(lru[lineNum][i] == 0)
				break;

		if(valid[lineNum][i] == 1 && dirty[lineNum][i]==0)
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			if(mode ==WTA)
				dirty[lineNum][i] = 0;
			updateLRU(lineNum,i);
			writeLineReplaceCount++;
		}
		else if(valid[lineNum][i] == 0 && dirty[lineNum][i] == 0)		
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			if(mode ==WTA)
				dirty[lineNum][i] = 0;
			updateLRU(lineNum,i);
		}
	}
	writeMiss = 1;
}
void readLine(unsigned int lineNum,int tagNum)
{
	unsigned int i,j;
	for(i=0;i<N;i++)							// finding the block to replace
	{
		if(valid[lineNum][i] == TRUE)
		{
			if(tag[lineNum][i] == tagNum)
			{
				readMiss = 0;
				updateLRU(lineNum,i);
				break;
			}
		}
	}
	if(!readMiss)		// bingo we got the entry
		readLineHitCount++;
	else
		readLineMissCount++;

	if(readMiss)		
	{
		for(i=0;i<N;i++)
		{
			if(lru[lineNum][i] == 0)
				break;
		}

		if(valid[lineNum][i] == 1 && dirty[lineNum][i]==0)
		{
			lru[lineNum][i] = N-1;			// replacing and assigning the N-1 value
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			updateLRU(lineNum,i);
			dirty[lineNum][i] = 0;
			readLineReplaceCount++;
		}
		else if(valid[lineNum][i] == 1 && dirty[lineNum][i] == 1)
		{
			lru[lineNum][i] = N-1;			// replacing and assigning the N-1 value
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			updateLRU(lineNum,i);
			dirty[lineNum][i] = 0;
			readLineReplaceCount++;
			readLineReplaceDirtyCount++;
		}
		
		else if(valid[lineNum][i] == 0 && dirty[lineNum][i] == 0)		
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			dirty[lineNum][i] = 0;
			updateLRU(lineNum,i);
		}
	}
	readMiss = 1;
}

void writeLine(unsigned int lineNum,int tagNum)
{
	unsigned int i,j;
	for(i=0;i<N;i++)							// finding the block to write data
		if(valid[lineNum][i] == TRUE)
			if(tag[lineNum][i] == tagNum)
			{
				writeMiss = 0;			// do for valid and dirty and tag
				valid[lineNum][i] = 1;
				tag[lineNum][i] = tagNum;
				dirty[lineNum][i] = 1;
				updateLRU(lineNum,i);
				break;
			}

	if(!writeMiss)		// bingo we got the entry
		writeLineHitCount++;
	else
		writeLineMissCount++;

	if(writeMiss)	 // remove WB and WTA from condition	
	{
		for(i=0;i<N;i++)
			if(lru[lineNum][i] == 0)
				break;

		if(valid[lineNum][i] == 1 && dirty[lineNum][i] == 1)	
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			dirty[lineNum][i] = 1;
			updateLRU(lineNum,i);
			writeLineReplaceCount++;
			writeLineReplaceDirtyCount++;
		}
		else if(valid[lineNum][i] == 1 && dirty[lineNum][i]==0)
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			dirty[lineNum][i] = 1;
			updateLRU(lineNum,i);
			writeLineReplaceCount++;
		}
		else if(valid[lineNum][i] == 0 && dirty[lineNum][i] == 0)		
		{
			lru[lineNum][i] = N-1;			
			valid[lineNum][i] = 1;
			tag[lineNum][i] = tagNum;
			dirty[lineNum][i] = 1;
			updateLRU(lineNum,i);
		}
	}
	writeMiss = 1;
}

unsigned int getLine(unsigned int add)
{
	unsigned int blockBits,setBits,numOfBlock,numSets,tagBits,lineNum,cacheBlock;

	switch(B){
		case 1: blockBits = 3;cacheBlock = 8*1024 ;break;
		case 2: blockBits = 4;cacheBlock = 16*1024;break;
		case 4: blockBits = 5;cacheBlock = 32*1024;break;
		case 8: blockBits = 6;cacheBlock = 64*1024;break;
	}

	switch(N){
		case 1: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 2: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 4: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 8: numOfBlock = (cacheSize/cacheBlock)*1024; break;
	}

	numSets = numOfBlock/N;
	setBits = log((float)numSets)/log(2.0); 
	tagBits = 32 - setBits - blockBits;
	lineNum = add<<tagBits;
	lineNum = lineNum>>(tagBits+blockBits);
	return lineNum;
}
unsigned int getTag(unsigned int add)
{
	unsigned int blockBits,setBits,numOfBlock,numSets,tagBits,tagNum,cacheBlock;

	switch(B){
		case 1: blockBits = 3;cacheBlock = 8*1024 ;break;
		case 2: blockBits = 4;cacheBlock = 16*1024;break;
		case 4: blockBits = 5;cacheBlock = 32*1024;break;
		case 8: blockBits = 6;cacheBlock = 64*1024;break;
	}

	switch(N){
		case 1: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 2: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 4: numOfBlock = (cacheSize/cacheBlock)*1024; break;
		case 8: numOfBlock = (cacheSize/cacheBlock)*1024; break;
	}

	numSets = numOfBlock/N;
	setBits = log((float)numSets)/log(2.0); 
	tagBits = 32 - setBits - blockBits;
	tagNum  = add>>(setBits + blockBits);
	return tagNum;
}

unsigned getMsb(unsigned int add)
{
	return(add/8);
}

void readMemory(void* add,unsigned int size)
{
	unsigned int add32 = (unsigned int)add;
	int oldLine = -1;
	unsigned int i,tag,line;
	readMemCount++;
	for(i=0;i<size;i++)
	{
		line = getLine(add32);
		tag  = getTag(add32);

		if(oldLine != line)
		{
			readLine(line,tag);
			readLineCount++;
			oldLine = line;
		}
	}
}

void writeMemory(void* add,unsigned int size)
{
	unsigned int i,tag,line,msb;
	unsigned int add32 = (unsigned int)add;
	writeMemCount++;
	if(mode == WB)
	{
		int oldLine = -1;
		for(i=0;i<size;i++)
		{
			line = getLine(add32);
			tag  = getTag(add32);
			if(oldLine != line)
			{
				writeLine(line,tag);
				writeLineCount++;
				oldLine = line;
			}
		}
	}
	if(mode == WTA || mode == WTNA )
	{
		int oldMsb = -1;
		for(i=0;i<size;i++)
		{
			msb  = getMsb(add32 + i);
			line = getLine(add32);
			tag  = getTag(add32);
			if(oldMsb != msb)
			{
				writeThroughMemory(line, tag);
				writeThroughMemCount++;
				oldMsb = msb;
			}
		}
	}
}

int main()
{
	double  CACHE_ALIGN **eigenMatrix = dmatrix(1,128,1,128);
	double  CACHE_ALIGN *diagonal = dvector(1,128);
	double **inputMatrix;	
	double  CACHE_ALIGN Matrix[16384] = {0};
	int  j,dimension = 128,i;
	clock_t start;
    clock_t finish;
	double interval;
	FILE *pFile;
	pFile = fopen("Output.txt","w");
	if(pFile == NULL)
	{
		fprintf(stderr, "can't open Output.txt File!\n");
		exit(1);
	}
	
	inputMatrix = convert_matrix(&Matrix, 1, dimension, 1, dimension);
	start = clock();
	for(i=1;i<=dimension;i++)
	{
		for(j=1;j<=dimension;j++)
			if(i==j)
				inputMatrix[i][j] = 1;
	}
	start = clock();
	printf("Running jacobi\n");
	fprintf(pFile,"N	B	mode	wMC	wLC	wTMC	wLHC	wLMC	wLRC	wLRDC	wMFC	rMC	rLC	rLHC	rLMC	rLRC	rLRDC\n");
	for(N=1;N<=8;N=N*2)
	{
		L = maxL/N;
		
		for(B=1;B<=8;B=B*2)
		{
			for(mode=0;mode<=2;mode++)
			{
				int jacobiRotation=0;
				clearCache();
				jacobi(inputMatrix,dimension,diagonal,eigenMatrix,&jacobiRotation);
				flushCache();
				fprintf(pFile,"%d	%d	%d	",N,B,mode);
				fprintf(pFile,"%d	%d	%d	%d	%d	%d	%d	%d	",writeMemCount,writeLineCount,writeThroughMemCount,writeLineHitCount,writeLineMissCount,writeLineReplaceCount,writeLineReplaceDirtyCount,writeMemoryFlushCount);
				fprintf(pFile,"%d	%d	%d	%d	%d	%d\n",readMemCount,readLineCount,readLineHitCount,readLineMissCount,readLineReplaceCount,readLineReplaceDirtyCount);
				clearCounter();
				memset(Matrix,0, 16384 );			// ReInitializing the Memory in the Previous State i.e Identity matrix
				for(i=1;i<=dimension;i++)
				{
					for(j=1;j<=dimension;j++)
						if(i==j)
							inputMatrix[i][j] = 1;
				}
			}
		}
	}
	printf("jacobi Ends\n");
	finish = clock() - start;
	interval = finish / CLOCKS_PER_SEC; 
	interval = interval/60;
	fprintf(pFile,"Time took in Minutes : %lf\n",interval);
	printf("Time took in Minutes : %lf\n",interval);
	fclose(pFile);
	printf("\n\n");
	getchar();
	return 0;
}

