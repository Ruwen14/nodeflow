#pragma once

struct Painter;
struct EditorStyle;
struct ConnectionStyle;
struct NodeStyle;
struct Point;

template <typename Impl>
class ANodeFlowPainter
{
public:
    void paintEditorSceneIntertnal(Painter* painter, EditorStyle editor_style);

    void paintNodeInternal(Painter* painter, NodeStyle node_style);

public:
    void drawEditorBackground(Painter* painter, EditorStyle editor_style);

    void drawEditorForeground(Painter* painter, EditorStyle editor_style);

    void drawNodePin(Painter* painter, NodeStyle node_style);

    void drawNodeBody(Painter* painter, NodeStyle node_style);

    void drawNodeCaption(Painter* painter, NodeStyle node_style);

    void drawNodeConnection(Painter* painter,
                            const Point& startPoint,
                            const Point& endPoint,
                            ConnectionStyle connection_syle);

private:
    Impl& impl()
    {
        return *static_cast<Impl*>(this);
    }
    const Impl& impl() const
    {
        return *static_cast<Impl const*>(this);
    }
};