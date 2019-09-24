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

#include <modules/tnm067lab4/processors/glyphrenderer.h>

#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/shader/shaderutils.h>

#include <inviwo/core/util/imagesampler.h>
#include <modules/tnm067lab4/jacobian.h>

#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/rendering/meshdrawergl.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

const ProcessorInfo GlyphRenderer::processorInfo_{
    "org.inviwo.TNM067GlyphRenderer", // Class identifier
    "Glyph Renderer",                 // Display name
    "TNM067",                         // Category
    CodeState::Experimental,          // Code state
    Tags::None,                       // Tags
};

const ProcessorInfo GlyphRenderer::getProcessorInfo() const {
    return processorInfo_;
}

GlyphRenderer::GlyphRenderer()
    : Processor()
    , background_("background")
    , vf_("vf")
    , outport_("outport")
    , glyphScale_("glyphScale", "Glyph Scale", 1, 0, 10)
    , gridSize_("gridSize", "Grid Size (N x N)", 5, 3, 30)
    , includeGrid_("includeGrid", "Ellipses On Grid", true)
    , includeMousePos_("includeMousePos_", "Ellipse under Mouse Position", false)
    , shader_("tensor_glyphrenderer.vert", "tensor_glyphrenderer.geom", "tensor_glyphrenderer.frag")

    , mouseMoveEvent_("mouseMoveEvent", "Mouse Move",
                      [&](Event *e) {
                          if (auto me = dynamic_cast<MouseEvent *>(e)) {
                              lastMousePos_ = me->posNormalized();
                              mesh_ = nullptr;
                              invalidate(InvalidationLevel::InvalidOutput);
                          }
                      },
                      MouseButton::None, MouseState::Move)

    , mesh_(nullptr)
    , drawer_(nullptr)

    , lastMousePos_(0.5f) {
    addPort(background_);
    addPort(vf_);
    addPort(outport_);

    addProperty(glyphScale_);
    addProperty(gridSize_);
    addProperty(includeGrid_);
    addProperty(includeMousePos_);
    
    addProperty(mouseMoveEvent_);

    includeGrid_.onChange([&]() {mesh_ = nullptr; });
    includeMousePos_.onChange([&]() {mesh_ = nullptr; });
    vf_.onChange([&]() {mesh_ = nullptr; });
    gridSize_.onChange([&]() {mesh_ = nullptr; });

    shader_.onReload([this](){ invalidate(InvalidationLevel::InvalidResources);});
}

GlyphRenderer::~GlyphRenderer() {}

void GlyphRenderer::process() {
    if (!mesh_ || !drawer_) { buildMesh(); }
    if (!mesh_) { LogError("Failed to build mesh"); return; }
    if (!drawer_) { LogError("Failed to create drawer"); return; }
    


    utilgl::activateTargetAndCopySource(outport_,background_,ImageType::ColorOnly);

    shader_.activate();
    TextureUnitContainer units;

    shader_.setUniform("radius", glyphScale_.get() * 0.005f);

    drawer_->draw();
    shader_.deactivate();
    utilgl::deactivateCurrentTarget();



}


void GlyphRenderer::buildMesh() {
    if(!vf_.hasData()){
        return;
    }
    
    std::vector<vec2> pos;

    if(includeGrid_){
        auto s = gridSize_.get();
        float ds = 1.0 / (s);
        for (size_t i = 0; i < s; i++) {
            float x = ds * (i + 0.5f);
            for (size_t j = 0; j < s; j++) {
                float y = ds * (j + 0.5f);
                pos.emplace_back(x,y);
            }
        }
    }

    if(includeMousePos_){
        pos.push_back(lastMousePos_);
    }

    ImageSampler sampler(vf_.getData());
    
    vec2 offset =  1.0f / vec2(vf_.getData()->getDimensions() - size2_t(1));
    

    mesh_ = util::make_unique<Mesh>();
    auto verticesBuf = std::make_shared<Buffer<vec3>>();
    auto jacobiansBuf = std::make_shared<Buffer<vec4>>();
    auto indicesBuf_ = std::make_shared<IndexBuffer>(std::make_shared<IndexBufferRAM>());
    
    auto &vertices = verticesBuf->getEditableRAMRepresentation()->getDataContainer();
    auto &jacobians = jacobiansBuf->getEditableRAMRepresentation()->getDataContainer();
    auto &indices = indicesBuf_->getEditableRAMRepresentation()->getDataContainer();    
    
    mesh_->addBuffer(BufferType::PositionAttrib, verticesBuf);  
    mesh_->addBuffer(BufferType::ColorAttrib, jacobiansBuf);
    mesh_->addIndicies(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), indicesBuf_);
    
    vertices.reserve(pos.size());
    jacobians.reserve(pos.size());
    indices.reserve(pos.size());

    for(const auto &p : pos){
        vertices.emplace_back( p,0 );
        auto J = util::jacobian(sampler,p,offset);
        jacobians.emplace_back(J[0], J[1]);
        indices.push_back(static_cast<std::uint32_t>(indices.size()));
    }

    drawer_ = util::make_unique<MeshDrawerGL>(mesh_.get());
}



} // namespace

