#pragma once
#ifndef OGL_UTILS_VEC_HPP
#define OGL_UTILS_VEC_HPP

#include <cstddef>
#include <type_traits>

namespace ogl {
    template<typename T> concept arithmetic = std::is_arithmetic_v<T>;

    template<arithmetic T, size_t N> struct vec { T x[N]; };

    template<arithmetic T> struct vec<T, 2> { T x, y; };
    template<arithmetic T> struct vec<T, 3> { T x, y, z; };
    template<arithmetic T> struct vec<T, 4> { T x, y, z, w; };

    template<arithmetic T> using vec2 = vec<T, 2>;
    template<arithmetic T> using vec3 = vec<T, 3>;
    template<arithmetic T> using vec4 = vec<T, 4>;

    using vec2i = vec2<int>;
    using vec3i = vec3<int>;
    using vec4i = vec4<int>;

    using vec2u = vec2<unsigned>;
    using vec3u = vec3<unsigned>;
    using vec4u = vec4<unsigned>;

    using vec2f = vec2<float>;
    using vec3f = vec3<float>;
    using vec4f = vec4<float>;

}

#endif//OGL_UTILS_VEC_HPP