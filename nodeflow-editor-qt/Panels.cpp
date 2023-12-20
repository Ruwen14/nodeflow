#include "pch.h"
#include "Panels.hpp"

#include <algorithm>

#include <QToolButton>
#include <QPixmap>
#include <QBitmap>
#include <QComboBox>
#include <QLineEdit>
#include <QLayout>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include "nodeflow/utility/dbgln.hpp"

namespace nf
{
	static QIcon createVariableTicTacIcon(const QColor& color)
	{
		QPixmap pixmap(24, 12);
		QBitmap mask(pixmap.size());
		mask.fill(Qt::color0);

		QPainter painter(&mask);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setBrush(Qt::color1);
		painter.drawRoundedRect(mask.rect(), 6, 6);

		pixmap.fill(color);
		pixmap.setMask(mask);

		return QIcon(pixmap);
	}

	static QIcon createPlusIcon()
	{
		QPixmap pixmap(12, 12);
		QBitmap mask(pixmap.size());
		mask.fill(Qt::color0);

		QPainter painter(&mask);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.setPen(QPen(Qt::black, 3));
		painter.drawLine(QPoint(0, mask.rect().height() / 2), QPoint(mask.rect().width(), mask.rect().height() / 2));
		painter.drawLine(QPoint(mask.rect().width() / 2, 0), QPoint(mask.rect().width() / 2, mask.rect().height()));
		pixmap.setMask(mask);

		return QIcon(pixmap);
	}

	class NewVarOptions : public QDialog
	{
	public:
		NewVarOptions(QWidget* parent = nullptr)
			: QDialog(parent)
		{
			setLayout(new QVBoxLayout());

			layout()->addWidget(new QPushButton());
		}
	};

	CollapsableSection::CollapsableSection(const QString& title, QWidget* content, QWidget* parent)
		: QWidget(parent)

	{
		contentLayout = new QVBoxLayout();
		contentLayout->addWidget(content);

		collapseButton = new QToolButton();
		collapseButton->setText(title);
		collapseButton->setCheckable(true);
		collapseButton->setChecked(false);
		collapseButton->setStyleSheet(R"""(
			QToolButton
			{
				border: none;
				color: lightGray;
				background-color: rgb(56, 56, 56);
				border-radius: 0px;
				padding: 5px;
			}
			)""");

		collapseButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		collapseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		collapseButton->setIcon(QIcon(":/icons/triangle-down-gray.svg"));
		collapseButton->setIconSize(QSize(15, 15));

		// 		collapseButton->setArrowType(Qt::RightArrow);
		auto font = QApplication::font();
		font.setPointSize(8);
		font.setBold(true);
		collapseButton->setFont(font);
		connect(collapseButton, &QToolButton::toggled, this, &CollapsableSection::toggle);

		collapseAnimation = new QParallelAnimationGroup(this);
		contentArea = new QScrollArea(this);
		contentArea->setMaximumHeight(0);
		contentArea->setMinimumHeight(0);
		contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		contentArea->setFrameShape(QFrame::NoFrame);


		auto sectionLayout = new QVBoxLayout(this);
		sectionLayout->setSpacing(0);
		sectionLayout->setContentsMargins(0, 0, 0, 0);
		sectionLayout->addWidget(collapseButton);

		sectionLayout->addWidget(contentArea);

		collapseAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
		collapseAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
		collapseAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

		contentArea->setLayout(contentLayout);

		recalculate();

		expand();


		connect(collapseAnimation, &QParallelAnimationGroup::finished, this, [this]() {
			emit toggled();
		});

	}
	CollapsableSection::CollapsableSection(const QString& title, QLayout* contentLayout, QWidget* parent)
		: QWidget(parent)
	{
		collapseButton = new QToolButton();
		collapseButton->setText(title);
		collapseButton->setCheckable(true);
		collapseButton->setChecked(false);
		collapseButton->setStyleSheet(R"""(
			QToolButton
			{
				border: none;
				color: lightGray;
				background-color: rgb(56, 56, 56);
				border-radius: 0px;
				padding: 5px;
			}
			)""");

		collapseButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		collapseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		collapseButton->setIcon(QIcon(":/icons/triangle-down-gray.svg"));
		collapseButton->setIconSize(QSize(15, 15));

		// 		collapseButton->setArrowType(Qt::RightArrow);
		auto font = QApplication::font();
		font.setPointSize(8);
		font.setBold(true);
		collapseButton->setFont(font);
		connect(collapseButton, &QToolButton::toggled, this, &CollapsableSection::toggle);

		collapseAnimation = new QParallelAnimationGroup(this);
		contentArea = new QScrollArea(this);
		contentArea->setMaximumHeight(0);
		contentArea->setMinimumHeight(0);
		contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		contentArea->setFrameShape(QFrame::NoFrame);
		auto hline = new QFrame;
		hline->setFrameStyle(QFrame::HLine);
		hline->setStyleSheet("color: rgb(79, 79, 79);");

		auto sectionLayout = new QVBoxLayout(this);
		sectionLayout->setSpacing(0);
		sectionLayout->setContentsMargins(0, 0, 0, 0);
		sectionLayout->addWidget(collapseButton);

		sectionLayout->addWidget(hline);
		sectionLayout->addWidget(contentArea);


		contentArea->setLayout(contentLayout);

		collapseAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
		collapseAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
		collapseAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

		recalculate();

		expand();
	}


	void CollapsableSection::recalculate()
	{
		auto collapsedHeight = std::abs(sizeHint().height() - contentArea->maximumHeight());

		qDebug() << collapsedHeight;
		auto contentHeight = contentLayout->sizeHint().height();

		for (int i = 0; i < collapseAnimation->animationCount(); ++i)
		{
			auto animation = static_cast<QPropertyAnimation*>(collapseAnimation->animationAt(i));
			animation->setDuration(0);
			animation->setStartValue(collapsedHeight);
			animation->setEndValue(collapsedHeight + contentHeight);

		}

		auto contentAnimation = static_cast<QPropertyAnimation*>(
			collapseAnimation->animationAt(collapseAnimation->animationCount() - 1)
			);

		contentAnimation->setDuration(0);
		contentAnimation->setStartValue(0);
		contentAnimation->setEndValue(contentHeight);
	}

	void CollapsableSection::expand()
	{
		if (!collapseButton->isChecked())
			collapseButton->toggle();
	}

	void CollapsableSection::collapse()
	{
		if (collapseButton->isChecked())
			collapseButton->toggle();
	}



	void CollapsableSection::toggle(bool checked)
	{
// 		recalculate();


		// 		collapseButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
		collapseButton->setIcon(checked ? QIcon(":/icons/triangle-down-gray.svg") : QIcon(":/icons/triangle-right-gray.svg"));

		collapseAnimation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);

		auto collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
		auto contentHeight = contentLayout->sizeHint().height();



		collapseAnimation->start();


		if (checked)
		{
			emit expanded();
		}
		else
		{
			emit collapsed();
		}




	}

	VariableAddConfigurator::VariableAddConfigurator(const QList<QString>& availabelVarTypes, QWidget* parent /*= nullptr*/)
		: QWidget(parent), m_varNameEdit(new QLineEdit), m_varTypeSelector(new QComboBox), m_varDefaultValueEdit(new QLineEdit)
	{
		auto lay = new QVBoxLayout(this);

		lay->setContentsMargins(2, 2, 2, 2);

		setObjectName("VariableAddConfigurator"),

			setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
		//setStyleSheet(R"""(
		//				#VariableAddConfigurator
		//				{
		//					border: 1px solid red;
		//				}
		//				QLabel, QLineEdit
		//				{
		//					font-size: 8pt;
		//				}
		//)""");

		auto box = new QGroupBox("Create New");
		box->setFlat(true);
		// 		box->setStyleSheet("QGroupBox::title {color: darkGray;} QGroupBox{background-color:rgb(61, 61, 60);}");
		auto font = QApplication::font();
		font.setBold(true);
		font.setPointSize(8);
		box->setFont(font);

		auto content = new QFormLayout(box);

		m_varNameEdit->setClearButtonEnabled(true);
		m_varNameEdit->setPlaceholderText("Name");
		content->addRow("Name", m_varNameEdit);

		m_varTypeSelector->setIconSize(QSize(12, 6));
		font.setPointSize(8);
		font.setBold(false);
		m_varTypeSelector->setFont(font);
		for (const auto& varType : availabelVarTypes)
			m_varTypeSelector->addItem(createVariableTicTacIcon(Qt::red), varType);

		content->addRow("Type", m_varTypeSelector);

		m_varDefaultValueEdit->setPlaceholderText("None");
		content->addRow("Default Value", m_varDefaultValueEdit);

		lay->addWidget(box);
		setFont(font);
	}

	AddVariableConfigPopup::AddVariableConfigPopup(const QList<QString>& availabelVarTypes, QWidget* parent /*= nullptr*/)
		: QDialog(parent), m_varNameEdit(new QLineEdit), m_varTypeSelector(new QComboBox), m_varDefaultValueEdit(new QLineEdit)
	{
		auto lay = new QVBoxLayout(this);

		lay->setContentsMargins(2, 2, 2, 2);

		setObjectName("AddVariableConfigPopup"),

			// 		setFrameStyle(QFrame::Panel | QFrame::Raised);
			setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
		//setStyleSheet(R"""(
		//				#AddVariableConfigPopup
		//				{
		//					border: 1px solid red;
		//				}
		//				QLabel, QLineEdit
		//				{
		//					font-size: 8pt;
		//				}
		//)""");

		auto box = new QGroupBox("Configuration");
		box->setFlat(true);
		// 		box->setStyleSheet("QGroupBox::title {color: darkGray;}");
		auto font = QApplication::font();
		font.setBold(true);
		font.setPointSize(8);
		box->setFont(font);

		auto content = new QFormLayout(box);

		m_varNameEdit->setClearButtonEnabled(true);
		m_varNameEdit->setPlaceholderText("Name");
		content->addRow("Name", m_varNameEdit);

		m_varTypeSelector->setIconSize(QSize(12, 6));
		font.setPointSize(8);
		font.setBold(false);
		m_varTypeSelector->setFont(font);
		for (const auto& varType : availabelVarTypes)
			m_varTypeSelector->addItem(createVariableTicTacIcon(Qt::red), varType);

		content->addRow("Type", m_varTypeSelector);

		m_varDefaultValueEdit->setPlaceholderText("None");
		content->addRow("Default Value", m_varDefaultValueEdit);

		lay->addWidget(box);
		setFont(font);
	}

	void AddVariableConfigPopup::resetVarConfig()
	{
		m_varNameEdit->clear();
		m_varDefaultValueEdit->clear();
		m_varTypeSelector->setCurrentIndex(0);
	}

	AddVariableConfigPopup::Config AddVariableConfigPopup::getCurrentVarConfig() const
	{
		return { m_varNameEdit->text(), m_varTypeSelector->currentText(), m_varDefaultValueEdit->text() };
	}

	ScriptVariablesTable::ScriptVariablesTable(const QList<QString>& availabelVarTypes, QWidget* parent /*= nullptr*/)
		: QWidget(parent), m_sceneVarTable(new QTableWidget(0, 2)), m_addVarButton(new QPushButton), m_addVarPopup(new AddVariableConfigPopup(availabelVarTypes, this)), m_availVarTypes(availabelVarTypes)
	{
		auto lay = new QVBoxLayout(this);
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(0);

		m_addVarButton->setIcon(createPlusIcon());
		m_addVarButton->setFixedSize(28, 24);
		m_addVarButton->setFlat(true);
		connect(m_addVarButton, &QPushButton::clicked, this, &ScriptVariablesTable::spawnAddVariableOptionDialog);

		m_sceneVarTable->setShowGrid(false);
		m_sceneVarTable->horizontalHeader()->hide();
		m_sceneVarTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		m_sceneVarTable->verticalHeader()->hide();
		m_sceneVarTable->setSelectionMode(QAbstractItemView::SingleSelection);
		m_sceneVarTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		m_sceneVarTable->setDragEnabled(false);
		m_sceneVarTable->setDragDropMode(QAbstractItemView::DragOnly);
		m_sceneVarTable->setIconSize(QSize(12, 6));
		m_sceneVarTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

		auto font = QApplication::font();
		font.setPointSize(10);
		setFont(font);

		lay->addWidget(m_addVarButton);
		lay->addWidget(new VariableAddConfigurator(availabelVarTypes));
		lay->addWidget(m_sceneVarTable);
		lay->setAlignment(m_addVarButton, Qt::AlignRight);
	}

	bool ScriptVariablesTable::addNewVariable(const QString& varName, const QString& varType, const QColor& color)
	{
		// varType not registered
		if (std::find(m_availVarTypes.begin(), m_availVarTypes.end(), varType) == m_availVarTypes.end())
			return false;

		auto row = m_sceneVarTable->rowCount();
		m_sceneVarTable->insertRow(row);

		auto varNameItem = new QTableWidgetItem(varName);
		m_sceneVarTable->setItem(row, 0, varNameItem);
		auto varTypeItem = new QTableWidgetItem(createVariableTicTacIcon(color), varType);

		m_sceneVarTable->setItem(row, 1, varTypeItem);

		m_sceneVarTable->resizeRowsToContents();
		return true;
	}

	void ScriptVariablesTable::spawnAddVariableOptionDialog()
	{
		m_addVarPopup->resetVarConfig();
		m_addVarPopup->move(m_addVarButton->mapToGlobal(QPoint(m_addVarButton->width(), m_addVarButton->height())));
		m_addVarPopup->exec();

		qDebug() << m_addVarPopup->getCurrentVarConfig().varName << "\n" << m_addVarPopup->getCurrentVarConfig().varType;

		// 		auto menu = new QMenu(this);
		// 		menu->setTitle("Hi");
		// 		menu->setContentsMargins(10, 10, 10, 10);
		//
		// 		auto varNameEditAction = new QWidgetAction(menu);
		// 		auto varNameEdit = new QLineEdit();
		// 		varNameEdit->setPlaceholderText("Variable Name");
		// 		varNameEditAction->setDefaultWidget(varNameEdit);
		//
		// 		auto varTypeSelectorAction = new QWidgetAction(menu);
		// 		auto varTypeSelector = new QComboBox();
		// 		varTypeSelector->setIconSize(QSize(12, 6));
		// 		varTypeSelectorAction->setDefaultWidget(varTypeSelector);
		//
		// 		for (const auto& varType : m_availVarTypes)
		// 		{
		// 			varTypeSelector->addItem(createVariableTicTacIcon(Qt::red), varType);
		// 			qDebug() << varType;
		// 		}
		//
		// 		menu->addAction(varNameEditAction);
		// 		menu->addAction(varTypeSelectorAction);
		// 		menu->exec(QCursor::pos());
	}

	ScriptVariablesSection::ScriptVariablesSection(const QList<QString>& availabelVarTypes, QWidget* parent)
		: QWidget(parent), m_varNameEdit(new QLineEdit), m_varTypeSelector(new QComboBox), m_varDefaultValueEdit(new QLineEdit), m_addVarButton(new QPushButton)
	{
		auto lay = new QVBoxLayout(this);
		lay->setContentsMargins(0, 0, 0, 0);
		lay->setSpacing(0);

		setupNewVariableCreator(availabelVarTypes, lay);
	}

	void ScriptVariablesSection::setupNewVariableCreator(const QList<QString>& availabelVarTypes, QVBoxLayout* mainLay)
	{
		auto box = new QGroupBox("Create New");
		//box->setStyleSheet(R"""(
		//				QGroupBox::title {color: darkGray;}
		//				QGroupBox {
		//					background-color: rgb(26, 26, 26);
		//					border: none;
		//					margin-top: 10px;
		//				}
		//				QLabel, QLineEdit { font-size: 8pt; }
		//
		//)""");

		auto font = QApplication::font();
		font.setBold(true);
		font.setPointSize(8);
		box->setFont(font);

		auto boxLay = new QHBoxLayout(box);
		auto lay = new QFormLayout();

		m_varNameEdit->setClearButtonEnabled(true);
		m_varNameEdit->setPlaceholderText("Name");
		lay->addRow("Name", m_varNameEdit);

		m_varTypeSelector->setIconSize(QSize(12, 6));
		font.setPointSize(8);
		font.setBold(false);
		m_varTypeSelector->setFont(font);
		for (const auto& varType : availabelVarTypes)
			m_varTypeSelector->addItem(createVariableTicTacIcon(Qt::red), varType);
		lay->addRow("Type", m_varTypeSelector);

		m_varDefaultValueEdit->setClearButtonEnabled(true);
		m_varDefaultValueEdit->setPlaceholderText("None");
		lay->addRow("Default Value", m_varDefaultValueEdit);

		boxLay->addLayout(lay);

		m_addVarButton->setIcon(createPlusIcon());
		// 		m_addVarButton->setFlat(true);
		//m_addVarButton->setStyleSheet(R"""(
		// QPushButton
		//    {
		//        background-color: rgb(87, 87, 87);
		//		border: none;
		//    }
		//
		//    QPushButton:pressed
		//    {
		//        background-color: rgb(47, 47, 47);
		//    }
		//
		//    QPushButton:hover:!pressed
		//    {
		//        background-color: rgb(87, 87, 87);
		//    }
		//)""");

		m_addVarButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

		boxLay->addWidget(m_addVarButton);

		mainLay->addWidget(box);
		auto hLine = new QFrame;
		hLine->setFrameStyle(QFrame::HLine);
		hLine->setStyleSheet("color: rgb(79, 79, 79);");
		mainLay->addWidget(hLine);
	}

	ScriptContentPanel::ScriptContentPanel(QWidget* parent)
		: QWidget(parent)
	{
		setObjectName("ScriptContentPanel");
		setLayout(new QVBoxLayout);
		auto contentWidget = new QWidget();
		contentLayout = new QVBoxLayout(contentWidget);
		auto contentArea = new QScrollArea();
		contentArea->setWidgetResizable(true);
		contentArea->setWidget(contentWidget);
		contentArea->setFrameShape(QFrame::NoFrame);

		layout()->addWidget(contentArea);
		layout()->setContentsMargins(0, 0, 0, 0);

		auto scriptVarablesSectionLayout = new QVBoxLayout();
		scriptVariablesTable = new ScriptVariablesTable({ "Boolean", "Byte", "Integer", "Float", "String", "Vector", "Rotator", "Transform" });
		scriptVarablesSectionLayout->setContentsMargins(9, 0, 0, 9);
		scriptVarablesSectionLayout->addWidget(scriptVariablesTable);

		auto scriptVariablesSection = new nf::CollapsableSection("VARIABLES", scriptVarablesSectionLayout);
		scriptVariablesTable->addNewVariable("MyNewVar", "Float", Qt::red);

		auto scriptRealVariablesSectionLayout = new QVBoxLayout();
		scriptRealVariablesSectionLayout->setContentsMargins(0, 0, 0, 0);
		scriptRealVariablesSectionLayout->setSpacing(0);
		auto scriptRealVariableSection = new ScriptVariablesSection({ "Boolean", "Byte", "Integer", "Float", "String", "Vector", "Rotator", "Transform" });
		scriptRealVariablesSectionLayout->addWidget(new QLineEdit());
		scriptRealVariablesSectionLayout->addWidget(new QLineEdit());
		scriptRealVariablesSectionLayout->addWidget(new QLineEdit());
		scriptRealVariablesSectionLayout->addWidget(new QLineEdit());
		scriptRealVariablesSectionLayout->addWidget(new QLineEdit());

		auto label = new QLabel("Hi");
		label->setText("Hi");
		auto labelLayout = new QVBoxLayout();
		labelLayout->addWidget(label);
		auto nestedSection = new nf::CollapsableSection("NESTED", labelLayout);

		scriptRealVariablesSectionLayout->addWidget(new MyPushButton());

		auto scriptVariablesSectionBox = new nf::CollapsableSection("REAL VARIABLES", scriptRealVariablesSectionLayout);

		auto lay = new QVBoxLayout(this);
		lay->addWidget(new MyPushButton());
		auto lastSection = new nf::CollapsableSection("REAL VARIABLES", lay);

		scriptVariablesSectionBox->setMinimumWidth(300);

		contentLayout->addWidget(scriptVariablesSection);
		contentLayout->addWidget(scriptVariablesSectionBox);
		contentLayout->addWidget(lastSection);

		contentLayout->setContentsMargins(2, 2, 2, 2);
		contentLayout->setSpacing(4);
		contentLayout->addStretch();

		// 		contentWidget->setStyleSheet("background-color: rgb(26, 26, 26);");
	}

	InspectorPanel::InspectorPanel(QWidget* parent /*= nullptr*/)
		: QWidget(parent)
	{
	}
}