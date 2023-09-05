#include "pch.h"
#include <QPainterPath>

#include "EditorUINode.hpp"
#include "nodeflow/utility/dbgln.hpp"
#include "nodeflow/reflection/type_reflection.hpp"

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
		const auto roundness = 10;

		painter->setRenderHint(QPainter::Antialiasing);
		painter->setRenderHint(QPainter::NonCosmeticDefaultPen, false);
		painter->setRenderHint(QPainter::TextAntialiasing);
		painter->setBrush(QBrush(QColor(31, 33, 37)));
		painter->setPen(QPen(Qt::transparent, 2));

		painter->drawRoundedRect(geometry, roundness, roundness);
	}

	void DefaultNodePainter::paintSelected(QPainter* painter, const QRectF& geometry, const EditorUINode& node)
	{
		const auto roundness = 10;

		painter->setPen(QPen(QColor(225, 163, 0), 3));
		painter->setBrush(Qt::NoBrush);
		painter->drawRoundedRect(geometry, roundness, roundness);
	}

	void DefaultNodePainter::paintError(QPainter* painter, const QRectF& geometry, const EditorUINode& node)
	{
		const auto roundness = 10;

		static const QIcon warningIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning));
		warningIcon.paint(painter, geometry.toRect().adjusted(10, 10, -10, 0), Qt::AlignTop | Qt::AlignRight);

		auto color = QColor(Qt::red);
		painter->setPen(QPen(color, 5, Qt::PenStyle::DashLine));
		color.setAlphaF(0.1);
		painter->setBrush(QBrush(color, Qt::SolidPattern));
		painter->drawRoundedRect(geometry, roundness, roundness);
	}

	void DefaultPortPainter::paintDataPort(QPainter* painter, QRectF& geom, const UINodeDataPort& port, const QString& caption, bool connected, const QColor& color)
	{
		const auto circleRadius = geom.height() / 3;
		static QRectF circleBBox(geom.x() + 3, geom.center().y() / 3., 2 * circleRadius, 2 * circleRadius);
		static QPointF triangle[3] = {
			QPointF(circleBBox.left() + 1.6 * circleRadius, circleBBox.center().y() - 0.85 * circleRadius),
			QPointF(circleBBox.left() + 2.6 * circleRadius, circleBBox.center().y()),
			QPointF(circleBBox.left() + 1.6 * circleRadius, circleBBox.center().y() + 0.85 * circleRadius)
		};

		// Triangle
		QPen pen(color, 2);
		pen.setWidth(2);
		pen.setCapStyle(Qt::RoundCap);
		painter->setPen(pen);
		painter->setBrush(color);
		painter->drawPolygon(triangle, 3);

		// Circle
		pen.setWidth(3);
		painter->setPen(pen);
		painter->setBrush(QColor(31, 33, 37));
		painter->drawEllipse(circleBBox);

		if (connected)
		{
			painter->setBrush(color);
			painter->drawEllipse(circleBBox.center(), circleRadius / 2, circleRadius / 2);
		}

		// draw caption
		painter->setBrush(Qt::NoBrush);
		QFont captionFont(painter->font());
		captionFont.setPointSize(circleRadius * 1.2);
		painter->setFont(captionFont);
		painter->setPen(Qt::lightGray);
		painter->drawText(geom.adjusted(4 * circleRadius, 0, 0, 0), Qt::AlignVCenter, caption);

		// when selected
		if (port.isSelected())
		{
			painter->setPen(Qt::NoPen);
			auto selectedColor = color;
			selectedColor.setAlphaF(0.3);
			painter->setBrush(selectedColor);
			painter->drawRoundedRect(geom, 8, 8);
		}
	}

	void DefaultPortPainter::paintFlowPort(QPainter* painter, QRectF& geom, const UINodeFlowPort& port, bool connected)
	{
		const auto height = geom.height() * 2 / 3;
		const auto width = height / 1.2;
		const auto offset = 3;
		static QPointF drawPath[5] = {
		   QPointF(offset, geom.center().y() - height / 2),
		   QPointF(offset + width / 2, geom.center().y() - height / 2),
		   QPointF(offset + width, geom.center().y()),
		   QPointF(offset + width / 2, geom.center().y() + height / 2),
		   QPointF(offset , geom.center().y() + height / 2)
		};

		QPen pen(Qt::white, 2);
		pen.setCapStyle(Qt::RoundCap);
		painter->setPen(pen);
		painter->setBrush(connected ? QBrush(Qt::white) : Qt::NoBrush);
		painter->drawPolygon(drawPath, 5);

		// when selected
		if (port.isSelected())
		{
			painter->setPen(Qt::NoPen);
			auto selectedColor = QColor(Qt::white);
			selectedColor.setAlphaF(0.3);
			painter->setBrush(selectedColor);
			painter->drawRoundedRect(geom.adjusted(-3, 0, -5, 0), 5, 5);
		}
	}

	UINodeDataPort::UINodeDataPort(QGraphicsItem* parent /*= nullptr*/)
		: UINodeDataPort(std::make_shared<DefaultPortPainter>(), parent)
	{
	}

	UINodeDataPort::UINodeDataPort(std::shared_ptr<PortPainterDelegate> portPainter, QGraphicsItem* parent /*= nullptr*/)
		: QGraphicsItem(parent), m_portGeometry(0, 0, 30, 30), m_portPainter(portPainter)
	{
		m_caption = QList<QString>({ "Attack Time", "Decay Time", "Count", "Value", "Index", "Array" })[qrand() % 5];
		m_portGeometry.setWidth(m_portGeometry.width() + 2 * QFontMetrics(QApplication::font()).width(m_caption));

		m_color = QList<QColor>({ QColor(153, 193, 115), QColor(149, 59, 75), QColor(86, 165, 147) })[qrand() % 3];

		setFlag(QGraphicsItem::ItemIsSelectable);
		setAcceptHoverEvents(true);

		setToolTip("Integer");
	}

	QRectF UINodeDataPort::boundingRect() const
	{
		return m_portGeometry.marginsAdded(QMarginsF(5.0, 5.0, 5.0, 5.0));
	}

	void UINodeDataPort::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */)
	{
		m_portPainter->paintDataPort(painter, m_portGeometry, *this, m_caption, bool(qrand() % 2), m_color);

#if DEBUG_DRAW_BOUNDING_RECT == 1
		painter->setPen(QPen(Qt::red, 1));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(boundingRect());
#endif
	}

	bool UINodeDataPort::connected() const noexcept
	{
		return true;
	}

	void UINodeDataPort::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
	{
		setSelected(true);

		QMenu contextMenu;
		contextMenu.addAction("Node Action 1");
		contextMenu.addAction("Node Action 2");
		contextMenu.exec(event->screenPos());

		setSelected(false);
	}

	UINodeFlowPort::UINodeFlowPort(QGraphicsItem* parent /*= nullptr*/)
		: UINodeFlowPort(std::make_shared<DefaultPortPainter>(), parent)
	{
		setFlag(QGraphicsItem::ItemIsSelectable);
		setAcceptHoverEvents(true);
	}

	UINodeFlowPort::UINodeFlowPort(std::shared_ptr<PortPainterDelegate> portPainter, QGraphicsItem* parent /*= nullptr*/)
		: QGraphicsItem(parent), m_portGeometry(0, 0, 30, 30), m_portPainter(portPainter)
	{
		setToolTip("Flow port");
	}

	QRectF UINodeFlowPort::boundingRect() const
	{
		return m_portGeometry.marginsAdded(QMarginsF(5.0, 5.0, 5.0, 5.0));
	}

	void UINodeFlowPort::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */)
	{
		m_portPainter->paintFlowPort(painter, m_portGeometry, *this, bool(qrand() % 2));

#if DEBUG_DRAW_BOUNDING_RECT == 1
		painter->setPen(QPen(Qt::red, 1));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(boundingRect());
#endif
	}

	bool UINodeFlowPort::connected() const noexcept
	{
		return true;
	}

	EditorUINode::EditorUINode()
		: EditorUINode(std::make_unique<DefaultNodePainter>(), nullptr, nullptr)
	{
	}

	EditorUINode::EditorUINode(std::unique_ptr<NodePainterDelegate> nodePainter, std::shared_ptr<PortPainterDelegate> portPainter, QGraphicsObject* parent /*= nullptr*/)
		: QGraphicsObject(parent), m_nodeGemoetry(0, 0, 400, 250), m_nodePainter(std::move(nodePainter)), m_portPainter(portPainter)
	{
		setFlag(QGraphicsItem::ItemIsSelectable);
		setFlag(QGraphicsItem::ItemIsMovable);
		setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		setFlag(QGraphicsItem::ItemIsFocusable);
		setAcceptDrops(true);
		setAcceptHoverEvents(true);

		auto flowPort = new UINodeFlowPort(this);
		flowPort->setPos(20, m_nodeGemoetry.height() * 0.25);

		auto inputPortCount = 3;
		QPointF portCoord(20, m_nodeGemoetry.height() * 0.3 + 40);
		for (size_t i = 0; i < inputPortCount; i++)
		{
			auto port = new UINodeDataPort(this);
			port->setPos(portCoord);
			portCoord.setY(portCoord.y() + 40);
		}
	}

	void EditorUINode::setNodePainterDelegate(std::unique_ptr<NodePainterDelegate> painter)
	{
		m_nodePainter = std::move(painter);
	}

	void EditorUINode::setPortPainterDelegate(std::shared_ptr<PortPainterDelegate> painter)
	{
		m_portPainter = painter;
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

		if (isInvalid())
			m_nodePainter->paintError(painter, m_nodeGemoetry, *this);

#if DEBUG_DRAW_BOUNDING_RECT == 1
		painter->setPen(QPen(Qt::red, 1));
		painter->drawRect(boundingRect());
#endif
	}
}