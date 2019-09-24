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

#include <modules/tnm067lab3/tnm067lab3module.h>
#include <modules/tnm067lab3/processors/lineintegralconvolution.h>
#include <modules/tnm067lab3/processors/vectorfieldinformation.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo {

TNM067Lab3Module::TNM067Lab3Module(InviwoApplication* app) : InviwoModule(app, "TNM067Lab3") {   
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:
    
    // Processors
    registerProcessor<LineIntegralConvolution>();
    registerProcessor<VectorFieldInformation>();
    
    // Properties
    // registerProperty<TNM067Lab3Property>();
    
    // Readers and writes
    // registerDataReader(util::make_unique<TNM067Lab3Reader>());
    // registerDataWriter(util::make_unique<TNM067Lab3Writer>());
    
    // Data converters
    // registerRepresentationConverter(util::make_unique<TNM067Lab3Disk2RAMConverter>());

    // Ports
    // registerPort<TNM067Lab3Outport>("TNM067Lab3Outport");
    // registerPort<TNM067Lab3Inport>("TNM067Lab3Inport");

    // PropertyWidgets
    // registerPropertyWidget<TNM067Lab3PropertyWidget, TNM067Lab3Property>("Default");
    
    // Dialogs
    // registerDialog<TNM067Lab3Dialog>(TNM067Lab3Outport);
    
    // Other varius things
    // registerCapabilities(util::make_unique<TNM067Lab3Capabilities>());
    // registerSettings(util::make_unique<TNM067Lab3Settings>());
    // registerMetaData(util::make_unique<TNM067Lab3MetaData>());   
    // registerPortInspector("TNM067Lab3Outport", "path/workspace.inv");
    // registerProcessorWidget(std::string processorClassName, std::unique_ptr<ProcessorWidget> processorWidget);
    // registerDrawer(util::make_unique_ptr<TNM067Lab3Drawer>());  
}

} // namespace
