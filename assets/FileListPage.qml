import bb.cascades 1.2
import com.leoz 1.0

Page {    
    property alias fileListPageTitle: titlebar.title
    property alias fileListPagePath: title_path.text
    property alias fileListPageModel: list_model
    
    titleBar: TitleBar {
        id: titlebar
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Top
            
            background: Color.DarkGray
            
            bottomPadding: 2

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                background: Color.White
                
                leftPadding: 10
                
                
                Label {
                    id: title_path
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.SubtitleText
                        color: Color.DarkGray
                    }
                }                
            }
        }

        ListView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            
            dataModel: list_model
                        
            listItemComponents: [
                ListItemComponent {
                    type: ""
                    StandardListItem {
                        image: ListItemData.icon
                        title: ListItemData.name
                        description: ListItemData.date
                        status: ListItemData.size
                    }
                }
            ]

            onTriggered: {
				var chosenItem = dataModel.data(indexPath)
                var path = chosenItem.path
                var go_back = false
                var new_path = _app.getPath(path)
                var new_title = _app.getTitle(path)
                
                if(navPane.count() > 1) {
                    var p = navPane.at(navPane.count() - 2)
                    if(p) {
                        if(new_path == p.fileListPagePath) {
                            go_back = true;
                        }
                    }                    
                }
                
                if (go_back) {
                    navPane.pop()                                            
                }
                else {
                    if(_app.showFileList(path)){
                        var nextPage = file_list_page.createObject()
                        nextPage.fileListPagePath = new_path
                        nextPage.fileListPageTitle = new_title
                        navPane.push(nextPage);                    
                    }
                    else {
                        if(_app.showImageView(path)) {
                            var p = image_view_page.createObject()
                            p.imageViewPageData.reset()
                            p.imageViewPageData.path = new_path
                            p.imageViewPageData.setSize(2)
                            p.imageViewPageData.load()
                            p.imageViewPageTitle = new_title
                            navPane.push(p);                                            
                        }
                    }                    
                }
            }
            
            attachedObjects: [
                FileDataListModel {
                    id: list_model
                }                
            ]
        }
    }
    
    onFileListPagePathChanged: {
        list_model.setDir(fileListPagePath)
    }   
    
    attachedObjects: [
        ComponentDefinition {
            id: file_list_page
            source: "FileListPage.qml"
        },
        ComponentDefinition {
            id: image_view_page
            source: "ImageViewPage.qml"            
        }
    ]
}
