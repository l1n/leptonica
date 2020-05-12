#include "leptfuzz.h"

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    leptSetStdNullHandler();

    PIX *pixs;
    BOX *box;

    pixs = pixReadMemSpix(data, size);
    if(pixs==NULL) return 0;

    PIX *pix1, *pix2, *pix3, *pix4, *pix5, *pix6;
    NUMA *na1, *na2, *na3, *na4, *na5, *na6;

    pix1 = pixConvertTo8(pixs, FALSE);
    box = boxCreate(120, 30, 200, 200);
    pixGetGrayHistogramInRect(pix1, box, 1);
    boxDestroy(&box);
    pixDestroy(&pix1);


    pixGetGrayHistogramTiled(pixs, 1, 1, 1);

    pix1 = pixConvertTo8(pixs, FALSE);
    pixGetCmapHistogramMasked(pix1, NULL, 1, 1, 1);
    pixDestroy(&pix1);

    pix1 = pixConvertTo8(pixs, FALSE);
    box = boxCreate(120, 30, 200, 200);
    pixGetCmapHistogramInRect(pix2, box, 1);
    boxDestroy(&box);
    pixDestroy(&pix1);

    l_int32 ncolors;
    pixCountRGBColors(pixs, 1, &ncolors);

    l_uint32  pval;
    pix1 = pixConvertTo8(pixs, FALSE);
    pixGetPixelAverage(pix1, NULL, 10, 10, 1, &pval);
    pixDestroy(&pix1);

    pix1 = pixConvertTo8(pixs, FALSE);
    l_uint32  pval2;
    pixGetPixelStats(pix1, 1, L_STANDARD_DEVIATION, &pval2);
    pixDestroy(&pix1);

    pix1 = pixConvertTo8(pixs, FALSE);
    if(pix1!=NULL){
        pix2 = pixConvert8To32(pix1);
        pixGetAverageTiledRGB(pix3, 2, 2, L_MEAN_ABSVAL, &pix4, &pix5, &pix6);
        pixDestroy(&pix1);
        pixDestroy(&pix2);
        pixDestroy(&pix3);
        pixDestroy(&pix4);
        pixDestroy(&pix5);
        pixDestroy(&pix6);
    }

    pixRowStats(pixs, NULL, &na1, &na2, &na3, &na4, &na5, &na6);
    numaDestroy(&na1);
    numaDestroy(&na2);
    numaDestroy(&na3);
    numaDestroy(&na4);
    numaDestroy(&na5);
    numaDestroy(&na6);

    //NUMA *na1, *na2, *na3, *na4, *na5, *na6;
    pixColumnStats(pixs, NULL, &na1, &na2, &na3, &na4, &na5, &na6);
    numaDestroy(&na1);
    numaDestroy(&na2);
    numaDestroy(&na3);
    numaDestroy(&na4);
    numaDestroy(&na5);
    numaDestroy(&na6);

    static const l_int32  nbins = 10;
    l_int32     minval, maxval;
    l_uint32    *gau32;
    pix1 = pixScaleBySampling(pixs, 0.2, 0.2);
    pixGetBinnedComponentRange(pix1, nbins, 2, L_SELECT_GREEN,
                                   &minval, &maxval, &gau32, 0);
    pixDestroy(&pix1);


    PIX *pixd = pixSeedspread(pixs, 4);
    PIX *pixc = pixConvertTo32(pixd);
    pixRankBinByStrip(pixc, L_SCAN_HORIZONTAL, 1, 10, L_SELECT_MAX);
    pixDestroy(&pixd);
    pixDestroy(&pixc);

    PIXA *pixa = pixaReadMem(data, size);
    pixaGetAlignedStats(pixa, L_MEAN_ABSVAL, 2, 2);
    pixaDestroy(&pixa);

    /*PIX *pix = pixConvertTo8(pixs, FALSE);
    l_float32  *rowvect = Generate3PtTransformVector();
    pixGetColumnStats(pix, L_MODE_VAL, 1, 1, rowvect);
    pixDestroy(&pix);*/

    l_int32 thresh, fgval, bgval;
    pix1 = pixConvertTo8(pixs, 0);
    pixSplitDistributionFgBg(pix1, 1.5, 1, &thresh, &fgval, &bgval, &pix2);
    pixDestroy(&pix1);
    pixDestroy(&pix2);

    return 0;
}