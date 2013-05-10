function nextLevel(currentLevel) {

        switch (currentLevel) {
            case 1:
                greenTube.rotationZ = 0;
                greenTube.translationY = 250;
                redTube.rotationZ = 0;
                redTube.translationY = 0;
                right1.easingCurve = StockCurve.Linear;
                left1.easingCurve = StockCurve.Linear;
                right2.easingCurve = StockCurve.Linear;
                left2.easingCurve = StockCurve.Linear;
                right1.duration = 3000;
                left1.duration = 2000;
                right2.duration = 2000;
                left2.duration = 2000;

                break;
            case 2:

                greenTube.rotationZ = -45.0;
                greenTube.translationY = 350.0;
                redTube.rotationZ = 0;
                redTube.translationY = 0;
                right1.easingCurve = StockCurve.QuadraticOut;
                left1.easingCurve = StockCurve.Linear;
                right2.easingCurve = StockCurve.Linear;
                left2.easingCurve = StockCurve.Linear;
                right1.duration = 3000;
                left1.duration = 2000;
                right2.duration = 2000;
                left2.duration = 1500;

                break;
            case 3:
                greenTube.rotationZ = 0.0;
                greenTube.translationY = 0.0;
                redTube.rotationZ = 90;
                redTube.translationY = 425;
                right1.easingCurve = StockCurve.Linear;
                left1.easingCurve = StockCurve.SineInOut;
                right2.easingCurve = StockCurve.ExponentialInOut;
                left2.easingCurve = StockCurve.Linear;
                right1.duration = 3000;
                left1.duration = 2000;
                right2.duration = 3000;
                left2.duration = 2000;

                break;

        }
        switch1.text = "on";
        mySwitch1.imageSource = "asset:///images/Switch11.png";
        switch2.text = "on";
        mySwitch2.imageSource = "asset:///images/Switch1.png";
        box1.translationX = 30.0;
        box2.translationX = 30.0;
        right1.play();
        right2.play();
        gameTimer.start();
        timeRemaining.value = 25;

        return;
    }