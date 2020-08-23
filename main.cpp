#include <stdio.h>
#include <string,h>
#include <malloc.h>
#include "nn,h"
#include <opencv2/opencv.hpp>

using namespace cv;

void read_img(char *path, float *pOut)
{
	Mat image;
	Mat bgr[3];
	image = imread(path, IMREAD_COLOR);
	memset(pOut, 0 , sizeof(float) * 3 * 224 * 224);
	if(image.empty())
	{
		printf("read failed %s\n", path);
	}
	split(image, bgr);
	for(int c = 0; c < 3; c++)
	{
		for (int i = 0; i < 224 * 224; i++)
		{
			pOut[c * 224 * 224 + i] = bgr[c].data[i] / 255.0f;
		}
	}
}

struct cnn* read_cnn(FILE fp, int inC, int outc, int h, int w){
	struct cnn * cnn = (struct cnn*)malloc(sizeof(struct cnn));
	cnn->inc = inC;
	cnn->outc = outC;
	cnn->h = h;
	cnn->w = w;
	cnn->weights = (float*)malloc(sizeof(float) * inC * outC * h * w);
	char tmp[128];
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fscanf(fp, "%s", cnn->name);
	printf("%s\n", cnn->name);
	for (int i = 0; i < inC * outC * h * w; i++)
		fscanf(fp, "%f", &cnn->weights[i]);
	fgets(tmp, 128, fp);
	return cnn;
}

struct bias * read_bias(FILE * fp, int s){
	struct bias * bias = (struct bias*)malloc(sizeof(struct bias));
	bias->s = s;

	bias ->weights =(float *)malloc(sizeof(float) * s);
	char tmp[128];
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fscanf(fp, "%s", bias->name);
	printf("%s\n", bias->name);
	for(int i = 0; i < s; i++)
		fscanf(fp, "%f", &bias->weights[i]);
	fgets(tmp, 128, fp);
	return bias;
}


struct fc * read_fc(FILE *fp, int in, int out){
	struct fc*fc = (struct fc*)malloc(sizeof(struct fc));
	fc->inS = in;
	fc->outS = out;

	fc->weights = (float*)malloc(sizeof(float) * in * out);
	char tmp[128];
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fgets(tmp, 128, fp);
	fscanf(fp, "%s", fc->name);
	printf("%s\n", fc->name);
	for(int i = 0; i < in * out; i++)
		fscanf(fp, "%f", &fc->weights[i]);
	fgets(tmp, 128, fp);
	return fc;
}


static struct cnn* cnn[8];
static struct bias* bias[8];
static struct fc * fc[2];
static struct bias* fc_bias[2];

void read_weight(char * filepath)
{
	FILE * fp = fopen(filepath, "r");

	cnn[0] = read_cnn(fp, 3, 64, 3, 3);
	bias[0] = read_bias(fp, 64);

	cnn[1] = read_cnn(fp, 64, 128, 3, 3);
	bias[1] = read_bias(fp, 128);

	cnn[2] = read_cnn(fp, 128, 256, 3, 3);
	bias[2] = read_bias(fp, 256);

	cnn[3] = read_cnn(fp, 128, 256, 3, 3);
	bias[3] = read_bias(fp, 256);

	cnn[4] = read_cnn(fp, 256, 512, 3, 3);
	bias[4] = read_bias(fp, 512);

	cnn[5] = read_cnn(fp, 512, 512, 3, 3);
	bias[5] = read_bias(fp, 512);

	cnn[6] = read_cnn(fp, 512, 512, 3, 3);
	bias[6] = read_bias(fp, 512);

	cnn[7] = read_cnn(fp, 512, 512, 3, 3);
	bias[7] = read_bias(fp, 512);

	fc[0] = read_fc(fp, 25088, 128);
	fc_bias[0] = read_bias(fp, 128);

	fc[1] = read_fc(fp, 128, 2);
	fc_bias[1] = read_bias(fp, 2);
	fclose(f);
}


int main()
{
	float *pOut;
	pOut = (float*) malloc(sizeof(float) * 3 * 224 * 224);
	read_weight((char *)"weights2_clean.txt");
	read_iomg((char*)"ant/0.png", pOut);
	classification_f(pOut, 3, 224, 224, cnn, bias, fc, fc_bias);
}

