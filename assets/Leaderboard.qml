import bb.cascades 1.0

Page
{
    id: topLeaderboardPage
    objectName: "leaderboardpage"
    Container
    {
        horizontalAlignment: HorizontalAlignment.Center
        Container
		{
			background: Color.create ("#4a4a4a")
			horizontalAlignment: HorizontalAlignment.Fill
			Container
			{
				leftPadding: 25
				Label
				{
					text: "Leaderboard"
					horizontalAlignment: HorizontalAlignment.Left
					textStyle
					{
						base: SystemDefaults.TextStyles.BigText
						color: Color.create("#00A8DF")
					}
				}
			}
		}
        Container
		{
			horizontalAlignment: HorizontalAlignment.Fill
			verticalAlignment: VerticalAlignment.Fill
			layout: DockLayout{}
			ActivityIndicator
			{
				id: loader
				preferredHeight: 500
				preferredWidth: 500
				verticalAlignment: VerticalAlignment.Center
				horizontalAlignment: HorizontalAlignment.Center
			}
			ListView
			{
				id: leaderboardList
				dataModel: GroupDataModel
				{
					id: leaderboardModel
					grouping: ItemGrouping.None
					sortingKeys: ["rank"]
				}
				visible: false
				listItemComponents:
				[
					ListItemComponent
					{
						type: "item"
						content: Container
						{
							Container
							{
								layout: StackLayout
								{
									orientation: LayoutOrientation.LeftToRight
								}
								preferredHeight: 60
								Label
								{
									text: ListItemData.rank
									textStyle.base: SystemDefaults.TextStyles.TitleText
									textStyle.textAlign: TextAlign.Center
									preferredWidth: 150
									verticalAlignment: VerticalAlignment.Center
									horizontalAlignment: HorizontalAlignment.Left
								}
								Container
								{
									layout: StackLayout
									{
										orientation: LayoutOrientation.LeftToRight
									}
									horizontalAlignment: HorizontalAlignment.Left
									verticalAlignment: VerticalAlignment.Center
									Container
									{
										Label
										{
											text: ListItemData.username
											textStyle.base: SystemDefaults.TextStyles.TitleText
										}
										layoutProperties: StackLayoutProperties
										{
											spaceQuota: 1
										}
									}
									Container
									{
										rightPadding:10
										Label
										{
											text: ListItemData.simpleScore + " points"
											textStyle.base: SystemDefaults.TextStyles.TitleText
										}
									}
								}
							}
							Divider{}
						}
					}
				]
			}
		}
        onCreationCompleted:
        {
            loader.start()
            App.scoreLoop().LoadLeaderboardCompleted.connect(topLeaderboardPage.handleLeaderboard)
            App.loadLeaderboard("all-time",100)//change to "24-hour" for a leaderboard of the past 24 hours
        }
    }
    function handleLeaderboard(leaderboardData)
    {
        loader.stop()
        loader.visible = false
        leaderboardList.visible = true
        leaderboardModel.insertList(leaderboardData)
    }
}
