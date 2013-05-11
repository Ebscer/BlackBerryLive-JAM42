import bb.cascades 1.0

Container
{
    property int locked: 0
    property int value: 0
    
    background: levelButtonBack.imagePaint
    preferredWidth: 144
    preferredHeight: 144

    attachedObjects:
    [
        ImagePaintDefinition
        {
            id: levelButtonBack
            imageSource: "asset:///images/levelButtonBackground.png"
        },
        GameDialog
        {
            id: lockedDialog
            title: "Locked"
            subtitle: "This level is locked"
            buttonText: "Return Home"
            
            onButtonClick:
            {
                lockedDialog.end()
            }
        }
    ]
    
    gestureHandlers:
    [
        TapHandler
        {
            onTapped:
            {
                if(locked!=1)
                {
                    levelSelectionPage.newLevel(value)
                }
                else
                {
                    lockedDialog.start()
                }
            }
        }
    ]
    
    layout: DockLayout {}

    Container
    {
        verticalAlignment: VerticalAlignment.Bottom
        horizontalAlignment: HorizontalAlignment.Center
        bottomPadding: 4
        Label
        {
            text:"Level "+value
            textStyle.color: Color.Black
            textStyle.fontWeight: FontWeight.W200
            textStyle.fontSize: FontSize.PointValue
            textStyle.fontSizeValue: 7
        }
    }
}