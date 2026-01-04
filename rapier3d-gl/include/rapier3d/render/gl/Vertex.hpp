#pragma once

#include <string>
#include <format>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// this is in layout intended to be passed to GLbuffer
struct GLvertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;

    bool operator==(const GLvertex& other) const {
        const float eps = 1e-6f;
        return glm::all(glm::lessThan(glm::abs(position - other.position), glm::vec3(eps)))
            && glm::all(glm::lessThan(glm::abs(normal - other.normal), glm::vec3(eps)))
            && glm::all(glm::lessThan(glm::abs(texcoords - other.texcoords), glm::vec2(eps)));
    }

    operator std::string() const {
        return std::format("({}, {}, {}), ({}, {}, {}), ({}, {})",
            position.x, position.y, position.z,
            normal.x, normal.y, normal.z,
            texcoords.x, texcoords.y);
    }
};

namespace std {
    template<> struct hash<GLvertex> {
        size_t operator()(const GLvertex& v) const {
            size_t seed = 0;
            glm::detail::hash_combine(seed, std::hash<glm::vec3>()(v.position));
            glm::detail::hash_combine(seed, std::hash<glm::vec3>()(v.normal));
            glm::detail::hash_combine(seed, std::hash<glm::vec2>()(v.texcoords));
            return seed;
        }
    };
}