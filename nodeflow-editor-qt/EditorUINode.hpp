#pragma once

#include <memory>
#include <string>

#include <QGraphicsObject>

# define DEBUG_DRAW_BOUNDING_RECT 0

namespace nf
{
	class EditorUINode;
	class NodePainterDelegate
	{
	public:
		NodePainterDelegate() = default;
		~NodePainterDelegate() = default;

		virtual void paintNodeCaption(QPainter* painter, const QRectF& geometry, const EditorUINode& node) = 0;

		virtual void paintNodeBody(QPainter* painter, const QRectF& geometry, const EditorUINode& node) = 0;

		virtual void paintSelected(QPainter* painter, const QRectF& geometry, const EditorUINode& node) = 0;
	};

	class PortPainterDelegate
	{
	public:
		PortPainterDelegate() = default;
		~PortPainterDelegate() = default;
	};

	class DefaultNodePainter : public NodePainterDelegate
	{
	public:
		void paintNodeCaption(QPainter* painter, const QRectF& geometry, const EditorUINode& node) override;

		void paintNodeBody(QPainter* painter, const QRectF& geometry, const EditorUINode& node) override;

		void paintSelected(QPainter* painter, const QRectF& geometry, const EditorUINode& node) override;
	};

	class TemporayNodeDrawContext
	{
		unsigned int portInCount = 0;
		unsigned int portOutCount = 0;
		std::string name;
	};

	class EditorUINode : public QGraphicsObject
	{
	public:
		EditorUINode();

		EditorUINode(std::unique_ptr<NodePainterDelegate> nodePainter, std::unique_ptr<PortPainterDelegate> portPainter, QGraphicsObject* parent = nullptr);

		void setNodePainterDelegate(std::unique_ptr<NodePainterDelegate> painter);

		void setPortPainterDelegate(std::unique_ptr<PortPainterDelegate> painter);

		QRectF boundingRect() const override;

		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */) override;

	private:
		QRectF m_nodeGemoetry;

		std::unique_ptr<NodePainterDelegate> m_nodePainter;
		std::unique_ptr<PortPainterDelegate> m_portPainter;
	};
}