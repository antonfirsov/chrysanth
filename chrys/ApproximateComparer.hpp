#pragma once

#include <cmath>
#include <glm/gtc/constants.hpp>
#include "VectorTraits.hpp"

namespace chrys
{
    template<typename _TScalar>
    class ApproximateComparer
    {
    public:
        typedef _TScalar TScalar;

    private:
        const TScalar _eps;
        const TScalar _negEps;
        const TScalar _eps2;
        const TScalar _negEps2;
        const TScalar _eps15;
        const TScalar _negEps15;
    public:

        TScalar Eps() const { return _eps; }

        TScalar Eps2() const { return _eps2; }

        ApproximateComparer(TScalar eps) :
            _eps(eps),
            _negEps(-eps),
            _eps2(eps*eps),
            _negEps2(-_eps2),
            _eps15(std::pow(eps, static_cast<TScalar>(1.5))),
            _negEps15(-_eps15)
        {
        }
        
        ApproximateComparer() : ApproximateComparer(DefaultEps())
        {
        }

        static constexpr TScalar DefaultEps()
        {
            return static_cast<TScalar>(1e-5);
        }
        
        static inline const ApproximateComparer<_TScalar>&  DefaultComparer()
        {
            static const ApproximateComparer<double> cmp;
            return cmp;
        }

        bool Zero(const TScalar d) const
        {
            return d > _negEps && d < _eps;
        }

        template<typename TVector>
        bool Zero(const TVector& v) const
        {
            TScalar squaredLength = glm::dot(v, v);
            return squaredLength < _eps2;
        }
/*
        inline bool Zero(const TVEC2(TScalar)& v)
        {
            return Zero(v.x) && Zero(v.y);
        }

        inline bool Zero(const TVEC3(TScalar)& v)
        {
            return Zero(v.x) && Zero(v.y) && Zero(v.z);
        }

        inline bool Zero2(const TScalar d) const
        {
            return d > _negEps2 && d < _eps2;
        }*/

        bool Equals(const TScalar a, const TScalar b) const
        {
            TScalar d = a - b;
            return d > _negEps && d < _eps;
        }

        bool GT(const TScalar a, const TScalar b) const
        {
            return a > b + _eps;
        }

        bool LT(const TScalar a, const TScalar b) const
        {
            return a < b + _negEps;
        }

        bool GE(const TScalar a, const TScalar b) const
        {
            return a > b + _negEps;
        }

        bool LE(const TScalar a, const TScalar b) const
        {
            return b > a + _negEps;
        }

        int Sign(const TScalar a) const
        {
            if (a > _eps) return 1;
            else if (a < _negEps) return -1;
            else return 0;
        }

        //inline bool Equals(const TVEC2(TScalar)& a, const TVEC2(TScalar)& b) const
        //{
        //    return Equals(a.x, b.x) && Equals(a.y, b.y);
        //}

        //inline bool Equals(const TVEC3(TScalar)& a, const TVEC3(TScalar)& b) const
        //{
        //    return Equals(a.x, b.x) && Equals(a.y, b.y) && Equals(a.z, b.z);
        //}

        //template<typename T>
        //typename std::enable_if<
        //    hirsch::geom::GeometryTraits<T>::IsLineSegment, bool
        //>::type
        //    Equals(const T& a, const T& b) const
        //{
        //    auto aStart = SegmentStart(a);
        //    auto aEnd = SegmentEnd(a);
        //    auto bStart = SegmentStart(b);
        //    auto bEnd = SegmentEnd(b);

        //    return Equals(aStart, bStart) && Equals(aEnd, bEnd) ||
        //        Equals(aStart, bEnd) && Equals(aEnd, bStart);
        //}

        //std::ostream& Print(std::ostream& s) const
        /*friend std::ostream& operator <<(std::ostream& s, const ApproximateComparer& apx)
        {
            return s << "(eps=" << apx._eps << ")";
        }*/

        //bool Horizontal(const LineSegment<TScalar, 2>& l) const
        //{
        //    TScalar d = l.Dir();
        //    return d > _negEps && d < _eps;
        //}

        //bool Vertical(const LineSegment<TScalar, 2>& l) const
        //{
        //    TScalar d = l.Dir();
        //    return d > _negEps && d < _eps;
        //}

        /// <summary>
        /// Calculate the following value:
        /// res = |a b| = a*b + c*d
        ///       |c d|
        /// Returns exactly 0.0 if IsZero15(res) is expected.
        /// 
        /// Idea taken from:
        /// http://realtimecollisiondetection.net/pubs/GDC07_Ericson_Physics_Tutorial_Numerical_Robustness.ppt
        /// (slide 42)
        /// </summary>
        TScalar MulSum(TScalar a, TScalar b, TScalar c, TScalar d)
        {
            using namespace glm;
            
            TScalar max_ = max(abs(a), abs(b), abs(c), abs(d));
            TScalar eps = max_ < glm::one<TScalar>() ? _eps15 : max_ * _eps15;

            TScalar det = a * b + c * d;
            if (det > -eps && det < eps) det = 0;
            return det;
        }

        /// <summary>
        /// Calculate corrected 2x2 determinant:
        /// det = |a b| = a*b - c*d
        ///       |c d|
        /// Returns exactly 0.0 if IsZero15(det) is expected.
        /// 
        /// Idea taken from:
        /// http://realtimecollisiondetection.net/pubs/GDC07_Ericson_Physics_Tutorial_Numerical_Robustness.ppt
        /// (slide 42)
        /// </summary>
        TScalar MulDiff(TScalar a, TScalar b, TScalar c, TScalar d)
        {
            using namespace glm;

            TScalar max_ = max(abs(a), abs(b), abs(c), abs(d));
            TScalar eps = max_ < glm::one<TScalar>() ? _eps15 : max_ * _eps15;

            TScalar det = a * b - c * d;
            if (det > -eps && det < eps) det = 0;
            return det;
        }


        ///// <summary>
        ///// "robust" dot product.
        ///// Idea taken from:
        ///// http://realtimecollisiondetection.net/pubs/GDC07_Ericson_Physics_Tutorial_Numerical_Robustness.ppt
        ///// </summary>
        //TScalar Dot(const TVEC2(TScalar)& a, const TVEC2(TScalar)&  b)
        //{
        //    return MulSum(a.x, b.x, a.y, b.y);
        //}
    };
}
