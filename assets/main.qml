import bb.cascades 1.0

NavigationPane
{
    id: navigationPane
    
    property int levelsPurchased: App.getValueFor("levelsPurchased",0)
    property int max_level: 10
    
    backButtonsVisible: false
    
    Page
    {
        id: levelSelectionPage
        
        attachedObjects:
        [
            ComponentDefinition
            {
                id: levelButtonObject
                source: "LevelButton.qml"
            },
            ComponentDefinition
            {
                id: rowObject
                source: "Row.qml"
            },
            ComponentDefinition
            {
                id: levelPageDefinition
                source: "LevelPage.qml"
            }
        ]
        
        function newLevel(lvl)
        {
            var lvlPage = levelPageDefinition.createObject()
            lvlPage.current_level = lvl
            navigationPane.push(lvlPage)
        }
        
        Container
        {
            background: myBackground.imagePaint
            attachedObjects: [

                ImagePaintDefinition {
                    id: myBackground
                    imageSource: "asset:///images/tile.amd"
                    repeatPattern: RepeatPattern.XY
                }
            ]
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            Label{
                text: "Crazy Tube Game!"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Center
            }
            ScrollView
            {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                Container
                {
                    id: levels
                    
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 4
                    bottomPadding: 4

                    function populateLevels()
                    {
                        for(var i=0;i<(max_level/5);i++)
                        {
                            var r = rowObject.createObject()
                            for(var j=0;j<5;j++)
                            {
                                var b = levelButtonObject.createObject()
                            	b.value = (i*5)+j+1
                            	if(b.value>2&&levelsPurchased==0)
                            	{
                            	    b.locked = 1
                            	}
                            	if(b.value<=max_level)
                            	{
                            	    r.add(b)
                            	}
                            }
                            levels.add(r)
                        }
                    }
                    
                    onCreationCompleted:
                    {
                        populateLevels()
                    }
                }
            }
            ImageView{
                imageSource: "asset:///images/MainArt.png"
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
            }
        }
    }
    
    onPopTransitionEnded:
    {
        if(page.objectName === "leaderboardpage")
        {
            navigationPane.backButtonsVisible = false
        }
        page.destroy()
    }
}
