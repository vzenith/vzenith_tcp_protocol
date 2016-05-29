#ifndef VZ_FILE_H_
#define VZ_FILE_H_

class CVzFile
{
public:
	CVzFile(void);
	~CVzFile(void);

	static bool WriteFile(const char *filepath, unsigned char *pData, unsigned int len); // Ð´ÎÄ¼þ
	static bool WriteTxtFile(const char *filepath, const char* pText);
	static bool ReadTextFile( const char *filepath, char *buffer, int max_len );
};
#endif