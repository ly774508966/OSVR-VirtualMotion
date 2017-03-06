#include "VirtualMotionData.h"

using namespace OSVRVirtualMotion;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
VirtualMotionData::VirtualMotionData(const std::vector<VML_GLOVE*>& gloves) : mGloves(gloves)
{
    for (auto& glove : gloves) {
        if (glove) {
            if (!vml_glove_set_stream_format(glove, VML_GLOVE_STREAM_FORMAT_QUAT)) {
                // @todo log error? hard error?
            }
        }
        mSamples.push_back(nullptr);
    }
}


void VirtualMotionData::update() {
    mSamples.clear();
    for (auto& glove : mGloves) {
        if (glove) {
            // we cast it back to const in VirtualMotionData::getSample()
            auto newSample = const_cast<VML_GLOVE_SAMPLE*>(vml_glove_get_sample(glove));
            mSamples.push_back(newSample);
        }
    }
}

const VML_GLOVE* VirtualMotionData::getLeftGlove() const {
    return getGlove(getLeftGloveIndex());
}

const VML_GLOVE* VirtualMotionData::getRightGlove() const {
    return getGlove(getRightGloveIndex());
}

const VML_GLOVE_SAMPLE* VirtualMotionData::getLeftGloveSample() const {
    return getSample(getLeftGloveIndex());
}

const VML_GLOVE_SAMPLE* VirtualMotionData::getRightGloveSample() const {
    return getSample(getRightGloveIndex());
}

const VML_GLOVE* VirtualMotionData::getGlove(uint32 gloveIndex) const {
    if (gloveIndex >= 0 && gloveIndex < mGloves.size()) {
        return mGloves[gloveIndex];
    }
    return nullptr;
}

const VML_GLOVE_SAMPLE* VirtualMotionData::getSample(uint32 gloveIndex) const {
    if (gloveIndex >= 0 && gloveIndex < mSamples.size()) {
        return mSamples[gloveIndex];
    }
    return nullptr;
}

bool VirtualMotionData::hasLeftGlove() const
{
    return getLeftGloveIndex() > 0;
}

bool VirtualMotionData::hasRightGlove() const {
    return getRightGloveIndex() > 0;
}

int VirtualMotionData::getLeftGloveIndex() const {
    return getGloveIndex(VML_GLOVE_HAND_LEFT);
}

int VirtualMotionData::getRightGloveIndex() const {
    return getGloveIndex(VML_GLOVE_HAND_RIGHT);
}

int VirtualMotionData::getGloveIndex(VML_GLOVE_HAND hand) const {
    for (size_t i = 0; i < mGloves.size(); i++) {
        auto& glove = mGloves[i];
        if (glove) {
            VML_GLOVE_INFO info;
            if (vml_glove_get_info(&info, glove) &&
                (info.Conn == VML_GLOVE_CONN_USB || info.Conn == VML_GLOVE_CONN_NETWORK) &&
                // info.IsStreaming && // @todo do we need to check this?
                info.Hand == hand) {
                return static_cast<int>(i);
            }
        }
    }
    return -1;
}
