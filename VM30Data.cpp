#include "VM30Data.h"

using namespace OSVRVM30;


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------*/
VM30Data::VM30Data(const std::vector<VML_GLOVE*>& gloves) : mGloves(gloves)
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


void VM30Data::update() {
    mSamples.clear();
    for (auto& glove : mGloves) {
        if (glove) {
            // we cast it back to const in VM30Data::getSample()
            auto newSample = const_cast<VML_GLOVE_SAMPLE*>(vml_glove_get_sample(glove));
            mSamples.push_back(newSample);
        }
    }
}

const VML_GLOVE* VM30Data::getLeftGlove() const {
    return getGlove(getLeftGloveIndex());
}

const VML_GLOVE* VM30Data::getRightGlove() const {
    return getGlove(getRightGloveIndex());
}

const VML_GLOVE_SAMPLE* VM30Data::getLeftGloveSample() const {
    return getSample(getLeftGloveIndex());
}

const VML_GLOVE_SAMPLE* VM30Data::getRightGloveSample() const {
    return getSample(getRightGloveIndex());
}

const VML_GLOVE* VM30Data::getGlove(uint32 gloveIndex) const {
    if (gloveIndex >= 0 && gloveIndex < mGloves.size()) {
        return mGloves[gloveIndex];
    }
    return nullptr;
}

const VML_GLOVE_SAMPLE* VM30Data::getSample(uint32 gloveIndex) const {
    if (gloveIndex >= 0 && gloveIndex < mSamples.size()) {
        return mSamples[gloveIndex];
    }
    return nullptr;
}

bool VM30Data::hasLeftGlove() const
{
    return getLeftGloveIndex() > 0;
}

bool VM30Data::hasRightGlove() const {
    return getRightGloveIndex() > 0;
}

int VM30Data::getLeftGloveIndex() const {
    return getGloveIndex(VML_GLOVE_HAND_LEFT);
}

int VM30Data::getRightGloveIndex() const {
    return getGloveIndex(VML_GLOVE_HAND_RIGHT);
}

int VM30Data::getGloveIndex(VML_GLOVE_HAND hand) const {
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
