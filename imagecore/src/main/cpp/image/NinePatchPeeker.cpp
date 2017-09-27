
#include "NinePatchPeeker.h"


bool NinePatchPeeker::readChunk(const char tag[], const void *data, size_t length) {
//    if (!strcmp("npTc", tag) && length >= sizeof(Res_png_9patch)) {
//        Res_png_9patch* patch = (Res_png_9patch*) data;
//        size_t patchSize = patch->serializedSize();
//        if (length != patchSize) {
//            return false;
//        }
//        // You have to copy the data because it is owned by the png reader
//        Res_png_9patch* patchNew = (Res_png_9patch*) malloc(patchSize);
//        memcpy(patchNew, patch, patchSize);
//        Res_png_9patch::deserialize(patchNew);
//        patchNew->fileToDevice();
//        free(mPatch);
//        mPatch = patchNew;
//        mPatchSize = patchSize;
//    } else
    if (!strcmp("npLb", tag) && length == sizeof(int32_t) * 4) {
        mHasInsets = true;
        memcpy(&mOpticalInsets, data, sizeof(int32_t) * 4);
    } else if (!strcmp("npOl", tag) && length == 24) { // 4 int32_ts, 1 float, 1 int32_t sized byte
        mHasInsets = true;
        memcpy(&mOutlineInsets, data, sizeof(int32_t) * 4);
        mOutlineRadius = ((const float *) data)[4];
        mOutlineAlpha = ((const int32_t *) data)[5] & 0xff;
    }
    return true;    // keep on decoding
}
