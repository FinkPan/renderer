//ÄÚ´æÐ¹Â©¼ì²â
#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#include <string>
#include <cstdlib> //int rand()

#include "gtest/gtest.h"

#include "hs_renderer/obj_renderer/obj_tiles_loader.hpp"
#include "hs_renderer/obj_utility/viewing_transformer.hpp"

inline void EnableMemLeakCheck()
{
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace
{
  typedef float Scalar;
  typedef hs::renderer::ViewingTransformer<Scalar> ViewingTransformer;

  TEST(OBJTilesLoader, CalculateGoal)
  {
     ViewingTransformer vt;

  }

}