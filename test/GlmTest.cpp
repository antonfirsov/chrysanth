#include <iostream>
#define GLM_FORCE_MESSAGES

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <catch.hpp>
#include "TestUtils.hpp"
#include "glm/gtc/type_aligned.hpp"
#include "glm/gtc/constants.inl"

TEST_CASE("hello GLM")
{
    INFO("lol");

    glm::vec2 a(1, 2);;
    glm::vec2 b(3, 4);
    glm::vec2 q{ 666, 777 };

    CAPTURE(q);

    auto c = a + b;

    REQUIRE(c == glm::vec2(4, 6));
    FAIL();
}