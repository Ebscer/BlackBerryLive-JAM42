#ifndef BBLiveGame_HPP_
#define BBLiveGame_HPP_

#include <QObject>
#include "ScoreLoopThread.hpp"

namespace bb { namespace cascades { class Application; }}

class BBLiveGame : public QObject
{
    Q_OBJECT
public:
    BBLiveGame(bb::cascades::Application *app);
    virtual ~BBLiveGame() {}

    Q_INVOKABLE QString getValueFor(const QString &objectName, const QString &defaultValue);
    Q_INVOKABLE void saveValueFor(const QString &objectName, const QString &inputValue);

	Q_INVOKABLE void submitScore(int score, int mode); //SCORELOOP
	Q_INVOKABLE void loadLeaderboard(const QString &timeframe, int size); //SCORELOOP
	Q_INVOKABLE ScoreLoopThread* scoreLoop(); //SCORELOOP

public Q_SLOTS:
    void scoreLoopLoaded(AppData_t *data); //SCORELOOP
    void onSubmitScoreCompleted(ScoreData_t *scoreData); //SCORELOOP

Q_SIGNALS:
	void submitScoreCompleted(); //SCORELOOP

private:

	AppData_t *mAppData; //SCORELOOP
	ScoreLoopThread *mScoreLoop; //SCORELOOP
	ScoreData_t *mLastScoreData; //SCORELOOP
};


#endif /* BBLiveGame_HPP_ */
