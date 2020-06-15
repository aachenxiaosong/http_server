#include <string>
#include <iostream>
#include "uni_uuid.h"

using namespace std;

int main() {
  int i;
  srand(time(0));
  char uuid[33] = {0};
  for (i = 0; i < 10; i++) {
    if (0 == GetUuid(uuid)) {
      cout << uuid << endl;
    }
  }
  return 0;
}
