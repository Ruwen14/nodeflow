#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QList>
#include <QIcon>

class QLayout;
class QToolButton;
class QParallelAnimationGroup;
class QScrollArea;
class QVBoxLayout;
class QComboBox;
class QLineEdit;
class QComboBox;
class QVBoxLayout;

namespace nf
{
	// based on https://stackoverflow.com/questions/52615115/how-to-create-collapsible-box-in-pyqt
	class CollapsableSection : public QWidget
	{
		Q_OBJECT
	public:
		CollapsableSection(const QString& title, QWidget* content, QWidget* parent = nullptr);

		CollapsableSection(const QString& title = "", QLayout* contentLayout = nullptr, QWidget* parent = nullptr);

		void expand();
		void collapse();


		void recalculate();

	signals:
		void expanded();
		void collapsed();

		void toggled();

	private:
		void toggle(bool checked);

	public:
		QVBoxLayout* contentLayout;
		QToolButton* collapseButton;
		QParallelAnimationGroup* collapseAnimation;
		QScrollArea* contentArea;
	};

	class VariableAddConfigurator : public QWidget
	{
		Q_OBJECT
	public:
		struct Config
		{
			QString varName;
			QString varType;
			QString defaultVal;
		};

		VariableAddConfigurator(const QList<QString>& availabelVarTypes, QWidget* parent = nullptr);

	private:
		QLineEdit* m_varNameEdit;
		QComboBox* m_varTypeSelector;
		QLineEdit* m_varDefaultValueEdit;
	};

	class AddVariableConfigPopup : public QDialog
	{
		Q_OBJECT
	public:
		struct Config
		{
			QString varName;
			QString varType;
			QString defaultVal;
		};

		AddVariableConfigPopup(const QList<QString>& availabelVarTypes, QWidget* parent = nullptr);

		void resetVarConfig();

		Config getCurrentVarConfig() const;

	private:
		QLineEdit* m_varNameEdit;
		QComboBox* m_varTypeSelector;
		QLineEdit* m_varDefaultValueEdit;
	};

	class ScriptVariablesTable : public QWidget
	{
		Q_OBJECT
	public:
		ScriptVariablesTable(const QList<QString>& varTypes, QWidget* parent = nullptr);

		bool addNewVariable(const QString& varName, const QString& varType, const QColor& color);

	private:

		void spawnAddVariableOptionDialog();

		QTableWidget* m_sceneVarTable;
		QPushButton* m_addVarButton;
		AddVariableConfigPopup* m_addVarPopup;
		QList<QString> m_availVarTypes;
	};

	class ScriptVariablesSection : public QWidget
	{
		Q_OBJECT
	public:
		ScriptVariablesSection(const QList<QString>& availabelVarTypes, QWidget* parent = nullptr);

	private:
		void setupNewVariableCreator(const QList<QString>& availabelVarTypes, QVBoxLayout* mainLay);

	private:
		QLineEdit* m_varNameEdit;
		QComboBox* m_varTypeSelector;
		QLineEdit* m_varDefaultValueEdit;
		QPushButton* m_addVarButton;
	};

	class MyPushButton : public QPushButton
	{
		Q_OBJECT
	public:
		MyPushButton(QWidget* parent = nullptr)
			: QPushButton(parent)
		{
			setObjectName("MyPushButton");
			setText("MyPushButton");
		}


	protected:

	};

	class ScriptContentPanel : public QWidget
	{
		Q_OBJECT

	public:
		ScriptContentPanel(QWidget* parent = nullptr);

	protected:

		void paintEvent(QPaintEvent*) override
		{
			QStyleOption opt;
			opt.init(this);
			QPainter p(this);
			style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
		}

	public:
		QVBoxLayout* contentLayout = nullptr;
		ScriptVariablesTable* scriptVariablesTable = nullptr;
	};

	class InspectorPanel : public QWidget
	{
		Q_OBJECT
	public:
		InspectorPanel(QWidget* parent = nullptr);
	};
}