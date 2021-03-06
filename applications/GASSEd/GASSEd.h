#ifndef GASSED_H
#define GASSED_H

#include <QMainWindow>
#include <QTextEdit>
#include "Sim/GASS.h"
#include "Modules/Editor/EditorMessages.h"

class GASSRenderWidget;

namespace GASS
{
	class EditorApplication;
}
	
class ToolBar;
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QSignalMapper)

class GASSEd : public QMainWindow, public GASS::StaticMessageListener
{
    Q_OBJECT
private:
    QTextEdit *m_Center;
    //QList<ToolBar*> m_ToolBars;
	QMenu *m_FileMenu;
	QMenu *m_EditMenu;
    
	QAction *m_CopyAct;
	QAction *m_CutAct;
	QAction *m_PasteAct;
	QAction *m_DeleteAct;
	QAction *m_AddWaypointsAct;
	QAction *m_ChangeSiteAct;
	QAction *m_ExportAct;
	QAction *m_AddGraphNodeAct;
	QAction *m_InsertGraphNodeAct;

	GASSRenderWidget* m_RenderWidget;
	QMenu *m_AddTemplateMenu;
    std::string m_Config;
	GASS::SceneWeakPtr m_Scene;
	std::vector<GASS::SceneObjectWeakPtr> m_SelectionCopyBuffer;
	std::vector<GASS::SceneObjectWeakPtr> m_SelectionCutBuffer;
	std::vector<GASS::SceneObjectWeakPtr> m_Selection;
	GASS::SceneObjectWeakPtr m_SelectedObject;
	bool m_Initialized;
public:
    GASSEd(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	GASS::EditorApplication* m_GASSApp;
	void Initialize();
	void ShowObjectContextMenu(GASS::SceneObjectPtr obj, const QPoint& pos);
	GASS::ScenePtr GetScene() const {return GASS::ScenePtr(m_Scene,boost::detail::sp_nothrow_tag());}
	void OnSceneObjectSelection(GASS::EditorSelectionChangedEventPtr message);
	void SetConfigFile(const std::string &name) {m_Config = name;}
	bool IsInitialized()const {return m_Initialized;}
public slots:
    void saveLayout();
    void loadLayout();
    void OnNew();
	void OnSave();
	void OnOpen();
	void OnCopy();
	void OnCut();
	void OnPaste();
	void OnDelete();
	void OnAddWaypoints();
	void OnAddGraphNode();
	void OnInsertGraphNode();
	void OnAddTemplate();
	void OnSetAsSite();
	void OnExport();
private:
    void SetupToolBar();
    void SetupMenuBar();

};


#endif
