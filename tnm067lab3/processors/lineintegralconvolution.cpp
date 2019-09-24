/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2016 Inviwo Foundation
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

#include <modules/tnm067lab3/processors/lineintegralconvolution.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/shader/shaderutils.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo LineIntegralConvolution::processorInfo_{
    "org.inviwo.LineIntegralConvolution",  // Class identifier
    "Line Integral Convolution",           // Display name
    "Vector Field Visualization",          // Category
    CodeState::Experimental,               // Code state
    Tags::None,                            // Tags
};
const ProcessorInfo LineIntegralConvolution::getProcessorInfo() const {
    return processorInfo_;
}

LineIntegralConvolution::LineIntegralConvolution()
    : Processor()
    , vf_("vf")
    , noise_("noise")
    , outport_("outport")

    , steps_("steps", "Steps", 20, 3, 100)
    , stepSize_("stepSize", "stepSize", 0.003f, 0.0001f, 0.01f, 0.0001f)

    , shader_("lineintegralconvolution.vert","lineintegralconvolution.frag")
{
    
    addPort(vf_);
    addPort(noise_);
    addPort(outport_);
    addProperty(steps_ );
    addProperty(stepSize_);

    shader_.onReload([this]() {invalidate(InvalidationLevel::InvalidOutput); });
}
    
void LineIntegralConvolution::process() {
    utilgl::activateAndClearTarget(outport_);

    shader_.activate();
    TextureUnitContainer units;
    utilgl::bindAndSetUniforms(shader_, units, vf_, ImageType::ColorOnly);
    utilgl::bindAndSetUniforms(shader_, units, noise_, ImageType::ColorOnly);

    utilgl::setUniforms(shader_, outport_, steps_, stepSize_);

    utilgl::singleDrawImagePlaneRect();
    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

} // namespace

