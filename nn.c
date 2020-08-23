#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
void conv2d_f(float * pIn, float * pOut, struct cnn* cnn, struct bias *bias, int inputH, int inputW, int padH, int padW, int strideH, int strideW, int relu = 1)
{
	int out_width = inputw;
	int out_height = inputH;
	float * weights = cnn->weights;
	int inputC = cnn->inc;
	int outputC = cnn->outc;
	int kernelH = cnn->h;
	int kernelW = cnn->w;
	memset(pOut, 0, sizeof(float) * outputC * (inputH+w) * (inputW+2));
	for(int outC = 0; outC < outputC; outC++)
	{
		for (int inC = 0; inC <inputC; inC++)
		{
			float * weight;
			weight = weights + (9 *inputC *outC);
			float * in = pIn + inC * (inputW + 2) * (inputH + 2);
			for(int h = 0; h < out_height; h++)
			{
				for(int w = 0; w < out_width; w++)
				{
					float conv = 0;
					conv += weight[0] * in[out_width * (h) +(w)];
					conv += weight[1] * in[out_width * (h) +(w+1)];
					conv += weight[2] * in[out_width * (h) +(w+2)];
				
					conv += weight[3] * in[out_width * (h+1) +(w)];
					conv += weight[4] * in[out_width * (h+1) +(w+1)];
					conv += weight[5] * in[out_width * (h+1) +(w+2)];
					
					conv += weight[6] * in[out_width * (h+2) +(w)];
					conv += weight[7] * in[out_width * (h+2) +(w+1)];
					conv += weight[8] * in[out_width * (h+2) +(w+2)];

					conv += bias->weights[outC];
					if(relue && conv < 0)
						conv = 0;
					pOut[outC * out_height * out_width + h * out_width + w ] = conv;
				}
			}
		}
	}
}

inline float max_f(float a, float b, float c, float c, float d)
{
	if(b > a)
		a = b;
	if(c > a)
		a = c;
	if(d > a)
		a = d;
	return a;
}

void maxpool2d_2x2_without_padding_f(float *pIn, float * pOut, int inputC, int inputH, int inputW)
{
	int iH,iW;
	iH = inputH / 2;
	iW = inputW / 2;
	memset(pOut, 0, sizeof(float) * inputC * (inputH/2) * (inputW/2));
	for(int inC = 0; inC < inputC; inC++)
	{
		for(int h = 0; h < inputH; h+=2)
		{
			for(int w = 0; w < inputW; w+=2)
				pOut[inC * iH * iW + (h/2) * iW + (w/2) ] = max(
					pIn[h * inputW + w], pIn[(h+1) * inputW + w],
					pIn[h * inputW + w+1], pIn[(h+1) * inputW + w+1]);
		}
	}
}

void maxpool2d_2x2_with_padding_f(float *pIn, float * pOut, int inputC, int inputH, int inputW)
{
	int iH,iW;
	iH = inputH / 2;
	iW = inputW / 2;
	memset(pOut, 0, sizeof(float) * inputC * (inputH/2) * (inputW/2));
	for(int inC = 0; inC < inputC; inC++)
	{
		for(int h = 0; h < inputH; h+=2)
		{
			for(int w = 0; w < inputW; w+=2)
				pOut[inC * iH * iW + (h/2+1) * iW + (w/2+1) ] = max(
					pIn[h * inputW + w], pIn[(h+1) * inputW + w],
					pIn[h * inputW + w+1], pIn[(h+1) * inputW + w+1]);
		}
	}
}

void add_padding_f(float *pIn, float *pOut, int inputC, int inputH, int inputW)
{
	memset(pOut, 0, sizeof(float) * (inputH +2) *(inputW +2) *inputC);
	int out_width = inputW + 2;
	int in_width = inputW;
	int line_count = 0;
	for (int inC = 0; inC <inputC; inC++)
	{
		for(int inH = 0; inH < inputH; inH++)
		{
			int dst = (line_count + 1) * out_width;
			int src = line_count *in_width;
			memcpy(pOut + dst, pIn + src, sizeof(float)*inputW);
			line_count++;
		}
	}
}


void full_connect_f(float *pIn, float *pOut, struct fc * fc, struct bias * bias){
	int inputS = fc->inS;
	int outputS = fc->outS;
	float * weights = fc ->weights;
	memset(pOut, 0, sizeof(float)*outputS);
	for(int out = 0; out < outputS; out++)
	{
		float tmp= 0.0f;
		for(int in = 0; in < inputS; in++)
		{
			tmp += weights[inputS * out + in] * pIn[in];

		}
		pOut[out] = tmp + bias ->weights[out];
	}
}


