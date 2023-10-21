#include "api_batch.h"

ApiBatch::~ApiBatch() { delete[] items; }

ApiBatch::ApiBatch(unsigned int size, const ApiBatchCallback callback,
                   ApiBatchPrepCallback prep_cb)
    : size{size}, callback{callback}, prep_cb{prep_cb} {
  // this->size = size;
  // this->callback = callback;
  // this->prep_cb = prep_cb;
  // TODO: move into the rest of the member initializations
  items = new ApiBatchItem[size];
}

void ApiBatch::submit() {
  auto payload = prep_cb(items, size);
  callback(payload.c_str());

  index = 0; // rest index back to original position
}

void ApiBatch::add(const char *str) {
  items[index++] = ApiBatchItem{str, millis()};

  if (index == size)
    submit();
}
