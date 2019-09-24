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

#include <modules/tnm067lab3/processors/vectorfieldinformation.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/shader/shaderutils.h>

namespace inviwo {

const ProcessorInfo VectorFieldInformation::processorInfo_{
    "org.inviwo.VectorFieldInformation",  // Class identifier
    "Vector Field Information",           // Display name
    "Vector Field Visualization",         // Category
    CodeState::Experimental,              // Code state
    Tags::None,                           // Tags
};
const ProcessorInfo VectorFieldInformation::getProcessorInfo() const {
    return processorInfo_;
}


VectorFieldInformation::VectorFieldInformation()
    : Processor()
    , vf_("vf")
    , outport_("outport" , DataFloat32::get())
    , outputType_("outputType","Output" , InvalidationLevel::InvalidResources)
    , outputTypeStr_("outputTypeStr","Output" , "Pass Through" , InvalidationLevel::Valid)
    , shader_("vectorfieldinformation.vert","vectorfieldinformation.frag" , false)
{
    
    outputTypeStr_.setReadOnly(true);

    addPort(vf_);
    addPort(outport_);
    addProperty(outputType_);
    addProperty(outputTypeStr_);


    shader_.onReload([this]() {invalidate(InvalidationLevel::InvalidOutput); });

    outputType_.addOption("passThoruh", "Vector pass through", Information::PassThoruh);
    outputType_.addOption("magnitude", "Vector magnitude", Information::Magnitude);
    outputType_.addOption("divergence", "Divergence" , Information::Divergence);
    outputType_.addOption("rotation", "Rotation" , Information::Rotation);
    outputType_.setCurrentStateAsDefault();

    outputType_.onChange([this]() {
        outputTypeStr_.set( outputType_.getSelectedDisplayName() );
    });
}
    

void VectorFieldInformation::initializeResources() {
    const static std::string outputKey = "OUTPUT(texCoord_)";
    std::string output = "";

    switch (outputType_.get()) {
        case Information::Rotation:
            output = "rotation(texCoord_.xy);";
            break;
        case Information::Divergence:
            output = "divergence(texCoord_.xy);";
            break;
        case Information::Magnitude:
            output = "magnitude(texCoord_.xy);";
            break;
        case Information::PassThoruh:
        default:
            output = "passThrough(texCoord_.xy);";
            break;
    }

    shader_.getFragmentShaderObject()->addShaderDefine(outputKey, output);
    shader_.build();

}

void VectorFieldInformation::process() {
    outport_.getEditableData()->getColorLayer()->setSwizzleMask(swizzlemasks::luminance);
    utilgl::activateAndClearTarget(outport_);
    shader_.activate();
    TextureUnitContainer units;
    utilgl::bindAndSetUniforms(shader_, units, vf_, ImageType::ColorOnly);

    utilgl::setUniforms(shader_, outport_);

    utilgl::singleDrawImagePlaneRect();
    shader_.deactivate();
    utilgl::deactivateCurrentTarget();
}

} // namespace

