#pragma once
#include "nodeflow-gui/NodeGeometry.hpp"
#include "nodeflow-gui/NodeStyle.hpp"

namespace nf::imgui
{
class ImGuiNodeWidget
{
public:
    void begin();
    void end();

    virtual void onEvent() = 0;

    virtual void drawBody(const NodeGeometry& geom, const NodeStyle& style) = 0;

    virtual void drawPort(const NodeGeometry& geom, const NodeStyle& style) = 0;

    virtual void drawHeader(const NodeGeometry& geom, const NodeStyle& style) = 0;

    virtual void drawHighlighted(const NodeGeometry& geom, const NodeStyle& style) = 0;

private:
    virtual void draw() = 0;
};
} // namespace nf::imgui