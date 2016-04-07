#include "vzfile.h"
#include <stdio.h>
#include <string.h>

CVzFile::CVzFile(void)
{
}

CVzFile::~CVzFile(void)
{
}

bool CVzFile::WriteFile(const char *filepath, unsigned char *pData, unsigned int len)
{
	int num = 0;

	FILE *fp = NULL;
	fp = fopen(filepath, "wb+");

	if( fp == NULL )
	{
		return false;
	}

	num = fwrite( pData, sizeof(char), len, fp );
	fclose( fp );

	return num > 0;
}

bool CVzFile::WriteTxtFile(const char *filepath, const char* pText)
{
	int num = 0;

	FILE *fp = NULL;
	fp = fopen(filepath, "w+");

	if( fp == NULL )
	{
		return false;
	}

	num = fwrite( pText, sizeof(char), strlen(pText), fp );
	fclose( fp );

	return num > 0;
}

bool CVzFile::ReadTextFile( const char *filepath, char *buffer, int max_len )
{
	int num = 0;

	FILE *fp = NULL;
	fp = fopen(filepath, "r");

	if( fp == NULL )
	{
		return false;
	}

	num = fread( buffer, sizeof(char), max_len, fp );
	fclose( fp );

	return num > 0;
}