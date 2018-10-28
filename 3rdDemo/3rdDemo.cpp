// 3rdDemo.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <iostream>

using namespace std;

#include "./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

int main()
{
	GDALDataset* poSrcSpaceDS;
	GDALDataset* poSrcSupermanDS;
	GDALDataset* poDstDS;
	int imgXlen, imgYlen, bandNum;
	char* space = "space.jpg";
	char* superman = "superman.jpg";
	char* dstPath = "dist.tif";
	GByte* buffTmpSpace, *buffTmpSuperman;

	GDALAllRegister();
	poSrcSpaceDS = (GDALDataset*)GDALOpenShared(space, GA_ReadOnly);
	poSrcSupermanDS = (GDALDataset*)GDALOpenShared(superman, GA_ReadOnly);

	imgXlen = poSrcSupermanDS->GetRasterXSize();	//图片长度
	imgYlen = poSrcSupermanDS->GetRasterYSize();	//图片宽度
	bandNum = poSrcSupermanDS->GetRasterCount();
	cout << "begin..." << endl;
	buffTmpSpace = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));	
	buffTmpSuperman = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//拷贝图像
	for (int i = 0; i < bandNum; i++) {
		poSrcSpaceDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmpSpace, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmpSpace, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}

	int buff[3] = { 0 };


	for (int i = 0; i < imgXlen; i++) {
		for (int j = 0; j < imgYlen; j++) {
			for (int k = 0; k < bandNum; k++){
				poSrcSupermanDS->GetRasterBand(k + 1)->RasterIO(GF_Read, i, j, 1, 1, buffTmpSuperman, 1, 1, GDT_Byte, 0, 0);
				buff[k] = (int)buffTmpSuperman[0];
				//cout << buff[k] << endl;
			}
			if (!((buff[0]<160 && buff[0]>10) && (buff[1]<220 && buff[1]>100) && (buff[2]<110 && buff[2]>10))){
				for (int k = 0; k < bandNum; ++k){
					poDstDS->GetRasterBand(k + 1)->RasterIO(GF_Write,i, j, 1, 1, buff+k, 1, 1, GDT_Byte, 0, 0);
				}
			}
		}
	}
	cout << "end..." << endl;
	CPLFree(buffTmpSpace);
	CPLFree(buffTmpSuperman);
	GDALClose(poDstDS);
	GDALClose(poSrcSupermanDS);
	GDALClose(poSrcSpaceDS);
	system("PAUSE");
	return 0;

}

