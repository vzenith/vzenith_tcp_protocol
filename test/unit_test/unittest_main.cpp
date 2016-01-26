// Copyright 2007 Google Inc. All Rights Reserved.

//         juberti@google.com (Justin Uberti)
//
// A reuseable entry point for gunit tests.

#ifdef WIN32
#include <crtdbg.h>
#endif

#include "gunit.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}