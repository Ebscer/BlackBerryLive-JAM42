import bb.cascades 1.0
import "level.js" as LevelFunctions
import com.blackberry.payment 1.0

Page {
    property int current_level: 0
    property int round_score: 0
    property int current_score: 0
    
    Container {
        background: myBackground1.imagePaint
        attachedObjects: [

            ImagePaintDefinition {
                id: myBackground1
                imageSource: "asset:///images/tile.amd"
                repeatPattern: RepeatPattern.XY
            }
        ]
        layout: DockLayout {}
        Container {
            background: Color.create("#2d2d2d")
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight

                }
                horizontalAlignment: HorizontalAlignment.Center
                Label {
                    text: "Level:"
                    textStyle.color: Color.White
                    textStyle.fontSize: FontSize.XLarge
                }
                Label {
                    id: level1
                    text: current_level
                    textStyle.color: Color.White
                    textStyle.fontSize: FontSize.XLarge
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                horizontalAlignment: HorizontalAlignment.Center

                ProgressIndicator {
                    id: timeRemaining
                    toValue: 25.0
                    value: 25.0
                    horizontalAlignment: HorizontalAlignment.Center
                    maxWidth: 650.0
                    verticalAlignment: VerticalAlignment.Center

                }
                Label {
                    id: timeLabel
                    text: "25"
                    textStyle.color: Color.White
                }
            }
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight

                }
                horizontalAlignment: HorizontalAlignment.Center
                Label {
                    text: "Score:"
                    textStyle.color: Color.White
                }
                Label {
                    id: score1
                    text: "0"
                    textStyle.color: Color.White
                }
            }

        }
        Container {
            id: gameMain
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center

            minHeight: 875.0
            layout: AbsoluteLayout {}
            Container {
                id: redTube
                layout: DockLayout {}
                Container {
                    id: box2

                    animations: [
                        TranslateTransition {
                            id: right2
                            toX: 500
                            duration: 2000
                            onEnded: {
                                left2.play();
                            }
                        },
                        TranslateTransition {
                            id: left2
                            toX: 30
                            duration: 2000
                            onEnded: {
                                right2.play();
                            }
                        }
                    ]
                    verticalAlignment: VerticalAlignment.Center
                    translationX: 30.0
                    minWidth: 100.0
                    minHeight: 100.0
                    background: Color.Red

                }
                ImageView {
                    imageSource: "asset:///images/Tube2.png"
                    verticalAlignment: VerticalAlignment.Center

                }
                ImageView {
                    imageSource: "asset:///images/Ans1.png"
                    translationX: 300.0
                    verticalAlignment: VerticalAlignment.Center

                }
            }

            Container {
                id: greenTube
                layout: DockLayout {

                }

                Container {
                    id: box1

                    animations: [
                        TranslateTransition {
                            id: right1
                            toX: 500
                            duration: 3000
                            onEnded: {
                                left1.play();
                            }
                        },
                        TranslateTransition {
                            id: left1
                            toX: 30
                            duration: 2000
                            onEnded: {
                                right1.play();
                            }
                        }
                    ]
                    verticalAlignment: VerticalAlignment.Center
                    translationX: 30.0
                    minWidth: 100.0
                    minHeight: 100.0
                    background: Color.Green

                }
                ImageView {
                    imageSource: "asset:///images/Tube1.png"
                    verticalAlignment: VerticalAlignment.Center

                }
                ImageView {
                    imageSource: "asset:///images/Ans1.png"
                    translationX: 200.0
                    verticalAlignment: VerticalAlignment.Center

                }
            }

        }

           
		Container {
			layout: StackLayout {
				orientation: LayoutOrientation.LeftToRight

			}
			horizontalAlignment: HorizontalAlignment.Center
			verticalAlignment: VerticalAlignment.Bottom
			Container {
				ImageView {
					id: mySwitch1
					imageSource: "asset:///images/Switch11.png"

				}
				Label {
					id: switch1
					text: "on"
					textStyle.color: Color.White
					horizontalAlignment: HorizontalAlignment.Center
					verticalAlignment: VerticalAlignment.Bottom
				}
				gestureHandlers: [
					TapHandler {
						onTapped: {
							if (switch1.text == "on") {
								if (right1.isPlaying() == true) {
									right1.stop();
								} else if (left1.isPlaying() == true) {
									left1.stop();
								}

								switch1.text = "off";
								mySwitch1.imageSource = "asset:///images/Switch12.png";
							} else {
								if (right1.isStopped() == true) {
									right1.play();
								} else if (left1.isStopped() == true) {
									left1.play();
								}
								switch1.text = "on";
								mySwitch1.imageSource = "asset:///images/Switch11.png";
							}
						}
					}
				]
				layout: DockLayout {

				}
			}

			Container {
				ImageView {
					id: mySwitch2
					imageSource: "asset:///images/Switch1.png"

				}
				Label {
					id: switch2
					text: "on"
					textStyle.color: Color.White
					horizontalAlignment: HorizontalAlignment.Center
					verticalAlignment: VerticalAlignment.Bottom
				}
				gestureHandlers: [
					TapHandler {
						onTapped: {
							if (switch2.text == "on") {
								if (right2.isPlaying() == true) {
									right2.stop();
								} else if (left2.isPlaying() == true) {
									left2.stop();
								}

								switch2.text = "off";
								mySwitch2.imageSource = "asset:///images/Switch2.png";
							} else {
								if (right2.isStopped() == true) {
									right2.play();
								} else if (left2.isStopped() == true) {
									left2.play();
								}
								switch2.text = "on";
								mySwitch2.imageSource = "asset:///images/Switch1.png";
							}
						}
					}
				]
				layout: DockLayout {

				}
			}
			
			ImageButton {
				defaultImageSource: "asset:///images/Sub1.png"
				pressedImageSource: "asset:///images/Sub2.png"
				onClicked: {
					var box1postion = Number(box1.translationX);
					var box2postion = Number(box2.translationX);
					var tube1score = Number(0);
					var tube2score = Number(0);
					if (box1postion > 185 && box1postion < 265) {
						tube1score = 100;
					} else if (box1postion > 100 && box1postion < 186) {
						tube1score = 75;
					} else if (box1postion > 264 && box1postion < 350) {
						tube1score = 75;
					} else if (box1postion < 101 || box1postion > 349) {
						tube1score = 25;
					}
					if (box2postion > 285 && box2postion < 365) {
						tube2score = 100;
					} else if (box2postion > 200 && box2postion < 286) {
						tube2score = 75;
					} else if (box2postion > 364 && box2postion < 450) {
						tube2score = 75;
					} else if (box2postion < 201 || box2postion > 449) {
						tube2score = 25;
					}
					gameTimer.stop();
					round_score = tube1score + tube2score;
					subDialog.open();
				}
			}
		}
    }
    
    onCurrent_levelChanged:
    {
        LevelFunctions.nextLevel(current_level)
    }

    attachedObjects: [
        QTimer {
            id: gameTimer
            interval: 1000
            onTimeout: {
                timeRemaining.value -= 1;
                timeLabel.text = timeRemaining.value;
                if (timeRemaining.value == 0) {
                    right1.stop();
                    left1.stop();
                    right2.stop();
                    left2.stop();
                    loseDialog.open();
                    gameTimer.stop();
                }
            }
        },
		GameDialog
		{
			id: subDialog
			title: "Level Complete!"
			subtitle: "Level Score: " + round_score
			buttonText: "Next Level"
			
			onButtonClick:
			{
			    var getScore = Number(score1.text);
			    current_score = getScore + round_score;
			    score1.text = current_score;
			    
			    if(current_level==3)
			    { //only 3 levels in this game so lets submit to scoreloop
			        App.submitScore(current_score,0)
			        scoreloopDialog.open()
			    }
			    else if(current_level==2)
			    { //need to use an in-app purches to get to level 3
			        inAppDialog.open()
			    }
			    else //otherwise advance to the next level
			    {
			        current_level++
			    }
			    subDialog.close()
			}
		},
		GameDialog
		{
		    id: inAppDialog
		    title: "Must purchase"
		    subtitle: "Level 3 requires an in-app purchase"
		    buttonText: "Buy"
		    onButtonClick:
		    {
		        inAppDialog.close()
		        var skuName = "UnlockAllTheLevels"
                var metadata = "Get to play level 3"
                paymentControl.purchase (skuName, skuName, skuName, metadata)
		    }
		},
		GameDialog
		{
		    id: loseDialog
		    title: "Level Failed!"
		    subtitle: "Time has run out!"
		    buttonText: "Go Home"
		    
		    onButtonClick:
		    {
		        loseDialog.close()
		        navigationPane.pop()
		    }
		},
		GameDialog
		{
		    id: scoreloopDialog
		    title: "Submited Score"
		    subtitle: "Score of " + current_score
		    buttonText: "View Leaderboard"
		    
		    onButtonClick:
		    {
		        scoreloopDialog.close()
		        navigationPane.pop()
		        
		        //show score loop chart
		        navigationPane.backButtonsVisible = true
		        var page = leaderPageDefinition.createObject();
		        navigationPane.push(page);
		    }
		},
		ComponentDefinition
		{
		    id: leaderPageDefinition;
		    source: "Leaderboard.qml"
		},
		PaymentServiceControl
		{
			id: paymentControl
			function itemPurchasedSuccess()
			{
				levelsPurchased = 1
				App.saveValueFor("levelsPurchased",levelsPurchased)
				current_level++
			}
			onPurchaseResponseSuccess:
			{
				paymentControl.itemPurchasedSuccess()
			}
			onInfoResponseError:
			{
				if(errorText=="alreadyPurchased")
				{
					paymentControl.itemPurchasedSuccess()
				}
				else
				{
				    navigationPane.pop()
				}
			}
	   }
    ]

}
