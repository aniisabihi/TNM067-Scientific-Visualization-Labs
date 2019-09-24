/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2016 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <warn/push>
#include <warn/ignore/all>
#include <gtest/gtest.h>
#include <warn/pop>

#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/util/imagesampler.h>
#include <inviwo/core/datastructures/image/image.h>

#include <modules/tnm067lab4/jacobian.h>

#include <memory>
#include <tuple>


#define EPS 3.0e-07

#define TEST_VEC2(e,r)  ASSERT_NEAR(e.x,r.x,EPS);  ASSERT_NEAR(e.y,r.y,EPS);
#define TEST_MAT2(e,r)  TEST_VEC2(e[0],r[0]);  TEST_VEC2(e[1],r[1]);

namespace inviwo {



    template<typename C>
    std::shared_ptr<Image> createTestImage(C callback){
        auto img = std::make_shared<Image>(size2_t(10,10) , DataVec2Float32::get());
        auto data = static_cast<vec2*>( img->getColorLayer()->getEditableRepresentation<LayerRAM>()->getData() );


        const size_t size = 10;
        size_t idx = 0;
        for(int j = 0;j<size;j++)for(int i = 0;i<size;i++){
            vec2 p(i,j);
            p /= size-1;
            p -= 0.5;
            data[idx++] = callback(p);
        }
        return img;
    }

    std::vector<std::pair<std::function<vec2(const vec2 &)>, mat2> > testValues =
    { 
          {[](const vec2 &p) { return p; }, mat2(1, 0, 0, 1)}
        , {[](const vec2 &p) { return vec2(p.y, p.x); }, mat2(0, 1, 1, 0)}
        , {[](const vec2 &p) { return vec2(-p.y, p.x); }, mat2(0, 1, -1, 0)}
        , {[](const vec2 &p) { return vec2(p.x+p.y, p.x+p.y); }, mat2(1, 1, 1, 1)}
    };


    const std::vector<vec2> points = {
        {0.25f,0.25f},
        {0.5f,0.25f},
        {0.75f,0.25f} ,
        {0.25f,0.5f},
        {0.5f,0.5f},
        {0.75f,0.5f} ,
        {0.25f,0.75f},
        {0.5f,0.57f},
        {0.75f,0.75f}
    };

    template<typename T>
    size_t tupleSize(T t){
        return std::tuple_size<T>::value
    }


    TEST(JacobianTest, FullTest) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                TEST_MAT2(e, r);
            }
        }
    }

    TEST(JacobianTest, IgnoringScaling ) {
        for(auto &testValue  : testValues){
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for(const auto &p : points){
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                r[0] = glm::normalize(r[0]);
                r[1] = glm::normalize(r[1]);
                auto e = testValue.second;
                e[0] = glm::normalize(e[0]);
                e[1] = glm::normalize(e[1]);
                TEST_MAT2(e,r);
            }
        }
    }


    TEST(JacobianTest, OnlyDiagonal) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[0][0], r[0][0], EPS);
                ASSERT_NEAR(e[1][1], r[1][1], EPS);
            }
        }
    }



    TEST(JacobianTest, OnlyNotDiagonal) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[0][1], r[0][1], EPS);
                ASSERT_NEAR(e[1][0], r[1][0], EPS);
            }
        }
    }
   


    TEST(JacobianTest, OnlyCol0Row0) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[0][0], r[0][0], EPS);
            }
        }
    }



    TEST(JacobianTest, OnlyCol0Row1) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[0][1], r[0][1], EPS);
            }
        }
    }



    TEST(JacobianTest, OnlyCol1Row0) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[1][0], r[1][0], EPS);
            }
        }
    }



    TEST(JacobianTest, OnlyCol1Row1) {
        for (auto &testValue : testValues) {
            auto img = createTestImage(testValue.first);
            ImageSampler s(img);
            for (const auto &p : points) {
                auto r = util::jacobian(s, p, vec2(0.1, 0.1));
                auto e = testValue.second;
                ASSERT_NEAR(e[1][1], r[1][1], EPS);
            }
        }
    }







}