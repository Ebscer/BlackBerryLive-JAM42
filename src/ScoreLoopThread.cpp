/*
 * This scoreloop thread owes a lot to James Paul Muir and his scoreloop code that is hosted at
 * https://github.com/jamespaulmuir/CascadesScoreloopSample
 * There are some differences, but much of the structure, and all of the bps eventing are lifted straight
 * from this source. This even includes much of the scoreloop code in BBLiveGame as well as the qml
 */

#include <scoreloop/scoreloopcore.h>
#include "ScoreLoopThread.hpp"

#ifndef NDEBUG
#include <assert.h>
#define ASSERT(a) (assert(a))
#else
#define ASSERT(a)
#endif

#define LOG(fmt, args...) do { qDebug() << sprintf("[Scoreloop] " fmt "\n", ##args)} while (0);

//Import this static data from your scoreloop account at https://developer.scoreloop.com/
static const char SCORELOOP_GAME_ID[] = "ce163852-01ee-44b1-aca3-e6e2704c6fc3";
static const char SCORELOOP_GAME_SECRET[] = "YllY3Jl72Ats7aCr1MV5I119oWHsq+DFw521j+kpsVTs2CQDmNHLQA==";
static const char SCORELOOP_GAME_VERSION[] = "1.0";
static const char SCORELOOP_GAME_CURRENCY[] = "KQF";
static const char SCORELOOP_GAME_LANGUAGE[] = "en";

static ScoreLoopThread* _pinstance = NULL;

ScoreLoopThread::ScoreLoopThread(QObject* parent) : QThread(parent), m_quit(false)
{
}

ScoreLoopThread::~ScoreLoopThread()
{
	m_quit = true;
	wait();
}

ScoreLoopThread* ScoreLoopThread::instance()
{
	if (_pinstance == NULL)
	{
		_pinstance = new ScoreLoopThread();
	}
	return _pinstance;
}

void ScoreLoopThread::run()
{
	qDebug() << "ScoreLoopThread run() started";
	AppData_t app;
	SC_InitData_t initData;
	SC_Error_t rc;
	char versionBuffer[0x100]; /* Thats 256 bytes */
	qDebug() << "Starting Scoreloop";

	/* Initialize the BPS event system */
	bps_initialize();
	bps_set_verbosity(0); /* Set to 1 or 2 for more output if you like */

	memset(&app, 0, sizeof(AppData_t));

	/* Initialize the Scoreloop platform dependent SC_InitData_t structure to default values. */
	SC_InitData_Init(&initData);

	/* What version of the Scoreloop library do we use? */
	if (SC_GetVersionInfo(&initData, versionBuffer, sizeof(versionBuffer)))
	{
		qDebug() << "Version-Info: " << versionBuffer;
	}

	/* Now, create the Scoreloop Client with the initialized SC_InitData_t structure as well as the game-id and game-secret as found on the developer portal. */
	rc = SC_Client_New(&app.client, &initData, SCORELOOP_GAME_ID, SCORELOOP_GAME_SECRET, SCORELOOP_GAME_VERSION, SCORELOOP_GAME_CURRENCY, SCORELOOP_GAME_LANGUAGE);
	if (rc != SC_OK)
	{
		HandleError(&app, rc);
	}
	else
	{
		RequestUser(&app);
	}

	while (!m_quit)
	{
		/* Get next BPS event */
		bps_event_t *event;
		bps_get_event(&event, -1);

		/* Scoreloop event handling */
		if (bps_event_get_domain(event) == SC_GetBPSEventDomain(&initData))
		{
			SC_HandleBPSEvent(&initData, event);
		}
		else if (bps_event_get_domain(event) == dialog_get_domain())
		{
			dialog_destroy(dialog_event_get_dialog_instance(event));
			app.dialog = 0;
		}
	}

	/* Cleanup the Scoreloop client */
	SC_Client_Release(app.client);
	bps_shutdown();
}

void ScoreLoopThread::RequestUser(AppData_t *app)
{
	SC_Error_t rc = SC_Client_CreateUserController(app->client, &app->userController, RequestUserCompletionCallback, app);
	if (rc != SC_OK)
	{
		HandleError(app, rc);
		return;
	}

	/* Make the asynchronous request */
	rc = SC_UserController_LoadUser(app->userController);
	if (rc != SC_OK)
	{
		SC_UserController_Release(app->userController);
		HandleError(app, rc);
		return;
	}
	qDebug() << "Requesting User...";
}

void ScoreLoopThread::RequestUserCompletionCallback(void *userData, SC_Error_t completionStatus)
{
	AppData_t *app = (AppData_t *) userData;

	/* Check completion status */
	if (completionStatus != SC_OK)
	{
		SC_UserController_Release(app->userController); /* Cleanup Controller */
		HandleError(app, completionStatus);
		return;
	}
	qDebug() << "Done requesting User";

	/* Get the session from the client. */
	SC_Session_h session = SC_Client_GetSession(app->client);

	/* Get the session user from the session. */
	SC_User_h user = SC_Session_GetUser(session);

	/* Write out some user data */
	SC_String_h login = SC_User_GetLogin(user);
	qDebug() << " User's login:" << (login ? SC_String_GetData(login) : "<unknown>");
	SC_String_h email = SC_User_GetEmail(user);
	qDebug() << " User's email: " << (email ? SC_String_GetData(email) : "<unknown>");

	/* We don't need the UserController anymore, so release it */
	SC_UserController_Release(app->userController);

	/* send this to the app so we can save it for future requests */
	emit(instance()->ScoreLoopUserReady(app));

	/* normal signal for emitting the username */
	emit(instance()->RequestUserCompleted(login ? SC_String_GetData(login) : "<unknown>"));
}

void ScoreLoopThread::HandleError(AppData_t *app, SC_Error_t error)
{	/* Log the error */
	qDebug() << SC_MapErrorToStr(error);
}

void ScoreLoopThread::SubmitScore(AppData_t *app, double result, unsigned int mode)
{
	qDebug() << "Submitting score " << result;
	/* Create a ScoreController */
	SC_Error_t rc = SC_Client_CreateScoreController(app->client, &app->scoreController, SubmitScoreCompletionCallback, app);
	if (rc != SC_OK)
	{
		HandleError(app, rc);
		return;
	}

	/* Create and configure the score */
	rc = SC_Client_CreateScore(app->client, &app->score);
	if (rc != SC_OK)
	{
		SC_ScoreController_Release(app->scoreController); /* Cleanup Controller */
		HandleError(app, rc);
		return;
	}
	SC_Score_SetLevel(app->score, 1);
	SC_Score_SetResult(app->score, result);
	SC_Score_SetMode(app->score, mode);

	rc = SC_ScoreController_SubmitScore(app->scoreController, app->score);
	if (rc != SC_OK)
	{
		SC_ScoreController_Release(app->scoreController); /* Cleanup Controller */
		SC_Score_Release(app->score); /* Cleanup Score */
		HandleError(app, rc);
		return;
	}
	qDebug() << "Submitting Score...";
}

void ScoreLoopThread::SubmitScoreCompletionCallback(void *userData, SC_Error_t completionStatus)
{
	/* Get the application from userData argument */
	AppData_t *app = (AppData_t *) userData;

	SC_Error_t rc ;
	if (completionStatus != SC_OK) /* Check completion status */
	{
		SC_ScoreController_Release(app->scoreController); /* Cleanup Controller */
		SC_Score_Release(app->score); /* Cleanup Score */
		HandleError(app, completionStatus);
		return;
	}
	qDebug() << "Done submitting Score";

	/* Cleanup Controller */
	SC_ScoreController_Release(app->scoreController);

	SC_Score_h lastScore;

	rc = SC_Client_CreateScore(app->client, &lastScore);
	if (rc != SC_OK)
	{
		HandleError(app, rc);
		return;
	}
	SC_Score_SetLevel(lastScore, SC_Score_GetLevel(app->score));
	SC_Score_SetResult(lastScore, SC_Score_GetResult(app->score));
	SC_Score_SetMode(lastScore, SC_Score_GetMode(app->score));

	ScoreData_t scoreData;
	scoreData.score = lastScore;

	emit(instance()->SubmitScoreCompleted(&scoreData));

	SC_Score_Release(app->score);
}

void ScoreLoopThread::LoadLeaderboard(AppData_t *app, SC_ScoresSearchList_t searchList, unsigned int count)
{
	SC_Error_t rc;
	rc = SC_Client_CreateScoresController(app->client, &app->scoresController, LoadLeaderboardCompletionCallback, app);

	if (rc != SC_OK)
	{
		HandleError(app, rc);
		return;
	}

	rc = SC_ScoresController_SetSearchList(app->scoresController, searchList);
	if (rc != SC_OK)
	{
		SC_ScoresController_Release(app->scoresController); /* Cleanup Controller */
		HandleError(app, rc);
		return;
	}

	SC_Range_t range;
	range.offset = 0;
	range.length = count;

	rc = SC_ScoresController_LoadScores(app->scoresController, range);
	if (rc != SC_OK)
	{
		SC_ScoresController_Release(app->scoresController); /* Cleanup Controller */
		HandleError(app, rc);
		return;
	}
	qDebug() << "Loading Leaderboard...";
}

void ScoreLoopThread::LoadLeaderboardCompletionCallback(void *userData, SC_Error_t completionStatus)
{
	AppData_t *app = (AppData_t *) userData;

	if (completionStatus != SC_OK)
	{
		SC_ScoresController_Release(app->scoresController); /* Cleanup Controller */
		HandleError(app, completionStatus);
		return;
	}

	SC_ScoreList_h scoreList = SC_ScoresController_GetScores(app->scoresController);
	if (scoreList == NULL)
	{
		SC_ScoresController_Release(app->scoresController); /* Cleanup Controller */
		HandleError(app, SC_NOT_FOUND);
		return;
	}

	QVariantList leaderboardData;

	unsigned int i, numScores = SC_ScoreList_GetCount(scoreList);
	for (i = 0; i < numScores; ++i)
	{
		SC_Score_h score = SC_ScoreList_GetAt(scoreList, i);
		SC_User_h user = SC_Score_GetUser(score);
		SC_String_h login = user ? SC_User_GetLogin(user) : NULL;

		qDebug() << " Rank: " << SC_Score_GetRank(score) << ", Result: "  << ", User: " << (login ? SC_String_GetData(login) : "<unknown>");

		QVariantMap scoreData;
		scoreData["rank"] = SC_Score_GetRank(score);
		scoreData["simpleScore"] = SC_Score_GetResult(score);
		scoreData["username"] = login ? SC_String_GetData(login) : "<unknown>";
		scoreData["largeString"] = QString(SC_String_GetData(login)) + " " +SC_Score_GetResult(score);

		leaderboardData.append(scoreData);
	}

	emit(instance()->LoadLeaderboardCompleted(leaderboardData));

	SC_ScoresController_Release(app->scoresController);
}
