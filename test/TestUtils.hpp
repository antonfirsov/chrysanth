#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "catch.hpp"
#include "glm/gtc/type_aligned.hpp"
#include "geom/VectorTraits.hpp"

namespace Catch {
    template<> 
    struct StringMaker<glm::vec2> 
    {
        static std::string convert(glm::vec2 const& v) {
            std::ostringstream s;
            s << '(' << v.x << ',' << v.y << ')';
            return s.str();
        }
    };

/*
    namespace detail
    {
        template<class T, typename Enabler = void>
        struct VectorStringMakerBase
        {
            static std::string convert(T const& v) {
                std::ostringstream s;
                s << '(';
                for (int i=0;i<mums::VectorTraits<T>::Dim();i++)
                {
                    s << v[i];
                    if (i < T::length() - 1) s << ',';
                }
                s << ')';
                return s.str();
            }
        };
    }

    template<
        template <glm::length_t, typename, glm::qualifier> class TVector,
        glm::length_t L,
        typename T,
        glm::qualifier Q
    >
    struct StringMaker<TVector<L, T, Q>> : detail::VectorStringMakerBase<
        TVector<L, T, Q>,
        typename std::enable_if<::mums::VectorTraits<TVector<L, T, Q>>::IsVector>::type
    >
    {
    };
*/  
}

namespace chrys
{
    namespace tests
    {
        template <typename T1, typename T2>
        constexpr bool SameTypes = std::is_same<T1, T2>::value;
    }
}

