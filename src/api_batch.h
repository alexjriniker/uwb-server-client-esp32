#ifndef __API_BATCH_H__
#define __API_BATCH_H__

#include <Arduino.h>
#include <functional>
#include <sstream>

struct ApiBatchItem {
  const char *payload;
  unsigned long time;
};
typedef struct ApiBatchItem ApiBatchItem;

typedef std::function<void(const char *)> ApiBatchCallback;
typedef std::function<String(ApiBatchItem *, unsigned int)>
    ApiBatchPrepCallback;

// typedef ApiBatchItem ApiBatchItem;

const ApiBatchPrepCallback API_BATCH_PREP_CALLBACK_DEFAULT =
    [](ApiBatchItem *items, unsigned int size) {
      std::stringstream ss;

      ss << "{\"data\":[";

      for (int i = 0; i < size; i++) {
        ss << "{\"payload\":" << items[i].payload
           << ",\"time\":" << items[i].time << "}";

        if (i + 1 != size)
          ss << ",";
      }

      ss << "]}";

      return String(ss.str().c_str());
    };

class ApiBatch {
private:
  unsigned int index = 0, size;
  ApiBatchItem *items;
  ApiBatchCallback callback;
  ApiBatchPrepCallback prep_cb;

  ApiBatch(ApiBatch &);
  ApiBatch &operator=(ApiBatch &);

  void submit();

public:
  ~ApiBatch();
  ApiBatch(unsigned int, ApiBatchCallback,
           ApiBatchPrepCallback = API_BATCH_PREP_CALLBACK_DEFAULT);

  void add(const char *);
};

#endif // __API_BATCH_H__