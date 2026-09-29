#include "pokemon/Move.hpp"

#include <algorithm>

namespace pokemon {

MoveInstance::MoveInstance(const MoveData* data)
    : data_(data), currentPP_(data ? std::max(0, data->pp) : 0) {}

const MoveData* MoveInstance::data() const { return data_; }
int MoveInstance::currentPP() const { return currentPP_; }
int MoveInstance::maxPP() const { return data_ ? std::max(0, data_->pp) : 0; }
bool MoveInstance::usable() const { return data_ && currentPP_ > 0; }

void MoveInstance::consumePP() {
    if (currentPP_ > 0) --currentPP_;
}

void MoveInstance::restorePP() {
    currentPP_ = maxPP();
}

} // namespace pokemon
