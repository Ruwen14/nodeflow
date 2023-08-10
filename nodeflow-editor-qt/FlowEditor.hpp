#pragma once

#include <QtWidgets/QMainWindow>


namespace nf {
	class InspectorPanel;
    class ScriptContentPanel;

    class FlowEditor : public QMainWindow
    {
        Q_OBJECT

    public:
        FlowEditor(QWidget* parent = nullptr);
        ~FlowEditor();

        void smartResizePanels() const;

    protected:
        void keyPressEvent(QKeyEvent* event) override;

    private:
        QGraphicsScene* scriptScene = nullptr;
        ScriptContentPanel* scriptContentPanel = nullptr;
        InspectorPanel* inspectorPanel = nullptr;
    };
}