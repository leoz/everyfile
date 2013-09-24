import bb.cascades 1.2
import com.leoz 1.0

Page {
    property alias imageViewPageTitle: title_bar.title
    property alias imageViewPageData: image_data

    titleBar: TitleBar {
        id: title_bar
    }
    
    Container {
        layout: DockLayout {}
        background: Color.Black
        
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            id: image_view
            image: image_data.icon
            scalingMethod: ScalingMethod.AspectFit
        }        
    }
 
    attachedObjects: [
	    ImageData {
	        id: image_data
	    }
    ]
}
