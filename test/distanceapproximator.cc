#include "test.h"

#include "include/config.h"
#include "geo/constants.h"
#include "geo/pointll.h"
#include "geo/distanceapproximator.h"

using namespace std;
using namespace valhalla::geo;

namespace {

void TryKmPerDegreeLongitude(const PointLL& p, const float d2) {
  DistanceApproximator approx;
  float d = approx.KmPerLngDegree(p.lat());
  if (fabs(d - d2) > kEpsilon)
    throw runtime_error("KmPerDegreeLongitude test failed");
}
void TestKmPerDegreeLongitude() {
  TryKmPerDegreeLongitude(PointLL(0.0f, -80.0f), kKmPerDegreeLat);
}

void TryDistanceSquaredFromTestPt(const PointLL& testpt, const PointLL& p, const float d2) {
  // Test if distance is within 2% of the spherical distance
  DistanceApproximator approx;
  approx.SetTestPoint(testpt);
  float d = sqrtf(approx.DistanceSquared(p));
  //std::cout << " d = " << d << " ArcDistance = " << d2 << std::endl;
  if (fabs(d - d2) / d2 > 0.02f)
    throw runtime_error("DistanceSquared from point test failed");
}
void TestDistanceSquaredFromTestPt() {
  PointLL p1(42.0f, -80.0f);
  PointLL p2(40.0f, -78.0f);
  TryDistanceSquaredFromTestPt(p2, p1, p1.Distance(p2));
  TryDistanceSquaredFromTestPt(p1, p2, p1.Distance(p2));
}

void TryDistanceSquared(const PointLL& a, const PointLL& b, const float d2) {
  // Test if distance is > 2% the spherical distance
  DistanceApproximator approx;
  float d = sqrtf(approx.DistanceSquared(a, b));
  //std::cout << " d = " << d << " ArcDistance = " << d2 << std::endl;
  if (fabs(d - d2) / d2 > 0.02f)
    throw runtime_error("DistanceSquared between 2 points test failed");
}
void TestDistanceSquared() {
  PointLL a(42.0f, -80.0f);
  PointLL b(40.0f, -78.0f);
  TryDistanceSquaredFromTestPt(a, b, a.Distance(b));
}

}

int main() {
  test::suite suite("distanceapproximator");

  // Test kilometer per degree longitude at a specified latitude
  suite.test(TEST_CASE(TestKmPerDegreeLongitude));

  // Test distance squared from a test point
  suite.test(TEST_CASE(TestDistanceSquaredFromTestPt));

  // Test distance squared between 2 points
  suite.test(TEST_CASE(TestDistanceSquared));

  return suite.tear_down();
}
