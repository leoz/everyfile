import bb.cascades 1.2

NavigationPane {
    id: navPane
    
    FileListPage {
        fileListPagePath: _app.getDefPath()
        fileListPageTitle: _app.getDefTitle()
    }
}

