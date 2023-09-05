#include "pch.h"
#include <cmath>

#include "FlowEditor.hpp"
#include "Panels.hpp"

#include "nodeflow/reflection/type_reflection.hpp"
#include "nodeflow/utility/dbgln.hpp"

#include "EditorUINode.hpp"

namespace nf {
	class FlowEditorSceneView : public QGraphicsView
	{
	public:
		FlowEditorSceneView(QGraphicsScene* scene, QWidget* parent = nullptr)
			: QGraphicsView(scene, parent)
		{
			setBackgroundBrush(QColor(53, 53, 53));
			setDragMode(QGraphicsView::ScrollHandDrag);
			setRenderHint(QPainter::Antialiasing, true);
			setRenderHint(QPainter::TextAntialiasing, true);
			setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
			setCacheMode(QGraphicsView::CacheBackground);
			setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
			setRenderHint(QPainter::Antialiasing);
		}

		void zoomInStep()
		{
			auto factor = std::pow(1.2, 1.0);
			auto t = transform();

			if (t.m11() > 2.0)
				return;
			scale(factor, factor);
		}

		void zoomOutStep()
		{
			auto factor = std::pow(1.2, -1.0);
			auto t = transform();

			if (t.m11() <= 0.1)
				return;
			scale(factor, factor);
		}

	protected:
		void drawBackground(QPainter* painter, const QRectF& r) override
		{
			QGraphicsView::drawBackground(painter, r);

			auto drawGrid = [&](int gridStep) -> void {
				auto windowRect = rect();
				auto tl = mapToScene(windowRect.topLeft());
				auto br = mapToScene(windowRect.bottomRight());

				auto left = std::floor(tl.x() / gridStep - 0.5);
				auto right = std::floor(br.x() / gridStep + 1.0);
				auto bottom = std::floor(tl.y() / gridStep - 0.5);
				auto top = std::floor(br.y() / gridStep + 1.0);

				for (auto xi = int(left); xi <= int(right); ++xi)
				{
					QLineF line(xi * gridStep, bottom * gridStep, xi * gridStep, top * gridStep);

					painter->drawLine(line);
				}

				for (auto yi = int(bottom); yi <= int(top); ++yi)
				{
					QLineF line(left * gridStep, yi * gridStep, right * gridStep, yi * gridStep);
					painter->drawLine(line);
				}
			};

			QPen pFine(QColor(60, 60, 60), 2.0);
			painter->setPen(pFine);
			drawGrid(40);
			QPen p(QColor(25, 25, 25), 2.0);
			painter->setPen(p);
			drawGrid(400);
		}

		void wheelEvent(QWheelEvent* event) override
		{
			auto delta = event->angleDelta();

			if (delta.y() == 0)
			{
				event->ignore();
				return;
			}

			if ((static_cast<double>(delta.y()) / std::abs(delta.y())) > 0.0)
				zoomInStep();
			else
				zoomOutStep();
		}

		void keyPressEvent(QKeyEvent* event) override
		{
			switch (event->key())
			{
			case Qt::Key_Shift:
				setDragMode(QGraphicsView::RubberBandDrag);
				break;
			case Qt::Key_Space:
				setSceneRect(scene()->itemsBoundingRect());
				fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
				break;
			default:
				break;
			}

			QGraphicsView::keyPressEvent(event);
		}

		void keyReleaseEvent(QKeyEvent* event) override
		{
			switch (event->key())
			{
			case Qt::Key_Shift:
				setDragMode(QGraphicsView::ScrollHandDrag);
				break;
			default:
				break;
			}
			QGraphicsView::keyReleaseEvent(event);
		}
		//
		void mousePressEvent(QMouseEvent* event) override
		{
			if (event->button() == Qt::LeftButton)
				m_lastClickPos = mapToScene(event->pos());
			QGraphicsView::mousePressEvent(event);
		}

		void mouseMoveEvent(QMouseEvent* event) override
		{
			if (scene()->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton)
			{
				if (event->modifiers() != Qt::ShiftModifier)
				{
					auto diff = m_lastClickPos - mapToScene(event->pos());
					setSceneRect(sceneRect().translated(diff.x(), diff.y()));
				}
			}

			QGraphicsView::mouseMoveEvent(event);
		}

	private:
		QPointF m_lastClickPos{ 0,0 };
	};

	FlowEditor::FlowEditor(QWidget* parent)
		: QMainWindow(parent)
	{
		setWindowState(Qt::WindowMaximized);
		setWindowTitle("Flow Editor");

		auto leftSideDock = new QDockWidget("Script Content");
		leftSideDock->setFeatures(
			QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable
		);
		scriptContentPanel = new nf::ScriptContentPanel();
		leftSideDock->setWidget(scriptContentPanel);

		scriptScene = new QGraphicsScene(this);
		auto flowView = new FlowEditorSceneView(scriptScene, this);

		setCentralWidget(flowView);
		addDockWidget(Qt::LeftDockWidgetArea, leftSideDock);

		scriptScene->addItem(new nf::EditorUINode());
	}

	FlowEditor::~FlowEditor()
	{}

	void FlowEditor::smartResizePanels() const
	{
		auto availWidth = QApplication::desktop()->screenGeometry().width();
	}

	void FlowEditor::keyPressEvent(QKeyEvent* event)
	{
		switch (event->key())
		{
		case Qt::Key_Escape:
			close();
			break;
		default:
			break;
		}

		QMainWindow::keyPressEvent(event);
	}
}