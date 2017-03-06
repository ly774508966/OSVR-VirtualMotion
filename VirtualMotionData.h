#if !defined(__VirtualMotionData_h__)
#define __VirtualMotionData_h__

#include "libvml.h"
#include <vector>

namespace OSVRVirtualMotion {

    class VirtualMotionData {

    public:

        VirtualMotionData(const std::vector<VML_GLOVE*>& gloves);

        void update();

        const VML_GLOVE* getLeftGlove() const;
        const VML_GLOVE* getRightGlove() const;
        const VML_GLOVE_SAMPLE* getLeftGloveSample() const;
        const VML_GLOVE_SAMPLE* getRightGloveSample() const;
        const VML_GLOVE* getGlove(uint32 gloveIndex) const;
        const VML_GLOVE_SAMPLE* getSample(uint32 gloveIndex) const;
        bool hasLeftGlove() const;
        bool hasRightGlove() const;
        int getLeftGloveIndex() const;
        int getRightGloveIndex() const;

    private:
        std::vector<VML_GLOVE*> mGloves;

        // do not make vml calls on this instance directly,
        // use getSample() to preserve its original const-ness
        std::vector<VML_GLOVE_SAMPLE*> mSamples;

        int getGloveIndex(VML_GLOVE_HAND hand) const;
    };

}

#endif //__VirtualMotionData_h__
