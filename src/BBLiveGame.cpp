#include "BBLiveGame.hpp"
#include "ScoreLoopThread.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

BBLiveGame::BBLiveGame(bb::cascades::Application *app)
: QObject(app)
{
	qmlRegisterType<ScoreLoopThread>("com.bblive.game", 1, 0, "ScoreLoop");  //SCORELOOP
	qmlRegisterType<QTimer>("bb.cascades", 1, 0, "QTimer");

	// create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    mScoreLoop = ScoreLoopThread::instance(); //SCORELOOP
    QObject::connect(mScoreLoop, SIGNAL(ScoreLoopUserReady(AppData_t*)), this, SLOT(scoreLoopLoaded(AppData_t*))); //SCORELOOP
    QObject::connect(mScoreLoop, SIGNAL(SubmitScoreCompleted(ScoreData_t*)), this, SLOT(onSubmitScoreCompleted(ScoreData_t*))); //SCORELOOP

    qml->setContextProperty("App", this); //Allow us to reference c++ from qml

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);

    mAppData = 0; //SCORELOOP
    mScoreLoop->start(); //SCORELOOP
}

void BBLiveGame::scoreLoopLoaded(AppData_t *data) //SCORELOOP
{
	mAppData = data;
}

void BBLiveGame::onSubmitScoreCompleted(ScoreData_t *scoreData) //SCORELOOP
{
	mLastScoreData = scoreData;
	emit(submitScoreCompleted());
}

void BBLiveGame::submitScore(int score, int mode) //SCORELOOP
{
	if(mAppData!=0)
	{
		ScoreLoopThread::SubmitScore(mAppData, score, mode);
	}
}

void BBLiveGame::loadLeaderboard(const QString &timeframe, int size) //SCORELOOP
{
	if(mAppData!=0)
	{
		if(timeframe=="24-hour")
		{
			ScoreLoopThread::LoadLeaderboard(mAppData, SC_SCORES_SEARCH_LIST_24H, size);
		}
		else
		{
			ScoreLoopThread::LoadLeaderboard(mAppData, SC_SCORES_SEARCH_LIST_ALL, size);
		}
	}
}

ScoreLoopThread* BBLiveGame::scoreLoop() //SCORELOOP
{
	return ScoreLoopThread::instance();
}

QString BBLiveGame::getValueFor(const QString &objectName, const QString &defaultValue)
{
    QSettings settings;
    if (settings.value(objectName).isNull())
    {
        return defaultValue;
    }
    return settings.value(objectName).toString();
}

void BBLiveGame::saveValueFor(const QString &objectName, const QString &inputValue)
{
    QSettings settings;
    settings.setValue(objectName, QVariant(inputValue));
}
