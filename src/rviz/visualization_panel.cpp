/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "visualization_panel.h"
#include "render_panel.h"
#include "displays_panel.h"
#include "visualization_manager.h"

#include <ros/package.h>
#include <ros/console.h>

#include <ogre_tools/initialization.h>

#include <wx/splitter.h>

#include <ros/node.h>

namespace rviz
{

VisualizationPanel::VisualizationPanel(wxWindow* parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(800,600), wxTAB_TRAVERSAL)
{
  if (!ros::Node::instance())
  {
    int argc;
    ros::init(argc, 0, "rviz", ros::init_options::AnonymousName);
  }

  wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(sizer);

  wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);

  render_panel_ = new RenderPanel( splitter );
  displays_panel_ = new DisplaysPanel( splitter );

  splitter->SplitVertically(displays_panel_, render_panel_, 300);
  sizer->Add(splitter, 1, wxEXPAND);

  std::string mediaPath = ros::package::getPath( "pr2_ogre" );
  mediaPath += "/Media/";
  ogre_tools::V_string paths;
  paths.push_back( mediaPath );
  paths.push_back( mediaPath + "fonts" );
  paths.push_back( mediaPath + "materials" );
  paths.push_back( mediaPath + "materials/scripts" );
  paths.push_back( mediaPath + "materials/programs" );
  paths.push_back( mediaPath + "materials/textures" );
  paths.push_back( mediaPath + "models" );
  paths.push_back( mediaPath + "models/pr2" );

  ogre_tools::initializeResources( paths );

  manager_ = new VisualizationManager(render_panel_);
  render_panel_->initialize(manager_);
  displays_panel_->initialize(manager_);

  manager_->initialize();

  Layout();
}

VisualizationPanel::~VisualizationPanel()
{
  render_panel_->Destroy();
  delete manager_;
}

}
