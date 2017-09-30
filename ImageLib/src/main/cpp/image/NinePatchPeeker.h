
#ifndef _NP_
#define _NP_

#include <malloc.h>
#include <cstring>

#include "SkPngChunkReader.h"

class NinePatchPeeker : public SkPngChunkReader {
public:
    NinePatchPeeker()
            : mPatchSize(0), mHasInsets(false), mOutlineRadius(0), mOutlineAlpha(0) {
        memset(mOpticalInsets, 0, 4 * sizeof(int32_t));
        memset(mOutlineInsets, 0, 4 * sizeof(int32_t));
    }

    ~NinePatchPeeker() {
    }

    bool readChunk(const char tag[], const void *data, size_t length) override;

    size_t mPatchSize;
    bool mHasInsets;
    int32_t mOpticalInsets[4];
    int32_t mOutlineInsets[4];
    float mOutlineRadius;
    uint8_t mOutlineAlpha;
};

#endif  // _NP_
