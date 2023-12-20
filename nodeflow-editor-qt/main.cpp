#include "pch.h"
#include <QtWidgets/QApplication>

#include "FlowEditor.hpp"
#include "Panels.hpp"
#include "nodeflow/utility/dbgln.hpp"
#include "nodeflow/reflection/type_reflection.hpp"
#include "ui_Example.h"
#include "Section.h"

/**
 * Helper function for forceUpdate(). Not self-sufficient!
 */
void invalidateLayout(QLayout* layout) {
	// Recompute the given layout and all its child layouts.
	for (int i = 0; i < layout->count(); i++) {
		QLayoutItem* item = layout->itemAt(i);
		if (item->layout()) {
			invalidateLayout(item->layout());
		}
		else {
			item->invalidate();
		}
	}
	layout->invalidate();
	layout->activate();
}


void forceUpdate(QWidget* widget) {
	// Update all child widgets.
	for (int i = 0; i < widget->children().size(); i++) {
		QObject* child = widget->children()[i];
		if (child->isWidgetType()) {
			forceUpdate((QWidget*)child);
		}
	}

	// Invalidate the layout of the widget.
	if (widget->layout()) {
		invalidateLayout(widget->layout());
	}
}



void setStyleMode(const QString& mode)
{
	auto palette = QPalette();

	if (mode == "darkmode")
	{
		palette.setColor(QPalette::Window, QColor(60, 63, 65));
		palette.setColor(QPalette::WindowText, Qt::lightGray);
		palette.setColor(QPalette::Base, QColor(53, 53, 53));
		palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
		palette.setColor(QPalette::ToolTipBase, Qt::black);
		palette.setColor(QPalette::ToolTipText, Qt::white);
		palette.setColor(QPalette::Text, Qt::lightGray);
		palette.setColor(QPalette::Button, QColor(53, 53, 53));
		palette.setColor(QPalette::ButtonText, Qt::lightGray);
		palette.setColor(QPalette::BrightText, Qt::white);
		palette.setColor(QPalette::Link, QColor(42, 130, 218));
		palette.setColor(QPalette::Highlight, QColor(6, 100, 195));
		palette.setColor(QPalette::HighlightedText, Qt::white);
		palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
	}

	QApplication::setPalette(palette);
}

void setAppFont(const QString& fontFamily, int pointSize = 10)
{
	auto font = QFont(fontFamily);
	font.setPointSize(pointSize),
		font.setHintingPreference(QFont::HintingPreference::PreferNoHinting);
	QApplication::setFont(font);
}

class CustomFilter : public QSortFilterProxyModel
{
public:
	CustomFilter(QObject* parent = nullptr)
		: QSortFilterProxyModel(parent)
	{
	}

protected:
	// see https://forum.qt.io/topic/65733/qtreeview-filtering-search/4
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override
	{
		// check the current item
		bool result = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
		QModelIndex currntIndex = sourceModel()->index(source_row, 0, source_parent);
		if (sourceModel()->hasChildren(currntIndex)) {
			// if it has sub items
			for (int i = 0; i < sourceModel()->rowCount(currntIndex) && !result; ++i) {
				// keep the parent if a children is shown
				result = result || filterAcceptsRow(i, currntIndex);
			}
		}

		return result;
	}
};

static QIcon createFunctionIcon(const QColor& color, const QString& letter)
{
	auto font = QApplication::font();
	QFontMetrics fontMetrics(font);
	auto bRect = fontMetrics.boundingRect(letter);
	QPixmap pixmap(bRect.width(), bRect.height());

	QBitmap mask(pixmap.size());
	mask.fill(Qt::color0);

	QPainter painter(&mask);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	painter.setBrush(Qt::color1);
	painter.setFont(font);
	painter.drawText(mask.rect(), QString(letter));

	pixmap.fill(color);
	pixmap.setMask(mask);

	return QIcon(pixmap);
}

QIcon generateCursiveFIcon(int width, int height) {
	QFont font("Times New Roman", height * 0.7, QFont::Bold);
	font.setItalic(true);

	QPixmap icon(width, height);
	icon.fill(Qt::transparent); // Set the initial background as transparent

	QPainter painter(&icon);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	painter.setPen(QPen(Qt::white, 1));
	painter.setFont(font);
	painter.drawText(0, -2, icon.rect().width(), icon.rect().height(), Qt::AlignCenter, "f");

	return QIcon(icon);
}

class SearchTreeWidget : public QWidget
{
public:
	SearchTreeWidget(QWidget* parent = nullptr)
		: QWidget(parent), model(new QStandardItemModel(this)), proxyModel(new QSortFilterProxyModel(this)), view(new QTreeView), searchEdit(new QLineEdit), searchFinishedTimer(new QTimer(this))
	{
		setObjectName("TreeWidget");
		setStyleSheet(R"(
						QTreeView
						{
							background-color: rgb(26, 26, 26);
						}
						#TreeWidget
						{
							background-color: rgb(56, 56, 56);
						}

						QLineEdit
						{
							background-color: rgb(15, 15, 15);
							border: 1px solid rgb(53, 53, 53);
							border-radius: 9px;
							padding: 1px;
							spacing: 0px;
							margin: 0px;
						}

						QLineEdit:focus
						{
							border: 1px solid rgb(6, 100, 195);
						}

						QCheckBox
						{
							border-radius: 10px;
							padding: 1px;
							spacing: 5px;
							color: rgb(14, 134, 255);
						}

						QCheckBox::indicator
						{
							width: 20 px;
							height: 20 px;
						}

						QCheckBox:unchecked
						{
							color: lightGray;
						}

						QCheckBox:checked
						{
							color: lightGray;
						}

		)");

		auto lay = new QVBoxLayout(this);

		QStandardItem* rootItem = model->invisibleRootItem();

		QMap<QString, QStringList> data = {
			{"Root", {"Child1", "Child2", "Child3"}},
			{"Child1", {"Grandchild1", "Grandchild2"}},
			{"Child2", {"Grandchild3"}},
			{"Grandchild2", {"GreatGrandchild1"}},
			{"Child3", {"Grandchild4"}},
		};

		for (const auto& parent : data.keys()) {
			auto* parentItem = new QStandardItem(parent);
			auto font = QApplication::font();
			font.setBold(true);
			parentItem->setFont(font);
			rootItem->appendRow(parentItem);
			for (const auto& child : data.value(parent)) {
				auto* childItem = new QStandardItem(child);
				parentItem->appendRow(childItem);
			}
		}

		auto cursiveFIcon = generateCursiveFIcon(10, 16);
		for (size_t i = 0; i < rootItem->rowCount(); i++)
		{
			auto item = rootItem->child(i);
			//
			for (size_t i = 0; i < 1000; i++)
				item->appendRow(new QStandardItem(cursiveFIcon, QString("item %0").arg(i)));
		}

		proxyModel->setSourceModel(model);
		proxyModel->setRecursiveFilteringEnabled(true);
		proxyModel->setDynamicSortFilter(false);

		view->setModel(proxyModel);
		view->setHeaderHidden(true);
		view->collapseAll();
		view->setIconSize(QSize(20, 20));

		searchFinishedTimer->setSingleShot(true);
		searchEdit->setClearButtonEnabled(true);
		searchEdit->setPlaceholderText("Search Node");
		searchEdit->addAction(QIcon("C:/Users/ruwen/OneDrive/Desktop/nodeflow/nodeflow-editor/icons/find.svg"),
			QLineEdit::LeadingPosition);

		// 		connect(searchEdit, &QLineEdit::textChanged, this, &SearchTreeWidget::waitForEditFinished);
		// 		connect(searchFinishedTimer, &QTimer::timeout, this, &SearchTreeWidget::filterSearchResults);
		connect(searchEdit, &QLineEdit::textChanged, this, &SearchTreeWidget::filterSearchResults);

		auto headerTitle = new QLabel("All Possible Actions");
		auto font = QApplication::font();
		font.setPointSize(13);
		headerTitle->setFont(font);

		auto caseSensitivityCB = new QCheckBox();
		caseSensitivityCB->setText("Case Sensitive");
		font.setPointSizeF(10.5);
		caseSensitivityCB->setFont(font);
		connect(caseSensitivityCB, &QCheckBox::stateChanged, this, [this](int state) {
			proxyModel->setFilterCaseSensitivity(state == Qt::Checked ? Qt::CaseSensitive : Qt::CaseInsensitive);
			view->expandAll(); // Current Sort might collapse matches, so update it.
		});

		auto headerLay = new QHBoxLayout();
		headerLay->addWidget(headerTitle);
		headerLay->addStretch();
		headerLay->addWidget(caseSensitivityCB);

		lay->addLayout(headerLay);
		lay->addWidget(searchEdit);
		lay->addWidget(view);

		setMinimumWidth(400);
	}

	void addEntry(const QString& entry)
	{
		auto itemNames = entry.split('/');
		auto* parent = model->invisibleRootItem();

		for (const auto& itemName : itemNames)
		{
			auto* child = new QStandardItem(itemName);
			parent->appendRow(child);
			parent = child;
		}
	}

	void waitForEditFinished()
	{
		searchFinishedTimer->start(200);
	}

	void filterSearchResults()
	{
		auto search = searchEdit->text();
		if (search.isEmpty())
		{
			qDebug() << "case";
			proxyModel->setFilterRegExp(QRegExp());
			view->collapseAll();
			return;
		}
		qDebug() << proxyModel->filterCaseSensitivity();
		proxyModel->setFilterRegExp(QRegExp(search, proxyModel->filterCaseSensitivity(), QRegExp::FixedString));
		view->expandAll();
	}

public:
	QStandardItemModel* model;
	QSortFilterProxyModel* proxyModel;
	QTreeView* view;
	QLineEdit* searchEdit;
	QTimer* searchFinishedTimer;
};

class Example : public QWidget
{
public:
	Example(QWidget* parent = nullptr) : QWidget(parent), _ui(new Ui::ExampleForm())
	{
		_ui->setupUi(this);


		// 		auto layout = new QVBoxLayout();
		// 		inner->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
		// 
		// 		inner->contentArea->setStyleSheet("border: 1px solid blue;");
		// 
		// 		layout->addWidget(inner);
		// 		layout->addWidget(new QLabel("hi"));
		// 		layout->addStretch();


		auto widget = new QWidget();
		// 		widget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));
		widget->setObjectName("widget");
		widget->setStyleSheet("#widget{border: 1px solid magenta;}");



		auto w = new QWidget();
		auto l = new QVBoxLayout(w);
		l->addWidget(new QLabel("Hi 1"));
		l->addWidget(new QLabel("Hi 2"));

		// 		auto inner = new nf::CollapsableSection("Inner", w);
		// 		inner->contentArea->setStyleSheet("QScrollArea{border: 1px solid blue;}");


		auto lel = new QWidget();
		auto la = new QVBoxLayout(lel);

		// 		la->addWidget(new QLabel("HWW"));
		// 		la->addWidget(inner);
		// 		la->addStretch();


				// 		widget->setFixedHeight(30);



		auto innerWidget = new QWidget();
		innerWidget->setLayout(new QVBoxLayout());
		innerWidget->layout()->addWidget(new QLabel("Inner 1"));
		innerWidget->layout()->addWidget(new QLabel("Inner 2"));

		auto innerSection = new nf::CollapsableSection("Inner", innerWidget);




		auto outerWidget = new QWidget();
		auto outerLay = new QVBoxLayout(outerWidget);
		outerLay->addWidget(innerSection);
		outerLay->addWidget(new QLabel("hi"));


		auto outerSection = new nf::CollapsableSection("Outer", outerWidget);



		_ui->tabTwoLayout->addWidget(outerSection);
		_ui->tabTwoLayout->addWidget(new nf::CollapsableSection("Secon", new QLabel("hi")));
		_ui->tabTwoLayout->addStretch();

		connect(innerSection, &nf::CollapsableSection::expanded, this, [this, outerSection, innerSection] {


			outerSection->recalculate();
			outerSection->collapseButton->toggle();
			outerSection->recalculate();
			outerSection->collapseButton->toggle();


		});
		connect(innerSection, &nf::CollapsableSection::collapsed, this, [this, outerSection, innerSection] {

			outerSection->recalculate();
			outerSection->collapseButton->toggle();
			outerSection->recalculate();
			outerSection->collapseButton->toggle();


		});




	}

	Ui::ExampleForm* _ui;
};

// TODO move, copy, assign, constructor

int main(int argc, char* argv[])
{
	constexpr auto a = 3;
	constexpr auto k = nf::type_id<int>();

	qsrand(QDateTime::currentDateTime().toTime_t());

	QApplication app(argc, argv);
	app.setStyle("Fusion");
	//
// 	setStyleMode("darkmode");
	setAppFont("Roboto");
	QFile file("ue.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	qApp->setStyleSheet(styleSheet);

	// 	QPixmap c;
	// 	c.load("C:/Users/ruwen/OneDrive/Desktop/nodeflow/nodeflow-editor/icons/icon_BluePrintEditor_Function_16px_png.png");
	// 	c.save("C:/Users/ruwen/OneDrive/Desktop/nodeflow/nodeflow-editor/icons/icon_BluePrintEditor_Function_16px_png.png");

// 	SearchTreeWidget w;
	// 	auto k = w.children();
// 	w.show();

	Example w;

	w.show();

	return app.exec();
}