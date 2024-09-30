
#include "PngProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void ImageProcessingGray(unsigned char* pOut
	, unsigned char* pIn
	, size_t nWidth
	, size_t nHeight);

void CircularHalftoneImage(unsigned char* pOut
	, unsigned char* pIn
	, size_t nWidth
	, size_t nHeight);


int main(int argc, char* argv[])
{

	class CBitsPtrGuard
	{
	public:
		CBitsPtrGuard(unsigned char** pB) : m_ppBits(pB) { }
		~CBitsPtrGuard() { if (*m_ppBits) delete* m_ppBits, * m_ppBits = 0; }
	protected:
		unsigned char** m_ppBits;
	};
	//printf(argv[1]);
	//printf(argv[2]);
	//std::cout << argc << std::endl;
	//std::cout << argv[1] << std::endl;
	// parse input parameters
	char	szInputFileName[256];
	char    szOutputFileName[256];
	if (argc < 2)
		printf("\nformat: pngtest <input_file> [<output_file>]");
	else
	{
		strcpy(szInputFileName, argv[1]);

		if (argc > 2)
			strcpy(szOutputFileName, argv[2]);
		else
		{
			strcpy(szOutputFileName, szInputFileName);
			strcat(szOutputFileName, "_out.png");
		}
	}


	size_t nReqSize = NPngProc::readPngFile(szInputFileName, 0, 0, 0, 0);
	if (nReqSize == NPngProc::PNG_ERROR)
	{
		printf("\nError ocured while pngfile was read");
		return -1;
	}


	unsigned char* pInputBits = new unsigned char[nReqSize];
	if (!pInputBits)
	{
		printf("\nCan't allocate memory for image, required size is %u", nReqSize);
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard(&pInputBits);


	unsigned char* pOutputBits = new unsigned char[nReqSize];
	if (!pOutputBits)
	{
		printf("\nCan't allocate memory for image, required size is %u", nReqSize);
		return -1;
	}


	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits);

	size_t nWidth, nHeight;
	unsigned int nBPP;

	size_t nRetSize = NPngProc::readPngFileGray(szInputFileName, pInputBits, &nWidth, &nHeight/*, &nBPP*/);
	nBPP = 8;


	// ASSERT(nRetSize == nReqSize);

	// TODO: image processing 
	//ImageProcessingGray(pOutputBits, pInputBits, nWidth, nHeight);
	CircularHalftoneImage(pOutputBits, pInputBits, nWidth, nHeight);


	if (NPngProc::writePngFile(szOutputFileName, pOutputBits, nWidth, nHeight, nBPP) == NPngProc::PNG_ERROR)
	{
		printf("\nError ocuured during png file was written");
		return -1;
	}

	
	return 0;
}

void ImageProcessingGray(unsigned char* pOut
	, unsigned char* pIn
	, size_t nWidth
	, size_t nHeight)
{
	// ASSERT(pOut != NULL && pIn != NULL && nWidth > 0 && nHeight > 0)

	// for example, inversion of image
	for (size_t y = 0; y < nHeight; ++y)
	{
		for (size_t x = 0; x < nWidth; ++x)
		{
			*pOut++ = 255 - *pIn++;
		}
	}
	return;
}

void CircularHalftoneImage(unsigned char* pOut
	, unsigned char* pIn
	, size_t nWidth
	, size_t nHeight)
{
	std::cout << nWidth << ' ' << nHeight;

	int R = min(nWidth, nHeight) / 2 - 1;
	int centralPixel_x = nWidth / 2;
	int centralPixel_y = nHeight / 2;
	for (size_t y = 0; y < nHeight; ++y)
	{
		for (size_t x = 0; x < nWidth; ++x)
		{
			
			size_t numberPixel = y * nWidth + x;
			if (((centralPixel_x - x) * (centralPixel_x - x) + (centralPixel_y - y) * (centralPixel_y - y)) > (R * R))
			{
				pOut[numberPixel] = 0;
			}
			else
			{
				pOut[numberPixel] = pIn[numberPixel];
			}

		}
	}
	return;
}

