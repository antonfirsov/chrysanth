#include <cmath>
#include <chrys/ApproximateComparer.hpp>

#include <catch.hpp>
#include "TestUtils.hpp"

namespace chrys
{
    namespace tests
    {
        namespace ApproximateComparerTests
        {
            template<typename TScalar>
            struct ApproximateComparerFixture
            {
                void TestConstruct(const double epsAsDouble) const
                {
                    TScalar eps = static_cast<TScalar>(epsAsDouble);
                    ApproximateComparer<TScalar> apx(eps);

                    REQUIRE(apx.Eps() == eps);
                    REQUIRE(apx.Eps2() == eps*eps);
                }

                void TestConstructAll() const
                {
                    TestConstruct(1.0);
                    TestConstruct(0.1);
                    TestConstruct(0.01);
                    TestConstruct(0.001);

                    TestConstruct(10);
                    TestConstruct(100);
                    TestConstruct(1000);
                }

                void Construct_ForNegativeEps_Throws() const
                {
                    TScalar eps = static_cast<TScalar>(-0.001);
                    CHECK_THROWS(ApproximateComparer<TScalar>(eps));
                }

                bool IsZeroWithEps(const double valueAsDouble, const double epsAsDouble) const
                {
                    const TScalar value = static_cast<TScalar>(valueAsDouble);
                    const TScalar eps = static_cast<TScalar>(epsAsDouble);
                    ApproximateComparer<TScalar> apx(eps);
                    return apx.Zero(value);
                }

                void Zero_WhenTrue() const
                {
                    REQUIRE(IsZeroWithEps(0, 0.1) == true);

                    REQUIRE(IsZeroWithEps(0.1, 0.1001) == true);
                    REQUIRE(IsZeroWithEps(-0.1, 0.1001) == true);

                    REQUIRE(IsZeroWithEps(0.1, 2000) == true);
                    REQUIRE(IsZeroWithEps(-0.1, 2000) == true);
                }

                void Zero_WhenFalse() const
                {
                    REQUIRE(IsZeroWithEps(0.001, 0) == false);
                    REQUIRE(IsZeroWithEps(0.001, 0.00099) == false);
                    REQUIRE(IsZeroWithEps(-0.001, 0.00099) == false);

                    REQUIRE(IsZeroWithEps(0, 0) == false);
                }
            };

            namespace Constructor
            {
                TEST_CASE_METHOD(ApproximateComparerFixture<double>, "double")
                {
                    TestConstructAll();
                }

                TEST_CASE_METHOD(ApproximateComparerFixture<float>, "float")
                {
                    TestConstructAll();
                }

                TEST_CASE_METHOD(ApproximateComparerFixture<double>, "float, throws for negative eps")
                {

                }
            }

            namespace Zero
            {
                TEST_CASE_METHOD(ApproximateComparerFixture<float>, "When true, float")
                {
                    Zero_WhenTrue();
                }

                TEST_CASE_METHOD(ApproximateComparerFixture<float>, "When false, float")
                {
                    Zero_WhenFalse();
                }

                TEST_CASE_METHOD(ApproximateComparerFixture<double>, "When true, double")
                {
                    Zero_WhenTrue();
                }

                TEST_CASE_METHOD(ApproximateComparerFixture<double>, "When false, double")
                {
                    Zero_WhenFalse();
                }
            }
        }
    }
}