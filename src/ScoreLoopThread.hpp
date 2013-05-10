#ifndef SCORELOOPTHREAD_HPP_
#define SCORELOOPTHREAD_HPP_

#include <QThread>
#include <QVariant>
#include <qdebug.h>

#include <bps/dialog.h>

#include <scoreloop/scoreloopcore.h>

typedef struct AppData_tag {
    SC_Client_h client;
    SC_Score_h score;
    SC_UserController_h userController;
    SC_ScoreController_h scoreController;
    SC_ScoresController_h scoresController;
    SC_LocalAchievementsController_h achievementsController;
    SC_ChallengeController_h challengeController;
    SC_ChallengesController_h challengesController;
    dialog_instance_t dialog;
} AppData_t;

typedef struct ScoreData_tag {
SC_Score_h score;
} ScoreData_t;

class ScoreLoopThread: public QThread {
Q_OBJECT

public:
ScoreLoopThread(QObject* parent = 0);
virtual ~ScoreLoopThread();

static ScoreLoopThread* instance();

static void RequestUser(AppData_t *app);
static void RequestUserCompletionCallback(void *userData, SC_Error_t completionStatus);

static void SubmitScore(AppData_t *app, double result, unsigned int mode);
static void SubmitScoreCompletionCallback(void *userData, SC_Error_t completionStatus);

static void LoadLeaderboard(AppData_t *app, SC_ScoresSearchList_t searchList, unsigned int count);
static void LoadLeaderboardCompletionCallback(void *userData, SC_Error_t completionStatus);

static void HandleError(AppData_t *app, SC_Error_t error);

signals:
void ScoreLoopUserReady(AppData_t *app);
void RequestUserCompleted(QString username);
void SubmitScoreCompleted(ScoreData_t *scoreData);
void LoadLeaderboardCompleted(QVariantList leaderboardData);


protected:
    /*
* Run method implementation for QThread.
*/
    virtual void run();

    bool m_quit; // controls lifetime of bpsEventLoop() function

private:

    void bpsEventLoop();
    void processEvents();
};

#endif /* SCORELOOPTHREAD_HPP_ */
