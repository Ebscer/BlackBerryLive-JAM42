import bb.cascades 1.0

Dialog
{
    id: gameDialog
    
	signal buttonClick()
	property alias title:lbl1.text
	property alias subtitle:lbl2.text
	property alias buttonText:mainButton.text
	
	function start()
	{
	    dialogInAnimation.play()
	}
	function end()
	{
	    dialogOutAnimation.play()
	}
	
	Container
	{
		background: Color.create(0.0, 0.0, 0.0, 0.5)
		layout: DockLayout {}
		Container {
			maxHeight: 875.0
			maxWidth: 700.0
			horizontalAlignment: HorizontalAlignment.Center
			verticalAlignment: VerticalAlignment.Center
			
			animations:
			[
				TranslateTransition
				{
					id: dialogOutAnimation
					toY: -850
					duration: 500
					onEnded:
					{
						gameDialog.close()
					}
				},
				TranslateTransition
				{
					id: dialogInAnimation
					toY: -850
					duration: 0
					easingCurve: StockCurve.QuadraticInOut
					onEnded:
					{
						gameDialog.open()
						dialogSlideInAnimation.play()
					}
				},
				TranslateTransition
				{
					id: dialogSlideInAnimation
					toY: 0
					duration: 500
					easingCurve: StockCurve.QuadraticInOut
				}
			]
			
			layout: DockLayout {}
			ImageView {
				imageSource: "asset:///images/DialogBG.png"
			}
			Container {
				horizontalAlignment: HorizontalAlignment.Center
				verticalAlignment: VerticalAlignment.Center
				maxWidth: 595.0
				Label {
				    id: lbl1
					textStyle.fontWeight: FontWeight.Bold
					textStyle.fontSize: FontSize.XXLarge
					horizontalAlignment: HorizontalAlignment.Center
					textStyle.color: Color.White
				}
				Label {
				    id: lbl2
					minHeight: 85.0
					multiline: true
					textStyle.color: Color.Black
					horizontalAlignment: HorizontalAlignment.Center
				}

				Button {
					id: mainButton
					onClicked: {
					    buttonClick()
					}
					horizontalAlignment: HorizontalAlignment.Center
				}
			}
		}

		verticalAlignment: VerticalAlignment.Fill
		horizontalAlignment: HorizontalAlignment.Fill
	}
}