#include "pch.h"
#include <QPainterPath>

#include "EditorUINode.hpp"
#include "nodeflow/utility/dbgln.hpp"

namespace nf
{
	void DefaultNodePainter::paintNodeCaption(QPainter* painter, const QRectF& geometry, const EditorUINode& node)
	{
		static QFont cursiveFFont("Times New Roman", 22, QFont::Bold, true);

		auto x = geometry.x();
		auto y = geometry.y();
		auto width = geometry.width();
		auto height = geometry.height();
		auto roundness = 10;

		// Draw label rect. Top left and top right corners are rounded
		QPainterPath path;
		path.moveTo(x + roundness, y);
		path.arcTo(x, y, 2 * roundness, 2 * roundness, 90, 90);
		path.lineTo(x, y + height);
		path.lineTo(x + width, y + height);
		path.arcTo(x + width - 2 * roundness, y, 2 * roundness, 2 * roundness, 0, 90);
		path.closeSubpath();
		painter->setBrush(QBrush(QGradient(QGradient::MidnightBloom)));
		painter->setPen(Qt::NoPen);
		painter->drawPath(path);

		// Draw node name and node category
		auto font = painter->font();
		font.setHintingPreference(QFont::PreferNoHinting);
		font.setPointSize(14);
		font.setBold(true);
		painter->setFont(font);
		painter->setPen(Qt::lightGray);
		painter->drawText(geometry.left() + 0.1 * geometry.width(), geometry.center().y(), "Example Node");
		font.setItalic(true);
		font.setBold(false);
		font.setPointSize(12);
		painter->setFont(font);
		painter->setPen(Qt::darkGray);
		painter->drawText(geometry.left() + 0.1 * geometry.width(), geometry.center().y() + font.pointSize() * 1.4, "Math");

		// Draw cursive 'f' icon symbol
		painter->setFont(cursiveFFont);
		painter->setPen(QColor(98, 162, 206));
		painter->drawText(geometry.top() + 0.04 * geometry.width(), geometry.center().y() + cursiveFFont.pointSize() / 3, "f");
	}

	void DefaultNodePainter::paintNodeBody(QPainter* painter, const QRectF& geometry, const EditorUINode& node)
	{
		painter->setRenderHint(QPainter::Antialiasing);
		painter->setRenderHint(QPainter::NonCosmeticDefaultPen, false);
		painter->setRenderHint(QPainter::TextAntialiasing);
		painter->setBrush(QBrush(QColor(31, 33, 37)));
		painter->setPen(QPen(Qt::transparent, 2));

		auto roundness = 10;
		painter->drawRoundedRect(geometry, roundness, roundness);
	}

	void DefaultNodePainter::paintSelected(QPainter* painter, const QRectF& geometry, const EditorUINode& node)
	{
		painter->setPen(QPen(QColor(225, 163, 0), 3));

		auto roundness = 10;
		painter->drawRoundedRect(geometry, roundness, roundness);
	}

	EditorUINode::EditorUINode()
		: EditorUINode(std::make_unique<DefaultNodePainter>(), nullptr, nullptr)
	{
	}

	EditorUINode::EditorUINode(std::unique_ptr<NodePainterDelegate> nodePainter, std::unique_ptr<PortPainterDelegate> portPainter, QGraphicsObject* parent /*= nullptr*/)
		: m_nodePainter(std::move(nodePainter)), m_portPainter(std::move(portPainter)), QGraphicsObject(parent)
	{
		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIsFocusable);
		setAcceptDrops(true);
		setAcceptHoverEvents(true);

		m_nodeGemoetry.setX(0);
		m_nodeGemoetry.setY(0);
		m_nodeGemoetry.setWidth(400);
		m_nodeGemoetry.setHeight(250);
	}

	void EditorUINode::setNodePainterDelegate(std::unique_ptr<NodePainterDelegate> painter)
	{
		m_nodePainter = std::move(painter);
	}

	void EditorUINode::setPortPainterDelegate(std::unique_ptr<PortPainterDelegate> painter)
	{
		m_portPainter = std::move(painter);
	}

	QRectF EditorUINode::boundingRect() const
	{
		// We need to add padding to the boundingRectangle, otherwise the borders of
		// the node are clipped weirdly during moving the item in the scene
		return m_nodeGemoetry.marginsAdded(QMarginsF(10.0, 10.0, 10.0, 10.0));
	}

	void EditorUINode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */)
	{
		painter->setClipRect(option->exposedRect);
		painter->save();
		m_nodePainter->paintNodeBody(painter, m_nodeGemoetry, *this);
		painter->restore();
		painter->save();
		m_nodePainter->paintNodeCaption(painter, QRectF(m_nodeGemoetry.x(), m_nodeGemoetry.y(), m_nodeGemoetry.width(), m_nodeGemoetry.height() * 0.2), *this);
		painter->restore();

		if (isSelected())
			m_nodePainter->paintSelected(painter, m_nodeGemoetry, *this);

#if DEBUG_DRAW_BOUNDING_RECT == 1
		painter->setPen(QPen(Qt::red, 1));
		painter->drawRect(boundingRect());
#endif
	}
}