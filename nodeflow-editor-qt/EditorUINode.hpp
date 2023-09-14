#pragma once

#include <memory>
#include <string>

#include <QGraphicsObject>

#define DEBUG_DRAW_BOUNDING_RECT 0

namespace nf
{
class EditorUINode;
class UINodeDataPort;
class UINodeFlowPort;

class NodePainterDelegate
{
public:
    NodePainterDelegate() = default;
    ~NodePainterDelegate() = default;

    virtual void paintNodeCaption(QPainter* painter,
                                  const QRectF& geometry,
                                  const EditorUINode& node) = 0;

    virtual void paintNodeBody(QPainter* painter,
                               const QRectF& geometry,
                               const EditorUINode& node) = 0;

    virtual void paintSelected(QPainter* painter,
                               const QRectF& geometry,
                               const EditorUINode& node) = 0;

    virtual void paintError(QPainter* painter,
                            const QRectF& geometry,
                            const EditorUINode& node) = 0;
};

class PortPainterDelegate
{
public:
    PortPainterDelegate() = default;
    ~PortPainterDelegate() = default;

    virtual void paintDataPort(QPainter* painter,
                               QRectF& geom,
                               const UINodeDataPort& port,
                               const QString& caption,
                               bool connected,
                               const QColor& color) = 0;

    virtual void paintFlowPort(QPainter* painter,
                               QRectF& geom,
                               const UINodeFlowPort& port,
                               bool connected) = 0;
};

class DefaultNodePainter : public NodePainterDelegate
{
public:
    void paintNodeCaption(QPainter* painter,
                          const QRectF& geometry,
                          const EditorUINode& node) override;

    void paintNodeBody(QPainter* painter,
                       const QRectF& geometry,
                       const EditorUINode& node) override;

    void paintSelected(QPainter* painter,
                       const QRectF& geometry,
                       const EditorUINode& node) override;

    void paintError(QPainter* painter,
                    const QRectF& geometry,
                    const EditorUINode& node) override;
};

class DefaultPortPainter : public PortPainterDelegate
{
public:
    void paintDataPort(QPainter* painter,
                       QRectF& geom,
                       const UINodeDataPort& port,
                       const QString& caption,
                       bool connected,
                       const QColor& color) override;

    void paintFlowPort(QPainter* painter,
                       QRectF& geom,
                       const UINodeFlowPort& port,
                       bool connected) override;
};

class TemporayNodeDrawContext
{
    unsigned int portInCount = 0;
    unsigned int portOutCount = 0;
    std::string name;
};

class UINodeDataPort : public QGraphicsItem
{
public:
    UINodeDataPort(QGraphicsItem* parent = nullptr);

    UINodeDataPort(std::shared_ptr<PortPainterDelegate> portPainter,
                   QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget /* = nullptr */) override;

    bool connected() const noexcept;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    QRectF m_portGeometry;

    std::shared_ptr<PortPainterDelegate> m_portPainter;
    QString m_caption;
    QColor m_color;
};

class UINodeFlowPort : public QGraphicsItem
{
public:
    UINodeFlowPort(QGraphicsItem* parent = nullptr);

    UINodeFlowPort(std::shared_ptr<PortPainterDelegate> portPainter,
                   QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget /* = nullptr */) override;

    bool connected() const noexcept;

private:
    QRectF m_portGeometry;
    std::shared_ptr<PortPainterDelegate> m_portPainter;
};

class EditorUINode : public QGraphicsObject
{
public:
    EditorUINode();

    EditorUINode(std::unique_ptr<NodePainterDelegate> nodePainter,
                 std::shared_ptr<PortPainterDelegate> portPainter,
                 QGraphicsObject* parent = nullptr);

    void setNodePainterDelegate(std::unique_ptr<NodePainterDelegate> painter);

    void setPortPainterDelegate(std::shared_ptr<PortPainterDelegate> painter);

    QRectF boundingRect() const override;

    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget /* = nullptr */) override;

    void setInvalid()
    {
    }

    bool isInvalid()
    {
        return false;
    }

private:
    QRectF m_nodeGemoetry;

    std::unique_ptr<NodePainterDelegate> m_nodePainter;
    std::shared_ptr<PortPainterDelegate> m_portPainter;
};
} // namespace nf