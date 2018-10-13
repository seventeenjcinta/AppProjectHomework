#include "stdafx.h"
#include <iostream>

#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")

using namespace std;

int main()
{
    GDALDataset* poSrcDS;
	GDALDataset* poDstDS;
	int imgXlen, imgYlen;
	char* srcPath = "srcimg.jpg";
	char* dstPath1 = "dstimg1.tif";
	char* dstPath2 = "dstimg2.tif";
	GByte* buffTmp;
	int i, j, bandNum;
	int StartX;
    int StartY;
    int tmpXlen;
    int tmpYlen;

	GDALAllRegister();

    poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);

    imgXlen = poSrcDS -> GetRasterXSize();
	imgYlen = poSrcDS -> GetRasterYSize();
	bandNum = poSrcDS -> GetRasterCount();

    cout << "Image X length" << imgXlen << endl;
    cout << "Image Y Length" << imgYlen << endl;
    cout << "Band number" << bandNum << endl;


    //mission 1
    StartX = 100;
    StartY = 100;
    tmpXlen = 200;
    tmpYlen = 150;

    buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager() -> GetDriverByName("GTiff") -> Create(dstPath1, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//拷贝图像
	for (i = 0; i < bandNum; i++){
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//调整选定区域RGB
    for(j = 0; j < tmpYlen; j++){
        for(i = 0; i < tmpXlen; i++){
            buffTmp[j * tmpXlen + i] = (GByte)255;
        }
    }

	//覆盖原图
    poDstDS -> GetRasterBand(1) -> RasterIO(GF_Write, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);

	CPLFree(buffTmp);
	GDALClose(poDstDS);
	cout << "Mission 1 Done" << endl;


	//mission2
	StartX = 300;
	StartY = 300;
	tmpXlen = 100;
	tmpYlen = 50;

	buffTmp = (GByte*)CPLMalloc(imgXlen * imgYlen * sizeof(GByte));
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(dstPath2, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);

	//拷贝图像
	for (i = 0; i < bandNum; i++) {
		poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, 0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
	}
	//调整选定区域RGB
	for (j = 0; j < tmpYlen; j++) {
		for (i = 0; i < tmpXlen; i++) {
			buffTmp[j * tmpXlen + i] = (GByte)255;
		}
	}
	//覆盖原图
	for (i = 0; i < bandNum; i++) {
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	}

	StartX = 500;
	StartY = 500;
	tmpXlen = 50;
	tmpYlen = 100;

	//调整选定区域RGB
	for (j = 0; j < tmpYlen; j++) {
		for (i = 0; i < tmpXlen; i++) {
			buffTmp[j * tmpXlen + i] = (GByte)0;
		}
	}
	//覆盖原图
	for (i = 0; i < bandNum; i++) {
		poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write, StartX, StartY, tmpXlen, tmpYlen, buffTmp, tmpXlen, tmpYlen, GDT_Byte, 0, 0);
	}


	CPLFree(buffTmp);
	GDALClose(poDstDS);
	GDALClose(poSrcDS);

	cout << "Mission 2 Done" << endl;

	system("pause");
    return 0;
}
