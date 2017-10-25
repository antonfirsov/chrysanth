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


                void Zero_Scalar_WhenTrue() const
                {
                    REQUIRE(IsZeroWithEps(0, 0.1) == true);

                    REQUIRE(IsZeroWithEps(0.1, 0.1001) == true);
                    REQUIRE(IsZeroWithEps(-0.1, 0.1001) == true);

                    REQUIRE(IsZeroWithEps(0.1, 2000) == true);
                    REQUIRE(IsZeroWithEps(-0.1, 2000) == true);
                }

                void Zero_Scalar_WhenFalse() const
                {
                    REQUIRE(IsZeroWithEps(0.001, 0) == false);
                    REQUIRE(IsZeroWithEps(0.001, 0.00099) == false);
                    REQUIRE(IsZeroWithEps(-0.001, 0.00099) == false);

                    REQUIRE(IsZeroWithEps(0, 0) == false);
                }

                template<glm::length_t L, typename T, glm::qualifier Q>
                struct TotallyNotAVector { };

            };

            template<typename TVector4>
            struct ApproximateComparerVector4Fixture : ApproximateComparerFixture<typename VectorTraits<TVector4>::TVector>
            {
                typedef typename VectorTraits<TVector4>::TScalar TScalar;

                bool IsZeroWithEps(
                    const double x, const double y, const double z, const double w,
                    const double epsAsDouble) const
                {
                    TVector4 vec(static_cast<TScalar>(x), static_cast<TScalar>(y), static_cast<TScalar>(z), static_cast<TScalar>(w));
                    ApproximateComparer<TScalar> apx(static_cast<TScalar>(epsAsDouble));
                    return apx.Zero(vec);
                }

                void Zero_Vector_WhenTrue() const
                {
                    REQUIRE(IsZeroWithEps(0., 0., 0., 0., 0.0001) == true);
                    REQUIRE(IsZeroWithEps(1., 1., 1., 1., 2.1) == true);

                    REQUIRE(IsZeroWithEps(0, 0, 2, 3, glm::sqrt(13.001)) == true);
                    REQUIRE(IsZeroWithEps(-2, 3, 0, 0, glm::sqrt(13.001)) == true);

                    REQUIRE(IsZeroWithEps(0.2, 0.3, 0, 0, glm::sqrt(13.001)) == true);
                }

                void Zero_Vector_WhenFalse() const
                {
                    REQUIRE(IsZeroWithEps(0, 0, 0, 0, 0) == false);
                    REQUIRE(IsZeroWithEps(0.1, 0.1, -0.1, 0, 0) == false);

                    REQUIRE(IsZeroWithEps(1., 1., 1., 1., glm::sqrt(3.99)) == false);

                    REQUIRE(IsZeroWithEps(0, 0, 2, 3, glm::sqrt(12.999)) == false);
                    REQUIRE(IsZeroWithEps(-2, 3, 0, 0, glm::sqrt(12.999)) == false);

                    REQUIRE(IsZeroWithEps(0.2, 0.3, 0, 0, glm::sqrt(0.1299)) == false);
                }
            };

            template<typename TVector2>
            struct ApproximateComparerVector2Fixture : ApproximateComparerFixture<typename VectorTraits<TVector2>::TVector>
            {
                typedef typename VectorTraits<TVector2>::TScalar TScalar;

                bool IsZeroWithEps(
                    const double x, const double y,
                    const double epsAsDouble) const
                {
                    TVector2 vec(static_cast<TScalar>(x), static_cast<TScalar>(y));
                    ApproximateComparer<TScalar> apx(static_cast<TScalar>(epsAsDouble));
                    return apx.Zero(vec);
                }

                void Zero_Vector_WhenTrue() const
                {
                    REQUIRE(IsZeroWithEps(0., 0., 0.0001) == true);
                    REQUIRE(IsZeroWithEps(1., 1., sqrt(2.001)) == true);

                    REQUIRE(IsZeroWithEps(2, 3, glm::sqrt(13.001)) == true);
                    REQUIRE(IsZeroWithEps(-2, 3, glm::sqrt(13.001)) == true);

                    REQUIRE(IsZeroWithEps(0.2, 0.3, glm::sqrt(13.001)) == true);
                }

                void Zero_Vector_WhenFalse() const
                {
                    REQUIRE(IsZeroWithEps(0, 0, 0) == false);
                    REQUIRE(IsZeroWithEps(0.1, -0.1, 0) == false);

                    REQUIRE(IsZeroWithEps(1., 1., glm::sqrt(1.99)) == false);

                    REQUIRE(IsZeroWithEps(2, 3, glm::sqrt(12.999)) == false);
                    REQUIRE(IsZeroWithEps(-2, 3, glm::sqrt(12.999)) == false);

                    REQUIRE(IsZeroWithEps(0.2, 0.3, glm::sqrt(0.1299)) == false);
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

                TEST_CASE_METHOD(ApproximateComparerFixture<double>, "double, throws for negative eps")
                {
                    Construct_ForNegativeEps_Throws();
                }
            }

            namespace Zero
            {
                namespace Scalar
                {
                    TEST_CASE_METHOD(ApproximateComparerFixture<float>, "float | When true")
                    {
                        Zero_Scalar_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerFixture<float>, "float | When false")
                    {
                        Zero_Scalar_WhenFalse();
                    }

                    TEST_CASE_METHOD(ApproximateComparerFixture<double>, "double | When true")
                    {
                        Zero_Scalar_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerFixture<double>, "double | When false")
                    {
                        Zero_Scalar_WhenFalse();
                    }
                }
                
                namespace Vector4
                {
                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::vec4>, "vec4 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::vec4>, "vec4 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::dvec4>, "dvec4 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::dvec4>, "dvec4 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::aligned_vec4>, "aligned_vec4 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector4Fixture<glm::aligned_vec4>, "aligned_vec4 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }
                }

                namespace Vector2
                {
                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::vec2>, "vec2 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::vec2>, "vec2 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::dvec2>, "dvec2 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::dvec2>, "dvec2 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::aligned_vec2>, "aligned_vec2 | When true")
                    {
                        Zero_Vector_WhenTrue();
                    }

                    TEST_CASE_METHOD(ApproximateComparerVector2Fixture<glm::aligned_vec2>, "aligned_vec2 | When false")
                    {
                        Zero_Vector_WhenFalse();
                    }
                }
            }
        }
    }
}