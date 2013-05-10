import bb.cascades 1.0

Dialog
{
	signal buttonClick()
	property alias title:lbl1.text
	property alias subtitle:lbl2.text
	property alias buttonText:mainButton.text
	
	Container
	{
		background: Color.create(0.0, 0.0, 0.0, 0.5)
		layout: DockLayout {}
		Container {
			maxHeight: 875.0
			maxWidth: 700.0
			horizontalAlignment: HorizontalAlignment.Center
			verticalAlignment: VerticalAlignment.Center
			layout: DockLayout {
			}
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